// Approved 11/14/2024

#include "EzCore.h"
#include <Windows.h>
#include <comdef.h>
#include <sstream>
#include <iomanip>

LPSTR EzError::ConstructMessage(LPCSTR text, LPCSTR source, LPCSTR file, UINT32 line) noexcept {
	// Try here incase memory allocations from ostringstream fail
	try {
		std::ostringstream messageStream = { };

		// Append file name
		try {
			if (file == NULL) {
				throw NULL;
			}

			int fileLength = lstrlenA(file);
			LPCSTR fileNameOnly = file + fileLength;
			while (fileNameOnly >= file && *fileNameOnly != '\\' && *fileNameOnly != '/') {
				fileNameOnly--;
			}
			messageStream << "ERROR in " << (fileNameOnly + 1);
		}
		catch (...) {
			messageStream << "ERROR in UnknownFile";
		}

		// Append line number
		if (line == 0xFFFFFFFF) {
			messageStream << " at UnknownLine";
		}
		else {
			messageStream << " at line " << line;
		}

		// Append current time
		SYSTEMTIME timeNow = { };
		GetLocalTime(&timeNow);
		if (timeNow.wHour == 0) {
			messageStream << " at 12:" << timeNow.wMinute << ":" << timeNow.wSecond << "am";
		}
		else if (timeNow.wHour < 12) {
			messageStream << " at " << (timeNow.wHour % 12) << ":" << timeNow.wMinute << ":" << timeNow.wSecond << "am";
		}
		else {
			messageStream << " at " << (timeNow.wHour % 12) << ":" << timeNow.wMinute << ":" << timeNow.wSecond << "pm";
		}
		messageStream << " on " << timeNow.wMonth << "/" << timeNow.wDay << "/" << timeNow.wYear;

		// Append error source
		try {
			if (source == NULL) {
				throw NULL;
			}

			messageStream << " from " << source;
		}
		catch (...) {

		}

		// Append error message
		try {
			if (text == NULL) {
				throw NULL;
			}

			messageStream << ": " << text;
			int textLength = lstrlenA(text);
			if (textLength >= 2) {
				LPCSTR lastTwoChars = text + (textLength - 2);
				if (lastTwoChars[0] != '\r' || lastTwoChars[1] != '\n') {
					messageStream << "\r\n";
				}
			}
		}
		catch (...) {
			messageStream << ": UnknownMessage\r\n";
		}

		// Copy string and return
		std::string messageString = messageStream.str();
		LPSTR output = EzAlloc<CHAR>(messageString.size() + 1);
		if (output == NULL) {
			throw NULL;
		}
		lstrcpyA(output, messageString.c_str());
		return output;
	}
	catch (...) {
		return NULL;
	}
}
void EzError::PrintToConsole(LPCSTR message) noexcept {
	if (message == NULL) {
		return;
	}
	int messageLength = 0;
	try {
		messageLength = lstrlenA(message);
	}
	catch (...) {
		return;
	}

	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdoutHandle == INVALID_HANDLE_VALUE) {
		return;
	}

	BOOL restoreAttributes = TRUE;
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo = { };
	if (!GetConsoleScreenBufferInfo(stdoutHandle, &consoleInfo)) {
		restoreAttributes = FALSE;
	}

	if (!SetConsoleTextAttribute(stdoutHandle, static_cast<WORD>(FOREGROUND_RED | FOREGROUND_INTENSITY))) {
		restoreAttributes = FALSE;
	}

	DWORD charsWritten = 0;
	WriteConsoleA(stdoutHandle, message, messageLength, &charsWritten, NULL);

	if (!restoreAttributes) {
		SetConsoleTextAttribute(stdoutHandle, consoleInfo.wAttributes);
	}
}
void EzError::PrintToLogFile(LPCSTR message) noexcept {
	if (message == NULL) {
		return;
	}
	int messageLength = 0;
	try {
		messageLength = lstrlenA(message);
	}
	catch (...) {
		return;
	}

	HANDLE logFile = CreateFileA(EzError::ErrorLogFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (logFile == INVALID_HANDLE_VALUE) {
		logFile = CreateFileA(ErrorLogFilePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	if (logFile == INVALID_HANDLE_VALUE) {
		return;
	}

	LONGLONG fileSize = 0;
	BYTE* fileContents = NULL;
	try {
		if (!GetFileSizeEx(logFile, reinterpret_cast<PLARGE_INTEGER>(&fileSize))) {
			throw NULL;
		}

		fileContents = new BYTE[fileSize];
		if (fileContents == NULL) {
			throw NULL;
		}

		DWORD bytesRead = 0;
		if (!ReadFile(logFile, fileContents, fileSize, &bytesRead, NULL) || bytesRead != fileSize) {
			throw NULL;
		}
	}
	catch (...) {
		fileSize = 0;
		if (fileContents != NULL) {
			delete[] fileContents;
			fileContents = NULL;
		}
	}

	SetFilePointer(logFile, 0, NULL, FILE_BEGIN);

	DWORD bytesWrittenMessage = 0;
	WriteFile(logFile, message, messageLength * sizeof(CHAR), &bytesWrittenMessage, NULL);

	DWORD bytesWrittenFileContents = 0;
	WriteFile(logFile, fileContents, fileSize, &bytesWrittenFileContents, NULL);

	CloseHandle(logFile);

	if (fileContents != NULL) {
		delete[] fileContents;
		fileContents = NULL;
	}
}

EzError::EzError(LPSTR message, DWORD code, HRESULT hr, NTSTATUS nt, DWORD se) noexcept {
	_message = message;
	_code = code;
	_hr = hr;
	_nt = nt;
	_se = se;
}
EzError::~EzError() noexcept {
	if (_message != NULL) {
		delete[] _message;
		_message = NULL;
	}

	_code = 0;
	_hr = 0;
	_nt = 0;
	_se = 0;
}
EzError::EzError(const EzError& other) noexcept {
	try {
		if (other._message == NULL) {
			throw NULL;
		}
		int messageLength = lstrlenA(other._message) + 1;
		_message = new CHAR[messageLength];
		if (_message == NULL) {
			throw NULL;
		}
		lstrcpyA(_message, other._message);
	}
	catch (...) {
		_message = NULL;
	}

	_code = other._code;
	_hr = other._hr;
	_nt = other._nt;
	_se = other._se;
}
EzError& EzError::operator=(const EzError& other) noexcept {
	if (this == &other) {
		return *this;
	}

	if (_message != NULL) {
		delete[] _message;
		_message = NULL;
	}

	try {
		if (other._message == NULL) {
			throw NULL;
		}
		int messageLength = lstrlenA(other._message) + 1;
		_message = new CHAR[messageLength];
		if (_message == NULL) {
			throw NULL;
		}
		lstrcpyA(_message, other._message);
	}
	catch (...) {
		_message = NULL;
	}

	_code = other._code;
	_hr = other._hr;
	_nt = other._nt;
	_se = other._se;

	return *this;
}

void EzError::Print() const noexcept {
	PrintToConsole(_message);
	PrintToLogFile(_message);
}
LPCSTR EzError::What() const noexcept {
	return _message;
}
DWORD EzError::GetCode() const noexcept {
	return _code;
}
HRESULT EzError::GetHR() const noexcept {
	return _hr;
}
NTSTATUS EzError::GetNT() const noexcept {
	return _nt;
}
DWORD EzError::GetSE() const noexcept {
	return _se;
}

EzError EzError::FromCode(DWORD code, LPCSTR file, UINT32 line) noexcept {
	LPSTR message = NULL;
	if (FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, code, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), reinterpret_cast<LPSTR>(&message), 0, NULL) == 0) {
		message = NULL;
	}

	LPSTR source = NULL;
	try {
		std::ostringstream sourceStream = { };
		sourceStream << "error code 0x" << std::hex << std::setw(sizeof(DWORD) * sizeof(CHAR)) << std::setfill('0') << code << std::setfill(' ') << std::setw(0) << std::dec;
		std::string sourceString = sourceStream.str();
		source = new CHAR[sourceString.size() + 1];
		if (source == NULL) {
			throw NULL;
		}
		lstrcpyA(source, sourceString.c_str());
	}
	catch (...) {
		source = NULL;
	}

	LPSTR constructedMessage = ConstructMessage(message, source, file, line);

	if (message != NULL) {
		LocalFree(message);
		message = NULL;
	}
	if (source != NULL) {
		delete[] source;
		source = NULL;
	}

	return EzError::EzError(constructedMessage, code, 0, 0, 0);
}
EzError EzError::FromHR(HRESULT hr, LPCSTR file, UINT32 line) noexcept {
	LPSTR message = NULL;
	if (FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), reinterpret_cast<LPSTR>(&message), 0, NULL) == 0) {
		message = NULL;
	}
	if (message == NULL) {
		try {
			_com_error comError(hr);
			LPCWSTR wideMessage = comError.ErrorMessage();
			if (wideMessage == NULL) {
				throw NULL;
			}
			int wideMessageLength = lstrlenW(wideMessage);

			std::ostringstream messageStream = { };
			messageStream << wideMessage;
			std::string messageString = messageStream.str();
			message = new CHAR[messageString.size() + 1];
			if (message == NULL) {
				throw NULL;
			}
			lstrcpyA(message, messageString.c_str());
		}
		catch (...) {
			message = NULL;
		}
	}

	LPSTR source = NULL;
	try {
		std::ostringstream sourceStream = { };
		sourceStream << "HRESULT 0x" << std::hex << std::setw(sizeof(HRESULT) * sizeof(CHAR)) << std::setfill('0') << hr << std::setfill(' ') << std::setw(0) << std::dec;
		std::string sourceString = sourceStream.str();
		source = new CHAR[sourceString.size() + 1];
		if (source == NULL) {
			throw NULL;
		}
		lstrcpyA(source, sourceString.c_str());
	}
	catch (...) {
		source = NULL;
	}

	LPSTR constructedMessage = ConstructMessage(message, source, file, line);

	if (message != NULL) {
		LocalFree(message);
		message = NULL;
	}
	if (source != NULL) {
		delete[] source;
		source = NULL;
	}

	return EzError::EzError(constructedMessage, 0, hr, 0, 0);
}
EzError EzError::FromNT(NTSTATUS nt, LPCSTR file, UINT32 line) noexcept {
	LPSTR message = NULL;
	if (FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, HRESULT_FROM_NT(nt), MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), reinterpret_cast<LPSTR>(&message), 0, NULL) == 0) {
		message = NULL;
	}

	LPSTR source = NULL;
	try {
		std::ostringstream sourceStream = { };
		sourceStream << "NTSTATUS 0x" << std::hex << std::setw(sizeof(NTSTATUS) * sizeof(CHAR)) << std::setfill('0') << nt << std::setfill(' ') << std::setw(0) << std::dec;
		std::string sourceString = sourceStream.str();
		source = new CHAR[sourceString.size() + 1];
		if (source == NULL) {
			throw NULL;
		}
		lstrcpyA(source, sourceString.c_str());
	}
	catch (...) {
		source = NULL;
	}

	LPSTR constructedMessage = ConstructMessage(message, source, file, line);

	if (message != NULL) {
		LocalFree(message);
		message = NULL;
	}
	if (source != NULL) {
		delete[] source;
		source = NULL;
	}

	return EzError::EzError(constructedMessage, 0, 0, nt, 0);
}
EzError EzError::FromSE(DWORD se, LPCSTR file, UINT32 line) noexcept {
	LPCSTR message = NULL;
	switch (se) {
	case EXCEPTION_ACCESS_VIOLATION: message = "Access violation/Segmentation fault"; break;
	case EXCEPTION_DATATYPE_MISALIGNMENT: message = "Data type misalignment"; break;
	case EXCEPTION_BREAKPOINT: message = "Breakpoint"; break;
	case EXCEPTION_SINGLE_STEP: message = "Single step"; break;
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: message = "Array bounds exceeded"; break;
	case EXCEPTION_FLT_DENORMAL_OPERAND: message = "Float denormal operand"; break;
	case EXCEPTION_FLT_DIVIDE_BY_ZERO: message = "Float divide by zero"; break;
	case EXCEPTION_FLT_INEXACT_RESULT: message = "Float inexact result"; break;
	case EXCEPTION_FLT_INVALID_OPERATION: message = "Float invalid operation"; break;
	case EXCEPTION_FLT_OVERFLOW: message = "Float overflow"; break;
	case EXCEPTION_FLT_STACK_CHECK: message = "Float stack check"; break;
	case EXCEPTION_FLT_UNDERFLOW: message = "Float underflow"; break;
	case EXCEPTION_INT_DIVIDE_BY_ZERO: message = "Integer divide by zero"; break;
	case EXCEPTION_INT_OVERFLOW: message = "Integer overflow"; break;
	case EXCEPTION_PRIV_INSTRUCTION: message = "Priv instruction"; break;
	case EXCEPTION_IN_PAGE_ERROR: message = "In page error"; break;
	case EXCEPTION_ILLEGAL_INSTRUCTION: message = "Illegal instruction"; break;
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: message = "Non-continuable exception"; break;
	case EXCEPTION_STACK_OVERFLOW: message = "Stack overflow"; break;
	case EXCEPTION_INVALID_DISPOSITION: message = "Invalid disposition"; break;
	case EXCEPTION_GUARD_PAGE: message = "Guard page"; break;
	case EXCEPTION_INVALID_HANDLE: message = "Invalid handle"; break;
	case CONTROL_C_EXIT: message = "DLL initialization failure"; break;
	default: message = "Unknown structured exception"; break;
	}

	LPSTR source = NULL;
	try {
		std::ostringstream sourceStream = { };
		sourceStream << "structured exception 0x" << std::hex << std::setw(sizeof(DWORD) * sizeof(CHAR)) << std::setfill('0') << se << std::setfill(' ') << std::setw(0) << std::dec;
		std::string sourceString = sourceStream.str();
		source = new CHAR[sourceString.size() + 1];
		if (source == NULL) {
			throw NULL;
		}
		lstrcpyA(source, sourceString.c_str());
	}
	catch (...) {
		source = NULL;
	}

	LPSTR constructedMessage = ConstructMessage(message, source, file, line);

	if (source != NULL) {
		delete[] source;
		source = NULL;
	}

	return EzError::EzError(constructedMessage, 0, 0, 0, se);
}
EzError EzError::FromException(std::exception ex, LPCSTR file, UINT32 line) noexcept {
	LPSTR constructedMessage = ConstructMessage(ex.what(), NULL, file, line);

	return EzError::EzError(constructedMessage, 0, 0, 0, 0);
}
EzError EzError::FromMessageA(LPCSTR message, LPCSTR file, UINT32 line) noexcept {
	LPSTR constructedMessage = ConstructMessage(message, NULL, file, line);

	return EzError::EzError(constructedMessage, 0, 0, 0, 0);
}
EzError EzError::FromMessageW(LPCWSTR message, LPCSTR file, UINT32 line) noexcept {
	LPSTR narrowMessage = NULL;
	try {
		std::ostringstream messageStream = { };
		messageStream << message;
		std::string messageString = messageStream.str();
		narrowMessage = new CHAR[messageString.size() + 1];
		if (narrowMessage == NULL) {
			throw NULL;
		}
		lstrcpyA(narrowMessage, messageString.c_str());
	}
	catch (...) {
		narrowMessage = NULL;
	}

	LPSTR constructedMessage = ConstructMessage(narrowMessage, NULL, file, line);

	return EzError::EzError(constructedMessage, 0, 0, 0, 0);
}

static void SE_Translator(unsigned int code, EXCEPTION_POINTERS* pExp) noexcept {
	DWORD exceptionCode = 0xFFFFFFFF;
	try {
		if (pExp == NULL) {
			throw NULL;
		}
		if (pExp->ExceptionRecord == NULL) {
			throw NULL;
		}
		exceptionCode = pExp->ExceptionRecord->ExceptionCode;
	}
	catch (...) {
		exceptionCode = 0xFFFFFFFF;
	}
	throw EzError::FromSE(exceptionCode, __FILE__, __LINE__);
}
void EzError::SetSEHandler() noexcept {
	_set_se_translator(SE_Translator);
}

void EzClose(HANDLE* handle) {
	if (handle == NULL) {
		throw EzError::FromMessageA("handle must be a valid pointer to a HANDLE.", __FILE__, __LINE__);
	}
	if (handle == INVALID_HANDLE_VALUE) {
		return;
	}
	if (!CloseHandle(handle)) {
		DWORD errorCode = GetLastError();
		throw EzError::FromCode(errorCode, __FILE__, __LINE__);
	}
	*handle = INVALID_HANDLE_VALUE;
}