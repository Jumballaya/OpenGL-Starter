#pragma once
#include <assert.h>


namespace Core {
	namespace IO {
		class FPSCounter {
		public:
			explicit FPSCounter(float _avgIntervalSec = 0.5f) : avgIntervalSec(_avgIntervalSec) {
				assert(_avgIntervalSec > 0.0f);
			}

			bool tick(double dT, bool frameRendered) {
				float deltaSeconds = static_cast<float>(dT);
				if (frameRendered) numFrames++;
				accumulatedTime += deltaSeconds;
				if (accumulatedTime < avgIntervalSec) {
					return false;
				}
				currentFPS = static_cast<float>(numFrames / accumulatedTime);
				numFrames = 0;
				accumulatedTime = 0;
				return true;
			}

			inline float getFPS() const { return currentFPS; }

		private:
			float avgIntervalSec;
			unsigned int numFrames = 0;
			double accumulatedTime = 0;
			float currentFPS = 0.0f;
		};
	}
}