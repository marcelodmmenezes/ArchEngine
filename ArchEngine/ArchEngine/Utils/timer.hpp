/*===========================================================================*
 * Arch Engine - "Utils/timer.hpp"                                           *
 *                                                                           *
 * Simple fps, time and time-related stuff measurer.                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 26/04/2018                                                       *
 * Last Modified: 26/04/2018                                                 *
 *===========================================================================*/


#ifndef UTILS_TIMER_HPP
#define UTILS_TIMER_HPP


#include "serviceLocator.hpp"

#if defined(__unix__)
#include <SDL2/SDL.h>
#elif defined(_MSC_VER)
#include <SDL.h>
#endif

#include <cstdlib>
#include <algorithm>


namespace Utils {
	class Timer {
	public:
		Timer();
		~Timer();

		void init();
		void calc();

		static int getCurrentTicks();
		int getFrameCount() const;

		double getDeltaTime() const;
		double getFrameRate() const;

	private:
		// Number of frames for average fps calculation
		static const unsigned N_AVG_FRAMES = 20;

		// Last N_FRAMES frame durations
		int m_frame_durations[N_AVG_FRAMES];
		// Last calculated SDL_GetTicks
		int m_last_ticks;
		// Total frames passed
		int m_frame_count;

		// Time stamp of current frame
		double m_current_frame;
		// Time stamp of last frame
		double m_last_frame;
		// Difference between current and last frames
		double m_delta_time;
		// Duration in milliseconds of current frame
		double m_average_frame_duration;
		// FPS
		double m_frames_per_second;
	};
}


#endif	// UTILS_TIMER_HPP