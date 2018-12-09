// WinApiDir.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "WinApiDir.h"

int main(int argc, char * argv[])
{

	if (argc == 1) {
		fprintf(stderr, "too few parameters!\n");
		return 1;
	}

	keys["-R"] = "off"; //Рекурсивное шествие

	for (int i = 2; i < argc; ++i) {
		auto it = keys.find(String(argv[i]));
		if (it != keys.end() && it->first == String("-R")) { //Проверка ключей, не требующих значений
			it->second ="on";
			continue;
		}
		//Проверка ключей, требующих значений
		//if (it == keys.end()) {
		//	fprintf(stderr, "Invalid key! (%s)\n", argv[i]);
		//	return 1;
		//}
		//if (argc - i <= 1) {
		//	fprintf(stderr, "Missing key value! (%s)\n", argv[i]);
		//	return 1;
		//}
		//it->second = String(argv[++i]);
	}

	LPWSTR szPath = ansi_decode(argv[3]);

	std::vector<WIN32_FIND_DATA> arFindData;
	RecursiveSearch(szPath, arFindData);
	system("pause");

    return 0;
}

