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
#include <dali/integration-api/android/android-framework.h>

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <dali/internal/adaptor/android/android-framework-impl.h>

namespace Dali
{

namespace Integration
{

AndroidFramework& AndroidFramework::New()
{
  return Internal::Adaptor::AndroidFramework::New();
}

void AndroidFramework::SetNativeApplication( android_app* application )
{
  mImpl->SetNativeApplication( application );
}

android_app* AndroidFramework::GetNativeApplication()
{
  return mImpl->GetNativeApplication();
}

void AndroidFramework::SetJVM( JavaVM* jvm )
{
  mImpl->SetJVM( jvm );
}

JavaVM* AndroidFramework::GetJVM()
{
  return mImpl->GetJVM();
}

void AndroidFramework::SetApplicationAssets( AAssetManager* assets )
{
  mImpl->SetApplicationAssets( assets );
}

AAssetManager* AndroidFramework::GetApplicationAssets()
{
  return mImpl->GetApplicationAssets();
}

void AndroidFramework::SetApplicationConfiguration( AConfiguration* configuration )
{
  mImpl->SetApplicationConfiguration( configuration );
}

AConfiguration* AndroidFramework::GetApplicationConfiguration()
{
  return mImpl->GetApplicationConfiguration();
}

void AndroidFramework::SetApplicationWindow( ANativeWindow* window )
{
  mImpl->SetApplicationWindow( window );
}

ANativeWindow* AndroidFramework::GetApplicationWindow()
{
  return mImpl->GetApplicationWindow();
}

void AndroidFramework::OnTerminate()
{
  mImpl->OnTerminate();
}

void AndroidFramework::OnPause()
{
  mImpl->OnPause();
}

void AndroidFramework::OnResume()
{
  mImpl->OnResume();
}

void AndroidFramework::OnWindowCreated( ANativeWindow* window )
{
  mImpl->OnWindowCreated( window );
}

void AndroidFramework::OnWindowDestroyed( ANativeWindow* window )
{
  mImpl->OnWindowDestroyed( window );
}

AndroidFramework::~AndroidFramework()
{
  delete mImpl;
  mImpl = nullptr;
}

AndroidFramework& AndroidFramework::Get()
{
  return Internal::Adaptor::AndroidFramework::Get();
}

AndroidFramework::AndroidFramework()
: mImpl( nullptr )
{
}

} // namespace Integration

} // namespace Dali
