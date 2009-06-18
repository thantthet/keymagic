#include <iostream>
#include <vector>
#include <wchar.h>

using namespace std;

#define EOS 0xFFFF

class script
{
public:

	script (){};

	script (wchar_t * script)
	{
		loadScript(script);
	}

	void loadScript(wchar_t * script)
	{
		size_t input_length = wcslen(script);
		s = new wchar_t[input_length+2];
		wcscpy(s, script);
		s[input_length] = EOS;
		parse_lines();
	}

	int getLineNum(int pos)
	{
		int i ;
		for (i = 0; i < stLinePos.size() && stLinePos[i] < pos; i++){}
		return i;
	}

	int getPosLine(int pos)
	{
		return pos - stLinePos[getLineNum(pos)-1];
	}

	wchar_t wCharAt(int index)
	{
		if (index > length)
			return NULL;
		return s[index];
	}

	wchar_t * lpwCharAt(int index)
	{
		if (index > length)
			return NULL;
		return &s[index];
	}

private:
	wchar_t* s;
	vector<int> stLinePos;
	size_t length;

	void parse_lines()
	{
		int i = 0;

		stLinePos.push_back(i);

		while (s[i] != EOS)
		{
			if (s[i] == '\n')
			{
				i++;
				stLinePos.push_back(i);
			}
			else {i++;}
		}

		stLinePos.push_back(i);

		length = i;
	}
};
