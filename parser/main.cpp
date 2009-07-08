#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <windows.h>
#include "stdafx.h"

#include "parser.h"

wchar_t * source = 
L"$consonant_uni = u1000\r\n"
L"$consonant_ansi = 'a'\r\n"
L"$consonant_uni[*] = $consonant_ansi[*]\r\n";

//wchar_t * source = L"$thant = \"Thant \"\r\n"
//L"$thet = \"Thet \"\r\n"
//L"$khin = \"Khin \"\r\n"
//L"$zaw = \"Zaw \"\r\n"
//L"$thant + \"T\" => $thant + $thet\r\n"
//L"\"T\" => $thant\r\n"
//L"$thant + $thet + \"K\" => $thant + $thet + $khin\r\n"
//L"$thant + $thet + $khin + \"Z\" => $thant + $thet + $khin + $zaw\r\n"
//L"$thant + $thet + $khin + $zaw + VK_BACK => $thant + $thet + $khin";

int _tmain(int argc, _TCHAR* argv[])
{
	parser p(source);
	p.begin_parse();
	p.generate("C:\\test.bin");
	//Kmklf kmklf;
	//kmklf.fromFile("C:\\test.bin");
	//kmklf.toFile("C:\\test2.bin");
	system("pause");
	return 0;
}