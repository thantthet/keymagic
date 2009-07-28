#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <windows.h>
#include "stdafx.h"

#include "parser.h"

wchar_t * source = 
L"$alphabet_ansi = 'uc*CipqZ]n#X!%Ewx\\'\"eyzAbr,&v0o[Vt'\n"
L"$alphabet_uni = u1000 + u1001 + u1002 + u1003 + u1004 + u1005 + u1006 + u1007 + /*u1008 +*/ u1009 + u100A + u100B + u100C + u100D + u100E + u100F + u1010 + u1011 + u1012 + u1013 + u1014 + u1015 + u1016 + u1017 + u1018 + u1019 + u101A + u101B + u101C + u101D + u101E + u101F + u1020 + u1021\n"
L"$alphabet_ansi[*] => $alphabet_uni[$1]\n"
L"$numeric_ansi = '123456789'\n"
L"$numeric_uni = u1041 + u1042 + u1043 + u1044 + u1045 + u1046 + u1047 + u1045 + u1046 + u1047 + u1048 + u1049\n"
L"$numeric_ansi[*] => $numeric_uni[$1]\n"
L"VK_KEY_Q => u103f + u1001\n"
L"VK_KEY_W => u103f + u1010\n"
L"VK_KEY_E => u103f + u100f\n"
L"VK_KEY_U => u103f + u1000\n"
L"VK_KEY_P => u103f + u1005\n"
L"'}' => u103f + u1007\n"
L"VK_KEY_B => u103f + u1018\n"
L"VK_KEY_N => u103f + u1012\n"
L"VK_KEY_M => u103f + u1014\n"
L"'<' => u103f + u1013\n"
L"'>' => u103f + u1006\n"
L"'|' => u103f + u1017\n"
L"'@' => u103f + u1011\n";

//wchar_t * source = 
//L"$consonant_uni = u1000 + u1001 + u1002\r\n"
//L"$consonant_ansi = 'uc*'\r\n"
//L"$consonant_ansi[*] => $consonant_uni[$1]\r\n";

//wchar_t * source = L"$thant = \"Thant \"\r\n"
//L"$thet = \"Thet \"\r\n"
//L"$khin = \"Khin \"\r\n"
//L"$zaw = \"Zaw \"\r\n"
//L"$thant + \"T\" => $1 + $thet\r\n"
//L"\"T\" => $thant\r\n"
//L"$thant + $thet + \"K\" => $1 + $2 + $khin\r\n"
//L"$thant + $thet + $khin + \"Z\" => $1 + $2 + $3 + $zaw\r\n"
//L"$thant + $thet + $khin + $zaw + VK_BACK => ''";

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3)
	{
		std::wcerr << L"Usage: " << argv[0] << L" <script file path> <output file path>";
		return false;
	}
	std::wstring buf;
	std::wstring line;
	std::wifstream in (argv[1]);

	if (in==0)
	{
		std::cerr << "File cannot be opened!";
		return 0;
	}
	
	while(std::getline(in,line))
	{
		buf += line;
		buf += L"\r\n";
	}

	parser p(buf.c_str());
	p.begin_parse();
	p.generate(argv[2]);
	//Kmklf kmklf;
	//kmklf.fromFile("C:\\test.bin");
	//kmklf.toFile("C:\\test2.bin");
	system("pause");
	return 0;
}