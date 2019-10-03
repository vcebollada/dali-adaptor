#ifndef DALI_INTEGRATION_ANDROID_FRAMEWORK_H
#define DALI_INTEGRATION_ANDROID_FRAMEWORK_H

/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <jni.h>
#include <dali/dali.h>

namespace Dali
{

namespace Adaptor
{

class DALI_ADAPTOR_API AndroidFramework
{
public:
  /**
   * @brief Create a new Android framework.
   *
   * @return a reference to the Android framework
   */
  static AndroidFramework& New();

  /**
   * @brief Sets the native application glue struct
   * @param[in] application A pointer to the application glue struct
   */
  void SetNativeApplication( android_app* application );

  /**
   * @brief Gets the native application glue struct
   * @return the native application glue struct
   */
  android_app* GetNativeApplication();

  /**
   * @brief Sets the JVM
   * @param[in] jvm A pointer to the JVM
   */
  void SetJVM( JavaVM* jvm );

  /**
   *  Sets the application assets manager.
   *  @param[in] assets A pointer to assets manager
   */
  void SetApplicationAssets( AAssetManager* assets );

  /**
   * @brief Gets the application assets manager.
   * @return The application assets manager
   */
  AAssetManager* GetApplicationAssets();

  /**
   * @brief Sets the Android application configuration
   * @param[in] configuration A pointer to Android application configuration
   */
  void SetApplicationConfiguration( AConfiguration* configuration );

  /**
   * @brief Gets the Android application configuration
   * @return A Android application configuration
   */
  AConfiguration* GetApplicationConfiguration();

  /**
   * @brief Gets the application native window
   * @return A native window
   */
  ANativeWindow* GetApplicationWindow();

  /**
   * Invoked when the application is to be initialised.
   */
  void OnInit();

  /**
   * Invoked when the application is to be terminated.
   */
  void OnTerminate();

  /**
   * Invoked when the application is to be paused.
   */
  void OnPause();

  /**
   * Invoked when the application is to be resumed.
   */
  void OnResume();

  /**
   * Invoked when the application native window is to be replaced.
   */
  void OnReplaceWindow( ANativeWindow* window );

  /**
   * Invoked when the application native window is to be deleted.
   */
  void OnDeleteWindow( ANativeWindow* window );

  /**
   * @brief Returns a reference to the instance of the Android framework used by the current thread.
   *
   * @return A reference to the framework.
   * @note This is only valid in the main thread.
   */
  static AndroidFramework& Get();

private:

  // Undefined
  AndroidFramework(const AndroidFramework&);
  AndroidFramework& operator=(AndroidFramework&);

private:

  /**
   * @brief Create an uninitialized AndroidFramework.
   */
  AndroidFramework();

  Internal::Adaptor::AndroidFramework* mImpl; ///< Implementation object
  friend class Internal::Adaptor::AndroidFramework;
}

#endif // DALI_INTEGRATION_ANDROID_FRAMEWORK_H

