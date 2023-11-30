#include "FileAccessManager.hpp"

#include <fcntl.h>
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

		return (fileFd);
	}

	void FileAccessManager::closeFd(fd_t fd) {
		eraseWait(fd);
		eraseFileInfo(fd);
		setStateAll();
	}

	FileAccessState& FileAccessManager::getState(fd_t fd) {
		return findFileInfo(fd).get()->state;
	}

	utils::shared_ptr<FileInfo> FileAccessManager::makeFileInfo(std::string& filename, FILE* fp,
																FileAccessState state) {
		utils::shared_ptr<FileInfo> fileInfo(new FileInfo(filename, fp, state));
		return fileInfo;
	}

	void FileAccessManager::addFileInfo(fd_t fd, utils::shared_ptr<FileInfo> fileInfo) {
		accessMap.insert(std::make_pair(fd, fileInfo));
	}

	void FileAccessManager::eraseFileInfo(fd_t fd) {
		accessMap.erase(fd);
		if (accessMap.empty()) {

			accessMap.clear();
		}
	}

	utils::shared_ptr<FileInfo> FileAccessManager::findFileInfo(fd_t fd) {
		std::map<fd_t, utils::shared_ptr<FileInfo> >::iterator it = accessMap.find(fd);
		if (it == accessMap.end()) {
			throw std::runtime_error("FileAccessManager::findFileInfo() : fd not found");
		} else {
			return it->second;
		}
	}

	void FileAccessManager::addWait(fd_t fd) {
		std::string fileName = findFileInfo(fd).get()->fileName;
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
		std::string fileName = findFileInfo(fd).get()->fileName;
		std::map<std::string, std::vector<fd_t> >::iterator it = waitVector.find(fileName);

		if (it != waitVector.end()) {
			std::vector<fd_t>::iterator newEnd;
			newEnd = std::remove(it->second.begin(), it->second.end(), fd);

			// 실제로 벡터에서 제거
			it->second.erase(newEnd, it->second.end());

			// 벡터가 비어 있다면, 맵에서 해당 항목 제거
			if (it->second.empty()) {
				waitVector.erase(it);
			}
		}
	}

	std::vector<fd_t>::iterator FileAccessManager::findWait(fd_t fd) {
		std::string fileName = findFileInfo(fd).get()->fileName;
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
		findFileInfo(fd).get()->state = state;
	}
	// waitVector의 모든 key값의 첫번째 fd_t의 state를 FILE_ACCESS로 변경하는 함수
	void FileAccessManager::setStateAll() {
		if (waitVector.empty()) {

			return;
		}
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
}  // namespace reactor