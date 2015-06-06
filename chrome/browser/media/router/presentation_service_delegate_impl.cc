// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/media/router/presentation_service_delegate_impl.h"

#include <string>

#include "base/containers/scoped_ptr_hash_map.h"
#include "base/guid.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "chrome/browser/media/router/create_session_request.h"
#include "chrome/browser/media/router/media_route.h"
#include "chrome/browser/media/router/media_router.h"
#include "chrome/browser/media/router/media_router_mojo_impl.h"
#include "chrome/browser/media/router/media_router_mojo_impl_factory.h"
#include "chrome/browser/media/router/media_sink.h"
#include "chrome/browser/media/router/media_source_helper.h"
#include "chrome/browser/media/router/presentation_media_sinks_observer.h"
#include "chrome/browser/sessions/session_tab_helper.h"
#include "content/public/browser/presentation_screen_availability_listener.h"
#include "content/public/browser/presentation_session.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/render_process_host.h"

DEFINE_WEB_CONTENTS_USER_DATA_KEY(
    media_router::PresentationServiceDelegateImpl);

using content::RenderFrameHost;

namespace media_router {

namespace {

using DelegateObserver = content::PresentationServiceDelegate::Observer;
using RenderFrameHostId = std::pair<int, int>;

// Returns the unique identifier for the supplied RenderViewHost.
RenderFrameHostId GetRenderFrameHostId(RenderFrameHost* render_view_host) {
  int render_process_id = render_view_host->GetProcess()->GetID();
  int render_frame_id = render_view_host->GetRoutingID();
  return RenderFrameHostId(render_process_id, render_frame_id);
}

std::string GetDisplayNameForFrame(RenderFrameHostId render_frame_host_id) {
  RenderFrameHost* render_view_host = RenderFrameHost::FromID(
      render_frame_host_id.first, render_frame_host_id.second);
  std::string host = render_view_host->GetLastCommittedURL().host();
  if (StartsWithASCII(host, "www.", false))
    host = host.substr(4);
  return host;
}

}  // namespace

// Used by PresentationServiceDelegateImpl to manage
// listeners and default presentation info in a render frame.
// Its lifetime:
//  * PresentationFrameManager AddDelegateObserver
//  * Reset 0+ times.
//  * PresentationFrameManager.RemoveDelegateObserver.
class PresentationFrame {
 public:
  PresentationFrame(content::WebContents* web_contents, MediaRouter* router);
  ~PresentationFrame();

  // Mirror corresponding APIs in PresentationServiceDelegateImpl.
  bool SetScreenAvailabilityListener(
      content::PresentationScreenAvailabilityListener* listener);
  bool RemoveScreenAvailabilityListener(
      content::PresentationScreenAvailabilityListener* listener);
  bool HasScreenAvailabilityListenerForTest(
      const MediaSource::Id& source_id) const;
  void SetDefaultPresentationInfo(const std::string& default_presentation_url,
                                  const std::string& default_presentation_id);
  std::string GetDefaultPresentationId() const;
  void Reset();

  void OnDefaultPresentationStarted(
      const content::PresentationSessionInfo& session) const;
  void OnPresentationServiceDelegateDestroyed() const;

  void set_delegate_observer(DelegateObserver* observer) {
    delegate_observer_ = observer;
  }

 private:
  MediaSource GetMediaSourceFromListener(
      content::PresentationScreenAvailabilityListener* listener) const;

  scoped_ptr<content::PresentationSessionInfo> default_presentation_info_;
  scoped_ptr<PresentationMediaSinksObserver> sinks_observer_;

  // References to the owning WebContents, and the corresponding MediaRouter.
  const content::WebContents* web_contents_;
  MediaRouter* router_;

  DelegateObserver* delegate_observer_;
};

PresentationFrame::PresentationFrame(content::WebContents* web_contents,
                                     MediaRouter* router)
    : web_contents_(web_contents),
      router_(router),
      delegate_observer_(nullptr) {
  DCHECK(web_contents_);
  DCHECK(router_);
}

PresentationFrame::~PresentationFrame() {
}

void PresentationFrame::OnPresentationServiceDelegateDestroyed() const {
  if (delegate_observer_)
    delegate_observer_->OnDelegateDestroyed();
}

void PresentationFrame::OnDefaultPresentationStarted(
    const content::PresentationSessionInfo& session) const {
  if (delegate_observer_)
    delegate_observer_->OnDefaultPresentationStarted(session);
}

bool PresentationFrame::SetScreenAvailabilityListener(
    content::PresentationScreenAvailabilityListener* listener) {
  if (sinks_observer_ && sinks_observer_->listener() == listener) {
    return false;
  }
  MediaSource source(GetMediaSourceFromListener(listener));
  sinks_observer_.reset(
      new PresentationMediaSinksObserver(router_, listener, source));
  return true;
}

bool PresentationFrame::RemoveScreenAvailabilityListener(
    content::PresentationScreenAvailabilityListener* listener) {
  if (sinks_observer_ && sinks_observer_->listener() == listener) {
    sinks_observer_.reset();
    return true;
  }
  return false;
}

bool PresentationFrame::HasScreenAvailabilityListenerForTest(
    const MediaSource::Id& source_id) const {
  return sinks_observer_ && sinks_observer_->source().id() == source_id;
}

void PresentationFrame::Reset() {
  sinks_observer_.reset();
  default_presentation_info_.reset();
}

void PresentationFrame::SetDefaultPresentationInfo(
    const std::string& default_presentation_url,
    const std::string& default_presentation_id) {
  if (default_presentation_url.empty() && default_presentation_id.empty()) {
    default_presentation_info_.reset();
  } else {
    default_presentation_info_.reset(new content::PresentationSessionInfo(
        default_presentation_url, default_presentation_id));
  }
}

std::string PresentationFrame::GetDefaultPresentationId() const {
  return default_presentation_info_
             ? default_presentation_info_->presentation_id
             : "";
}

MediaSource PresentationFrame::GetMediaSourceFromListener(
    content::PresentationScreenAvailabilityListener* listener) const {
  // If the default presentation URL is empty then fall back to tab mirroring.
  std::string presentation_url(listener->GetPresentationUrl());
  return presentation_url.empty()
             ? MediaSourceForTab(SessionTabHelper::IdForTab(web_contents_))
             : MediaSourceForPresentationUrl(presentation_url);
}

// Used by PresentationServiceDelegateImpl to manage PresentationFrames.
class PresentationFrameManager {
 public:
  PresentationFrameManager(content::WebContents* web_contents,
                           MediaRouter* router);
  ~PresentationFrameManager();

  // Mirror corresponding APIs in PresentationServiceDelegateImpl.
  bool SetScreenAvailabilityListener(
      const RenderFrameHostId& render_frame_host_id,
      content::PresentationScreenAvailabilityListener* listener);
  bool RemoveScreenAvailabilityListener(
      const RenderFrameHostId& render_frame_host_id,
      content::PresentationScreenAvailabilityListener* listener);
  void SetDefaultPresentationInfo(const RenderFrameHostId& render_frame_host_id,
                                  const std::string& default_presentation_url,
                                  const std::string& default_presentation_id);
  void AddDelegateObserver(const RenderFrameHostId& render_frame_host_id,
                           DelegateObserver* observer);
  void RemoveDelegateObserver(const RenderFrameHostId& render_frame_host_id);
  void Reset(const RenderFrameHostId& render_frame_host_id);
  bool HasScreenAvailabilityListenerForTest(
      const RenderFrameHostId& render_frame_host_id,
      const MediaSource::Id& source_id) const;
  void SetMediaRouterForTest(MediaRouter* router);

  // Returns default presentation ID, or empty string if no default
  // presentation ID is set in the frame.
  std::string GetDefaultPresentationId(
      const RenderFrameHostId& render_frame_host_id) const;

  void OnDefaultPresentationStarted(
      const RenderFrameHostId& render_frame_host_id,
      const content::PresentationSessionInfo& session) const;

 private:
  PresentationFrame* GetOrAddPresentationFrame(
      const RenderFrameHostId& render_frame_host_id);

  // Maps a frame identifier to a PresentationFrame object for frames
  // that are using presentation API.
  base::ScopedPtrHashMap<RenderFrameHostId, scoped_ptr<PresentationFrame>>
      presentation_frames_;

  // References to the owning WebContents, and the corresponding MediaRouter.
  MediaRouter* router_;
  content::WebContents* web_contents_;
};

PresentationFrameManager::PresentationFrameManager(
    content::WebContents* web_contents,
    MediaRouter* router)
    : router_(router), web_contents_(web_contents) {
  DCHECK(web_contents_);
  DCHECK(router_);
}

PresentationFrameManager::~PresentationFrameManager() {
  for (auto& frame : presentation_frames_)
    frame.second->OnPresentationServiceDelegateDestroyed();
}

void PresentationFrameManager::OnDefaultPresentationStarted(
    const RenderFrameHostId& render_frame_host_id,
    const content::PresentationSessionInfo& session) const {
  auto presentation_frame = presentation_frames_.get(render_frame_host_id);
  if (presentation_frame)
    presentation_frame->OnDefaultPresentationStarted(session);
}

bool PresentationFrameManager::SetScreenAvailabilityListener(
    const RenderFrameHostId& render_frame_host_id,
    content::PresentationScreenAvailabilityListener* listener) {
  DCHECK(listener);
  auto presentation_frame = GetOrAddPresentationFrame(render_frame_host_id);
  return presentation_frame->SetScreenAvailabilityListener(listener);
}

bool PresentationFrameManager::RemoveScreenAvailabilityListener(
    const RenderFrameHostId& render_frame_host_id,
    content::PresentationScreenAvailabilityListener* listener) {
  DCHECK(listener);
  auto presentation_frame = presentation_frames_.get(render_frame_host_id);
  return presentation_frame &&
         presentation_frame->RemoveScreenAvailabilityListener(listener);
}

bool PresentationFrameManager::HasScreenAvailabilityListenerForTest(
    const RenderFrameHostId& render_frame_host_id,
    const MediaSource::Id& source_id) const {
  auto presentation_frame = presentation_frames_.get(render_frame_host_id);
  return presentation_frame &&
         presentation_frame->HasScreenAvailabilityListenerForTest(source_id);
}

void PresentationFrameManager::SetDefaultPresentationInfo(
    const RenderFrameHostId& render_frame_host_id,
    const std::string& default_presentation_url,
    const std::string& default_presentation_id) {
  auto presentation_frame = GetOrAddPresentationFrame(render_frame_host_id);
  presentation_frame->SetDefaultPresentationInfo(default_presentation_url,
                                                 default_presentation_id);
}

std::string PresentationFrameManager::GetDefaultPresentationId(
    const RenderFrameHostId& render_frame_host_id) const {
  auto presentation_frame = presentation_frames_.get(render_frame_host_id);
  return presentation_frame ? presentation_frame->GetDefaultPresentationId()
                            : "";
}

void PresentationFrameManager::AddDelegateObserver(
    const RenderFrameHostId& render_frame_host_id,
    DelegateObserver* observer) {
  auto presentation_frame = GetOrAddPresentationFrame(render_frame_host_id);
  presentation_frame->set_delegate_observer(observer);
}

void PresentationFrameManager::RemoveDelegateObserver(
    const RenderFrameHostId& render_frame_host_id) {
  auto presentation_frame = presentation_frames_.get(render_frame_host_id);
  if (presentation_frame) {
    presentation_frame->set_delegate_observer(nullptr);
    presentation_frames_.erase(render_frame_host_id);
  }
}

void PresentationFrameManager::Reset(
    const RenderFrameHostId& render_frame_host_id) {
  auto presentation_frame = presentation_frames_.get(render_frame_host_id);
  if (presentation_frame)
    presentation_frame->Reset();
}

PresentationFrame* PresentationFrameManager::GetOrAddPresentationFrame(
    const RenderFrameHostId& render_frame_host_id) {
  if (!presentation_frames_.contains(render_frame_host_id)) {
    presentation_frames_.add(
        render_frame_host_id,
        scoped_ptr<PresentationFrame>(
            new PresentationFrame(web_contents_, router_)));
  }
  return presentation_frames_.get(render_frame_host_id);
}

void PresentationFrameManager::SetMediaRouterForTest(MediaRouter* router) {
  router_ = router;
}

PresentationServiceDelegateImpl*
PresentationServiceDelegateImpl::GetOrCreateForWebContents(
    content::WebContents* web_contents) {
  DCHECK(web_contents);
  // CreateForWebContents does nothing if the delegate instance already exists.
  PresentationServiceDelegateImpl::CreateForWebContents(web_contents);
  return PresentationServiceDelegateImpl::FromWebContents(web_contents);
}

PresentationServiceDelegateImpl::PresentationServiceDelegateImpl(
    content::WebContents* web_contents)
    : web_contents_(web_contents),
      router_(MediaRouterMojoImplFactory::GetApiForBrowserContext(
          web_contents_->GetBrowserContext())),
      frame_manager_(new PresentationFrameManager(web_contents, router_)),
      weak_factory_(this) {
  DCHECK(web_contents_);
  DCHECK(router_);
}

PresentationServiceDelegateImpl::~PresentationServiceDelegateImpl() {
}

void PresentationServiceDelegateImpl::AddObserver(int render_process_id,
                                                  int render_frame_id,
                                                  DelegateObserver* observer) {
  DCHECK(observer);
  frame_manager_->AddDelegateObserver(
      RenderFrameHostId(render_process_id, render_frame_id), observer);
}

void PresentationServiceDelegateImpl::RemoveObserver(int render_process_id,
                                                     int render_frame_id) {
  frame_manager_->RemoveDelegateObserver(
      RenderFrameHostId(render_process_id, render_frame_id));
}

bool PresentationServiceDelegateImpl::AddScreenAvailabilityListener(
    int render_process_id,
    int render_frame_id,
    content::PresentationScreenAvailabilityListener* listener) {
  DCHECK(listener);
  return frame_manager_->SetScreenAvailabilityListener(
      RenderFrameHostId(render_process_id, render_frame_id), listener);
}

void PresentationServiceDelegateImpl::RemoveScreenAvailabilityListener(
    int render_process_id,
    int render_frame_id,
    content::PresentationScreenAvailabilityListener* listener) {
  DCHECK(listener);
  frame_manager_->RemoveScreenAvailabilityListener(
      RenderFrameHostId(render_process_id, render_frame_id), listener);
}

void PresentationServiceDelegateImpl::Reset(int render_process_id,
                                            int render_frame_id) {
  RenderFrameHostId render_frame_host_id(render_process_id, render_frame_id);
  frame_manager_->Reset(render_frame_host_id);
  if (IsMainFrame(render_process_id, render_frame_id))
    UpdateDefaultMediaSourceAndNotifyObservers(MediaSource(), std::string());
}

void PresentationServiceDelegateImpl::SetDefaultPresentationUrl(
    int render_process_id,
    int render_frame_id,
    const std::string& default_presentation_url,
    const std::string& default_presentation_id) {
  RenderFrameHostId render_frame_host_id(render_process_id, render_frame_id);
  frame_manager_->SetDefaultPresentationInfo(
      render_frame_host_id, default_presentation_url, default_presentation_id);
  if (IsMainFrame(render_process_id, render_frame_id)) {
    // This is the main frame, which means tab-level default presentation
    // might have been updated.
    MediaSource default_source;
    if (!default_presentation_url.empty())
      default_source = MediaSourceForPresentationUrl(default_presentation_url);

    std::string default_frame_display_name(
        GetDisplayNameForFrame(render_frame_host_id));
    UpdateDefaultMediaSourceAndNotifyObservers(default_source,
                                               default_frame_display_name);
  }
}

bool PresentationServiceDelegateImpl::IsMainFrame(int render_process_id,
                                                  int render_frame_id) const {
  RenderFrameHost* main_frame = web_contents_->GetMainFrame();
  return main_frame &&
         GetRenderFrameHostId(main_frame) ==
             RenderFrameHostId(render_process_id, render_frame_id);
}

void PresentationServiceDelegateImpl::
    UpdateDefaultMediaSourceAndNotifyObservers(
        const MediaSource& default_source,
        const std::string& default_frame_display_name) {
  if (!default_source.Equals(default_source_) ||
      default_frame_display_name != default_frame_display_name_) {
    default_source_ = default_source;
    default_frame_display_name_ = default_frame_display_name;
    FOR_EACH_OBSERVER(DefaultMediaSourceObserver,
                      default_media_source_observers_,
                      OnDefaultMediaSourceChanged(default_source_,
                                                  default_frame_display_name_));
  }
}

void PresentationServiceDelegateImpl::StartSession(
    int render_process_id,
    int render_frame_id,
    const std::string& presentation_url,
    const std::string& presentation_id,
    const PresentationSessionSuccessCallback& success_cb,
    const PresentationSessionErrorCallback& error_cb) {
  // BUG=464205
  NOTIMPLEMENTED();
}

void PresentationServiceDelegateImpl::JoinSession(
    int render_process_id,
    int render_frame_id,
    const std::string& presentation_url,
    const std::string& presentation_id,
    const PresentationSessionSuccessCallback& success_cb,
    const PresentationSessionErrorCallback& error_cb) {
  // BUG=464205
  NOTIMPLEMENTED();
}

void PresentationServiceDelegateImpl::ListenForSessionMessages(
    int render_process_id,
    int render_frame_id,
    const PresentationSessionMessageCallback& message_cb) {
  // BUG=464205
  NOTIMPLEMENTED();
}

void PresentationServiceDelegateImpl::SendMessage(
    int render_process_id,
    int render_frame_id,
    scoped_ptr<content::PresentationSessionMessage> message_request,
    const SendMessageCallback& send_message_cb) {
  // BUG=464205
  NOTIMPLEMENTED();
}

void PresentationServiceDelegateImpl::OnRouteCreated(const MediaRoute& route) {
  const MediaSource& source = route.media_source();
  DCHECK(!source.Empty());
  if (!default_source_.Equals(source))
    return;
  RenderFrameHost* main_frame = web_contents_->GetMainFrame();
  if (!main_frame)
    return;
  RenderFrameHostId render_frame_host_id(GetRenderFrameHostId(main_frame));
  // TODO(imcheng): Pass in valid default presentation ID once it is
  // available from MediaRoute URN. BUG=493365
  frame_manager_->OnDefaultPresentationStarted(
      render_frame_host_id,
      content::PresentationSessionInfo(PresentationUrlFromMediaSource(source),
                                       std::string()));
}

void PresentationServiceDelegateImpl::AddDefaultMediaSourceObserver(
    DefaultMediaSourceObserver* observer) {
  default_media_source_observers_.AddObserver(observer);
}

void PresentationServiceDelegateImpl::RemoveDefaultMediaSourceObserver(
    DefaultMediaSourceObserver* observer) {
  default_media_source_observers_.RemoveObserver(observer);
}

void PresentationServiceDelegateImpl::SetMediaRouterForTest(
    MediaRouter* router) {
  router_ = router;
  frame_manager_->SetMediaRouterForTest(router);
}

base::WeakPtr<PresentationServiceDelegateImpl>
PresentationServiceDelegateImpl::GetWeakPtr() {
  return weak_factory_.GetWeakPtr();
}

bool PresentationServiceDelegateImpl::HasScreenAvailabilityListenerForTest(
    int render_process_id,
    int render_frame_id,
    const MediaSource::Id& source_id) const {
  RenderFrameHostId render_frame_host_id(render_process_id, render_frame_id);
  return frame_manager_->HasScreenAvailabilityListenerForTest(
      render_frame_host_id, source_id);
}

}  // namespace media_router
