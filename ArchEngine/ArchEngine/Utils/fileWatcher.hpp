/*===========================================================================*
 * Arch Engine - "Utils/fileWatcher.hpp"                                     *
 *                                                                           *
 * Class for watching and alerting of any modifications in registered files. *
 * Based in:                                                                 *
 * - (https://stackoverflow.com/questions/40504281/                          *
 *    c-how-to-check-the-last-modified-time-of-a-file)                       *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 06/05/2018                                                       *
 * Last Modified: 25/06/2018                                                 *
 *===========================================================================*/


#ifndef UTILS_FILE_WATCHER_HPP
#define UTILS_FILE_WATCHER_HPP


#include "../Core/eventManager.hpp"
#include "serviceLocator.hpp"

#include <sys/types.h>
#include <sys/stat.h>

#if defined(__unix__)
#include <unistd.h>
#define ARCH_ENGINE_FILE_WATCHER_STAT stat
#elif defined(_MSC_VER)
#define ARCH_ENGINE_FILE_WATCHER_STAT _stat
#endif

#include <string>
#include <vector>


namespace Utils {
	//-----------------------
	//---- FileWatcher events
	//-----------------------

	class FileModifiedEvent : public Core::IEvent {
	public:
		FileModifiedEvent(const std::string& path);
		~FileModifiedEvent();

		std::string getPath() const;

	private:
		std::string m_path;
	};

	class WatchFileEvent : public Core::IEvent {
	public:
		WatchFileEvent(const std::string& path, bool add);
		~WatchFileEvent();

		bool add() const;
		std::string getPath() const;

	private:
		bool m_add;
		std::string m_path;
	};
	//-------------------------------------------------------------------------


	class FileWatcher {
	public:
		FileWatcher();
		~FileWatcher();

		void update();

		void onWatchFileEvent(Core::EventPtr e);

		// True if added, false otherwise
		bool addFile(const std::string& path);
		// True if removed, false otherwise
		bool removeFile(const std::string& path);

	private:
		// File name and last modified time
		std::vector<std::pair<std::string, time_t>> m_files;
		Core::EventListener m_watch_file_listener;
	};
}


#endif	// UTILS_FILE_WATCHER_HPP