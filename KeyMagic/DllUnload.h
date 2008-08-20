#include <Windows.h>
#include <Psapi.h>

void Scanner ( );
void GetModules( DWORD );
void UnloadDLL ( char *DLLPath, HANDLE hProcess );
