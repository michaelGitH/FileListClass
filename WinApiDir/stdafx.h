// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

typedef std::string String;
typedef std::wstring WString;
typedef std::vector<String> sVector;
typedef std::map<String, String> c_map;
typedef std::pair<String, String> c_pair;

// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
