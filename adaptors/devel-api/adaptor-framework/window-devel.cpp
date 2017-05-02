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

// INTERNAL INCLUDES
#include <adaptors/devel-api/adaptor-framework/window-devel.h>
#include <adaptors/common/window-impl.h>

namespace Dali
{

namespace DevelWindow
{

FocusSignalType& FocusChangedSignal( Window window )
{
  return GetImplementation( window ).FocusChangedSignal();
}

void SetAcceptFocus( Window window, bool accept )
{
  GetImplementation( window ).SetAcceptFocus( accept );
}

bool IsFocusAcceptable( Window window )
{
  return GetImplementation( window ).IsFocusAcceptable();
}

void Show( Window window )
{
  GetImplementation( window ).Show();
}

void Hide( Window window )
{
  GetImplementation( window ).Hide();
}

bool IsVisible( Window window )
{
  return GetImplementation( window ).IsVisible();
}

void * GetNativeWindowHandler( Window window )
{
  return GetImplementation( window ).GetNativeWindowHandler();
}

} // namespace DevelWindow

} // namespace Dali