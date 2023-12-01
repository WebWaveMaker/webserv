#pragma once
#ifndef FILECLOSEMANAGER_HPP
#define FILECLOSEMANAGER_HPP

#include "FileCloseManager.h"
// 파일 접근 관리를 위한 클래스

class FileInfo {
   public:
	std::string fileName;
	FILE* fp;

	FileInfo(std::string fileName, FILE* fp) : fileName(fileName), fp(fp) {}
	~FileInfo() { fclose(fp); }
	FileInfo(const FileInfo& fileInfo) : fileName(fileInfo.fileName), fp(fileInfo.fp) {}
	FileInfo& operator=(const FileInfo& fileInfo) {
		if (this == &fileInfo)
			return *this;

		this->fileName = fileInfo.fileName;
		this->fp = fileInfo.fp;
		return *this;
	}
};

namespace reactor {
	class FileCloseManager : public utils::TSingleton<FileCloseManager> {
	   public:
		FileCloseManager();
		~FileCloseManager();
		// 파일 접근 요청 (읽기 권한 옵션 포함)
		fd_t makeFd(std::string fileName, std::string option);
		// 파일 접근 종료
		void closeFd(fd_t fd);

	   private:
		// 접근 중인 파일 목록
		std::map<fd_t, utils::shared_ptr<FileInfo> > accessMap;	 // 접근 중인 파일 목록
		// FileInfo를 생성하는 함수
		utils::shared_ptr<FileInfo> makeFileInfo(std::string& fileName, FILE* fp);
		// FileInfo를 accessMap에 추가
		void addFileInfo(fd_t fd, utils::shared_ptr<FileInfo> fileInfo);
		// fd_t값으로 FileInfo를 accessMap에서 찾아 삭제하는 함수
		void eraseFileInfo(fd_t fd);
		// fd_t값으로 FileInfo를 찾는 함수
		void clear();
	};
}  // namespace reactor
#endif