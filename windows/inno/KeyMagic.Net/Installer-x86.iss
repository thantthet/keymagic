#include "Installer.iss"

[Setup]
OutputBaseFilename=KeyMagic-Installer-32bit

[Files]
Source: .\vendor/vcredist_x86.exe; DestDir: {tmp}

[Run]
Filename: {tmp}\vcredist_x86.exe; Parameters: "/passive /Q:a /c:""msiexec /qb /i vcredist.msi"" "; StatusMsg: Installing 2010 32bit RunTime...
