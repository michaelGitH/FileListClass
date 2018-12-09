#ifndef WINAPIDIR_H
#define WINAPIDIR_H

#include "stdafx.h"

c_map keys;

bool RecursiveSearch(LPWSTR lpSearch, std::vector<WIN32_FIND_DATA> & arFindData);

std::wstring ToLower(const std::wstring & str) {
	std::wstring rstr;
	std::transform(str.begin(), str.end(), rstr.begin(), towlower);
	return rstr;
}

VOID FindFirstFileFailed(LPWSTR CONST szPath) {
	fwprintf(stderr, L"FindFirstFile() failed, GetLastError() = %d, szPath = %s\r\n",
		GetLastError(), szPath);
}

VOID ProcessFoundFile(LPWSTR CONST szPath,
	WIN32_FIND_DATA CONST * CONST fdFindData,
	LPWSTR CONST lpSearch) {
	TCHAR szEnd[] = L"\r\n";
	DWORD dwTemp;
	WString str1 = WString(fdFindData->cFileName);
	WString str2 = WString(lpSearch);
	std::transform(str1.begin(), str1.end(), str1.begin(), towlower);
	std::transform(str2.begin(), str2.end(), str2.begin(), towlower);


	if (str1
		!= str2) {
		wprintf(L"%s\r\n", fdFindData->cFileName);
	}
}

std::vector<WIN32_FIND_DATA> & GetFiles(LPWSTR lpSearch) {
	std::vector<WIN32_FIND_DATA> arFindData;
	RecursiveSearch(lpSearch, arFindData);
	return arFindData;
}

bool RecursiveSearch(LPWSTR lpSearch, std::vector<WIN32_FIND_DATA> & arFindData) {

	TCHAR szPath[MAX_PATH + 1];

	WIN32_FIND_DATA fdFindData;
	HANDLE hFind;
	TCHAR * CONST lpLastChar = szPath + lstrlen(szPath);
	lstrcpy(szPath, lpSearch);
	lstrcat(szPath, L"\\*");
	hFind = FindFirstFile(szPath, &fdFindData);
	*lpLastChar = '\0';

	if (INVALID_HANDLE_VALUE == hFind) {
		FindFirstFileFailed(szPath);
		return false;
	}
	WCHAR f;
	
	do {
		if ((0 == lstrcmp(fdFindData.cFileName, L".")) ||
			(0 == lstrcmp(fdFindData.cFileName, L".."))) {
			continue;
		}
		lstrcat(szPath, fdFindData.cFileName);
		if (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (keys["-R"] == "on") {
				lstrcat(szPath, L"\\");
				RecursiveSearch(lpSearch, arFindData);
			}
			continue;
		}
		else {
			arFindData.push_back(fdFindData);
		}
		*lpLastChar = '\0';
	} while (FindNextFile(hFind, &fdFindData));

	FindClose(hFind);

	return true;
}

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

// Convert a wide Unicode string to default code page
char * ansi_encode(LPWSTR wstr)
{
	if (wcscmp(wstr, L"")==0) return nullptr;
	int size_needed = WideCharToMultiByte(CP_ACP, 0, &wstr[0], static_cast<int>(wcslen(wstr)), NULL, 0, NULL, NULL);
	char * strTo = new char[size_needed+1];
	WideCharToMultiByte(CP_ACP, 0, &wstr[0], static_cast<int>(wcslen(wstr)), &strTo[0], size_needed, NULL, NULL);
	strTo[size_needed] = '\0';
	return strTo;
}

// Convert an default code page string to a wide Unicode String
LPWSTR ansi_decode(const char * str)
{
	if (strcmp(str, "")==0) return nullptr;
	int size_needed = MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(strlen(str)), NULL, 0);
	LPWSTR wstrTo = new WCHAR[size_needed+1];
	MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(strlen(str)), &wstrTo[0], size_needed);
	wstrTo[size_needed] = '\0';
	return wstrTo;
}

#endif
