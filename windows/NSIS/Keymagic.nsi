;Define application version
!define VERSION "1.3"

; Define the application name
!define APPNAME "KeyMagic"
!define APPNAMEANDVERSION "${APPNAME} ${VERSION}"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\${APPNAME}"
OutFile "${APPNAMEANDVERSION}.exe"

; Modern interface settings
	!include "MUI.nsh"
	
	;!define MUI_ICON ".\Keymagic.ico"
	
	!define MUI_WELCOMEFINISHPAGE_BITMAP ".\GUI\Welcome.bmp"
	!define MUI_WELCOMEFINISHPAGE_BITMAP_NOSTRETCH

	!define MUI_HEADERIMAGE
	!define MUI_HEADERIMAGE_RIGHT
	!define MUI_HEADERIMAGE_BITMAP ".\GUI\Head.bmp" ; optional
	!define MUI_ABORTWARNING

	!insertmacro MUI_PAGE_WELCOME
	!insertmacro MUI_PAGE_LICENSE ".\GNU LICENSE"
	!insertmacro MUI_PAGE_DIRECTORY
	!insertmacro MUI_PAGE_COMPONENTS
	!insertmacro MUI_PAGE_INSTFILES
	!insertmacro MUI_PAGE_FINISH

	!insertmacro MUI_UNPAGE_COMPONENTS
	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES
	!insertmacro MUI_UNPAGE_FINISH

	!insertmacro MUI_LANGUAGE "English"

Section "" mainSection
	SectionIn 1 2
	SetOverwrite on

	UserInfo::GetAccountType
	Pop $1
	StrCmp $1 "Admin" 0 +2
	
	SetShellVarContext all
	
	Delete "$DESKTOP\KeyMagic.lnk"
	Delete "$SMPROGRAMS\${APPNAME}\KeyMagic.lnk"
	;Delete "$SMPROGRAMS\${APPNAME}\Keyboard Maker.lnk"
	;Delete "$SMPROGRAMS\${APPNAME}\User Manual.pdf"
	Delete "$SMPROGRAMS\${APPNAME}\Uninstall.lnk"
	
	; add all the shortcuts for all user or current user
	CreateDirectory "$SMPROGRAMS\${APPNAME}\"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
	
	;SetOutPath "$INSTDIR\Docs\"
	;File ".\doc\Authors.txt"
	;File ".\doc\Changelog.txt"
	;File ".\doc\GPL.txt"
	;File ".\doc\License.txt"
	;File ".\doc\User Manual MM.pdf"
	;File ".\doc\User Manual EN.pdf"
	;CreateShortCut "$SMPROGRAMS\${APPNAME}\User Manual.lnk" "$INSTDIR\Docs\User Manual MM.pdf"
	;CreateShortCut "$SMPROGRAMS\${APPNAME}\User Manual.lnk" "$INSTDIR\Docs\User Manual EN.pdf"
	
	SetShellVarContext current
	
	WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\KeyMagic.exe" "" "$INSTDIR\KeyMagic.exe"
	
SectionEnd

SubSection "KeyMagic" PackageKeyMagic
	SetOverwrite on
	
	Section "KeyMagic Smart IME" KeyMagic
	UserInfo::GetAccountType
	Pop $1
	StrCmp $1 "Admin" 0 +2
	
	SetShellVarContext all
	
	SetOutPath "$INSTDIR\"
	File ".\KeyMagic.exe"
	File ".\KeyMagicDll.dll"
	
	File ".\SetElevatedStartupTask.exe"
	File /oname=KeyMagic.ini Normal.ini
	
	;SetOutPath "$INSTDIR\Icons\"
	;File ".\GUI\Keymap.ico"	
	
	File /oname=$TEMP\nsisos.dll nsisos.dll
	CallInstDLL $TEMP\nsisos.dll osversion
	StrCpy $R0 $0
	${If} $R0 >= "6"
		Exec '"$INSTDIR\SetElevatedStartupTask.exe"'
	${Else}
		WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "KeyMagic" "$INSTDIR\KeyMagic.exe -s"
	${EndIf}
	Delete $TEMP\nsisos.dll
	
	CreateDirectory "$SMPROGRAMS\${APPNAME}\"
	CreateDirectory "$APPDATA\KeyMagic\Keyboards\"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\KeyMagic.lnk" "$INSTDIR\KeyMagic.exe" "-s"
	
	CreateShortCut "$DESKTOP\KeyMagic.lnk" "$INSTDIR\KeyMagic.exe" "-s"
	
	;WriteRegStr HKCR ".km2" "" "KeyMagic.Keymap.File"
	;WriteRegStr HKCR "KeyMagic.Keymap.File" "" "KeyMagic Keymap"
	;WriteRegStr HKCR "KeyMagic.Keymap.File\DefaultIcon" "" "$INSTDIR\Icons\Keymap.ico"
	;WriteRegStr HKCR "KeyMagic.Keymap.File\shell\Install\command" "" "$INSTDIR\KeyMagic.exe -i %1"
	SectionEnd
	
	;Section "Keymapper for KeyMagic's Keymap file" Keymapper
	;UserInfo::GetAccountType
	;Pop $1
	;StrCmp $1 "Admin" 0 +2
	
	;SetShellVarContext all
	
	;SetOverwrite on
	;SetOutPath "$INSTDIR\"
	;File ".\Keymapper.exe"
	;SetOutPath "$INSTDIR\Icons\"
	;File ".\GUI\Keyscript.ico"
	
	;CreateDirectory "$SMPROGRAMS\${APPNAME}\"
	;CreateShortCut "$SMPROGRAMS\${APPNAME}\Keyboard Maker.lnk" "$INSTDIR\Keymapper.exe"
	
	;WriteRegStr HKCR ".kms" "" "KeyMagic.Keyscript.File"
	;WriteRegStr HKCR "KeyMagic.Keyscript.File" "" "KeyMagic Keymap Script"
	;WriteRegStr HKCR "KeyMagic.Keyscript.File\DefaultIcon" "" "$INSTDIR\Icons\Keyscript.ico"
	;WriteRegStr HKCR "KeyMagic.Keyscript.File\shell\Make Keymap File\command" "" "$INSTDIR\Keymapper.exe -c %1"
	;WriteRegStr HKCR "KeyMagic.Keyscript.File\shell\Open\command" "" "$INSTDIR\Keymapper.exe -o %1"
	;SectionEnd
	
	Section "Keyboard Layout files" Keymap
	SetOverwrite on
	SetOutPath "$INSTDIR\Keyboards\"
	File ".\Keyboards\Zawgyi L.km2"
	File ".\Keyboards\Zawgyi L - Unicode.km2"
	;File ".\Keyboards\Myanmar3.km2"
	;File ".\Keyboards\Parabaik.km2"
	;File ".\Keyboards\Uniburma.km2"
	;File ".\Keyboards\Zawgyi.km2"
	SectionEnd
	
	;Section "Keyboard Script files" Keyscript
	;SetOverwrite on
	;SetOutPath "$INSTDIR\Keyscripts\"
	;File ".\Keyscript\Myanmar3.kms"
	;File ".\Keyscript\Parabaik.kms"
	;File ".\Keyscript\UniBurma.kms"
	;File ".\Keyscript\Zawgyi.kms"
	;SectionEnd
	
SubSectionEnd

Section /o "Portable" Portable
	SetOverwrite on
	SetOutPath "$INSTDIR\"
	File /oname=KeyMagic.ini Portable.ini
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  
  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${PackageKeyMagic} 'KeyMagic Smart IME.'
	!insertmacro MUI_DESCRIPTION_TEXT ${KeyMagic} 'KeyMagic Smart IME.'
	;!insertmacro MUI_DESCRIPTION_TEXT ${Keymapper} 'KeyMagic Keymapper.'
	!insertmacro MUI_DESCRIPTION_TEXT ${Keymap} 'Keyboard Layout files.'
	;!insertmacro MUI_DESCRIPTION_TEXT ${Keyscript} 'Keyboard Script files.'
	!insertmacro MUI_DESCRIPTION_TEXT ${Portable} 'Make KeyMagic portable.'
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

;Uninstall section

SubSection un.KeyMagic

	Section un.KeyMagic
	UserInfo::GetAccountType
	Pop $1
	StrCmp $1 "Admin" 0 +2
	
	SetShellVarContext all
	
	;Processes::FindProcess "KeyMagic"
	;Pop $R0
	
	;StrCmp $R0 "1" running notrunning
	
	;running:
	
	;Processes::KillProcess "KeyMagic"
	
	Exec '"$INSTDIR\KeyMagic.exe" -u'
	Sleep "3000"
	
	;notrunning:
	
	Delete "$INSTDIR\KeyMagic.exe"
	Delete "$INSTDIR\KeyMagicDll.dll"
	Delete "$INSTDIR\KeyMagic.ini"
	;Delete "$INSTDIR\Icons\Keymap.ico"
	;RMDir  "$INSTDIR\Icons\"
	;Delete "$INSTDIR\Docs\Authors.txt"
	;Delete "$INSTDIR\Docs\Changelog.txt"
	;Delete "$INSTDIR\Docs\GPL.txt"
	;Delete "$INSTDIR\Docs\License.txt"
	;Delete "$INSTDIR\Docs\User Manual MM.pdf"
	;Delete "$INSTDIR\Docs\User Manual EN.pdf"
	;RMDir  "$INSTDIR\Docs\"
	Delete "$INSTDIR\Keyboards\Zawgyi L.km2"
	Delete "$INSTDIR\Keyboards\Zawgyi L - Unicode.km2"
	;Delete "$INSTDIR\Keyboards\Myanmar3.km2"
	;Delete "$INSTDIR\Keyboards\Parabaik.km2"
	;Delete "$INSTDIR\Keyboards\Uniburma.km2"
	;Delete "$INSTDIR\Keyboards\Zawgyi.km2"
	RMDir  "$INSTDIR\Keyboards"
	;Delete "$INSTDIR\Keyscripts\Myanmar3.kms"
	;Delete "$INSTDIR\Keyscripts\Parabaik.kms"
	;Delete "$INSTDIR\Keyscripts\Uniburma.kms"
	;Delete "$INSTDIR\Keyscripts\Zawgyi.kms"
	;RMDir  $INSTDIR\Keyscripts"
	Delete "$INSTDIR\SetElevatedStartupTask.exe"
	RMDir  "$INSTDIR\"
	
	DeleteRegValue HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "KeyMagic"
	
	Delete "$SMPROGRAMS\${APPNAME}\KeyMagic.lnk"
	Delete "$DESKTOP\KeyMagic.lnk"
	
	DeleteRegKey HKCR ".km2"
	DeleteRegKey HKCR "KeyMagic.Keymap.File"
	SectionEnd

	;Section un.Keymapper
	;UserInfo::GetAccountType
	;Pop $1
	;StrCmp $1 "Admin" 0 +2
	
	;SetShellVarContext all
	
	;Delete "$INSTDIR\Keymapper.exe"
	;Delete "$INSTDIR\Icons\Keyscript.ico"
	;RMDir  "$INSTDIR\Icons\"
	;RMDir  /r "$INSTDIR\"
	
	;Delete "$SMPROGRAMS\${APPNAME}\Keyboard Maker.lnk"
	
	;DeleteRegKey HKCR ".kms"
	;DeleteRegKey HKCR "KeyMagic.Keyscript.File"
	;SectionEnd

SubSectionEnd

Section un.
	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\KeyMagic.exe" 
	
	; Delete self
	Delete "$INSTDIR\uninstall.exe"
	
	UserInfo::GetAccountType
	Pop $1
	StrCmp $1 "Admin" 0 +2
	SetShellVarContext all
	
	; Delete Shortcuts	
	Delete "$SMPROGRAMS\${APPNAME}\Uninstall.lnk"
	Delete "$SMPROGRAMS\${APPNAME}\User Manual.lnk"
	
	; Remove remaining directories
	RMDir "$SMPROGRAMS\KeyMagic"
	RMDir "$INSTDIR\"
	
SectionEnd

BrandingText "${APPNAMEANDVERSION}"
