#include "Helper.h"
#include <Windows.h>

// These handles are global to the entire process and all threads.
// Do not call CloseHandle for these handles as they will be perminantly closed for the entire process.
const HANDLE InputHandle = GetStdHandle(STD_INPUT_HANDLE);
const HANDLE OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
const HANDLE ErrorHandle = GetStdHandle(STD_ERROR_HANDLE);

Error::Error(LPCWSTR message) {
	_wideMessage = TRUE;
	_constMessage = TRUE;
	_disposal = ErrorDisposal::Delete;
	_message = const_cast<void*>(reinterpret_cast<const void*>(message));
}
Error::Error(LPCSTR message) {
	_wideMessage = FALSE;
	_constMessage = TRUE;
	_disposal = ErrorDisposal::Delete;
	_message = const_cast<void*>(reinterpret_cast<const void*>(message));
}
Error::Error(LPWSTR message, ErrorDisposal disposal) {
	_wideMessage = TRUE;
	_constMessage = FALSE;
	_disposal = disposal;
	_message = const_cast<void*>(reinterpret_cast<const void*>(message));
}
Error::Error(LPSTR message, ErrorDisposal disposal) {
	_wideMessage = FALSE;
	_constMessage = FALSE;
	_disposal = disposal;
	_message = const_cast<void*>(reinterpret_cast<const void*>(message));
}
void Error::PrintAndFree() {
	PrintInColor(L"ERROR: ", ConsoleColor::Red);
	PrintInColor(_message, ConsoleColor::Red);
	if (!_constMessage) {
		switch (_disposal) {
		case ErrorDisposal::Free:
			free(const_cast<void*>(_message));
			break;
		case ErrorDisposal::Delete:
			delete[] const_cast<void*>(_message);
			break;
		case ErrorDisposal::LocalFree:
			::LocalFree(const_cast<void*>(_message));
			break;
		default: break;
		}
	}
}
Error::~Error() {
	_wideMessage = FALSE;
	_constMessage = FALSE;
	_disposal = ErrorDisposal::Delete;
	_message = nullptr;
}

void PrintError(LPCWSTR errorMessage) {
	PrintInternal(ErrorHandle, L"ERROR: ", 7, ConsoleColor::Red);
	PrintInternal(ErrorHandle, errorMessage, lstrlen(errorMessage), ConsoleColor::Red);
}
void PrintError(LPCSTR errorMessage) {
	PrintInternal(ErrorHandle, "ERROR: ", 7, ConsoleColor::Red);
	PrintInternal(ErrorHandle, errorMessage, strlen(errorMessage), ConsoleColor::Red);
}
void PrintWarning(LPCWSTR warningMessage) {
	PrintInternal(OutputHandle, L"Warning: ", 9, ConsoleColor::Yellow);
	PrintInternal(OutputHandle, warningMessage, lstrlen(warningMessage), ConsoleColor::Yellow);
}
void PrintWarning(LPCSTR warningMessage) {
	PrintInternal(OutputHandle, "Warning: ", 9, ConsoleColor::Yellow);
	PrintInternal(OutputHandle, warningMessage, strlen(warningMessage), ConsoleColor::Yellow);
}
void PrintInColor(LPCWSTR message, ConsoleColor color) {
	PrintInternal(OutputHandle, message, lstrlen(message), color);
}
void PrintInColor(LPCSTR message, ConsoleColor color) {
	PrintInternal(OutputHandle, message, strlen(message), color);
}
void PrintInternal(HANDLE consoleHandle, const void* message, DWORD messageLength, ConsoleColor color) {
	// Get initial console color.
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo);
	WORD originalColor = consoleInfo.wAttributes;

	// Set console color.
	SetConsoleTextAttribute(consoleHandle, color);

	// Print warning message.
	DWORD charsWritten;
	WriteConsole(consoleHandle, message, messageLength, &charsWritten, NULL);

	// Restore initial console attributes.
	SetConsoleTextAttribute(consoleHandle, originalColor);
}

void PressAnyKey() {
	DWORD originalConsoleMode;
	GetConsoleMode(InputHandle, &originalConsoleMode);

	SetConsoleMode(InputHandle, originalConsoleMode & ~ENABLE_ECHO_INPUT);

	INPUT_RECORD irInBuf[1];
	DWORD cNumRead;
	char ch = '\0';
	while (true) {
		if (ReadConsoleInput(InputHandle, irInBuf, 1, &cNumRead)) {
			if (irInBuf[0].EventType == KEY_EVENT) {
				KEY_EVENT_RECORD ker = irInBuf[0].Event.KeyEvent;
				if (ker.bKeyDown && !(ker.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED | SHIFT_PRESSED | NUMLOCK_ON | SCROLLLOCK_ON | CAPSLOCK_ON))) {
					ch = ker.uChar.AsciiChar;
					break;
				}
			}
		}
	}

	SetConsoleMode(InputHandle, originalConsoleMode);
}
void ThrowSysError() {
	DWORD errorCode = GetLastError();

	if (errorCode == 0) {
		PrintWarning(L"PrintLastError called even though nothing went wrong.");
		return;
	}

	WCHAR* errorMessage = nullptr;
	DWORD size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errorMessage, 0, NULL);

	throw Error(errorMessage, Error::ErrorDisposal::LocalFree);
}