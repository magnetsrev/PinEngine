#include "BuildPath.h"

namespace PinEngine
{

	std::wstring BuildPath()
	{
	#ifdef _DEBUG //Debug Mode
		#ifdef _WIN64 //x64
			return L"..\\x64\\Debug\\";
		#else  //x86 (Win32)
			return L"..\\Debug\\";
		#endif
	#else //Release Mode
		#ifdef _WIN64 //x64
			return L"..\\x64\\Release\\";
		#else  //x86 (Win32)
			return L"..\\Release\\";
		#endif
	#endif
	}
}