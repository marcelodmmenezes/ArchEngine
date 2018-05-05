/*===========================================================================*
 * Arch Engine - "Utils/timer.cpp"                                           *
 *                                                                           *
 * Simple fps, time and time-related stuff measurer.                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 26/04/2018                                                       *
 * Last Modified: 26/04/2018                                                 *
 *===========================================================================*/


#include "timer.hpp"


namespace Utils {
	Timer::Timer() : m_last_frame(0.0), m_frame_count(0),
		m_frames_per_second(0.0) {
		std::fill(m_frame_durations, m_frame_durations + N_AVG_FRAMES, 0);
	}

	Timer::~Timer() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Timer destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	void Timer::init() {
		m_last_ticks = SDL_GetTicks();
	}

	void Timer::calc() {
		int frame_durations_index; // m_frame_durations index
		int ticks; // Current SDL_GetTicks value

		frame_durations_index = m_frame_count % N_AVG_FRAMES;
		ticks = SDL_GetTicks();

		// Delta time calculation
		m_current_frame = ticks / 1000.0;
		m_delta_time = m_current_frame - m_last_frame;
		m_last_frame = m_current_frame;

		// Adding current frame
		m_frame_durations[frame_durations_index] = ticks - m_last_ticks;
		m_last_ticks = ticks;
		m_frame_count++;

		// Avoiding invalid access
		int count = std::min(m_frame_count, (int)N_AVG_FRAMES);

		// Averaging FPS
		m_frames_per_second = 0;

		for (int i = 0; i < count; i++)
			m_frames_per_second += m_frame_durations[i];

		m_frames_per_second /= count;
		m_frames_per_second = 1000.0 / m_frames_per_second;
	}

	int Timer::getCurrentTicks() {
		return SDL_GetTicks();
	}

	int Timer::getFrameCount() const {
		return m_frame_count;
	}

	double Timer::getDeltaTime() const {
		return m_delta_time;
	}

	double Timer::getFrameRate() const {
		return m_frames_per_second;
	}
}