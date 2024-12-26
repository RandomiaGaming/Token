// Approved 12/06/2024

// EZ STANDALONE COMPONENT

/*
REQUIRED SETTING:
You must go to VcxProj > Properties > Configuration Properties > C/C++ > Code Generation > Enable C++ Exceptions
and set it to "Yes with SEH Exceptions (/EHa)" in order for EzError::SetSEHandler() to function properly.
The reason for this is outlined in the documentation for _set_se_translator.
https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/set-se-translator
*/

/*
ERROR HANDLING BEST PRACTICES:
Start by calling EzError::SetSEHandler() to ensure any SE exceptions like segfaults are converted into EzErrors.
Next wrap the main function in a try { } catch (EzError error) { error.Print(); }.
You don't need to call SetSEHandler() in any other places in your code. It's global and works on all threads.
You also don't need to worry about catching other types of errors like std::exceptions since no Ez functions
will ever throw these types of errors. It is standard practice to convert any other types of exceptions to EzErrors
and rethrow them. For example if I call a std function which can throw an error I would use
try { std::ThrowableFunction(); } catch (std::exception ex) { throw EzError::FromException(ex); }
*/

#pragma once
#include <Windows.h>
#include <exception>

class EzError final {
public:
	EzError() = delete;
	EzError(EzError&&) = default;
	EzError& operator=(EzError&&) = default;

	~EzError() noexcept;
	EzError(const EzError& other) noexcept;
	EzError& operator=(const EzError& other) noexcept;

	void Print() const noexcept;
	LPCSTR What() const noexcept;
	DWORD GetCode() const noexcept;
	HRESULT GetHR() const noexcept;
	NTSTATUS GetNT() const noexcept;
	DWORD GetSE() const noexcept;

	static EzError FromCode(DWORD code, LPCSTR file, UINT32 line) noexcept;
	static EzError FromHR(HRESULT hr, LPCSTR file, UINT32 line) noexcept;
	static EzError FromNT(NTSTATUS nt, LPCSTR file, UINT32 line) noexcept;
	static EzError FromSE(DWORD se, LPCSTR file, UINT32 line) noexcept;
	static EzError FromException(std::exception ex, LPCSTR file, UINT32 line) noexcept;
	static EzError FromMessageA(LPCSTR message, LPCSTR file, UINT32 line) noexcept;
	static EzError FromMessageW(LPCWSTR message, LPCSTR file, UINT32 line) noexcept;
#ifdef UNICODE
#define FromMessage FromMessageW
#else
#define FromMessage FromMessageA
#endif // UNICODE

	static void SetSEHandler() noexcept;

private:
	static constexpr LPCSTR ErrorLogFilePath = "C:\\ProgramData\\EzLog.txt";

	static LPSTR ConstructMessage(LPCSTR text, LPCSTR source, LPCSTR file, UINT32 line) noexcept;
	static void PrintToConsole(LPCSTR message) noexcept;
	static void PrintToLogFile(LPCSTR message) noexcept;

	explicit EzError(LPSTR message, DWORD code, HRESULT hr, NTSTATUS nt, DWORD se) noexcept;

	LPSTR _message = NULL;
	DWORD _code = 0;
	HRESULT _hr = 0;
	NTSTATUS _nt = 0;
	DWORD _se = 0;
};

template<typename T>
T* EzAlloc(size_t size) {
	T* output = reinterpret_cast<T*>(new BYTE[size]);
	if (output == NULL) {
		throw EzError::FromMessageA("Memory allocation failed.", __FILE__, __LINE__);
	}
	return output;
}
template<typename T>
T* EzAlloc() {
	return EzAlloc<T>(sizeof(T));
}
template<typename T>
T* EzAllocArray(size_t count) {
	return EzAlloc<T>(count * sizeof(T));
}
template<typename T>
void EzFree(T** ptr) {
	if (ptr == NULL) {
		throw EzError::FromMessageA("ptr must be a valid pointer to a T*.", __FILE__, __LINE__);
	}
	if (*ptr == NULL) {
		return;
	}
	delete[] *ptr;
	*ptr = NULL;
}
template<typename T>
class EzScopeFree final {
public:
	EzScopeFree(T** target) {
		if (target == NULL) {
			throw EzError::FromMessageA("target must be a valid pointer to a T*.", __FILE__, __LINE__);
		}
		_target = target;
	}
	~EzScopeFree() noexcept {
		EzFree<T>(_target);
	}
	EzScopeFree() = delete;
	EzScopeFree(const EzScopeFree&) = delete;
	EzScopeFree& operator=(const EzScopeFree&) = delete;
	EzScopeFree(EzScopeFree&&) = delete;
	EzScopeFree& operator=(EzScopeFree&&) = delete;
private:
	T** _target;
};
#define EzScopeAlloc1(name, type) \
type* name = EzAlloc<type>(); \
EzScopeFree __scopefor_##name = EzScopeFree(name);
#define EzScopeAlloc(name, size, type) \
type* name = EzAlloc<type>(size); \
EzScopeFree<type> __scopefor_##name = EzScopeFree<type>(&name);
#define EzScopeAllocArray(name, count, type) \
type* name = EzAllocArray<type>(count); \
EzScopeFree<type> __scopefor_##name = EzScopeFree<type>(&name);
void EzClose(HANDLE* handle);
class EzScopeClose final {
public:
	EzScopeClose(HANDLE* target) {
		if (target == NULL) {
			throw EzError::FromMessageA("target must be a valid pointer to a HANDLE.", __FILE__, __LINE__);
		}
		_target = target;
	}
	~EzScopeClose() noexcept {
		EzClose(_target);
	}
	EzScopeClose() = delete;
	EzScopeClose(const EzScopeClose&) = delete;
	EzScopeClose& operator=(const EzScopeClose&) = delete;
	EzScopeClose(EzScopeClose&&) = delete;
	EzScopeClose& operator=(EzScopeClose&&) = delete;
private:
	HANDLE* _target;
};
#define EzScopeHandle(name) \
EzScopeClose __scopefor_##name = EzScopeClose(&name);