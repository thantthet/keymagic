#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <windows.h>
#include <wchar.h> 
#include <shlwapi.h>
#include "stdafx.h"

#include "parser.h"

#include "UniConversion.h"

//wchar_t * source = 
//L"$alphabet_ansi = 'uc*CipqZ]n#X!%Ewx\\'\"eyzAbr,&v0o[Vt'\n"
//L"$alphabet_uni = u1000 + u1001 + u1002 + u1003 + u1004 + u1005 + u1006 + u1007 + /*u1008 +*/ u1009 + u100A + u100B + u100C + u100D + u100E + u100F + u1010 + u1011 + u1012 + u1013 + u1014 + u1015 + u1016 + u1017 + u1018 + u1019 + u101A + u101B + u101C + u101D + u101E + u101F + u1020 + u1021\n"
//L"$alphabet_ansi[*] => $alphabet_uni[$1]\n"
//L"$numeric_ansi = '123456789'\n"
//L"$numeric_uni = u1041 + u1042 + u1043 + u1044 + u1045 + u1046 + u1047 + u1045 + u1046 + u1047 + u1048 + u1049\n"
//L"$numeric_ansi[*] => $numeric_uni[$1]\n"
//L"VK_KEY_Q => u103f + u1001\n"
//L"VK_KEY_W => u103f + u1010\n"
//L"VK_KEY_E => u103f + u100f\n"
//L"VK_KEY_U => u103f + u1000\n"
//L"VK_KEY_P => u103f + u1005\n"
//L"'}' => u103f + u1007\n"
//L"VK_KEY_B => u103f + u1018\n"
//L"VK_KEY_N => u103f + u1012\n"
//L"VK_KEY_M => u103f + u1014\n"
//L"'<' => u103f + u1013\n"
//L"'>' => u103f + u1006\n"
//L"'|' => u103f + u1017\n"
//L"'@' => u103f + u1011\n";

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

unsigned char UTF32_LE[4] = { 0xFE, 0xFF, 0x00, 0x00 };
unsigned char UTF32_BE[4] = { 0x00, 0x00, 0xFE, 0xFF };
unsigned char UTF16_LE[2] = { 0xFF, 0xFE };
unsigned char UTF16_BE[2] = { 0xFE, 0xFF };
unsigned char UTF8[3] = { 0xEF, 0xBB, 0xBF };

int _tmain(int argc, _TCHAR* argv[])
{
	int length;
	int uni_length;

	char * buffer;
	wchar_t * uni_buffer;

	if (argc < 3){
		std::wcerr << L"Usage: " << argv[0] << L" <script file path> <output file path>";
		return false;
	}

	wchar_t sz_curdir[MAX_PATH];
	wchar_t sz_filein[MAX_PATH];
	wchar_t sz_fileout[MAX_PATH];

	GetCurrentDirectory(MAX_PATH,sz_curdir);
	wcscpy(sz_filein, argv[1]);
	wcscpy(sz_fileout, argv[2]);

	if (PathIsRelative(sz_filein)){
		wcscpy(sz_filein, sz_curdir);
		PathAppend(sz_filein, argv[1]);
	}
	if (PathIsRelative(sz_fileout)){
		wcscpy(sz_fileout, sz_curdir);
		PathAppend(sz_fileout, argv[2]);
	}

	ifstream is;
	is.open (sz_filein, ios::binary );
	if (!is.is_open()){
		cout << "Error opening file...";
		return 0;
	}

	// get length of file:
	is.seekg (0, ios::end);
	length = is.tellg();
	is.seekg (0, ios::beg);

	// allocate memory:
	buffer = new char [length];

	// read data as a block:
	is.read (buffer, length);
	is.close();

	if ( memcmp(buffer, UTF16_LE, sizeof(UTF16_LE))==0 ){
		uni_length = (length-sizeof(UTF16_LE)) / sizeof(short);
		uni_buffer = new wchar_t[uni_length+1];
		memcpy(uni_buffer, &buffer[sizeof(UTF16_LE)], length-sizeof(UTF16_LE));
	}

	else if ( memcmp(buffer, UTF16_BE, sizeof(UTF16_BE))==0 ){
		uni_length = (length-sizeof(UTF16_LE)) / sizeof(short);
		uni_buffer = new wchar_t[uni_length+1];
		memcpy(uni_buffer, &buffer[sizeof(UTF16_BE)], length-sizeof(UTF16_LE));

		for (int i=0; i < uni_length; i++) { 
			uni_buffer[i] = ((uni_buffer[i] & 0xFF00) >> 8) | ((uni_buffer[i] & 0x00FF) << 8) ;
		}
	}

	else if ( memcmp(buffer, UTF8, sizeof(UTF8))==0 ){
		uni_length = UTF8Length((wchar_t*)&buffer[sizeof(UTF8)], length-sizeof(UTF8));
		uni_buffer = new wchar_t[uni_length+1];
		UCS2FromUTF8(&buffer[sizeof(UTF8)], length-sizeof(UTF8), uni_buffer, uni_length); 
	}

	else{
		uni_length = UTF8Length((wchar_t*)buffer, length);
		uni_buffer = new wchar_t[uni_length+1];
		UCS2FromUTF8(buffer, length, uni_buffer, uni_length); 
	}

	//uni_buffer[uni_length] = 0;
	parser p(uni_buffer);
	if (p.begin_parse()){
		p.generate(sz_fileout);
	}
	else{
		std::wcout << p.getLastError().c_str() << std::endl;
	}
	//Kmklf kmklf;
	//kmklf.fromFile("C:\\test.bin");
	//kmklf.toFile("C:\\test2.bin");
	system("pause");

//	delete[] buffer;
//	delete[] uni_buffer;
	return 0;
}
