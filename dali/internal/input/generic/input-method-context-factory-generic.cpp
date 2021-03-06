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

// EXTERNAL INCLUDES
#include <memory>

// INTERNAL INCLUDES
#include <dali/internal/input/common/input-method-context-factory.h>
#include <dali/internal/input/generic/input-method-context-impl-generic.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

class InputMethodContext;

namespace InputMethodContextFactory
{

// InputMethodContext Factory to be implemented by the platform
InputMethodContextPtr CreateInputMethodContext( Dali::Actor actor )
{
  return Dali::Internal::Adaptor::InputMethodContextGeneric::New( actor );
}

}

}

}

}
