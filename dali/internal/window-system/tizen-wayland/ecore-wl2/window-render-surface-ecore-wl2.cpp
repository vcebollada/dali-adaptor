/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
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
#include <dali/internal/window-system/tizen-wayland/ecore-wl2/window-render-surface-ecore-wl2.h>

// EXTERNAL INCLUDES
#include <dali/integration-api/gl-abstraction.h>
#include <dali/integration-api/debug.h>

// INTERNAL INCLUDES
#include <dali/integration-api/wayland/wl-types.h>
#include <dali/internal/graphics/gles20/egl-implementation.h>
#include <dali/internal/adaptor/common/adaptor-impl.h>
#include <dali/integration-api/trigger-event-factory-interface.h>

namespace Dali
{

#if defined(DEBUG_ENABLED)
extern Debug::Filter* gRenderSurfaceLogFilter;
#endif

namespace ECore
{

namespace
{

const int MINIMUM_DIMENSION_CHANGE( 1 ); ///< Minimum change for window to be considered to have moved

} // unnamed namespace

WindowRenderSurface::WindowRenderSurface( Dali::PositionSize positionSize,
                                          Any surface,
                                          const std::string& name,
                                          bool isTransparent)
: EcoreWlRenderSurface( positionSize, surface, name, isTransparent ),
  mWlWindow( NULL ),
  mEcore_Wl2_display( NULL ),
  mWlSurface( NULL ),
  mEglWindow( NULL ),
  mThreadSynchronization( NULL ),
  mRotationTrigger( NULL ),
  mRotationAngle( 0 ),
  mScreenRotationAngle( 0 ),
  mRotationSupported( false ),
  mRotationFinished( true ),
  mScreenRotationFinished( true ),
  mResizeFinished( true )
{
  DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "Creating Window\n" );
  Init( surface );
}

WindowRenderSurface::~WindowRenderSurface()
{
  if( mEglWindow != NULL )
  {
    DALI_LOG_RELEASE_INFO("Egl window is destroyed by wl2 WindowRenderSurface Destructor\n");
    wl_egl_window_destroy(mEglWindow);
    mEglWindow = NULL;
  }

  if( mOwnSurface )
  {
    ecore_wl2_window_free( mWlWindow );
  }

  if( mRotationTrigger )
  {
    delete mRotationTrigger;
  }

}

Ecore_Wl2_Window* WindowRenderSurface::GetDrawable()
{
  // already an e-core type
  return mWlWindow;
}

Any WindowRenderSurface::GetSurface()
{
  // already an e-core type
  return Any( mWlWindow );
}

Ecore_Wl2_Window* WindowRenderSurface::GetWlWindow()
{
  return mWlWindow;
}

void WindowRenderSurface::RequestRotation( int angle, int width, int height )
{
  if( !mRotationSupported )
  {
    DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "WindowRenderSurface::Rotate: Rotation is not supported!\n" );
    return;
  }

  if( !mRotationTrigger )
  {
    TriggerEventFactoryInterface& triggerFactory = Internal::Adaptor::Adaptor::GetImplementation( Adaptor::Get() ).GetTriggerEventFactoryInterface();
    mRotationTrigger = triggerFactory.CreateTriggerEvent( MakeCallback( this, &WindowRenderSurface::ProcessRotationRequest ), TriggerEventInterface::KEEP_ALIVE_AFTER_TRIGGER );
  }

  mPositionSize.width = width;
  mPositionSize.height = height;

  mRotationAngle = angle;
  mRotationFinished = false;

  ecore_wl2_window_rotation_set( mWlWindow, mRotationAngle );

  DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "WindowRenderSurface::Rotate: angle = %d screen rotation = %d\n", mRotationAngle, mScreenRotationAngle );
}

void WindowRenderSurface::OutputTransformed()
{
  int transform;

  if( ecore_wl2_window_ignore_output_transform_get( mWlWindow ) )
  {
    transform = 0;
  }
  else
  {
    transform = ecore_wl2_output_transform_get( ecore_wl2_window_output_find( mWlWindow ) );
  }

  mScreenRotationAngle = transform * 90;
  mScreenRotationFinished = false;

  DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "WindowRenderSurface::OutputTransformed: angle = %d screen rotation = %d\n", mRotationAngle, mScreenRotationAngle );
}

void WindowRenderSurface::SetTransparency( bool transparent )
{
  ecore_wl2_window_alpha_set( mWlWindow, transparent );
}

void WindowRenderSurface::InitializeEgl( EglInterface& eglIf )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( eglIf );

  eglImpl.ChooseConfig(true, mColorDepth);
}

void WindowRenderSurface::CreateEglSurface( EglInterface& eglIf )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( eglIf );

  // create the EGL window
  if( mScreenRotationAngle == 0 || mScreenRotationAngle == 180 )
  {
    mEglWindow = wl_egl_window_create( mWlSurface, mPositionSize.width, mPositionSize.height );
  }
  else
  {
    mEglWindow = wl_egl_window_create( mWlSurface, mPositionSize.height, mPositionSize.width );
  }

  EGLNativeWindowType windowType( mEglWindow );
  eglImpl.CreateSurfaceWindow( windowType, mColorDepth );

  // Check capability
  wl_egl_window_capability capability = static_cast< wl_egl_window_capability >( wl_egl_window_get_capabilities( mEglWindow ) );
  if( capability == WL_EGL_WINDOW_CAPABILITY_ROTATION_SUPPORTED )
  {
    DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "WindowRenderSurface::CreateEglSurface: capability = %d\n", capability );
    mRotationSupported = true;
  }

  DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "WindowRenderSurface::CreateEglSurface: w = %d h = %d angle = %d screen rotation = %d\n", mPositionSize.width, mPositionSize.height, mRotationAngle, mScreenRotationAngle );
}

void WindowRenderSurface::DestroyEglSurface( EglInterface& eglIf )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( eglIf );
  eglImpl.DestroySurface();

  if( mEglWindow != NULL )
  {
    DALI_LOG_RELEASE_INFO("Egl window is destroyed by wl2 WindowRenderSurface::DestroyEglSurface()\n");
    wl_egl_window_destroy(mEglWindow);
    mEglWindow = NULL;
  }
}

bool WindowRenderSurface::ReplaceEGLSurface( EglInterface& egl )
{
  DALI_LOG_TRACE_METHOD( gRenderSurfaceLogFilter );

  if( mEglWindow != NULL )
  {
    DALI_LOG_RELEASE_INFO("Egl window is destroyed by wl2 WindowRenderSurface::ReplaceEGLSurface()\n");
    wl_egl_window_destroy(mEglWindow);
    mEglWindow = NULL;
  }

  if( mScreenRotationAngle == 0 || mScreenRotationAngle == 180 )
  {
    mEglWindow = wl_egl_window_create( mWlSurface, mPositionSize.width, mPositionSize.height );
  }
  else
  {
    mEglWindow = wl_egl_window_create( mWlSurface, mPositionSize.height, mPositionSize.width );
  }

  // Set screen rotation
  mScreenRotationFinished = false;

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( egl );
  EGLNativeWindowType windowType( mEglWindow );
  return eglImpl.ReplaceSurfaceWindow( windowType );
}

void WindowRenderSurface::MoveResize( Dali::PositionSize positionSize )
{
  bool needToMove = false;
  bool needToResize = false;

  // check moving
  if( (fabs(positionSize.x - mPositionSize.x) > MINIMUM_DIMENSION_CHANGE) ||
      (fabs(positionSize.y - mPositionSize.y) > MINIMUM_DIMENSION_CHANGE) )
  {
    needToMove = true;
  }

  // check resizing
  if( (fabs(positionSize.width - mPositionSize.width) > MINIMUM_DIMENSION_CHANGE) ||
      (fabs(positionSize.height - mPositionSize.height) > MINIMUM_DIMENSION_CHANGE) )
  {
    needToResize = true;
  }

  if( needToMove || needToResize )
  {
    ecore_wl2_window_geometry_set( mWlWindow, positionSize.x, positionSize.y, positionSize.width, positionSize.height );
    if (needToResize)
    {
      mResizeFinished = false;
    }
  }

  mPositionSize = positionSize;

  DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "WindowRenderSurface::MoveResize: %d, %d, %d, %d\n", mPositionSize.x, mPositionSize.y, mPositionSize.width, mPositionSize.height );
}

void WindowRenderSurface::Map()
{
  ecore_wl2_window_show(mWlWindow);
}

void WindowRenderSurface::StartRender()
{
}

bool WindowRenderSurface::PreRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction, bool resizingSurface )
{
  if( resizingSurface )
  {
#ifdef OVER_TIZEN_VERSION_4
    // Window rotate or screen rotate
    if( !mRotationFinished || !mScreenRotationFinished )
    {
      wl_egl_window_rotation rotation;
      wl_output_transform bufferTransform;
      int totalAngle = (mRotationAngle + mScreenRotationAngle) % 360;

      switch( totalAngle )
      {
        case 0:
        {
          rotation = ROTATION_0;
          bufferTransform = WL_OUTPUT_TRANSFORM_NORMAL;
          break;
        }
        case 90:
        {
          rotation = ROTATION_270;
          bufferTransform = WL_OUTPUT_TRANSFORM_90;
          break;
        }
        case 180:
        {
          rotation = ROTATION_180;
          bufferTransform = WL_OUTPUT_TRANSFORM_180;
          break;
        }
        case 270:
        {
          rotation = ROTATION_90;
          bufferTransform = WL_OUTPUT_TRANSFORM_270;
          break;
        }
        default:
        {
          rotation = ROTATION_0;
          bufferTransform = WL_OUTPUT_TRANSFORM_NORMAL;
          break;
        }
      }

      wl_egl_window_set_rotation( mEglWindow, rotation );

      wl_egl_window_set_buffer_transform( mEglWindow, bufferTransform );

      // Reset only screen rotation flag
      mScreenRotationFinished = true;

      DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "WindowRenderSurface::PreRender: Set rotation [%d] [%d]\n", mRotationAngle, mScreenRotationAngle );
    }

    // Only window rotate
    if( !mRotationFinished )
    {
      wl_output_transform windowTransform;

      switch( mRotationAngle )
      {
        case 0:
        {
          windowTransform = WL_OUTPUT_TRANSFORM_NORMAL;
          break;
        }
        case 90:
        {
          windowTransform = WL_OUTPUT_TRANSFORM_90;
          break;
        }
        case 180:
        {
          windowTransform = WL_OUTPUT_TRANSFORM_180;
          break;
        }
        case 270:
        {
          windowTransform = WL_OUTPUT_TRANSFORM_270;
          break;
        }
        default:
        {
          windowTransform = WL_OUTPUT_TRANSFORM_NORMAL;
          break;
        }
      }

      wl_egl_window_set_window_transform( mEglWindow, windowTransform );
    }
#endif

    // Resize case
    if( !mResizeFinished )
    {
      wl_egl_window_resize( mEglWindow, mPositionSize.width, mPositionSize.height, mPositionSize.x, mPositionSize.y );
      mResizeFinished = true;

      DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "WindowRenderSurface::PreRender: Set resize\n" );
    }
  }

  return true;
}

void WindowRenderSurface::PostRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction, DisplayConnection* displayConnection, bool replacingSurface, bool resizingSurface )
{
  if( resizingSurface )
  {
    if( !mRotationFinished )
    {
      DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "WindowRenderSurface::PostRender: Trigger rotation event\n" );

      mRotationTrigger->Trigger();

      if( mThreadSynchronization )
      {
        // Wait until the event-thread complete the rotation event processing
        mThreadSynchronization->PostRenderWaitForCompletion();
      }
    }
  }

  Internal::Adaptor::EglImplementation& eglImpl = static_cast<Internal::Adaptor::EglImplementation&>( egl );
  eglImpl.SwapBuffers();

  if( mRenderNotification )
  {
    mRenderNotification->Trigger();
  }
}

void WindowRenderSurface::StopRender()
{
}

void WindowRenderSurface::SetViewMode( ViewMode viewMode )
{
  //FIXME
}

void WindowRenderSurface::CreateWlRenderable()
{
  // TODO ::
  // use default display name as getenv("WAYLAND_DISPLAY")
  // If specific name is needed,
  // the name will be changed and should change the code of using ecore_wl2_connected_display_get()
  // Because default name is used in ecore_wl2_connected_display_get()'s use case.
  if ( mEcore_Wl2_display == NULL )
  {
      mEcore_Wl2_display = ecore_wl2_display_connect(NULL);
  }
  if ( mEcore_Wl2_display == NULL )
  {
      DALI_LOG_ERROR("Fail to connect ecore_wl2_display");
      return;
  }

   // if width or height are zero, go full screen.
  if ( (mPositionSize.width == 0) || (mPositionSize.height == 0) )
  {
    // Default window size == screen size
    mPositionSize.x = 0;
    mPositionSize.y = 0;

    ecore_wl2_display_screen_size_get( mEcore_Wl2_display, &mPositionSize.width, &mPositionSize.height );
  }

  mWlWindow = ecore_wl2_window_new( mEcore_Wl2_display, NULL, mPositionSize.x, mPositionSize.y, mPositionSize.width, mPositionSize.height );
  if ( mWlWindow == 0 )
  {
    DALI_ASSERT_ALWAYS(0 && "Failed to create Wayland window");
  }

  mWlSurface = ecore_wl2_window_surface_get( mWlWindow );

  if( mColorDepth == COLOR_DEPTH_32 )
  {
    ecore_wl2_window_alpha_set( mWlWindow, true );
  }
  else
  {
    ecore_wl2_window_alpha_set( mWlWindow, false );
  }

  ecore_wl2_window_type_set( mWlWindow, ECORE_WL2_WINDOW_TYPE_TOPLEVEL);

  // Get output transform
  if( !ecore_wl2_window_ignore_output_transform_get( mWlWindow ) )
  {
    Ecore_Wl2_Output* output = ecore_wl2_window_output_find( mWlWindow );

    int transform = ecore_wl2_output_transform_get( output );

    mScreenRotationAngle = transform * 90;
    mScreenRotationFinished = false;
  }
}

void WindowRenderSurface::UseExistingRenderable( unsigned int surfaceId )
{
  mWlWindow = AnyCast< Ecore_Wl2_Window* >( surfaceId );
}

void WindowRenderSurface::SetThreadSynchronization( ThreadSynchronizationInterface& threadSynchronization )
{
  DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "WindowRenderSurface::SetThreadSynchronization: called\n" );

  mThreadSynchronization = &threadSynchronization;
}

void WindowRenderSurface::ReleaseLock()
{
  // Nothing to do.
}

void WindowRenderSurface::ProcessRotationRequest()
{
  mRotationFinished = true;

  ecore_wl2_window_rotation_change_done_send( mWlWindow, mRotationAngle, mPositionSize.width, mPositionSize.height );

  DALI_LOG_INFO( gRenderSurfaceLogFilter, Debug::Verbose, "WindowRenderSurface::ProcessRotationRequest: Rotation Done\n" );

  if( mThreadSynchronization )
  {
    mThreadSynchronization->PostRenderComplete();
  }
}

} // namespace ECore

} // namespace Dali