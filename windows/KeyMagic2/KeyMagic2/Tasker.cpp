#include "stdafx.h"

//#define _WIN32_DCOM

#include "Tasker.h"
#include "ConfigUtils.h"

//# pragma comment(lib, "taskschd.lib")
//# pragma comment(lib, "comsupp.lib")
//# pragma comment(lib, "credui.lib")
//
//using namespace std;

//void RegisterTask(wstring executablePath)
//{
//	//  ------------------------------------------------------
//	//  Initialize COM.
//	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
//	if (FAILED(hr))
//	{
//		printf("\nCoInitializeEx failed: %x", hr);
//		return;
//	}
//
//	//  Set general COM security levels.
//	hr = CoInitializeSecurity(
//		NULL,
//		-1,
//		NULL,
//		NULL,
//		RPC_C_AUTHN_LEVEL_PKT,
//		RPC_C_IMP_LEVEL_IMPERSONATE,
//		NULL,
//		0,
//		NULL);
//
//	if (FAILED(hr))
//	{
//		printf("\nCoInitializeSecurity failed: %x", hr);
//		CoUninitialize();
//		return;
//	}
//
//	//  ------------------------------------------------------
//	//  Create a name for the task.
//	LPCWSTR wszTaskName = L"KeyMagic";
//
//	//  Get the windows directory and set the path to notepad.exe.
//	wstring wstrExecutablePath = executablePath;
//
//
//	//  ------------------------------------------------------
//	//  Create an instance of the Task Service. 
//	ITaskService *pService = NULL;
//	hr = CoCreateInstance(CLSID_TaskScheduler,
//		NULL,
//		CLSCTX_INPROC_SERVER,
//		IID_ITaskService,
//		(void**)&pService);
//	if (FAILED(hr))
//	{
//		printf("Failed to create an instance of ITaskService: %x", hr);
//		CoUninitialize();
//		return;
//	}
//
//	//  Connect to the task service.
//	hr = pService->Connect(_variant_t(), _variant_t(),
//		_variant_t(), _variant_t());
//	if (FAILED(hr))
//	{
//		printf("ITaskService::Connect failed: %x", hr);
//		pService->Release();
//		CoUninitialize();
//		return;
//	}
//
//	//  ------------------------------------------------------
//	//  Get the pointer to the root task folder.  
//	//  This folder will hold the new task that is registered.
//	ITaskFolder *pRootFolder = NULL;
//	hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
//	if (FAILED(hr))
//	{
//		printf("Cannot get Root Folder pointer: %x", hr);
//		pService->Release();
//		CoUninitialize();
//		return;
//	}
//
//	//  If the same task exists, remove it.
//	pRootFolder->DeleteTask(_bstr_t(wszTaskName), 0);
//
//	//  Create the task builder object to create the task.
//	ITaskDefinition *pTask = NULL;
//	hr = pService->NewTask(0, &pTask);
//
//	pService->Release();  // COM clean up.  Pointer is no longer used.
//	if (FAILED(hr))
//	{
//		printf("Failed to create a task definition: %x", hr);
//		pRootFolder->Release();
//		CoUninitialize();
//		return;
//	}
//
//	//  ------------------------------------------------------
//	//  Get the registration info for setting the identification.
//	IRegistrationInfo *pRegInfo = NULL;
//	hr = pTask->get_RegistrationInfo(&pRegInfo);
//	if (FAILED(hr))
//	{
//		printf("\nCannot get identification pointer: %x", hr);
//		pRootFolder->Release();
//		pTask->Release();
//		CoUninitialize();
//		return;
//	}
//
//	hr = pRegInfo->put_Author(L"KeyMagic");
//	pRegInfo->Release();  // COM clean up.  Pointer is no longer used.
//	if (FAILED(hr))
//	{
//		printf("\nCannot put identification info: %x", hr);
//		pRootFolder->Release();
//		pTask->Release();
//		CoUninitialize();
//		return;
//	}
//
//	ITaskSettings *pTaskSettings = NULL;
//	hr = pTask->get_Settings(&pTaskSettings);
//	if (FAILED(hr))
//	{
//		printf("\nCannot get task settings: %x", hr);
//		pRootFolder->Release();
//		pTask->Release();
//		CoUninitialize();
//		return;
//	}
//
//	pTaskSettings->put_DisallowStartIfOnBatteries(VARIANT_BOOL(FALSE));
//	pTaskSettings->put_StopIfGoingOnBatteries(VARIANT_BOOL(FALSE));
//	pTaskSettings->put_ExecutionTimeLimit(BSTR(_T("PT0S")));
//	pTaskSettings->put_StartWhenAvailable(VARIANT_BOOL(TRUE));
//
//	pTask->put_Settings(pTaskSettings);
//
//	//  ------------------------------------------------------
//	//  Get the trigger collection to insert the boot trigger.
//	ITriggerCollection *pTriggerCollection = NULL;
//	hr = pTask->get_Triggers(&pTriggerCollection);
//	if (FAILED(hr))
//	{
//		printf("\nCannot get trigger collection: %x", hr);
//		pRootFolder->Release();
//		pTask->Release();
//		CoUninitialize();
//		return;
//	}
//
//	ITrigger *pTrigger = NULL;
//	hr = pTriggerCollection->Create(TASK_TRIGGER_BOOT, &pTrigger);
//	pTriggerCollection->Release();
//	if (FAILED(hr))
//	{
//		printf("\nCannot create the trigger: %x", hr);
//		pRootFolder->Release();
//		pTask->Release();
//		CoUninitialize();
//		return;
//	}
//
//	IWeeklyTrigger *pBootTrigger = NULL;
//	hr = pTrigger->QueryInterface(
//		IID_IBootTrigger, (void**)&pBootTrigger);
//	pTrigger->Release();
//	if (FAILED(hr))
//	{
//		printf("\nQueryInterface call for IBootTrigger failed: %x", hr);
//		pRootFolder->Release();
//		pTask->Release();
//		CoUninitialize();
//		return;
//	}
//
//	hr = pBootTrigger->put_Id(_bstr_t(L"Trigger1"));
//	if (FAILED(hr))
//		printf("\nCannot put trigger ID: %x", hr);
//	//  ------------------------------------------------------
//	//  Add an Action to the task. This task will execute notepad.exe.     
//	IActionCollection *pActionCollection = NULL;
//
//	//  Get the task action collection pointer.
//	hr = pTask->get_Actions(&pActionCollection);
//	if (FAILED(hr))
//	{
//		printf("\nCannot get Task collection pointer: %x", hr);
//		pRootFolder->Release();
//		pTask->Release();
//		CoUninitialize();
//		return;
//	}
//
//	//  Create the action, specifying that it is an executable action.
//	IAction *pAction = NULL;
//	hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
//	pActionCollection->Release();
//	if (FAILED(hr))
//	{
//		printf("\nCannot create the action: %x", hr);
//		pRootFolder->Release();
//		pTask->Release();
//		CoUninitialize();
//		return;
//	}
//
//	IExecAction *pExecAction = NULL;
//	//  QI for the executable task pointer.
//	hr = pAction->QueryInterface(
//		IID_IExecAction, (void**)&pExecAction);
//	pAction->Release();
//	if (FAILED(hr))
//	{
//		printf("\nQueryInterface call failed on IExecAction: %x", hr);
//		pRootFolder->Release();
//		pTask->Release();
//		CoUninitialize();
//		return;
//	}
//
//	//  Set the path of the executable to notepad.exe.
//	hr = pExecAction->put_Path(_bstr_t(wstrExecutablePath.c_str()));
//	pExecAction->Release();
//	if (FAILED(hr))
//	{
//		printf("\nCannot add path for executable action: %x", hr);
//		pRootFolder->Release();
//		pTask->Release();
//		CoUninitialize();
//		return;
//	}
//
//	//  ------------------------------------------------------
//	//  Save the task in the root folder.
//	IRegisteredTask *pRegisteredTask = NULL;
//	VARIANT varPassword;
//	varPassword.vt = VT_EMPTY;
//	hr = pRootFolder->RegisterTaskDefinition(
//		_bstr_t(wszTaskName),
//		pTask,
//		TASK_CREATE_OR_UPDATE,
//		_variant_t(L"SYSTEM"),
//		varPassword,
//		TASK_LOGON_SERVICE_ACCOUNT,
//		_variant_t(L""),
//		&pRegisteredTask);
//	if (FAILED(hr))
//	{
//		printf("\nError saving the Task : %x", hr);
//		pRootFolder->Release();
//		pTask->Release();
//		CoUninitialize();
//		return;
//	}
//
//	printf("\n Success! Task succesfully registered. ");
//
//	//  Clean up
//	pRootFolder->Release();
//	pTask->Release();
//	pRegisteredTask->Release();
//	CoUninitialize();
//	return;
//}

void UnRegisterProgramForStartup(PCWSTR pszAppName)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwRegType = REG_SZ;

	std::string szAppName = converter.to_bytes(pszAppName);

	lResult = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hKey);
	
	if (fSuccess)
	{
		lResult = RegQueryValueEx(hKey, pszAppName, NULL, &dwRegType, NULL, NULL);
		fSuccess = (lResult == 0);
	}

	if (fSuccess)
	{
		lResult = RegDeleteValue(hKey, pszAppName);
		fSuccess = (lResult == 0);
	}
}

BOOL IsRegisteredForStartup(PCWSTR pszAppName)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwRegType = REG_SZ;
	BYTE bData[1000] = {};
	wchar_t szPathToExe[MAX_PATH] = {};
	DWORD dwSize = sizeof(bData);

	lResult = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey);

	fSuccess = (lResult == 0);

	if (fSuccess)
	{
		lResult = RegQueryValueEx(hKey, pszAppName, NULL, &dwRegType, bData, &dwSize);
		wcscpy_s(szPathToExe, (wchar_t*) bData);
		fSuccess = (lResult == 0);
	}

	if (fSuccess)
	{
		fSuccess = (wcslen(szPathToExe) > 0) ? TRUE : FALSE;
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}

BOOL RegisterMyProgramForStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args)
{
	HKEY hKey = NULL;
	LONG lResult = 0;
	BOOL fSuccess = TRUE;
	DWORD dwSize;

	const size_t count = MAX_PATH * 2;
	wchar_t szValue[count] = {};


	wcscpy_s(szValue, count, L"\"");
	wcscat_s(szValue, count, pathToExe);
	wcscat_s(szValue, count, L"\" ");

	if (args != NULL)
	{
		// caller should make sure "args" is quoted if any single argument has a space
		// e.g. (L"-name \"Mark Voidale\"");
		wcscat_s(szValue, count, args);
	}

	lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);

	fSuccess = (lResult == 0);

	if (fSuccess)
	{
		dwSize = (wcslen(szValue) + 1) * 2;
		lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
		fSuccess = (lResult == 0);
	}

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	return fSuccess;
}

void RegisterProgram()
{
	wchar_t szPathToExe[MAX_PATH];

	GetModuleFileNameW(NULL, szPathToExe, MAX_PATH);
	RegisterMyProgramForStartup(L"KeyMagic", szPathToExe, L"/startup");
}