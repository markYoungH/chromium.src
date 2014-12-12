// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/events/ozone/evdev/event_converter_evdev.h"

#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "ui/events/devices/input_device.h"

namespace ui {

EventConverterEvdev::EventConverterEvdev(int fd,
                                         const base::FilePath& path,
                                         int id,
                                         InputDeviceType type)
    : fd_(fd), path_(path), id_(id), type_(type) {
}

EventConverterEvdev::~EventConverterEvdev() {
  Stop();
}

void EventConverterEvdev::Start() {
  base::MessageLoopForUI::current()->WatchFileDescriptor(
      fd_, true, base::MessagePumpLibevent::WATCH_READ, &controller_, this);
}

void EventConverterEvdev::Stop() {
  controller_.StopWatchingFileDescriptor();
}

void EventConverterEvdev::OnFileCanWriteWithoutBlocking(int fd) {
  NOTREACHED();
}

bool EventConverterEvdev::HasTouchscreen() const {
  return false;
}

gfx::Size EventConverterEvdev::GetTouchscreenSize() const {
  NOTREACHED();
  return gfx::Size();
}

}  // namespace ui
