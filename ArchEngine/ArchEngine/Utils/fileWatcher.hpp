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
 * Last Modified: 06/05/2018                                                 *
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
		FileModifiedEvent(const std::string& path) :
			Core::IEvent(Core::EVENT_FILE_MODIFIED), m_path(path) {}

		~FileModifiedEvent() {}

		Core::EventType getType() const override {
			return m_type;
		}

		std::string getPath() const {
			return m_path;
		}

	private:
		std::string m_path;
	};
	//-------------------------------------------------------------------------


	class FileWatcher {
	public:
		FileWatcher() {}
		~FileWatcher() {}

		void update() {
			struct ARCH_ENGINE_FILE_WATCHER_STAT result;

			for (auto& it : m_files) {
				if (ARCH_ENGINE_FILE_WATCHER_STAT(
					it.first.c_str(), &result) == 0) {
					auto mod_time = result.st_mtime;

					if (it.second != mod_time) {
						// File was modified
						Core::EventPtr evnt(new FileModifiedEvent(it.first));
						Core::EventManager::getInstance().postEvent(evnt);
						it.second = mod_time;
					}
				}
			}
		}

		bool addFile(const std::string& path) {
			struct ARCH_ENGINE_FILE_WATCHER_STAT result;
			time_t mod_time;

			if (ARCH_ENGINE_FILE_WATCHER_STAT(path.c_str(), &result) == 0)
				mod_time = result.st_mtime;
			else {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
				ServiceLocator::getFileLogger()->log<LOG_ERROR>(
					"Could not hot-reload " + path);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
				return false;
			}

			m_files.push_back(std::make_pair(path, mod_time));
		}

		// True if removed, false otherwise
		bool removeFile(const std::string& path) {
			// O(m_files.size()) + O("elements copied").
			// It's fine, since there shouldn't be many files
			// nor calls to this method
			for (auto it = m_files.begin(); it != m_files.end(); ++it) {
				if (it->first == path) {
					m_files.erase(it);
					return true;
				}
			}
		
			return false;
		}

	private:
		// File name and last modified time
		std::vector<std::pair<std::string, time_t>> m_files;
	};
}


#endif	// UTILS_FILE_WATCHER_HPP