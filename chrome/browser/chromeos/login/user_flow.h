// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_USER_FLOW_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_USER_FLOW_H_

#include "base/compiler_specific.h"
#include "chrome/browser/profiles/profile.h"
#include "chromeos/login/auth/auth_status_consumer.h"
#include "components/user_manager/user.h"

namespace chromeos {

class UserContext;

class LoginDisplayHost;
// Defines possible variants of user flow upon logging in.
// See UserManager::SetUserFlow for usage contract.
class UserFlow {
 public:
  UserFlow();
  virtual ~UserFlow() = 0;

  // Provides ability to alter command line before session has started.
  virtual void AppendAdditionalCommandLineSwitches() = 0;

  // Indicates if screen locking should be enabled or disabled for a flow.
  virtual bool CanLockScreen() = 0;
  virtual bool ShouldShowSettings() = 0;
  virtual bool ShouldLaunchBrowser() = 0;
  virtual bool ShouldSkipPostLoginScreens() = 0;
  virtual bool SupportsEarlyRestartToApplyFlags() = 0;
  virtual bool HandleLoginFailure(const AuthFailure& failure) = 0;
  virtual void HandleLoginSuccess(const UserContext& context) = 0;
  virtual bool HandlePasswordChangeDetected() = 0;
  virtual void HandleOAuthTokenStatusChange(
      user_manager::User::OAuthTokenStatus status) = 0;
  virtual void LaunchExtraSteps(Profile* profile) = 0;
  void SetHost(LoginDisplayHost* host);

  LoginDisplayHost* host() {
    return host_;
  }

 private:
  LoginDisplayHost* host_;
};

// UserFlow implementation for regular login flow.
class DefaultUserFlow : public UserFlow {
 public:
  virtual ~DefaultUserFlow();

  virtual void AppendAdditionalCommandLineSwitches() override;
  virtual bool CanLockScreen() override;
  virtual bool ShouldShowSettings() override;
  virtual bool ShouldLaunchBrowser() override;
  virtual bool ShouldSkipPostLoginScreens() override;
  virtual bool SupportsEarlyRestartToApplyFlags() override;
  virtual bool HandleLoginFailure(const AuthFailure& failure) override;
  virtual void HandleLoginSuccess(const UserContext& context) override;
  virtual bool HandlePasswordChangeDetected() override;
  virtual void HandleOAuthTokenStatusChange(
      user_manager::User::OAuthTokenStatus status) override;
  virtual void LaunchExtraSteps(Profile* profile) override;
};

// UserFlow stub for non-regular flows.
class ExtendedUserFlow : public UserFlow {
 public:
  explicit ExtendedUserFlow(const std::string& user_id);
  virtual ~ExtendedUserFlow();

  virtual void AppendAdditionalCommandLineSwitches() override;
  virtual bool ShouldShowSettings() override;
  virtual void HandleOAuthTokenStatusChange(
      user_manager::User::OAuthTokenStatus status) override;

 protected:
  // Subclasses can call this method to unregister flow in the next event.
  virtual void UnregisterFlowSoon();
  std::string user_id() {
    return user_id_;
  }

 private:
  std::string user_id_;
};

}  // namespace chromeos

#endif  // CHROME_BROWSER_CHROMEOS_LOGIN_USER_FLOW_H_
