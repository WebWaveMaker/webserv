#pragma once
#ifndef FILEACCESSMANAGER_HPP
#define FILEACCESSMANAGER_HPP

#include "FileAccessManager.h"
// 파일 접근 관리를 위한 클래스

class FileAccessManager : public utils::TSingleton<FileAccessManager> {
   public:
	// 파일 접근 요청 (읽기 권한 옵션 포함)
	fd_t getFd(const std::string& fileName, const std::string& option) {
		fd_t fd = utils::makeFd(fileName.c_str(), option.c_str());
		return fd;
	}

	// 파일 작업 완료 알림 (fd_t fd를 사용)
	void notifyAccessCompletion(fd_t fd) {
		std::string fileName = findFileNameByFd(fd);
		if (!fileName.empty() && waitQueue.find(fileName) != waitQueue.end()) {
			waitQueue[fileName].pop();
			if (waitQueue[fileName].empty()) {
				waitQueue.erase(fileName);
			}
		}
	}

	bool isFdClosed(fd_t fd) {}

   protected:
	FileAccessManager() {}
	~FileAccessManager() {}

   private:
	std::map<std::string, std::queue<fd_t> > waitQueue;	 // 대기열
	std::map<fd_t, FileAccessState> accessMap;			 // 파일 이름과 접근 상태
	std::map<fd_t, std::string> fdToFileMap;			 // fd와 파일 이름 매핑

	// fd로부터 파일 이름 찾기
	std::string findFileNameByFd(fd_t fd) {
		if (fdToFileMap.find(fd) != fdToFileMap.end()) {
			return fdToFileMap[fd];
		}
		return "";	// fd에 해당하는 파일 이름이 없는 경우
	}
};

#endif