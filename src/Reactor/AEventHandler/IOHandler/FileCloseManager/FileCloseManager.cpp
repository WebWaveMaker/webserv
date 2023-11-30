#include "FileCloseManager.hpp"

#include <fcntl.h>
namespace reactor {
	FileCloseManager::FileCloseManager() {}
	FileCloseManager::~FileCloseManager() {
		clear();
	}

	fd_t FileCloseManager::makeFd(std::string filename, std::string option) {
		FILE* file = fopen(filename.c_str(), option.c_str());
		if (file == NULL)
			return FD_ERROR;
		fd_t fileFd = fileno(file);
		addFileInfo(fileFd, makeFileInfo(filename, file));
		return (fileFd);
	}

	void FileCloseManager::closeFd(fd_t fd) {
		if (fd < 0) {
			return;
		}
		eraseFileInfo(fd);
	}

	utils::shared_ptr<FileInfo> FileCloseManager::makeFileInfo(std::string& filename, FILE* fp) {
		utils::shared_ptr<FileInfo> fileInfo(new FileInfo(filename, fp));
		return fileInfo;
	}

	void FileCloseManager::addFileInfo(fd_t fd, utils::shared_ptr<FileInfo> fileInfo) {
		accessMap.insert(std::make_pair(fd, fileInfo));
	}

	void FileCloseManager::eraseFileInfo(fd_t fd) {
		accessMap.erase(fd);
		if (accessMap.empty()) {
			accessMap.clear();
		}
	}

	void FileCloseManager::clear() {
		accessMap.clear();
	}
}  // namespace reactor