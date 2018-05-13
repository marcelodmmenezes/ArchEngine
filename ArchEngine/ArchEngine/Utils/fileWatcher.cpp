/*===========================================================================*
 * Arch Engine - "Utils/fileWatcher.hpp"                                     *
 *                                                                           *
 * Class for watching and alerting of any modifications in registered files. *
 * Based in:                                                                 *
 * - (https://stackoverflow.com/questions/40504281/                          *
 *    c-how-to-check-the-last-modified-time-of-a-file)                       *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 07/05/2018                                                       *
 * Last Modified: 07/05/2018                                                 *
 *===========================================================================*/


#include "fileWatcher.hpp"


using namespace Core;


namespace Utils {
	//------------------------------------------------------------------ Events
	FileModifiedEvent::FileModifiedEvent(const std::string& path) :
		IEvent(EVENT_FILE_MODIFIED), m_path(path) {}
	FileModifiedEvent::~FileModifiedEvent() {}
	EventType FileModifiedEvent::getType() const { return m_type; }
	std::string FileModifiedEvent::getPath() const { return m_path; }

	WatchFileEvent::WatchFileEvent(const std::string& path, bool add) :
		IEvent(EVENT_WATCH_FILE), m_path(path), m_add(add) {}
	WatchFileEvent::~WatchFileEvent() {}
	EventType WatchFileEvent::getType() const { return m_type; }
	bool WatchFileEvent::add() const { return m_add; }
	std::string WatchFileEvent::getPath() const { return m_path; }

	//------------------------------------------------------------- FileWatcher
	FileWatcher::FileWatcher() {
		m_watch_file_listener.bind
			<FileWatcher, &FileWatcher::onWatchFileEvent>(this);
		EventManager::getInstance().addListener(
			m_watch_file_listener, EVENT_WATCH_FILE);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"FileWatcher constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	FileWatcher::~FileWatcher() {
		EventManager::getInstance().removeListener(
			m_watch_file_listener, EVENT_WATCH_FILE);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"FileWatcher destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	void FileWatcher::update() {
		struct ARCH_ENGINE_FILE_WATCHER_STAT result;

		for (auto& it : m_files) {
			if (ARCH_ENGINE_FILE_WATCHER_STAT(
				it.first.c_str(), &result) == 0) {
				auto mod_time = result.st_mtime;

				if (it.second != mod_time) {
					// File was modified
					Core::EventPtr evnt = std::make_shared<FileModifiedEvent>(
						FileModifiedEvent(it.first));
					Core::EventManager::getInstance().postEvent(evnt);
					it.second = mod_time;
				}
			}
		}
	}

	void FileWatcher::onWatchFileEvent(EventPtr e) {
		auto evnt = std::static_pointer_cast<WatchFileEvent>(e);

		if (evnt->add())
			addFile(evnt->getPath());
		else
			removeFile(evnt->getPath());
	}

	bool FileWatcher::addFile(const std::string& path) {
		if (path == "")
			return false;

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

		// Checks if file is already in vector
		for (auto it = m_files.begin(); it != m_files.end(); ++it) {
			if (it->first == path)
				it->second = mod_time;
		}

		m_files.push_back(std::make_pair(path, mod_time));

		return true;
	}

	bool FileWatcher::removeFile(const std::string& path) {
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
}