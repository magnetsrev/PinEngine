#pragma once
#include <comdef.h>
#include <string>

namespace PinEngine
{
//The WIDE1/WIDE2/WFILE defines are used to be able to get a wstring version of __FILE__ and __FUNCTION__
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)
#define WFUNCTION WIDE1(__FUNCTION__)
#define COM_ERROR_IF_FAILED( hr, msg ) if( FAILED( hr ) ) throw COMException( hr, msg, WFILE, WFUNCTION, __LINE__ )
#define COM_WARNING_IF_FAILED( hr, msg ) if( FAILED( hr ) ) { COMException ex( hr, msg, WFILE, WFUNCTION, __LINE__ ); MessageBoxW(NULL, L"Warning!", ex.what(), MB_SYSTEMMODAL); }

	class COMException
	{
	public:
		COMException(HRESULT hr, const std::wstring& msg, const std::wstring& file, const std::wstring& function, int line)
		{
			_com_error error(hr);
			whatmsg = L"Msg: " + std::wstring(msg) + L"\n";
			whatmsg += error.ErrorMessage();
			whatmsg += L"\nFile: " + file;
			whatmsg += L"\nFunction: " + function;
			whatmsg += L"\nLine: " + std::to_wstring(line);
		}

		const wchar_t* what() const
		{
			return whatmsg.c_str();
		}
	private:
		std::wstring whatmsg;
	};
}