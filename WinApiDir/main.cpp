// WinApiDir.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "WinApiDir.h"

using namespace winapins;

int main(int argc, char * argv[])
{

	if (argc == 1) {
		fprintf(stderr, "not enough parameters!\n");
		return 1;
	}

	std::vector<std::string> vec;
	for (int i = 1; i < argc; ++i) {
		vec.push_back(static_cast<std::string>(argv[i]));
	}

	FileList wdir(vec);

	std::vector<WIN32_FIND_DATA> arFindData;
	try {
		wdir.Search();
	}
	catch (FileListException & ex) {
		setlocale(LC_ALL, "Rus");
		fprintf(stderr, "FileListException: %s", ex.what());
		setlocale(LC_ALL, "C");
	}

	FileList wd2 = wdir;

	wd2.SortBySize(true);

	wd2.Print(35, true);

	system("pause");

	return 0;
}

