#include "stdafx.h"
#include "FileList.h"

namespace winapins {
	FileList::FileList()
	{
		BadInit = true;
		ArgErrorMessage("Path is not set!\n%s", "");
		lpPath = L"";
		codePage = CP_OEMCP;
	}
	
	FileList::FileList(std::vector<std::string> arglist)
	{
		BadInit = false;
		lpPath = L"";
		codePage = CP_OEMCP;
		if (!FillingArguments(arglist)) {
			BadInit = true;
		}
	}

	FileList::FileList(const std::string path, const bool isrec, const int cp)
		: lpPath(any_decode(path.data(), CP_ACP)), isRecSearch(isrec), codePage(cp)
	{}

	FileList::FileList(const FileList & right)
		: std::vector<FileStruct>(right)
	{
		CopyHelper(right);
	}

	FileList::FileList(FileList && right)
		: std::vector<FileStruct>(std::move(right)),
		lpPath(std::move(right.lpPath)),
		ErrorMessage(std::move(right.ErrorMessage))
	{
		BadInit = right.BadInit;
		codePage = right.codePage;
		isRecSearch = right.isRecSearch;
	}

	FileList & FileList::operator=(const FileList & right)
	{
		CopyHelper(right);
		std::vector<FileStruct>::operator=(right);
		return (*this);
	}

	FileList & FileList::operator=(FileList && right)
	{
		BadInit = right.BadInit;
		codePage = right.codePage;
		isRecSearch = right.isRecSearch;
		std::vector<FileStruct>::operator=(std::move(right));
		lpPath = std::move(right.lpPath);
		ErrorMessage = std::move(right.ErrorMessage);
		//right.BadInit = true;
		//right.ArgErrorMessage("Path is not set!\n%s", "");
		return *this;
	}

	bool FileList::operator==(const FileList & right)
	{
		if(!
			(lpPath == right.lpPath &&
			BadInit == right.BadInit &&
			codePage == right.codePage &&
			ErrorMessage == right.ErrorMessage &&
			isRecSearch == right.isRecSearch)
			)
			return false;
		if (this->size() != right.size())
			return false;
		for (int i = 0; i < this->size() && i < right.size(); ++i) {
			if ((*this)[i] != right[i])
				return false;
		}
		return true;
	}

	bool FileList::operator!=(const FileList & right)
	{
		return !operator==(right);
	}

	bool FileStruct::operator==(const FileStruct & right)
	{
		return
			this->name == right.name &&
			this->path == right.path &&
			this->size == right.size &&
			this->strDate == right.strDate &&
			this->ullDate == right.ullDate;
	}

	bool FileStruct::operator!=(const FileStruct & right)
	{
		return !operator==(right);
	}
	
	FileList::~FileList()
	{
		DestructorHelper();
	}

	bool FileList::FillingArguments(std::vector<std::string> arglist)
	{
		std::string cur_key;
		isRecSearch = false;
		size_t argsize = arglist.size();
		for (int i = 0; i < argsize; ++i) {
			//Проверка ключей, не требующих значений
			cur_key = arglist[i];
			if (cur_key == "-R") {
				isRecSearch = true;
				continue;
			}
			//Проверка ключей, требующих значений
			else if (ToLower(cur_key) == "-path") {
				if (argsize - i <= 1) {
					ArgErrorMessage("Missing argument value! (%s)\n", cur_key);
					return false;
				}
				lpPath = any_decode(arglist[++i].data(), CP_ACP);
			}
			else if (ToLower(cur_key) == "-cp") {
				if (argsize - i <= 1) {
					ArgErrorMessage("Missing argument value! (%s)\n", cur_key);
					return false;
				}
				try {
					codePage = atoi(cur_key.data());
				}
				catch (std::exception & e) {
					fprintf(stderr, "Error in \"-cp\" value:\n\t%s",
						e.what());
				}
				
			}
			else {
				ArgErrorMessage("Invalid argument! (%s)\n", cur_key);
				return false;
			}
		}
		if (lpPath == L"") {
			ArgErrorMessage("Path is not set!\n%s", "");
			return false;
		}
		else if (lpPath[lpPath.size() - 1] != L'\\') {
			lpPath.push_back(L'\\');
		}
		return true;
	}

	bool FileList::Search()
	{
		if (BadInit) {
			throw FileListException(ErrorMessage);
		}
		return RecursiveSearch(lpPath);
	}

	void FileList::Print(int widthsize, bool path)
	{
		std::cout << std::left << std::setw(widthsize) << "Name"
			<< (path ? std::setw(widthsize) : std::setw(0)) << (path ? "Path" : "")
			<< std::setw(widthsize) << "Date"
			<< std::setw(widthsize) << "Size";
		for (auto & f : (*this)) {
			std::string name = f.name;
			CutString(name, widthsize);
			std::string spath = f.path;
			CutString(spath, widthsize);
			std::cout << std::endl << std::left << std::setw(widthsize) << name
				<< std::setw(widthsize) << spath
				<< std::setw(widthsize) << f.strDate
				<< std::setw(widthsize) << f.size;
		}
		printf("\n");
	}

	void FileList::SortByDate(bool desc){
		if (!desc) {
			std::sort(begin(), end(), [](const FileStruct & left, const FileStruct & right) {
				if (left.ullDate < right.ullDate)
					return true;
				return false;
			});
		}
		else {
			std::sort(begin(), end(), [](const FileStruct & left, const FileStruct & right) {
				if (left.ullDate > right.ullDate)
					return true;
				return false;
			});
		}
	}

	void FileList::SortByName(bool desc){
		if (!desc) {
			std::sort(begin(), end(), [](const FileStruct & left, const FileStruct & right) {
				if (ToLower(left.name) < ToLower(right.name))
					return true;
				return false;
			});
		}
		else {
			std::sort(begin(), end(), [](const FileStruct & left, const FileStruct & right) {
				if (ToLower(left.name) > ToLower(right.name))
					return true;
				return false;
			});
		}
	}

	void FileList::SortByFullName(bool desc){
		if (!desc) {
			std::sort(begin(), end(), [](const FileStruct & left, const FileStruct & right) {
				if (ToLower(left.path + "\\" + left.name) < ToLower(right.path + "\\" + right.name))
					return true;
				return false;
			});
		}
		else {
			std::sort(begin(), end(), [](const FileStruct & left, const FileStruct & right) {
				if (ToLower(left.path + "\\" + left.name) > ToLower(right.path + "\\" + right.name))
					return true;
				return false;
			});
		}
	}

	void FileList::SortBySize(bool desc){
		if (!desc) {
			std::sort(begin(), end(), [](const FileStruct & left, const FileStruct & right) {
				if (left.size < right.size)
					return true;
				return false;
			});
		}
		else {
			std::sort(begin(), end(), [](const FileStruct & left, const FileStruct & right) {
				if (left.size > right.size)
					return true;
				return false;
			});
		}
	}

	bool FileList::RecursiveSearch(const std::wstring & lpSearch) {

		TCHAR szPath[MAX_PATH + 1];
		//TCHAR dirPath[MAX_PATH + 1];
		WIN32_FIND_DATA fdFindData;
		HANDLE hFind;
		lstrcpy(szPath, lpSearch.data());
		lstrcat(szPath, L"*");
		TCHAR * CONST lpLastChar = szPath + lstrlen(szPath) - 1;
		hFind = FindFirstFile(szPath, &fdFindData);

		if (INVALID_HANDLE_VALUE == hFind) {
			FindFirstFileFailed(szPath);
			return false;
		}

		*lpLastChar = '\0';

		do {
			if ((0 == lstrcmp(fdFindData.cFileName, L".")) ||
				(0 == lstrcmp(fdFindData.cFileName, L".."))) {
				continue;
			}
			lstrcat(szPath, fdFindData.cFileName);
			if (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (isRecSearch) {
					lstrcat(szPath, L"\\");
					RecursiveSearch(szPath);
				}
			}
			else {
				AddFile(fdFindData, lpSearch);
			}
			*lpLastChar = '\0';
		} while (FindNextFile(hFind, &fdFindData));

		FindClose(hFind);

		return true;
	}

	void FileList::ArgErrorMessage(const std::string & pattern, const std::string & arg)
	{
		ErrorMessage = ErrorMessage == "" ? ErrorMessage : ErrorMessage + "\n";
		char mess[500];
		sprintf(mess, pattern.data(), arg.data());
		ErrorMessage += std::string(mess);
	}

	void FileList::CutString(std::string & str, int maxwidth)
	{
		if (str.size() >= maxwidth) {
			str[maxwidth - 4] = '.';
			str[maxwidth - 3] = '.';
			str[maxwidth - 2] = '.';
			str.resize(maxwidth - 1);
		}
	}

	void FileList::CopyHelper(const FileList & right){
		lpPath = right.lpPath;
		BadInit = right.BadInit;
		codePage = right.codePage;
		ErrorMessage = right.ErrorMessage;
		isRecSearch = right.isRecSearch;
	}
	
	VOID FileList::FindFirstFileFailed(LPWSTR CONST szPath) {
		setlocale(LC_ALL, "Rus");
		fwprintf(stderr, L"FindFirstFile() failed, GetLastError() = %d, szPath = %s\r\n",
			GetLastError(), szPath);
		setlocale(LC_ALL, "C");
	}
	
	void FileList::DestructorHelper() {
		//if (lpPath != nullptr)
		//	delete [] lpPath;
	}

	void FileList::AddFile(WIN32_FIND_DATA & w32fd, const std::wstring & path)
	{
		FileStruct newFS;
		SYSTEMTIME st;
		std::string name;
		newFS.ullDate = (((ULONGLONG)w32fd.ftLastWriteTime.dwHighDateTime) << 32) + w32fd.ftLastWriteTime.dwLowDateTime;
		newFS.strDate = "";
		if (FileTimeToSystemTime(&(w32fd.ftLastWriteTime), &st)) {
			newFS.strDate = DateTime(st);
		}
		newFS.name = any_encode(w32fd.cFileName, codePage);
		newFS.path = any_encode(path, codePage);
		newFS.size = w32fd.nFileSizeLow;
		this->push_back(newFS);
	}

	unsigned long long GetMSecDateTime(SYSTEMTIME & st, bool add_msec)
	{
		
		return
			st.wYear * 365.25 * 24 * 60 * 60 * add_msec ? 1000 : 1; // +
			//st.w
	}

	std::string DateTime(SYSTEMTIME & st, bool add_msec) {
		char dt[150];
		char day[3];
		char month[3];
		char hour[3];
		char minute[3];
		char second[3];
		st.wDay > 9 ? sprintf(day, "%d\0", st.wDay) : sprintf(day, "0%d\0", st.wDay);
		st.wMonth > 9 ? sprintf(month, "%d\0", st.wMonth) : sprintf(month, "0%d\0", st.wMonth);
		st.wHour > 9 ? sprintf(hour, "%d\0", st.wHour) : sprintf(hour, "0%d\0", st.wHour);
		st.wMinute > 9 ? sprintf(minute, "%d\0", st.wMinute) : sprintf(minute, "0%d\0", st.wMinute);
		st.wSecond > 9 ? sprintf(second, "%d\0", st.wSecond) : sprintf(second, "0%d\0", st.wSecond);
		add_msec ?
			sprintf(dt, "%s/%s/%d %s:%s:%s:%d\0", day, month, st.wYear, hour, minute, second, st.wMilliseconds):
			sprintf(dt, "%s/%s/%d %s:%s:%s\0", day, month, st.wYear, hour, minute, second);
		return std::string(dt);
	}

	LPWSTR any_decode(const char * str, int codepage)
	{
		if (strcmp(str, "") == 0) return (LPWSTR)(L"");
		int size_needed = MultiByteToWideChar(codepage, 0, &str[0], static_cast<int>(strlen(str)), NULL, 0);
		LPWSTR wstrTo = new WCHAR[size_needed + 1];
		MultiByteToWideChar(codepage, 0, &str[0], static_cast<int>(strlen(str)), &wstrTo[0], size_needed);
		wstrTo[size_needed] = '\0';
		return wstrTo;
	}

	std::string any_encode(const std::wstring & wstr, int codepage) {
		if(wstr == L"")
			return "";
		int size_needed = WideCharToMultiByte(codepage, 0, &wstr[0], wstr.size(), NULL, 0, NULL, NULL);
		char * strTo = new char[size_needed + 1];
		WideCharToMultiByte(codepage, 0, &wstr[0], wstr.size(), &strTo[0], size_needed, NULL, NULL);
		strTo[size_needed] = '\0';
		std::string rval(strTo);
		delete[] strTo;
		return rval;
	}

	std::wstring ToLower(const std::wstring & str) {
		std::wstring rstr;
		std::transform(str.begin(), str.end(), rstr.begin(), towlower);
		return rstr;
	}

	std::string ToLower(const std::string & str) {
		std::string rstr;
		//std::transform(str.begin(), str.end(), rstr.begin(), tolower); //ЗАКЛАДКА : ВЫБРОС ИСКЛЮЧЕНИЯ
		for (const auto & s : str) {
			rstr.push_back(tolower(s));
		}
		return rstr;
	}

}










