/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
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
#include <dali/internal/adaptor/common/framework.h>

// EXTERNAL INCLUDES
#include <unistd.h>
#include <queue>
#include <unordered_set>

#include <jni.h>
#include <sys/types.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/configuration.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

#include <dali/integration-api/debug.h>
#include <dali/public-api/events/touch-point.h>
#include <dali/public-api/events/key-event.h>
#include <dali/public-api/adaptor-framework/application.h>
#include <dali/devel-api/adaptor-framework/application-devel.h>

// INTERNAL INCLUDES
#include <dali/internal/adaptor/common/application-impl.h>
#include <dali/internal/system/common/callback-manager.h>

using namespace Dali;

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

namespace
{

/// Application Status Enum
enum
{
  APP_WINDOW_CREATED = 0,
  APP_WINDOW_DESTROYED,
  APP_PAUSE,
  APP_RESUME,
  APP_RESET,
  APP_LANGUAGE_CHANGE,
  APP_DESTROYED,
};

struct ApplicationContext
{
  JNIEnv* jniEnv;
  AAssetManager* assetManager;
  AConfiguration* config;
  ANativeWindow* window;
  Framework* framework;
};

struct ApplicationContext applicationContext;

} // Unnamed namespace

/**
 * Impl to hide android data members
 */
struct Framework::Impl
{
  // Constructor

  Impl(void* data)
  : mAbortCallBack( nullptr ),
    mCallbackManager( CallbackManager::New() ),
    mLanguage( "NOT_SUPPORTED" ),
    mRegion( "NOT_SUPPORTED" ),
    mFinishRequested( false )
  {
    applicationContext.framework = static_cast<Framework*>( data );
  }

  ~Impl()
  {
    applicationContext.framework = nullptr;

    delete mAbortCallBack;
    mAbortCallBack = nullptr;

    // we're quiting the main loop so
    // mCallbackManager->RemoveAllCallBacks() does not need to be called
    // to delete our abort handler
    delete mCallbackManager;
    mCallbackManager = nullptr;
  }

  std::string GetLanguage() const
  {
    return mLanguage;
  }

  std::string GetRegion() const
  {
    return mRegion;
  }

  CallbackBase* mAbortCallBack;
  CallbackManager* mCallbackManager;
  std::string mLanguage;
  std::string mRegion;
  bool mFinishRequested;
};

Framework::Framework( Framework::Observer& observer, int *argc, char ***argv, Type type )
: mObserver( observer ),
  mInitialised( false ),
  mPaused( false ),
  mRunning( false ),
  mArgc( argc ),
  mArgv( argv ),
  mBundleName( "" ),
  mBundleId( "" ),
  mAbortHandler( MakeCallback( this, &Framework::AbortCallback ) ),
  mImpl( NULL )
{
  InitThreads();
  mImpl = new Impl( this );
}

Framework::~Framework()
{
  if( mRunning )
  {
    Quit();
  }

  delete mImpl;
  mImpl = nullptr;
}

void Framework::Run()
{
  mRunning = true;
}

unsigned int Framework::AddIdle( int timeout, void* data, bool ( *callback )( void *data ) )
{
  JNIEnv* env = applicationContext.jniEnv;
  jclass clazz = env->FindClass( "com/sec/daliview/DaliView" );
  if ( !clazz )
    return -1;

  jmethodID addIdle = env->GetStaticMethodID( clazz, "addIdle", "(JJJ)I" );
  if (!addIdle)
    return -1;

  jint id = env->CallStaticIntMethod( clazz, addIdle, reinterpret_cast<jlong>( callback ), reinterpret_cast<jlong>( data ), static_cast<jlong>( timeout ) );
  return static_cast<unsigned int>( id );
}

void Framework::RemoveIdle( unsigned int id )
{
  JNIEnv* env = applicationContext.jniEnv;
  jclass clazz = env->FindClass( "com/sec/daliview/DaliView" );
  if( !clazz )
    return;

  jmethodID removeIdle = env->GetStaticMethodID( clazz, "removeIdle", "(I)V" );
  if( removeIdle )
    env->CallStaticVoidMethod( clazz, removeIdle, static_cast<jint>( id ) );
}

void Framework::Quit()
{
}

bool Framework::IsMainLoopRunning()
{
  return mRunning;
}

void Framework::AddAbortCallback( CallbackBase* callback )
{
  mImpl->mAbortCallBack = callback;
}

std::string Framework::GetBundleName() const
{
  return mBundleName;
}

void Framework::SetBundleName(const std::string& name)
{
  mBundleName = name;
}

std::string Framework::GetBundleId() const
{
  return mBundleId;
}

std::string Framework::GetResourcePath()
{
  return APPLICATION_RESOURCE_PATH;
}

std::string Framework::GetDataPath()
{
  return "";
}

void Framework::SetApplicationContext(void* context)
{
  memset( &applicationContext, 0, sizeof( ApplicationContext ) );
  applicationContext.jniEnv = static_cast<JNIEnv*>( context );
}

void* Framework::GetApplicationContext()
{
  DALI_ASSERT_ALWAYS( applicationContext.jniEnv && "Failed to get Android context" );
  return applicationContext.jniEnv;
}

Framework* Framework::GetApplicationFramework()
{
  DALI_ASSERT_ALWAYS( applicationContext.framework && "Failed to get Android framework" );
  return applicationContext.framework;
}

void* Framework::GetApplicationAssets()
{
  DALI_ASSERT_ALWAYS( applicationContext.assetManager && "Failed to get Android Asset manager" );
  return applicationContext.assetManager;
}

void Framework::SetApplicationAssets(void* assets)
{
  applicationContext.assetManager = static_cast<AAssetManager*>( assets );
}

void* Framework::GetApplicationConfiguration()
{
  DALI_ASSERT_ALWAYS( applicationContext.config && "Failed to get Android configuration" );
  return applicationContext.config;
}

void Framework::SetApplicationConfiguration(void* configuration)
{
  applicationContext.config = static_cast<AConfiguration*>( configuration );
}

void* Framework::GetApplicationWindow()
{
  return applicationContext.window;
}

void Framework::SetBundleId(const std::string& id)
{
  mBundleId = id;
}

void Framework::AbortCallback( )
{
  // if an abort call back has been installed run it.
  if (mImpl->mAbortCallBack)
  {
    CallbackBase::Execute( *mImpl->mAbortCallBack );
  }
  else
  {
    Quit();
  }
}

bool Framework::AppStatusHandler(int type, void* data)
{
  switch (type)
  {
    case APP_WINDOW_CREATED:
      applicationContext.window = static_cast< ANativeWindow* >( data );
      if( !mInitialised )
      {
        mObserver.OnInit();
        mInitialised = true;
      }

      mObserver.OnReplaceSurface( data );
      break;

    case APP_WINDOW_DESTROYED:
      mObserver.OnReplaceSurface( data );
      break;

    case APP_RESET:
      mObserver.OnReset();
      break;

    case APP_RESUME:
      mObserver.OnResume();
      break;

    case APP_PAUSE:
      mObserver.OnPause();
      break;

    case APP_LANGUAGE_CHANGE:
      mObserver.OnLanguageChanged();
      break;

    case APP_DESTROYED:
      mObserver.OnTerminate();
      mInitialised = false;
      break;

    default:
      break;
  }

  return true;
}

void Framework::InitThreads()
{
}

std::string Framework::GetLanguage() const
{
  return mImpl->GetLanguage();
}

std::string Framework::GetRegion() const
{
  return mImpl->GetRegion();
}

} // namespace Adaptor

} // namespace Internal

} // namespace Dali
