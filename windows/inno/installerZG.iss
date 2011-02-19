[Files]
Source: KeyMagic.exe; DestDir: {app}; Flags: uninsrestartdelete 64bit; 
Source: KeyMagic.ini; DestDir: {app};
Source: KeyMagicDll.dll; DestDir: {app}; Flags: uninsrestartdelete 64bit; 
Source: SetElevatedStartupTask.exe; DestDir: {app};
Source: "Keyboards\Zawgyi L - Unicode.km2"; DestDir: {app}\Keyboards; 
Source: "Keyboards\Zawgyi L.km2"; DestDir: {app}\Keyboards;
Source: mm3.ttf; DestDir: {fonts}; FontInstall: Myanmar3; Flags: uninsneveruninstall onlyifdoesntexist;
Source: Parabaik_UTN11-3.ttf; DestDir: {fonts}; FontInstall: Parabaik; Flags: uninsneveruninstall onlyifdoesntexist;
Source: Yunghkio.ttf; DestDir: {fonts}; FontInstall: Yungkio; Flags: uninsneveruninstall onlyifdoesntexist;

[Setup]
AppCopyright=Thant Thet Khin Zaw
AppName=KeyMagic
AppVerName=1.3.3
DefaultDirName={pf}\KeyMagic
DefaultGroupName=KeyMagic
PrivilegesRequired=poweruser
SetupLogging=true
AppID={{A528DC93-F421-4DFC-92CE-35EAC9E1AC42}
SolidCompression=true
Compression=lzma/Ultra64
InternalCompressLevel=Ultra64
UninstallLogMode=new
OutputBaseFilename=KeyMagic1.3-ZG-Fonts
VersionInfoVersion=1.3.3
VersionInfoCompany=Thant Thet Khin Zaw

[Run]
Filename: {app}\SetElevatedStartupTask.exe; WorkingDir: {app}; Flags: RunHidden 32bit; MinVersion: 0,6.0.6000; 
Filename: {app}\KeyMagic.exe; WorkingDir: {app}; Flags: NoWait; Tasks: runkm; 

[Registry]
Root: HKCU; SubKey: Software\Microsoft\Windows\Run; ValueType: string; ValueName: KeyMagic; ValueData: {app}\KeyMagic; Flags: UninsDeleteValue; OnlyBelowVersion: 0,6.0.6000; 

[UninstallRun]
MinVersion: 0,6.0.6000; Filename: {app}\SetElevatedStartupTask.exe; Parameters: -d; WorkingDir: {app}; Flags: RunHidden 32bit;
Filename: {app}\KeyMagic.exe; Parameters: -u; WorkingDir: {app}; 

[Tasks]
Name: runkm; Description: "Run KeyMagic after installation is completed"; 

[Icons]
Name: {group}\KeyMagic; Filename: {app}\KeyMagic.exe; IconFilename: {app}\KeyMagic.exe; IconIndex: 0; 
Name: {group}\Uninstall; Filename: {uninstallexe};
Name: "{group}\{cm:UninstallProgram, KeyMagic}"; Filename: {uninstallexe};
