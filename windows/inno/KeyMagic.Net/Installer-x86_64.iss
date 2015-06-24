#include "Installer.iss"

[Setup]
OutputBaseFilename=KeyMagic-Installer-64bit

[Files]
Source: .\vendor/vcredist_x86.exe; DestDir: {tmp}
Source: .\vendor/vcredist_x64.exe; DestDir: {tmp}

[Run]
Filename: {tmp}\vcredist_x86.exe; Parameters: "/passive /Q:a /c:""msiexec /qb /i vcredist.msi"" "; StatusMsg: Installing 2010 32bit RunTime...
Filename: {tmp}\vcredist_x64.exe; Flags: 64bit; Check: IsWin64; Parameters: "/passive /Q:a /c:""msiexec /qb /i vcredist.msi"" "; StatusMsg: Installing 2010 64bit RunTime...
