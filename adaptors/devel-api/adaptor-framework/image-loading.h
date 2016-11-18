#ifndef DALI_IMAGE_LOADING_H
#define DALI_IMAGE_LOADING_H

/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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
 */

// EXTERNAL INCLUDES
#include <string>
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/images/image-operations.h>
#include <dali/public-api/images/pixel-data.h>

namespace Dali
{

/**
 * @brief Load an image synchronously from local file.
 *
 * @note This method is thread safe, i.e. can be called from any thread.
 *
 * @param [in] url The URL of the image file to load.
 * @param [in] size The width and height to fit the loaded image to, 0.0 means whole image
 * @param [in] fittingMode The method used to fit the shape of the image before loading to the shape defined by the size parameter.
 * @param [in] samplingMode The filtering method used when sampling pixels from the input image while fitting it to desired size.
 * @param [in] orientationCorrection Reorient the image to respect any orientation metadata in its header.
 * @return handle to the loaded PixelData object or an empty handle in case loading failed.
 */
DALI_IMPORT_API PixelData LoadImageFromFile( const std::string& url,
                                             ImageDimensions size = ImageDimensions( 0, 0 ),
                                             FittingMode::Type fittingMode = FittingMode::DEFAULT,
                                             SamplingMode::Type samplingMode = SamplingMode::BOX_THEN_LINEAR,
                                             bool orientationCorrection = true );

} // Dali

#endif // DALI_IMAGE_LOADING_H
