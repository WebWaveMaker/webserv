#pragma once
#ifndef FILEACCESSMANAGER_HPP
#define FILEACCESSMANAGER_HPP

#include "FileAccessManager.h"
// 파일 접근 관리를 위한 클래스

class FileInfo {
   public:
	std::string fileName;
	FILE* fp;
	FileAccessState& state;

	FileInfo(std::string fileName, FILE* fp, FileAccessState state) : fileName(fileName), fp(fp), state(state) {}
	~FileInfo() { fclose(fp); }
};

class FileAccessManager : public utils::TSingleton<FileAccessManager> {
   public:
	// 파일 접근 요청 (읽기 권한 옵션 포함)
	fd_t makeFd(std::string fileName, std::string option);
	// 파일 접근 종료
	void closeFd(fd_t fd);
	// fd_t값으로 FileInfo->state를 반환하는 함수
	FileAccessState& getState(fd_t fd);

   protected:
	FileAccessManager() {}
	~FileAccessManager() {}

   private:
	std::map<std::string, std::vector<fd_t> > waitVector;  // 대기열
	std::map<fd_t, FileInfo> accessMap;					   // 접근 중인 파일 목록

	// FileInfo를 생성하는 함수
	FileInfo makeFileInfo(std::string& fileName, const fd_t& fd, FILE* fp, FileAccessState state);
	// FileInfo를 accessMap에 추가
	void addFileInfo(fd_t fd, FileInfo fileInfo);
	// fd_t값으로 FileInfo를 accessMap에서 찾아 삭제하는 함수
	void eraseFileInfo(fd_t fd);
	// fd_t값으로 FileInfo를 찾는 함수
	FileInfo& findFileInfo(fd_t fd);

	// 대기열에 fd_t값을 추가하는 함수
	void addWait(fd_t fd);
	// 대기열에 fd_t값을 추가하는 함수
	void addWait(fd_t fd, std::string fileName);
	// 대기열에 있는 fd_t값을 삭제하는 함수
	void eraseWait(fd_t fd);
	// 대기열에 있는 fd_t값을 찾는 함수
	std::vector<fd_t>::iterator findWait(fd_t fd);

	// fd_t값으로 FileInfo->state를 변경하는 함수
	void setState(fd_t fd, FileAccessState state);
	// waitVector의 모든 key값의 첫번째 fd_t의 state를 FILE_ACCESS로 변경하는 함수
	void setStateAll();
};

#endif