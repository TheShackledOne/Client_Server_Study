#include "Common.h"

void error_display(const char* msg, int err_no)
{
	WCHAR* msgbuf = nullptr;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&msgbuf, 0, nullptr);
	std::cout << msg;
	std::wcout << msgbuf << std::endl;
	LocalFree(msgbuf);
}