#include "TokenInfo.h"
#include "Helper.h"
#include <Windows.h>

int main(int argc, char** argv) {
	HANDLE token = GetCurrentProcessToken();

	PrintTokenInfo(token);

	CloseHandle(token);

	return 0;
}