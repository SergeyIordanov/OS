#pragma once
#include "stdafx.h"
#include<Windows.h>
#include <iostream>
using namespace std;

#define CreateUnsuspendedProcess(path,directory, startupInfo, processInfo) CreateProcess(0, path, 0, 0, 0, 0, 0, directory, startupInfo, processInfo);
#define CreateSuspendedProcess(path,directory, startupInfo, processInfo) CreateProcess(0, path, 0, 0, 0, CREATE_SUSPENDED, 0, directory, startupInfo, processInfo);
#define IHV INVALID_HANDLE_VALUE