#pragma once

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <comdef.h>
#include <wincred.h>
//  Include the task header file.
#include <taskschd.h>

//void RegisterTask(std::wstring executablePath);
void RegisterProgram();
void UnRegisterProgramForStartup(PCWSTR = L"KeyMagic");
BOOL IsRegisteredForStartup(PCWSTR = L"KeyMagic");