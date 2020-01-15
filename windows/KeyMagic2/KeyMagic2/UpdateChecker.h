#pragma once
#include "stdafx.h"

#include "Utils.h"

#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "version.lib")

struct UpdateChecker {
public:
	static bool ParseVersionString(unsigned digits[4], const std::string& i_version)
	{
		return sscanf_s(i_version.c_str(), "%d.%d.%d.%d", &digits[0], &digits[1],
			&digits[2], &digits[3]) == 4;
	}

	static void GetBinaryVersion(unsigned digits[4])
	{
		DWORD  verHandle = 0;
		UINT   size = 0;
		LPBYTE lpBuffer = NULL;
		TCHAR szVersionFile[255];
		GetModuleFileName(NULL, szVersionFile, 255);
		DWORD  verSize = GetFileVersionInfoSize(szVersionFile, &verHandle);

		if (verSize != NULL)
		{
			LPSTR verData = new char[verSize];

			if (GetFileVersionInfo(szVersionFile, verHandle, verSize, verData))
			{
				if (VerQueryValue(verData, _T("\\"), (VOID FAR * FAR*) & lpBuffer, &size))
				{
					if (size)
					{
						VS_FIXEDFILEINFO* verInfo = (VS_FIXEDFILEINFO*)lpBuffer;
						if (verInfo->dwSignature == 0xfeef04bd)
						{
							digits[0] = (verInfo->dwFileVersionMS >> 16) & 0xffff;
							digits[1] = (verInfo->dwFileVersionMS >> 0) & 0xffff;
							digits[2] = (verInfo->dwFileVersionLS >> 16) & 0xffff;
							digits[3] = (verInfo->dwFileVersionLS >> 0) & 0xffff;
						}
					}
				}
			}
			delete[] verData;
		}
	}

	static BOOL CheckForUpdate(BOOL* newVersionAvailable)
	{
		TCHAR LocalCacheFilePath[MAX_PATH] = { 0 };
		LPTSTR Url = _T("http://storage.keymagic.net.s3-ap-southeast-1.amazonaws.com/releases/windows/latest/version.txt");

		DWORD dwSize = 0;
		DWORD dwDownloaded = 0;
		LPSTR pszOutBuffer;
		BOOL  bResults = FALSE;
		HINTERNET  hSession = NULL,
			hConnect = NULL,
			hRequest = NULL;

		BOOL success = FALSE;
		*newVersionAvailable = FALSE;

		// Use WinHttpOpen to obtain a session handle.
		hSession = WinHttpOpen(_T("KeyMagic/1.0"),
			WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
			WINHTTP_NO_PROXY_NAME,
			WINHTTP_NO_PROXY_BYPASS, 0);

		// Specify an HTTP server.
		if (hSession)
			hConnect = WinHttpConnect(hSession, _T("storage.keymagic.net.s3-ap-southeast-1.amazonaws.com"),
				INTERNET_DEFAULT_HTTP_PORT, 0);

		// Create an HTTP request handle.
		if (hConnect) {
			LPCWSTR types[] = { _T("*/*"), NULL };
			hRequest = WinHttpOpenRequest(hConnect, _T("GET"), _T("/releases/windows/latest/version.txt"),
				NULL, WINHTTP_NO_REFERER,
				types,
				WINHTTP_FLAG_REFRESH);
		}

		// Send a request.
		if (hRequest)
			bResults = WinHttpSendRequest(hRequest,
				WINHTTP_NO_ADDITIONAL_HEADERS, 0,
				WINHTTP_NO_REQUEST_DATA, 0,
				0, 0);

		// End the request.
		if (bResults)
			bResults = WinHttpReceiveResponse(hRequest, NULL);

		// Keep checking for data until there is nothing left.
		if (bResults)
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				DebugLog(_T("Error in WinHttpQueryDataAvailable : ") << GetLastError() << std::endl);

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				DebugLog(_T("Out of memory") << std::endl);
				dwSize = 0;
			}
			else
			{
				// Read the data.
				ZeroMemory(pszOutBuffer, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
					dwSize, &dwDownloaded))
				{
					DebugLog(_T("Error in WinHttpReadData : ") << GetLastError() << std::endl);
				}
				else
				{
					printf("%s", pszOutBuffer);
					unsigned digitsRemote[4];
					if (ParseVersionString(digitsRemote, pszOutBuffer))
					{
						unsigned digitsCurrent[4];
						GetBinaryVersion(digitsCurrent);

						int result = memcmp(digitsCurrent, digitsRemote, sizeof(digitsCurrent));
						if (result < 0) {
							*newVersionAvailable = TRUE;
						}
						success = TRUE;
					}
				}

				// Free the memory allocated to the buffer.
				delete[] pszOutBuffer;
			}
		}


		// Report any errors.
		if (!bResults)
			DebugLog(_T("Error has occurred : ") << GetLastError() << std::endl);

		// Close any open handles.
		if (hRequest) WinHttpCloseHandle(hRequest);
		if (hConnect) WinHttpCloseHandle(hConnect);
		if (hSession) WinHttpCloseHandle(hSession);

		return success;
	}

};