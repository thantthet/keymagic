#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <windows.h>
#include "stdafx.h"

#include "parser.h"

//wchar_t * source = _T("$bb = \"BB\"\r\n")
//_T("$BB = \"BB\"\r\n")
//_T("$b = \"B\"\r\n")
//_T("$a = \"A\"\r\n")
//_T("$aa = $a+$a\r\n")
//_T("$aaa = $aa+$a\r\n")
//_T("$aaaa = $a+$a+$a+$a\r\n")
//_T("$aa[*] & $b => $2 + u1000 + $1\r\n")
//_T("$bb & $a[*] => $1 + u2000 + $2\r\n")
//_T("$aa[*] + $b => $1 + $2\r\n")
//_T("$aa + $b[*] => $1 + $2\r\n")
//_T("\"BBB\" => $bb");

wchar_t * source = L"$thant = \"Thant \"\r\n"
L"$thet = \"Thet \"\r\n"
L"$khin = \"Khin \"\r\n"
L"$zaw = \"Zaw \"\r\n"
L"$thant + \"T\" => $thant + $thet\r\n"
L"\"T\" => $thant\r\n"
L"$thant + $thet + \"K\" => $thant + $thet + $khin\r\n"
L"$thant + $thet + $khin + \"Z\" => $thant + $thet + $khin + $zaw\r\n"
L"$thant + $thet + $khin + $zaw + VK_BACK => $thant + $thet + $khin";

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