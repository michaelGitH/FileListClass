#pragma once
#ifndef FILELIST_H
#define FILELIST_H
#pragma warning(disable : 4996)

#include "stdafx.h"

namespace winapins {

	class FileListException : public std::runtime_error {
	public:
		FileListException(const std::string & message) :
			std::runtime_error(message.data())
		{}
	};

	struct FileStruct {
		std::string name;
		std::string path;
		unsigned long long ullDate;
		std::string strDate;
		unsigned long size;
		bool operator==(const FileStruct & right);
		bool operator!=(const FileStruct & right);
	};

	class FileList : public std::vector<FileStruct> {
		bool BadInit;
		std::string ErrorMessage;
		bool isRecSearch;
		std::wstring lpPath;
		int codePage;
		VOID FindFirstFileFailed(LPWSTR CONST szPath);
		void DestructorHelper();
		void AddFile(WIN32_FIND_DATA & w32fd, const std::wstring & path);
		bool RecursiveSearch(const std::wstring & lpSearch);
		void ArgErrorMessage(const std::string & pattern, const std::string & arg);
		void CutString(std::string & str, int maxwidth);
		void CopyHelper(const FileList & right);
	public:
		FileList();
		FileList(std::vector<std::string> arglist);
		FileList(const std::string path, const bool isrec = false, const int cp = CP_OEMCP);
		FileList(const FileList & right);
		FileList(FileList && right);
		FileList & operator=(const FileList & right);
		FileList & operator=(FileList && right);
		bool operator==(const FileList & right);
		bool operator!=(const FileList & right);
		~FileList();
		bool FillingArguments(std::vector<std::string> arglist);
		bool Search();
		void Print(int widthsize = 30, bool path = false);
		void SortByDate(bool desc = false);
		void SortByName(bool desc = false);
		void SortByFullName(bool desc = false);
		void SortBySize(bool desc = false);
	};

	std::string DateTime(SYSTEMTIME & st, bool add_msec = false);
	LPWSTR any_decode(const char * str, int codepage);
	std::string any_encode(const std::wstring  & wstr, int codepage);
	unsigned long long GetMSecDateTime(SYSTEMTIME & st, bool add_msec);
	std::wstring ToLower(const std::wstring & str);
	std::string ToLower(const std::string & str);
}


#endif
