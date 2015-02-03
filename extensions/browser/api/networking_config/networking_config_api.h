// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_API_NETWORKING_CONFIG_NETWORKING_CONFIG_API_H_
#define EXTENSIONS_BROWSER_API_NETWORKING_CONFIG_NETWORKING_CONFIG_API_H_

#include "extensions/common/api/networking_config.h"
#include "extensions/browser/extension_function.h"

namespace extensions {

class NetworkingConfigSetNetworkFilterFunction
    : public UIThreadExtensionFunction {
 public:
  NetworkingConfigSetNetworkFilterFunction();

  ResponseAction Run() override;

  DECLARE_EXTENSION_FUNCTION("networking.config.setNetworkFilter",
                             NETWORKING_CONFIG_SETNETWORKFILTER);

 protected:
  ~NetworkingConfigSetNetworkFilterFunction() override;

  scoped_ptr<core_api::networking_config::SetNetworkFilter::Params>
      parameters_;

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkingConfigSetNetworkFilterFunction);
};

class NetworkingConfigFinishAuthenticationFunction
    : public UIThreadExtensionFunction {
 public:
  NetworkingConfigFinishAuthenticationFunction();

  ResponseAction Run() override;

  DECLARE_EXTENSION_FUNCTION("networking.config.finishAuthenticationFunction",
                             NETWORKING_CONFIG_FINISHAUTHENTICATION);

 protected:
  ~NetworkingConfigFinishAuthenticationFunction() override;

  scoped_ptr<core_api::networking_config::FinishAuthentication::Params>
      parameters_;

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkingConfigFinishAuthenticationFunction);
};

}  // namespace extensions

#endif  // EXTENSIONS_BROWSER_API_NETWORKING_CONFIG_NETWORKING_CONFIG_API_H_
