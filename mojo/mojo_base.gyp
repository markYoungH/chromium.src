# Copyright 2014 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# Essential components (and their tests) that are needed to build
# Chrome should be here.  Other components that are useful only in
# Mojo land like mojo_shell should be in mojo.gyp.
{
  'includes': [
    'mojo_variables.gypi',
  ],
  'targets': [
    {
      'target_name': 'mojo_base',
      'type': 'none',
      'dependencies': [
        # NOTE: If adding a new dependency here, please consider whether it
        # should also be added to the list of Mojo-related dependencies of
        # build/all.gyp:All on iOS, as All cannot depend on the mojo_base
        # target on iOS due to the presence of the js targets, which cause v8
        # to be built.
        'mojo_common_lib',
        'mojo_common_unittests',
        'mojo_message_generator',
      ],
      'conditions': [
        ['OS == "android"', {
          'dependencies': [
            'libmojo_system_java',
            'mojo_system_java',
            'mojo_test_apk',
            'public/mojo_public.gyp:mojo_bindings_java',
            'public/mojo_public.gyp:mojo_public_java',
          ],
        }],
      ]
    },
    {
      'target_name': 'mojo_none',
      'type': 'none',
    },
    {
      # GN version: //mojo/common
      'target_name': 'mojo_common_lib',
      'type': '<(component)',
      'defines': [
        'MOJO_COMMON_IMPLEMENTATION',
      ],
      'dependencies': [
        '../base/base.gyp:base',
        '../url/url.gyp:url_lib',
        '../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '<(mojo_system_for_component)',
      ],
      'export_dependent_settings': [
        '../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
      ],
      'sources': [
        'common/common_type_converters.cc',
        'common/common_type_converters.h',
        'common/data_pipe_utils.cc',
        'common/data_pipe_utils.h',
        'common/handle_watcher.cc',
        'common/handle_watcher.h',
        'common/message_pump_mojo.cc',
        'common/message_pump_mojo.h',
        'common/message_pump_mojo_handler.h',
        'common/time_helper.cc',
        'common/time_helper.h',
      ],
    },
    {
      # GN version: //mojo/common:mojo_common_unittests
      'target_name': 'mojo_common_unittests',
      'type': 'executable',
      'dependencies': [
        '../base/base.gyp:base',
        '../base/base.gyp:base_message_loop_tests',
        '../testing/gtest.gyp:gtest',
        '../url/url.gyp:url_lib',
        'edk/mojo_edk.gyp:mojo_common_test_support',
        'edk/mojo_edk.gyp:mojo_run_all_unittests',
        'mojo_common_lib',
        'mojo_environment_chromium',
        'public/mojo_public.gyp:mojo_cpp_bindings',
        'public/mojo_public.gyp:mojo_public_test_utils',
      ],
      'sources': [
        'common/common_type_converters_unittest.cc',
        'common/handle_watcher_unittest.cc',
        'common/message_pump_mojo_unittest.cc',
        'edk/test/multiprocess_test_helper_unittest.cc',
      ],
      'conditions': [
        ['OS=="ios"', {
          'sources!': [
            'edk/test/multiprocess_test_helper_unittest.cc',
          ],
        }],
      ],
    },
    {
      # GN version: //mojo/environment:chromium
      'target_name': 'mojo_environment_chromium',
      'type': 'static_library',
      'dependencies': [
        'mojo_environment_chromium_impl',
      ],
      'sources': [
        'environment/environment.cc',
        # TODO(vtl): This is kind of ugly. (See TODO in logging.h.)
        "public/cpp/environment/logging.h",
        "public/cpp/environment/lib/logging.cc",
      ],
      'include_dirs': [
        '..',
      ],
      'export_dependent_settings': [
        'mojo_environment_chromium_impl',
      ],
    },
    {
      # GN version: //mojo/environment:chromium_impl
      'target_name': 'mojo_environment_chromium_impl',
      'type': '<(component)',
      'defines': [
        'MOJO_ENVIRONMENT_IMPL_IMPLEMENTATION',
      ],
      'dependencies': [
        '../base/base.gyp:base',
        '../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        'mojo_common_lib',
        '<(mojo_system_for_component)',
      ],
      'sources': [
        'environment/default_async_waiter_impl.cc',
        'environment/default_async_waiter_impl.h',
        'environment/default_logger_impl.cc',
        'environment/default_logger_impl.h',
      ],
      'include_dirs': [
        '..',
      ],
    },
    {
     # GN version: //mojo/application
     'target_name': 'mojo_application_chromium',
     'type': 'static_library',
     'sources': [
       'application/application_runner_chromium.cc',
       'application/application_runner_chromium.h',
      ],
      'dependencies': [
        'mojo_common_lib',
        'mojo_environment_chromium',
        'public/mojo_public.gyp:mojo_application_base',
       ],
      'export_dependent_settings': [
        'public/mojo_public.gyp:mojo_application_base',
       ],
    },
    {
      # GN version: //mojo/bindings/js
      'target_name': 'mojo_js_bindings_lib',
      'type': 'static_library',
      'dependencies': [
        '../base/base.gyp:base',
        '../gin/gin.gyp:gin',
        '../v8/tools/gyp/v8.gyp:v8',
        'mojo_common_lib',
      ],
      'export_dependent_settings': [
        '../base/base.gyp:base',
        '../gin/gin.gyp:gin',
        'mojo_common_lib',
      ],
      'sources': [
        # Sources list duplicated in GN build.
        'bindings/js/core.cc',
        'bindings/js/core.h',
        'bindings/js/drain_data.cc',
        'bindings/js/drain_data.h',
        'bindings/js/handle.cc',
        'bindings/js/handle.h',
        'bindings/js/handle_close_observer.h',
        'bindings/js/support.cc',
        'bindings/js/support.h',
        'bindings/js/waiting_callback.cc',
        'bindings/js/waiting_callback.h',
      ],
    },
    {
      # GN version: //mojo/tools:message_generator
      'target_name': 'mojo_message_generator',
      'type': 'executable',
      'dependencies': [
        '../base/base.gyp:base',
        '../testing/gtest.gyp:gtest',
        'edk/mojo_edk.gyp:mojo_system_impl',
        'mojo_common_lib',
        'mojo_environment_chromium',
        'public/mojo_public.gyp:mojo_cpp_bindings',
      ],
      'sources': [
        'tools/message_generator.cc',
      ],
    },
  ],
  'conditions': [
    ['OS=="android"', {
      'targets': [
        {
          'target_name': 'mojo_jni_headers',
          'type': 'none',
          'dependencies': [
            'mojo_java_set_jni_headers',
          ],
          'sources': [
            'android/javatests/src/org/chromium/mojo/MojoTestCase.java',
            'android/javatests/src/org/chromium/mojo/bindings/ValidationTestUtil.java',
            'android/system/src/org/chromium/mojo/system/impl/CoreImpl.java',
            'services/native_viewport/android/src/org/chromium/mojo/PlatformViewportAndroid.java',
            'shell/android/apk/src/org/chromium/mojo_shell_apk/MojoMain.java',
          ],
          'variables': {
            'jni_gen_package': 'mojo',
         },
          'includes': [ '../build/jni_generator.gypi' ],
        },
        {
          'target_name': 'mojo_java_set_jni_headers',
          'type': 'none',
          'variables': {
            'jni_gen_package': 'mojo',
            'input_java_class': 'java/util/HashSet.class',
          },
          'includes': [ '../build/jar_file_jni_generator.gypi' ],
        },
        {
          'target_name': 'mojo_system_java',
          'type': 'none',
          'dependencies': [
            '../base/base.gyp:base_java',
            'public/mojo_public.gyp:mojo_public_java',
          ],
          'variables': {
            'java_in_dir': '<(DEPTH)/mojo/android/system',
          },
          'includes': [ '../build/java.gypi' ],
        },
        {
          'target_name': 'libmojo_system_java',
          'type': 'static_library',
          'dependencies': [
            '../base/base.gyp:base',
            '../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
            'edk/mojo_edk.gyp:mojo_system_impl',
            'mojo_common_lib',
            'mojo_environment_chromium',
            'mojo_jni_headers',
          ],
          'sources': [
            'android/system/core_impl.cc',
            'android/system/core_impl.h',
          ],
        },
        {
          'target_name': 'libmojo_java_unittest',
          'type': 'shared_library',
          'dependencies': [
            '../base/base.gyp:base',
            '../base/base.gyp:test_support_base',
            'libmojo_system_java',
            'mojo_jni_headers',
            'public/mojo_public.gyp:mojo_public_bindings_test_utils',
          ],
          'defines': [
            'UNIT_TEST'  # As exported from testing/gtest.gyp:gtest.
          ],
          'sources': [
            'android/javatests/mojo_test_case.cc',
            'android/javatests/mojo_test_case.h',
            'android/javatests/init_library.cc',
            'android/javatests/validation_test_util.cc',
            'android/javatests/validation_test_util.h',
          ],
        },
        {
          'target_name': 'mojo_test_apk',
          'type': 'none',
          'dependencies': [
            '../base/base.gyp:base_java_test_support',
            'public/mojo_public.gyp:mojo_bindings_java',
            'mojo_system_java',
            'public/mojo_public.gyp:mojo_public_test_interfaces',
          ],
          'variables': {
            'apk_name': 'MojoTest',
            'java_in_dir': '<(DEPTH)/mojo/android/javatests',
            'resource_dir': '<(DEPTH)/mojo/android/javatests/apk',
            'native_lib_target': 'libmojo_java_unittest',
            'is_test_apk': 1,
            # Given that this apk tests itself, it needs to bring emma with it
            # when instrumented.
            'conditions': [
              ['emma_coverage != 0', {
                'emma_instrument': 1,
              }],
            ],
          },
          'includes': [ '../build/java_apk.gypi' ],
        },
      ]
    }],
  ]
}
