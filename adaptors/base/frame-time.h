#ifndef __DALI_INTERNAL_ADAPTOR_FRAME_TIME_H__
#define __DALI_INTERNAL_ADAPTOR_FRAME_TIME_H__

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

#include <stdint.h>

namespace Dali
{

namespace Integration
{
class PlatformAbstraction;
}

namespace Internal
{
namespace Adaptor
{

/**
 * FrameTime stores the time of the last VSync. It can then be used by the update thread to predict
 * the current update will be rendered.
 */
class FrameTime
{
public:

  // Called from Event thread

  /**
   * Constructor
   * @param[in]  platform  The platform used to retrieve the current time
   */
  FrameTime( Integration::PlatformAbstraction& platform );

  /**
   * Destructor, non virtual
   */
  ~FrameTime();

  /**
   * Sets the expected minimum frame time interval.
   * @param[in]  interval  The interval in microseconds.
   */
  void SetMinimumFrameTimeInterval( unsigned int interval );

  /**
   * Suspends the FrameTime object when the application state changes
   */
  void Suspend();

  /**
   * Resumes the FrameTime object when the application state changes
   */
  void Resume();

  // Called from Update thread

  /**
   * Sets the FrameTime object to sleep, i.e. when there are no more updates required.
   */
  void Sleep();

  /**
   * Wakes the FrameTime object from a sleep state.
   */
  void WakeUp();

  /**
   * Predicts when the next render time will occur.
   *
   * @param[out]  lastFrameDeltaSeconds      The delta, in seconds (with float precision), between the last two renders.
   * @param[out]  lastVSyncTimeMilliseconds  The time, in milliseconds, of the last VSync.
   * @param[out]  nextVSyncTimeMilliseconds  The estimated time, in milliseconds, at the next VSync.
   *
   * @note Should only be called once per tick, from the update thread.
   */
  void PredictNextVSyncTime( float& lastFrameDeltaSeconds, unsigned int& lastVSyncTimeMilliseconds, unsigned int& nextVSyncTimeMilliseconds );

  // Called from VSync thread

  /**
   * Tells the FrameTime object that a VSync has occurred.
   *
   * @param[in]  frameNumber  The frame number of the current VSync.
   *
   * @note Should only be called from the VSync thread.
   */
  void SetVSyncTime( unsigned int frameNumber );

private:

  /**
   * Sets the current time to be the last Vsync time.
   */
  inline void SetLastVSyncTime();

private:

  Integration::PlatformAbstraction& mPlatform;    ///< The platform abstraction.

  unsigned int mMinimumFrameTimeInterval;   ///< The minimum frame time interval, set by Adaptor.

  uint64_t mLastVSyncTime;                  ///< The last VSync time (in microseconds).
  uint64_t mLastVSyncTimeAtUpdate;          ///< The last VSync time at Update (in microseconds).

  unsigned int mLastVSyncFrameNumber;       ///< The last VSync frame number
  unsigned int mLastUpdateFrameNumber;      ///< The last VSync frame number handled in Update.

  bool         mRunning:1;                  ///< The state of the FrameTime object.
  bool         mFirstFrame:1;               ///< Whether the current update is the first frame (after initialisation, resume or wake up).

  unsigned int mPreviousUpdateFrames[3];    ///< Array holding the number of frames Update took in the last three iterations.
  unsigned int writePos;                    ///< The current write position in the array.

  unsigned int mExtraUpdatesSinceVSync;     ///< The number of extra updates since the last VSync.
};

} // namespace Adaptor
} // namespace Internal
} // namespace Dali

#endif // __DALI_INTERNAL_ADAPTOR_FRAME_TIME_H__
