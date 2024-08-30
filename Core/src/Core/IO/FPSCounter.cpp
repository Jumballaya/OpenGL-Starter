#include "FPSCounter.h"
#include <iostream>

namespace Core {
	namespace IO {
		bool FPSCounter::tick(double dT, bool frameRendered) {
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
	}
}