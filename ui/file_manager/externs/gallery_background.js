// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * This is a type to let closure compiler recognize extended methods to Window
 * instance at gallery/js/gallery.js.
 * @constructor
 * @extends {Window}
 */
function GalleryWindow() {}

/**
 * @param {!BackgroundComponents} backgroundComponents Background components.
 */
window.initialize = function(backgroundComponents) {};

/**
 * @param {!Array.<!Entry>} entries Array of entries.
 * @param {!Array.<!Entry>} selectedEntries Array of selected entries.
 */
window.loadEntries = function(entries, selectedEntries) {};

/**
 * This definition is required by
 * ui/file_manager/file_manager/common/js/util.js.
 * @type {string}
 */
Window.prototype.appID;
