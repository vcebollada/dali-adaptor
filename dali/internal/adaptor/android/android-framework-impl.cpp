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

// CLASS HEADER
#include <dali/internal/adaptor/android/android-framework-impl.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{
AndroidFramework* gAndroidFramework = nullptr; // raw specific pointer to allow AndroidFramework::Get
}

Dali::Integration::AndroidFramework& AndroidFramework::New()
{
  Dali::Integration::AndroidFramework* androidFramework = new Dali::Integration::AndroidFramework;
  AndroidFramework* impl = new AndroidFramework( *androidFramework );
  androidFramework->mImpl = impl;
  return *androidFramework;
}

void AndroidFramework::SetNativeApplication( android_app* application )
{
  mNativeApplication = application;
}

android_app* AndroidFramework::GetNativeApplication()
{
  return mNativeApplication;
}

void AndroidFramework::SetJVM( JavaVM* jvm )
{
  mJVM = jvm;
}

JavaVM* AndroidFramework::GetJVM()
{
  return mJVM;
}

void AndroidFramework::SetApplicationAssets( AAssetManager* assets )
{
  mAssets = assets;
}

AAssetManager* AndroidFramework::GetApplicationAssets()
{
  return mAssets;
}

void AndroidFramework::SetApplicationConfiguration( AConfiguration* configuration )
{
  mConfiguration = configuration;
}

AConfiguration* AndroidFramework::GetApplicationConfiguration()
{
  return mConfiguration;
}

void AndroidFramework::SetApplicationWindow( ANativeWindow* window )
{
  mWindow = window;
}

ANativeWindow* AndroidFramework::GetApplicationWindow()
{
  return mWindow;
}

void AndroidFramework::OnTerminate()
{
  mFramework->AppStatusHandler( APP_DESTROYED, nullptr );
}

void AndroidFramework::OnPause()
{
  mFramework->AppStatusHandler( APP_PAUSE, nullptr );
}

void AndroidFramework::OnResume()
{
  mFramework->AppStatusHandler( APP_RESUME, nullptr );
}

void AndroidFramework::OnWindowCreated( ANativeWindow* window )
{
  mFramework->AppStatusHandler( APP_WINDOW_CREATED, window );
}

void AndroidFramework::OnWindowDestroyed( ANativeWindow* window )
{
  mFramework->AppStatusHandler( APP_WINDOW_DESTROYED, window );
}

Dali::Integration::AndroidFramework& AndroidFramework::Get()
{
  DALI_ASSERT_ALWAYS( gAndroidFramework != nullptr && "AndroidFramework not instantiated" );

  return gAndroidFramework->mAndroidFramework;
}

AndroidFramework::AndroidFramework( Dali::Integration::AndroidFramework& androidFramework )
 : mAndroidFramework( androidFramework ), 
   mFramework( nullptr ),
   mNativeApplication( nullptr ),
   mWindow( nullptr ),
   mAssets( nullptr ),
   mConfiguration( nullptr ),
   mJVM( nullptr )
{
  DALI_ASSERT_ALWAYS( gAndroidFramework == nullptr && "Cannot create more than one AndroidFramework per thread" );

  gAndroidFramework = this;
}

AndroidFramework::~AndroidFramework()
{
  gAndroidFramework = nullptr;
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
