#include <iostream>
#include <Windows.h>
#include <string>
#include <ctype.h>
#include <tlhelp32.h>
#include <codecvt>
#include <locale>

void Color(int color = 0x07) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); } // color the console to make the program look cool

void ERRORmsg()   { std::cout << "["; Color(0x04); std::cout << "ERROR";   Color(0x0f); std::cout << "] "; } // add 
void SUCCESSmsg() { std::cout << "["; Color(0x02); std::cout << "SUCCESS"; Color(0x0f); std::cout << "] "; } // message
void ALERTmsg()   { std::cout << "["; Color(0x06); std::cout << "ALERT";   Color(0x0f); std::cout << "] "; } // prefixes


DWORD getPID(LPCWSTR window_title_) {
	// find window handle from parameter
	HWND handle = FindWindow(NULL, window_title_);
	if (handle == NULL) {return NULL;}
	// return pid of window
	DWORD pid;
	GetWindowThreadProcessId(handle, &pid);
	return pid;
}
std::wstring s2ws(const std::string& s) { // i copy pasted this, i need it to convert strings to the proper type
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
int main() {
	Color(0x0B); std::cout << "DLL Injector by TheTetrisDude\nPress return/enter to start\n"; Color(0x0f); std::cin.get();
	ALERTmsg(); std::cout << "Starting the injector...\n";

	// get PID
	ALERTmsg(); std::cout << "Enter window name below:\n";
	std::string i; std::cin >> i; // create variable and write input to it
	std::wstring stemp = s2ws(i); // convert it to wstring
	LPCWSTR procname = stemp.c_str(); // convert it to LPCWSTR
	DWORD pid = getPID(procname); // store PID to DWORD, then check it
	if (pid != 0 && pid != NULL) {
		SUCCESSmsg(); std::cout << "Successfully obtained pid of window: " << pid << "\n";
	} else {
		ERRORmsg(); std::cout << "Task failed: could not obtain pid of window.\n"; system("pause"); exit(1);
	}

	//
}