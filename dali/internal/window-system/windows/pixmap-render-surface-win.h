#ifndef DALI_WIN_PIXMAP_RENDER_SURFACE_H
#define DALI_WIN_PIXMAP_RENDER_SURFACE_H

/*
 * Copyright (c) 2018 Samsung Electronics Co., Ltd.
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

// INTERNAL INCLUDES
#include <dali/integration-api/egl-interface.h>
#include <dali/internal/graphics/gles20/egl-implementation.h>
#include <dali/internal/window-system/common/pixmap-render-surface.h>
#include <dali/internal/window-system/windows/platform-implement-win.h>

// EXTERNAL INCLUDES
#include <dali/public-api/common/dali-common.h>
#include <dali/devel-api/threading/conditional-wait.h>

namespace Dali
{
namespace Internal
{
namespace Adaptor
{

/**
 * Windows Pixmap implementation of render surface.
 */
class PixmapRenderSurfaceEcoreWin : public PixmapRenderSurface
{
public:

  /**
    * Uses an Windows surface to render to.
    * @param [in] positionSize the position and size of the surface
    * @param [in] surface can be a Win32-window or Win32-pixmap (type must be unsigned int).
    * @param [in] name optional name of surface passed in
    * @param [in] isTransparent if it is true, surface has 32 bit color depth, otherwise, 24 bit
    */
  PixmapRenderSurfaceEcoreWin( Dali::PositionSize positionSize, Any surface, bool isTransparent = false );

  /**
   * @brief Destructor
   */
  virtual ~PixmapRenderSurfaceEcoreWin();

public: // from WindowRenderSurface

  /**
   * @copydoc Dali::Internal::Adaptor::PixmapRenderSurface::GetSurface()
   */
  virtual Any GetSurface() override;

  /**
   * @copydoc Dali::Internal::Adaptor::PixmapRenderSurface::SetRenderNotification()
   */
  virtual void SetRenderNotification( TriggerEventInterface* renderNotification ) override;

public: // from Dali::RenderSurface

  /**
   * @copydoc Dali::RenderSurface::GetPositionSize()
   */
  virtual PositionSize GetPositionSize() const override;

  /**
   * @copydoc Dali::RenderSurface::GetDpi()
   */
  virtual void GetDpi( unsigned int& dpiHorizontal, unsigned int& dpiVertical ) override;

  /**
   * @copydoc Dali::RenderSurface::InitializeEgl()
   */
  virtual void InitializeEgl( EglInterface& egl ) override;

  /**
   * @copydoc Dali::RenderSurface::CreateEglSurface()
   */
  virtual void CreateEglSurface( EglInterface& egl ) override;

  /**
   * @copydoc Dali::RenderSurface::DestroyEglSurface()
   */
  virtual void DestroyEglSurface( EglInterface& egl ) override;

  /**
   * @copydoc Dali::RenderSurface::ReplaceEGLSurface()
   */
  virtual bool ReplaceEGLSurface( EglInterface& egl ) override;

  /**
   * @copydoc Dali::RenderSurface::MoveResize()
   */
  virtual void MoveResize( Dali::PositionSize positionSize) override {}

  /**
   * @copydoc Dali::RenderSurface::SetViewMode()
   */
  void SetViewMode( ViewMode viewMode ) override {}

  /**
   * @copydoc Dali::RenderSurface::StartRender()
   */
  virtual void StartRender() override;

  /**
   * @copydoc Dali::RenderSurface::PreRender()
   */
  virtual bool PreRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction, bool resizingSurface ) override;

  /**
   * @copydoc Dali::RenderSurface::PostRender()
   */
  virtual void PostRender( EglInterface& egl, Integration::GlAbstraction& glAbstraction, DisplayConnection* displayConnection, bool replacingSurface, bool resizingSurface );

  /**
   * @copydoc Dali::RenderSurface::StopRender()
   */
  virtual void StopRender() override;

  /**
   * @copydoc Dali::RenderSurface::SetThreadSynchronization
   */
  virtual void SetThreadSynchronization( ThreadSynchronizationInterface& threadSynchronization ) override;

  /**
   * @copydoc Dali::RenderSurface::GetSurfaceType()
   */
  virtual RenderSurface::Type GetSurfaceType() override;

private: // from PixmapRenderSurface

  /**
   * @copydoc Dali::RenderSurface::ReleaseLock()
   */
  virtual void ReleaseLock() override;

  /**
   * @copydoc Dali::Internal::Adaptor::PixmapRenderSurface::Initialize()
   */
  virtual void Initialize( Any surface ) override;

  /**
   * @copydoc Dali::Internal::Adaptor::PixmapRenderSurface::Initialize()
   */
  virtual void CreateRenderable() override;

  /**
   * @copydoc Dali::Internal::Adaptor::PixmapRenderSurface::Initialize()
   */
  virtual void UseExistingRenderable( unsigned int surfaceId ) override;

private:

  /**
   * Get the surface id if the surface parameter is not empty
   * @param surface Any containing a surface id, or can be empty
   * @return surface id, or zero if surface is empty
   */
  unsigned int GetSurfaceId( Any surface ) const;

private: // Data

  static const int BUFFER_COUNT = 2;

  PositionSize                    mPosition;               ///< Position
  TriggerEventInterface*          mRenderNotification;     ///< Render notification trigger
  ColorDepth                      mColorDepth;             ///< Color depth of surface (32 bit or 24 bit)
  bool                            mOwnSurface;             ///< Whether we own the surface (responsible for deleting it)

  int                             mProduceBufferIndex;
  int                             mConsumeBufferIndex;
  WinPixmap                       mWinPixmaps[BUFFER_COUNT];  ///< Win-Pixmap
  EGLSurface                      mEglSurfaces[BUFFER_COUNT];
  ThreadSynchronizationInterface* mThreadSynchronization;     ///< A pointer to the thread-synchronization
  ConditionalWait                 mPixmapCondition;           ///< condition to share pixmap
};

} // namespace Adaptor

} // namespace internal

} // namespace Dali

#endif // DALI_WIN_PIXMAP_RENDER_SURFACE_H
