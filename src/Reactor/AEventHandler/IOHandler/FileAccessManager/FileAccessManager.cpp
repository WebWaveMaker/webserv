#include "FileAccessManager.hpp"

namespace reactor {
	FileAccessManager::FileAccessManager() {}
	FileAccessManager::~FileAccessManager() {
		clear();
	}

	fd_t FileAccessManager::makeFd(std::string filename, std::string option) {
		FILE* file = fopen(filename.c_str(), option.c_str());
		if (file == NULL)
			return FD_ERROR;
		fd_t fileFd = fileno(file);
		addFileInfo(fileFd, makeFileInfo(filename, file, FILE_WAIT));
		addWait(fileFd, filename);
		setStateAll();
		std::cerr << "makeFd: " << fileFd << std::endl;
		test_print_all_fd();
		return (fileFd);
	}

	void FileAccessManager::closeFd(fd_t fd) {
		FileInfo& fileInfo = findFileInfo(fd);
		if (fileInfo.state == FILE_WAIT) {
			eraseWait(fd);
		}
		eraseFileInfo(fd);
		setStateAll();
		std::cerr << "closeFd: " << fd << std::endl;
	}

	FileAccessState& FileAccessManager::getState(fd_t fd) {
		return findFileInfo(fd).state;
	}

	FileInfo FileAccessManager::makeFileInfo(std::string& filename, FILE* fp, FileAccessState state) {
		FileInfo fileInfo(filename, fp, state);
		return fileInfo;
	}

	void FileAccessManager::addFileInfo(fd_t fd, FileInfo fileInfo) {
		accessMap.insert(std::make_pair(fd, fileInfo));
	}

	void FileAccessManager::eraseFileInfo(fd_t fd) {
		accessMap.erase(fd);
	}

	FileInfo& FileAccessManager::findFileInfo(fd_t fd) {
		std::map<fd_t, FileInfo>::iterator it = accessMap.find(fd);
		if (it == accessMap.end()) {
			throw std::runtime_error("FileAccessManager::findFileInfo() : fd not found");
		}
		return it->second;
	}

	void FileAccessManager::addWait(fd_t fd) {
		std::string fileName = findFileInfo(fd).fileName;
		this->addWait(fd, fileName);
	}

	void FileAccessManager::addWait(fd_t fd, std::string fileName) {
		std::map<std::string, std::vector<fd_t> >::iterator it = waitVector.find(fileName);
		if (it == waitVector.end()) {
			std::vector<fd_t> fdVector;
			fdVector.push_back(fd);
			waitVector.insert(std::make_pair(fileName, fdVector));
		} else {
			it->second.push_back(fd);
		}
	}

	void FileAccessManager::eraseWait(fd_t fd) {
		std::string fileName = findFileInfo(fd).fileName;
		std::map<std::string, std::vector<fd_t> >::iterator it = waitVector.find(fileName);
		if (it == waitVector.end()) {
			throw std::runtime_error("FileAccessManager::eraseWait() : fd not found");
		} else {
			std::vector<fd_t>::iterator it2 = it->second.begin();
			while (it2 != it->second.end()) {
				if (*it2 == fd) {
					it->second.erase(it2);
					break;
				}
				it2++;
			}
		}
	}

	std::vector<fd_t>::iterator FileAccessManager::findWait(fd_t fd) {
		std::string fileName = findFileInfo(fd).fileName;
		std::map<std::string, std::vector<fd_t> >::iterator it = waitVector.find(fileName);
		if (it == waitVector.end()) {
			throw std::runtime_error("FileAccessManager::findWait() : fd not found");
		} else {
			std::vector<fd_t>::iterator it2 = it->second.begin();
			while (it2 != it->second.end()) {
				if (*it2 == fd) {
					return it2;
				}
				it2++;
			}
			throw std::runtime_error("FileAccessManager::findWait() : fd not found");
		}
	}

	void FileAccessManager::setState(fd_t fd, FileAccessState state) {
		findFileInfo(fd).state = state;
	}
	// waitVector의 모든 key값의 첫번째 fd_t의 state를 FILE_ACCESS로 변경하는 함수
	void FileAccessManager::setStateAll() {
		std::map<std::string, std::vector<fd_t> >::iterator it = waitVector.begin();
		while (it != waitVector.end()) {
			setState(it->second.front(), FILE_ACCESS);
			it++;
		}
	}

	void FileAccessManager::clear() {
		accessMap.clear();
		waitVector.clear();
	}

	void FileAccessManager::test_print_all_fd() {
		std::map<fd_t, FileInfo>::iterator it = FileAccessManager::getInstance()->accessMap.begin();
		while (it != FileAccessManager::getInstance()->accessMap.end()) {
			std::cerr << "fd: " << it->first << ", fileName: " << it->second.fileName << ", state: " << it->second.state
					  << std::endl;
			it++;
		}
		std::cerr << std::endl;
	}
}  // namespace reactor