;Define application version
!define VERSION "1.1 Build 3"

; Define the application name
!define APPNAME "Keymagic"
!define APPNAMEANDVERSION "${APPNAME} ${VERSION}"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\${APPNAME}"
OutFile "${APPNAMEANDVERSION}.exe"

; Modern interface settings
	!include "MUI.nsh"
	
	;!define MUI_ICON ".\Keymagic.ico"
	
	!define MUI_WELCOMEFINISHPAGE_BITMAP ".\Welcome.bmp"
	!define MUI_WELCOMEFINISHPAGE_BITMAP_NOSTRETCH

	!define MUI_HEADERIMAGE
	!define MUI_HEADERIMAGE_RIGHT
	!define MUI_HEADERIMAGE_BITMAP ".\Head.bmp" ; optional
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
	
	Delete "$DESKTOP\Keymagic.lnk"
	Delete "$SMPROGRAMS\${APPNAME}\Keymagic.lnk"
	Delete "$SMPROGRAMS\${APPNAME}\Keymap Generator.lnk"
	Delete "$SMPROGRAMS\${APPNAME}\Help.lnk"
	Delete "$SMPROGRAMS\${APPNAME}\Uninstall.lnk"
	
	; add all the shortcuts for all user or current user
	CreateDirectory "$SMPROGRAMS\${APPNAME}\"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
	

	SetOutPath "$INSTDIR\Docs\"
	File ".\doc\Authors.txt"
	File ".\doc\Changelog.txt"
	File ".\doc\GPL.txt"
	File ".\doc\License.txt"
	File ".\Help.chm"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\Help.lnk" "$INSTDIR\Docs\Help.chm"
	
	SetShellVarContext current
	
	WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\Keymagic.exe" "" "$INSTDIR\Keymagic.exe"
	
SectionEnd

SubSection "Keymagic" PackageKeymagic
	SetOverwrite on
	
	Section "Keymagic Input Customizer" Keymagic
	UserInfo::GetAccountType
	Pop $1
	StrCmp $1 "Admin" 0 +2
	
	SetShellVarContext all
	
	SetOutPath "$INSTDIR\"
	File ".\Keymagic.exe"
	File ".\KeyMagicDll.dll"
	SetOverwrite off
	File ".\KeyMagic.ini"
	SetOverwrite on
	SetOutPath "$INSTDIR\Icons\"
	File ".\Keymap.ico"	
	
	CreateDirectory "$SMPROGRAMS\${APPNAME}\"
	CreateDirectory "$APPDATA\Keymagic\Keyboards\"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\Keymagic.lnk" "$INSTDIR\Keymagic.exe"
	
	CreateShortCut "$DESKTOP\Keymagic.lnk" "$INSTDIR\Keymagic.exe" "-s"
	
	WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "Keymagic" "$INSTDIR\Keymagic.exe -s"
	WriteRegStr HKCR ".kmk" "" "Keymagic.Keymap.File"
	WriteRegStr HKCR "Keymagic.Keymap.File" "" "Keymagic Keymap"
	WriteRegStr HKCR "Keymagic.Keymap.File\DefaultIcon" "" "$INSTDIR\Icons\Keymap.ico"
	WriteRegStr HKCR "Keymagic.Keymap.File\shell\Install\command" "" "$INSTDIR\KeyMagic.exe -i %1"
	SectionEnd
	
	Section "Keymapper for Keymagic's keymap file" Keymapper
	UserInfo::GetAccountType
	Pop $1
	StrCmp $1 "Admin" 0 +2
	
	SetShellVarContext all
	
	SetOverwrite on
	SetOutPath "$INSTDIR\"
	File ".\Keymapper.exe"
	SetOutPath "$INSTDIR\Icons\"
	File ".\Keyscript.ico"
	
	CreateDirectory "$SMPROGRAMS\${APPNAME}\"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\Keymap Generator.lnk" "$INSTDIR\Keymapper.exe"
	
	WriteRegStr HKCR ".kms" "" "Keymagic.Keyscript.File"
	WriteRegStr HKCR "Keymagic.Keyscript.File" "" "Keymagic Keymap Script"
	WriteRegStr HKCR "Keymagic.Keyscript.File\DefaultIcon" "" "$INSTDIR\Icons\Keyscript.ico"
	WriteRegStr HKCR "Keymagic.Keyscript.File\shell\Make Keymap File\command" "" "$INSTDIR\Keymapper.exe -c %1"
	WriteRegStr HKCR "Keymagic.Keyscript.File\shell\Open\command" "" "$INSTDIR\Keymapper.exe -o %1"
	SectionEnd
	
	Section "Keymaps Files" Keymap
	SetOverwrite on
	SetOutPath "$INSTDIR\Keyboards\"
	File ".\Keyboards\Myanmar3.kmk"
	File ".\Keyboards\Parabaik.kmk"
	File ".\Keyboards\Uniburma.kmk"
	File ".\Keyboards\Zawgyi.kmk"
	SectionEnd
	
	Section "Keyscripts Files" Keyscript
	SetOverwrite on
	SetOutPath "$INSTDIR\Keyscripts\"
	File ".\Keyscript\Myanmar3.kms"
	File ".\Keyscript\Parabaik.kms"
	File ".\Keyscript\UniBurma.kms"
	File ".\Keyscript\Zawgyi.kms"
	SectionEnd
	
SubSectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  
  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${PackageKeymagic} 'Keymagic Input Customizer, Keymagic Keymapper, Keymap files and Keyscript files'
	!insertmacro MUI_DESCRIPTION_TEXT ${Keymagic} 'Keymagic Input Customizer.'
	!insertmacro MUI_DESCRIPTION_TEXT ${Keymapper} 'Keymagic Keymapper.'
	!insertmacro MUI_DESCRIPTION_TEXT ${Keymap} 'Keymagic Keymap files.'
	!insertmacro MUI_DESCRIPTION_TEXT ${Keyscript} 'Keymagic Keyscript files.'
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

;Uninstall section

SubSection un.Keymagic

	Section un.Keymagic
	UserInfo::GetAccountType
	Pop $1
	StrCmp $1 "Admin" 0 +2
	
	SetShellVarContext all
	
	Delete "$INSTDIR\Keymagic.exe"
	Delete "$INSTDIR\KeyMagicDll.dll"
	Delete "$INSTDIR\KeyMagic.ini"
	Delete "$INSTDIR\Icons\Keymap.ico"
	RMDir  "$INSTDIR\Icons\"
	RMDir  "$INSTDIR\"
	
	DeleteRegValue HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Run" "Keymagic"
	
	Delete "$SMPROGRAMS\${APPNAME}\Keymagic.lnk"
	Delete "$DESKTOP\Keymagic.lnk"
	
	DeleteRegKey HKCR ".kmk"
	DeleteRegKey HKCR "Keymagic.Keymap.File"
	SectionEnd

	Section un.Keymapper
	UserInfo::GetAccountType
	Pop $1
	StrCmp $1 "Admin" 0 +2
	
	SetShellVarContext all
	
	Delete "$INSTDIR\Keymapper.exe"
	Delete "$INSTDIR\Icons\Keyscript.ico"
	RMDir  "$INSTDIR\Icons\"
	RMDir  "$INSTDIR\"
	
	Delete "$SMPROGRAMS\${APPNAME}\Keymap Generator.lnk"
	
	DeleteRegKey HKCR ".kms"
	DeleteRegKey HKCR "Keymagic.Keyscript.File"
	SectionEnd

SubSectionEnd

Section un.
	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\Keymagic.exe" 
	
	; Delete self
	Delete "$INSTDIR\uninstall.exe"
	
	UserInfo::GetAccountType
	Pop $1
	StrCmp $1 "Admin" 0 +2
	SetShellVarContext all
	
	; Delete Shortcuts	
	Delete "$SMPROGRAMS\${APPNAME}\Uninstall.lnk"
	
	; Remove remaining directories
	RMDir "$SMPROGRAMS\Keymagic"
	RMDir "$INSTDIR\"
	
SectionEnd

BrandingText "${APPNAMEANDVERSION}"
