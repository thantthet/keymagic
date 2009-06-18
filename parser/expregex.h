#include <wchar.h>

enum OP_CODE {
	
};

class expRegex {
public:
	expRegex(wchar_t* s);
	~expRegex();
private:
}

expRegex(wchar_t* s)
{
	while (*s != NULL)
	{
		switch (s)
		{
		case L'[':
			break;
		case L']':
			break;
		case L'(':
			break;
		case L')':
			break;
		}
	}
}