#include <windows.h>
#include <string>
#include <vector>
#include <list>

using namespace std;

typedef vector<wstring> KEYBOARD_LIST;

KEYBOARD_LIST * GetKeyboards();
BOOL MapVK (UINT inVK, UINT * VKey, UINT * Char);