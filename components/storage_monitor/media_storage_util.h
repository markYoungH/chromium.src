// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// MediaStorageUtil provides information about storage devices attached
// to the computer.

#ifndef COMPONENTS_STORAGE_MONITOR_MEDIA_STORAGE_UTIL_H_
#define COMPONENTS_STORAGE_MONITOR_MEDIA_STORAGE_UTIL_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"

class StorageInfo;

class MediaStorageUtil {
 public:
  typedef std::set<std::string /*device id*/> DeviceIdSet;

  // Check if the file system at the passed mount point looks like a media
  // device using the existence of DCIM directory.
  // Returns true if it looks like a media device, otherwise returns false.
  // Mac OS X behaves similarly, but this is not the only heuristic it uses.
  // TODO(vandebo) Try to figure out how Mac OS X decides this, and rename
  // if additional OS X heuristic is implemented.
  static bool HasDcim(const base::FilePath& mount_point);

  // Returns true if we will be able to create a filesystem for this device.
  static bool CanCreateFileSystem(const std::string& device_id,
                                  const base::FilePath& path);

  // Removes disconnected devices from |devices| and then calls |done|.
  static void FilterAttachedDevices(DeviceIdSet* devices,
                                    const base::Closure& done);

  // Given |path|, fill in |device_info|, and |relative_path|
  // (from the root of the device).
  static bool GetDeviceInfoFromPath(const base::FilePath& path,
                                    StorageInfo* device_info,
                                    base::FilePath* relative_path);

  // Get a base::FilePath for the given |device_id|.  If the device isn't a mass
  // storage type, the base::FilePath will be empty.  This does not check that
  // the device is connected.
  static base::FilePath FindDevicePathById(const std::string& device_id);

  // Record device information histogram for the given |device_uuid| and
  // |device_label|. |mass_storage| indicates whether the current device is a
  // mass storage device, as defined by IsMassStorageDevice().
  static void RecordDeviceInfoHistogram(bool mass_storage,
                                        const std::string& device_uuid,
                                        const base::string16& device_label);

  // Returns true if the |id| is both a removable device and also
  // currently attached.
  static bool IsRemovableStorageAttached(const std::string& id);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(MediaStorageUtil);
};

#endif  // COMPONENTS_STORAGE_MONITOR_MEDIA_STORAGE_UTIL_H_
