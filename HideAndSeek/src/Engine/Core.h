#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS
	#ifdef ENGINE_BUILD_DLL
		#define ENGINE_API //__declspec(dllexport) if engine was exported to dll
	#else
		#define ENGINE_API //__declspec(dllimport) if endine was to be imported as a dll
	#endif
#elif ENGINE_PLATFORM_LINUX
	#error Linux is not supported!
#endif
