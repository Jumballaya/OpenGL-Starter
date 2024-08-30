#pragma once
#include <assert.h>


namespace Core {
	namespace IO {
		class FPSCounter {
		public:
			explicit FPSCounter(float _avgIntervalSec = 0.5f) : avgIntervalSec(_avgIntervalSec) {
				assert(_avgIntervalSec > 0.0f);
			}

			bool tick(double dT, bool frameRendered = true);

			inline float getFPS() const { return currentFPS; }

		private:
			float avgIntervalSec;
			unsigned int numFrames = 0;
			double accumulatedTime = 0;
			float currentFPS = 0.0f;
		};
	}
}