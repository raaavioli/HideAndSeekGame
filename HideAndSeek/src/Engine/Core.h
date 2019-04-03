#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS
	#ifdef ENGINE_BUILD_DLL
		#define ENGINE_API //__declspec(dllexport) if engine was exported to dll
	#else
		#define ENGINE_API //__declspec(dllimport) if engine was to be imported as a dll
	#endif
#elif ENGINE_PLATFORM_LINUX
	#error Linux is not supported!
#endif

#ifdef DEBUG
	#define ENGINE_ENABLE_ASSERTS
#endif

#ifdef ENGINE_ENABLE_ASSERTS
	#define APP_ASSERT(x, ...)\
	{\
		if(!(x)) {\
			APP_ERROR("	ASSERT FAILED: {0}\n			FILE: {1}\n			LINE: {2}\n", __VA_ARGS__, __FILE__, __LINE__);\
			__debugbreak();\
		}\
	}
	#define CORE_ASSERT(x, ...)\
	{\
		if(!(x)) {\
			CORE_ERROR("	ASSERT FAILED: {0}\n			FILE: {1}\n			LINE: {2}\n", __VA_ARGS__, __FILE__, __LINE__);\
			__debugbreak();\
		}\
	}

#else
	#define APP_ASSERT(x, ...)
	#define CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)