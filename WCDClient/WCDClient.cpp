// WCDClient.cpp : Defines the entry point for the application.
//

#include <Windows.h>
#include <stdio.h>
#include "..\WCD\WCDCommon.h"
#include <string>

int error(const char* msg)
{
	printf("%s [error=%d]\n", msg, GetLastError());
	return 1;
}

enum request_type {READ, WRITE, PRIORITY};

int main(int argc, const char* argv[])
{
	if (argc < 2) {
		printf("Usage: WCDClient [-r] [-w] [-p <threadid> <priority>]");
		return 0;
	}
	std::string arg_ = argv[1];
	enum request_type type_ = READ;
	UserData data;
	DWORD operation = 0;

	if (arg_ == "-r") {
		type_ = READ;
		operation = (DWORD)IOCTL_WCD_READ_ACCESS;
	}
	else if (arg_ == "-w") {
		type_ = WRITE;
		operation = (DWORD)IOCTL_WCD_WRITE_ACCESS;
	}
	else if (arg_ == "-p") {
		if (argc < 4) {
			return error("Usage: WCDClient -p <threadid> <priority>");
		}
		type_ = PRIORITY;
		operation = (DWORD)IOCTL_WCD_SET_THREAD_PRIORITY;
		data.ThreadId = atoi(argv[1]);
		data.Priority = atoi(argv[2]);
	}


	HANDLE hDev = CreateFile("\\\\.\\WCD", GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hDev == INVALID_HANDLE_VALUE)
		return error("ERROR: Failed to open device");


	DWORD returned;
	BOOL success = false;
	if (type_ == PRIORITY) {
		success = DeviceIoControl(hDev,
			operation,
			&data, sizeof(data), nullptr, 0,
			&returned, nullptr);

		if (success)
			printf("Priority changed !");
		else
			error("Priority change failed");
	}
	else {
		success = DeviceIoControl(hDev,
			operation,
			&data, sizeof(data), nullptr, 0,
			&returned, nullptr);

		if (success)
			printf("Operation executed ");
		else
			error("Operation failed");

	}
	CloseHandle(hDev);

	return 0;
}
