#ifndef DALI_GRAPHICS_VULKAN_API_FRAMEBUFFER_H
#define DALI_GRAPHICS_VULKAN_API_FRAMEBUFFER_H

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

// INTERNAL INCLUDES
#include <dali/graphics-api/graphics-api-framebuffer-factory.h>
#include <dali/graphics-api/graphics-api-framebuffer.h>
#include <dali/graphics/vulkan/internal/vulkan-types.h>
#include <dali/graphics/vulkan/api/vulkan-api-framebuffer-factory.h>

namespace Dali
{
namespace Graphics
{
namespace VulkanAPI
{
class Controller;

/**
 * Framebuffer implementation
 */
class Framebuffer : public Dali::Graphics::Framebuffer
{
public:

  explicit Framebuffer( Controller& controller, uint32_t width, uint32_t height );

  ~Framebuffer() override;

  bool Initialise( const std::vector<FramebufferFactory::ColorAttachment>& colorAttachments,
                   const FramebufferFactory::DepthAttachment& depthAttachment );

  Vulkan::RefCountedFramebuffer GetFramebufferRef() const;

private:
  Controller& mController;
  uint32_t mWidth;
  uint32_t mHeight;

  Vulkan::RefCountedFramebuffer mFramebufferRef;
};

} // namespace VulkanAPI
} // namespace Graphics
} // namespace Dali
#endif // DALI_GRAPHICS_VULKAN_API_FRAMEBUFFER_H