#pragma once

namespace Core {
	namespace Math {
	
		template <typename T>
		T clamp(T v, T a, T b)
		{
			if (v < a) return a;
			if (v > b) return b;
			return v;
		}
	}
}

