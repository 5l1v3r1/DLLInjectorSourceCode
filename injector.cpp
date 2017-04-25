#include <iostream>
#include <windows.h>

bool Inject(DWORD pId, char *dllName);

using namespace std;

int main()
{
	string strDLL;
	system("title Dll Injector");
	cout << endl;
	cout << "                     Dll Injector\n\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xC);
	cout << "                                        |\n";
	cout << "                  ,------------=--------|___________|\n";
	cout << "--=============%%%|         |  |______|_|___________|\n";
	cout << "                  | | | | | | ||| | | | |___________|\n";
	cout << "pb                `------------=--------|           |\n";
	cout << "                                        |\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	cout <<"\n-------------------------------------------------------\n\n";
	int pid;
	cout << "Enter target PID: ";
	cin >> pid;
	TCHAR full_path[MAX_PATH];
	cout << "Enter target DLL: ";
	cin >> strDLL;
	GetFullPathName((strDLL.c_str()), MAX_PATH, full_path, NULL);
	Inject(pid, full_path);
    //Inject(pid, "C:\\Users\\Ryan\\Documents\\projects\\DllInjector\\dll.dll");
    system("pause");
    return 0;
}

bool Inject(DWORD pId, char *dllName)
{
    HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, false, pId);
    if(h)
    {
        LPVOID LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
        cout << "[!] Initialized Library\n";
        LPVOID dereercomp = VirtualAllocEx(h, NULL, strlen(dllName), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		cout << "[!] Initialized memory allocation\n";
        WriteProcessMemory(h, dereercomp, dllName, strlen(dllName), NULL);
        cout << "[!] Wrote dll name to memory: " << strlen(dllName) << " byte(s)\n";
        HANDLE asdc = CreateRemoteThread(h, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddr, dereercomp, 0, NULL);
		cout << "[!] Created remote thread: " << asdc << endl;
		cout << "[!] Waiting for Dll exit...\n";
        WaitForSingleObject(asdc, INFINITE);
        VirtualFreeEx(h, dereercomp, strlen(dllName), MEM_RELEASE);
        cout << "[!] Freeing memory\n";
        CloseHandle(asdc);
        CloseHandle(h);
        cout << "[!] Closed all handles\n";
        return true;
        cout << "[!] Complete!\n";
    }
    return false;
    cout << "[!] That process does not exist\n";
}
