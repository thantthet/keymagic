call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsMSBuildCmd.bat"
msbuild KeyMagic2.sln /t:Build /p:Configuration=Release
copy /B ".\x64\Release\KeyMagic2.exe" ".\..\inno\KeyMagic2\KeyMagic2.exe"
pause