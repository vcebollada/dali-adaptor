﻿#ifndef DALI_INTERNAL_ACCESSIBILITY_COMMON_TTS_PLAYER_IMPL_H
#define DALI_INTERNAL_ACCESSIBILITY_COMMON_TTS_PLAYER_IMPL_H

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

// EXTERNAL INCLUDES
#include <dali/integration-api/debug.h>
#include <dali/public-api/object/base-object.h>

// INTERNAL INCLUDES
#include <dali/public-api/adaptor-framework/tts-player.h>

namespace Dali
{

namespace Internal
{

namespace Adaptor
{

/**
 * Text-to-speech player
 */
class TtsPlayer : public Dali::BaseObject
{

public:

  /**
   * Create a TtsPlayer with the given mode.
   * This should only be called once by the Adaptor class for each given mode.
   * @param mode the mode of tts-player
   * @return A newly created TtsPlayer.
   */
  static Dali::TtsPlayer New(Dali::TtsPlayer::Mode mode);

  /**
   * @copydoc TtsPlayer::Play()
   */
  virtual void Play(const std::string& text);

  /**
   * @copydoc TtsPlayer::Stop()
   */
  virtual void Stop();

  /**
   * @copydoc TtsPlayer::Pause()
   */
  virtual void Pause();

  /**
   * @copydoc TtsPlayer::Resume()
   */
  virtual void Resume();

  /**
   * @copydoc TtsPlayer::GetState()
   */
  virtual Dali::TtsPlayer::State GetState();

  /**
   * @copydoc TtsPlayer::StateChangedSignal()
   */
  virtual Dali::TtsPlayer::StateChangedSignalType& StateChangedSignal();
};

} // namespace Adaptor

} // namespace Internal

// Helpers for public-api forwarding methods

inline Internal::Adaptor::TtsPlayer& GetImplementation(Dali::TtsPlayer& player)
{
  DALI_ASSERT_ALWAYS( player && "TtsPlayer handle is empty" );

  BaseObject& handle = player.GetBaseObject();

  return static_cast<Internal::Adaptor::TtsPlayer&>(handle);
}

inline const Internal::Adaptor::TtsPlayer& GetImplementation(const Dali::TtsPlayer& player)
{
  DALI_ASSERT_ALWAYS( player && "TtsPlayer handle is empty" );

  const BaseObject& handle = player.GetBaseObject();

  return static_cast<const Internal::Adaptor::TtsPlayer&>(handle);
}

} // namespace Dali

#endif // DALI_INTERNAL_ACCESSIBILITY_COMMON_TTS_PLAYER_IMPL_H
