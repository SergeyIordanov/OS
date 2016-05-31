#pragma once
#include "stdafx.h"
#include<Windows.h>
#include <iostream>
using namespace std;

#define IHV INVALID_HANDLE_VALUE

#define GW GENERIC_WRITE

#define FSW FILE_SHARE_WRITE

#define OA OPEN_ALWAYS


#define CreateUnsuspendedProcess(path,directory, startupInfo, processInfo) CreateProcess(0, path, 0, 0, 0, 0, 0, directory, startupInfo, processInfo);

#define CreateSuspendedProcess(path,directory, startupInfo, processInfo) CreateProcess(0, path, 0, 0, 0, CREATE_SUSPENDED, 0, directory, startupInfo, processInfo);

#define MyCreateThread(threadFunc, value, id) CreateThread(0, 0, threadFunc, value, 0, id);

#define CreateFileForWrite(fname) CreateFile(fname, GW, FSW, 0, OA, 0, 0)
