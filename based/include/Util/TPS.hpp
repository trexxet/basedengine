#pragma once

#include <cstdint>

#include "Defs.hpp"
#include "SDL3/SDL_timer.h"

namespace Based::Util {

class BASED_API TPSCounter {
	uint16_t tps = 0;
public:
	void tick() {
		static thread_local uint16_t ticks = 0;
		static thread_local uint64_t ms_last = 0;
		const uint64_t one_second = 1000; // in ms

		ticks++;

		uint64_t ms_curr = SDL_GetTicks();
		if (ms_curr - ms_last >= one_second) {
			ms_last = ms_curr;
			tps = ticks;
			ticks = 0;
		}
	}

	inline uint16_t get() { return tps; }
};

}
