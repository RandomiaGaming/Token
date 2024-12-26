#include "EzCpp/EzTokens.h"
#include "EzCpp/EzTokenTools.h"
#include "EzCpp/EzHelper.h"
#include "EzCpp/EzCore.h"
#include <iostream>
#include <Windows.h>

int main(int argc, char** argv) {
	EzError::SetSEHandler();
	try {
		//lstrcpyA(message, "Hello World");
		//printf("%s", message);
		throw NULL;


		//HANDLE currentToken = EzOpenCurrentToken();
		//EzPrintTokenInfoW(currentToken, std::wcout);
		//EzCloseHandleSafely(currentToken);
	}
	catch (EzError error) {
		error.Print();
	}
	return 0;
}