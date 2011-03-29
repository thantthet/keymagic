[Files]
Source: KeyMagic.exe; DestDir: {app}; Flags: uninsrestartdelete; 
Source: KeyMagic.ini; DestDir: {app}; DestName: KeyMagic.ini; 
Source: KeyMagicDll.dll; DestDir: {app}; Flags: uninsrestartdelete; 
Source: SetElevatedStartupTask.exe; DestDir: {app};
Source: "Keyboards\Myanmar3.km2"; DestDir: {app}\Keyboards; 
Source: "Keyboards\myWin.km2"; DestDir: {app}\Keyboards; 
Source: "Keyboards\Parabaik.km2"; DestDir: {app}\Keyboards; 
Source: "Keyboards\Zawgyi L - Unicode.km2"; DestDir: {app}\Keyboards; 
Source: "Keyboards\Zawgyi L.km2"; DestDir: {app}\Keyboards;
Source: mm3.ttf; DestDir: {fonts}; FontInstall: Myanmar3; Flags: uninsneveruninstall onlyifdoesntexist;
Source: Parabaik_UTN11-3.ttf; DestDir: {fonts}; FontInstall: Parabaik; Flags: uninsneveruninstall onlyifdoesntexist;
Source: Yunghkio.ttf; DestDir: {fonts}; FontInstall: Yungkio; Flags: uninsneveruninstall onlyifdoesntexist;

[Setup]
AppCopyright=Thant Thet Khin Zaw
AppName=KeyMagic
AppVerName=1.4
DefaultDirName={pf}\KeyMagic
DefaultGroupName=KeyMagic
PrivilegesRequired=poweruser
SetupLogging=true
AppID={{BEADDC93-F421-4DFC-92CE-35EAC9E1AC42}
SolidCompression=true
Compression=lzma/Ultra64
InternalCompressLevel=Ultra64
UninstallLogMode=new
OutputBaseFilename=KeyMagic-1.4-Fonts
VersionInfoVersion=1.4
VersionInfoCompany=Thant Thet Khin Zaw

[Run]
Filename: {app}\KeyMagic.exe; WorkingDir: {app}; Flags: NoWait; Tasks: runkm; 

[UninstallRun]
MinVersion: 0,6.0.6000; Filename: {app}\SetElevatedStartupTask.exe; Parameters: -d; WorkingDir: {app}; Flags: RunHidden 32bit;
Filename: {app}\KeyMagic.exe; Parameters: -u; WorkingDir: {app}; 

[Tasks]
Name: runkm; Description: "Run KeyMagic after installation is completed"; 

[Icons]
Name: {group}\KeyMagic; Filename: {app}\KeyMagic.exe; IconFilename: {app}\KeyMagic.exe; IconIndex: 0; 
Name: {group}\Uninstall; Filename: {uninstallexe};
Name: "{group}\{cm:UninstallProgram, KeyMagic}"; Filename: {uninstallexe};
