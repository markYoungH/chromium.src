// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * @fileoverview
 * Loading dialog for AppRemoting apps.
 */

'use strict';

/** @suppress {duplicate} */
var remoting = remoting || {};

/**
 * Namespace for loading window functions.
 * @type {Object}
 */
remoting.LoadingWindow = function() {};

/**
 * When the loading window times out, replace it with a generic
 * "Service Unavailable" message.
 * @private
 */
remoting.LoadingWindow.onTimeout_ = function() {
  remoting.MessageWindow.showErrorMessage(
      chrome.i18n.getMessage(/**i18n-content*/'PRODUCT_NAME'),
      chrome.i18n.getMessage(remoting.Error.SERVICE_UNAVAILABLE));
};

/**
 * Show the loading dialog and start a timer When the timer expires, an error
 * message will be displayed and the application will quit.
 */
remoting.LoadingWindow.show = function() {
  if (remoting.loadingWindow_) {
    return;
  }

  // TODO(garykac): Choose better default timeout.
  // Timeout is currently 15min to handle when we need to spin up a new VM.
  var kConnectionTimeout = 15 * 60 * 1000;

  var transparencyWarning = '';
  if (navigator.platform.indexOf('Mac') != -1) {
    transparencyWarning =
        chrome.i18n.getMessage(/**i18n-content*/'NO_TRANSPARENCY_WARNING');
  }
  remoting.loadingWindow_ = remoting.MessageWindow.showTimedMessageWindow(
      chrome.i18n.getMessage(/**i18n-content*/'PRODUCT_NAME'),
      chrome.i18n.getMessage(/**i18n-content*/'FOOTER_CONNECTING'),
      transparencyWarning,
      chrome.i18n.getMessage(/**i18n-content*/'CANCEL'),
      remoting.MessageWindow.quitApp,
      kConnectionTimeout,
      remoting.LoadingWindow.onTimeout_);
};

/**
 * Close the loading window.
 */
remoting.LoadingWindow.close = function() {
  if (remoting.loadingWindow_) {
    remoting.loadingWindow_.close();
  }
  remoting.loadingWindow_ = null;
};

/** @type {remoting.MessageWindow} */
remoting.loadingWindow_ = null;
