#include <windows.h>

struct MenuCr{
	COLORREF Text;
	COLORREF Frame;
	COLORREF Fill;
};

void DrawMyButton(LPDRAWITEMSTRUCT lpdis);

extern MenuCr Up, Down, Hover;