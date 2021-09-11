// this is my first project with Visual Studio!
#include <iostream>
#include <Windows.h>
#include <ctype.h>
#include <tlhelp32.h>
#include <codecvt>
#include <locale>
#include <string>

void Color(int color = 0x07) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); } // color the console to make the program look cool

void ERRORmsg(std::string message)   { std::cout << "["; Color(0x04); std::cout << "ERROR";   Color(0x0f); std::cout << "] " << message << "\n"; system("pause"); exit(1); } // add message texts.
void SUCCESSmsg(std::string message) { std::cout << "["; Color(0x02); std::cout << "SUCCESS"; Color(0x0f); std::cout << "] " << message << "\n"; } // Honestly I really
void ALERTmsg(std::string message)   { std::cout << "["; Color(0x06); std::cout << "ALERT";   Color(0x0f); std::cout << "] " << message << "\n"; } // like the way it looks


DWORD getPID(LPCSTR window_title_) {
	// find window handle from parameter
	HWND handle = FindWindowA(NULL, window_title_);
	if (handle == NULL) { ALERTmsg("Could not get handle for window"); }
	// return pid of window
	DWORD pid;
	GetWindowThreadProcessId(handle, &pid);
	return pid;
}

int main() {
	std::string Logo = "d8888b. db      db           d888888b d8b   db    d88b d88888b  .o88b. d888888b  .d88b.  d8888b. \n88  `8D 88      88             `88'   888o  88    `8P' 88'     d8P  Y8 `~~88~~' .8P  Y8. 88  `8D \n88   88 88      88              88    88V8o 88     88  88ooooo 8P         88    88    88 88oobY' \n88   88 88      88              88    88 V8o88     88  88~~~~~8b          88    88    88 88`8b	 \n88  .8D 88booo. 88booo.        .88.   88  V888 db. 88  88.     Y8b  d8    88    `8b  d8' 88 `88. \nY8888D' Y88888P Y88888P      Y888888P VP   V8P Y8888P  Y88888P  `Y88P'    YP     `Y88P'  88   YD \n"; // Lol
	Color(0x0B); std::cout << Logo << "\n\nBy TheTetrisDude\nPress return/enter to start\n"; Color(0x0f); std::cin.get();
	ALERTmsg("Starting the injector...");  malloc(4000000); // random malloc that i put because i feel like it. i dont think it even does anything

	// get PID
	ALERTmsg("Enter window name below:");
	std::string i; std::getline(std::cin, i); // create variable and write input to it
	ALERTmsg("Searching for window name " + i + " ...");
	LPCSTR procname = i.c_str(); // convert it to LPCSTR
	DWORD pid = getPID(procname); // store PID to DWORD, then check it
	if (pid != 0) {
		SUCCESSmsg("Successfully obtained pid of window: "); std::cout << pid << "\n";
	} else {
		ERRORmsg("Failed to find or obtain pid of window.\n");
	}

	// get DLL info
	ALERTmsg("Enter DLL path below: ( E.G: C:/Users/Username/Documents/MyDLL.dll )");
	std::string DLLpathstring; std::cin >> DLLpathstring; // store DLL path in variable
	struct stat s; // check if there is a file
	const char* DLLpath = DLLpathstring.c_str();
	if (stat(DLLpath, &s) == 0) {
		if (s.st_mode & S_IFDIR) {
			//it's a directory
			ERRORmsg("Instead of a DLL file, a directory was found at location " + DLLpathstring);
		} else if (s.st_mode & S_IFREG) {
			//it's a file
			SUCCESSmsg("Located DLL file successfully: " + DLLpathstring);
		} else {
			//something else
			ERRORmsg("Instead of a DLL file, something else was found at location " + DLLpathstring);
		}
	} else {
		//error
		ERRORmsg("Could not find location " + DLLpathstring);
	}

	// get LoadLibraryA
	ALERTmsg("Attempting to retrieve LoadLibraryA...");
	LPVOID loadlibrary = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
	if (loadlibrary) { SUCCESSmsg("Retrieved LoadLibraryA"); }
	else { ERRORmsg("Failed retrieving LoadLibraryA"); }

	// open the process
	ALERTmsg("Attempting to open process...");
	HANDLE procHandle = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, pid); // opens process with full access and uses pid to find it
	if (procHandle) { SUCCESSmsg("Opened target process succesfully"); }
	else { ERRORmsg("Failed opening target process"); }

	// allocate memory for the DLL
	ALERTmsg("Attempting to allocate memory...");
	LPVOID dlladdress = VirtualAllocEx(procHandle, 0, strlen(DLLpath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (dlladdress) { SUCCESSmsg("Allocated memory succesfully"); }
	else { CloseHandle(procHandle); ERRORmsg("Failed to allocate memory"); }

	// write memory
	ALERTmsg("Attempting to write DLL to memory...");
	BOOL writememory = WriteProcessMemory(procHandle, dlladdress, DLLpath, strlen(DLLpath), NULL);
	if (writememory) { SUCCESSmsg("Wrote DLL succesfully"); }
	else { CloseHandle(procHandle); ERRORmsg("Failed to write DLL"); }

	// execute the DLL
	ALERTmsg("Setup complete. To execute the DLL, "); system("pause");
	ALERTmsg("Attempting to execute the DLL...");
	HANDLE dllthread_handle = CreateRemoteThread(procHandle, 0, 0, (LPTHREAD_START_ROUTINE)loadlibrary, dlladdress, 0, 0);
	if (dllthread_handle) { SUCCESSmsg("Executed DLL successfully"); system("pause"); }
	else { CloseHandle(procHandle); ERRORmsg("Failed to execute DLL"); }

	// close handles
	CloseHandle(dllthread_handle); CloseHandle(procHandle);
}