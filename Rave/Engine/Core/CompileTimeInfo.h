#pragma once

#ifndef NDEBUG
#	define RV_DEBUG
#	define rv_only_debug(code)		code
#	define rv_only_release(code)	
#else
#	define RV_RELEASE
#	define rv_only_debug(code)		
#	define rv_only_release(code)	code
#endif

#ifdef _WIN32
#	define RV_PLATFORM_WINDOWS
#	define rv_only_windows(code)	code
#	define rv_only_apple(code)		
#	define rv_only_linux(code)		
#	define rv_only_android(code)	
#endif
#ifdef __APPLE__
#	define RV_PLATFORM_APPLE
#	define rv_only_windows(code)	
#	define rv_only_apple(code)		code
#	define rv_only_linux(code)		
#	define rv_only_android(code)	
#endif
#ifdef __linux__
#	define RV_PLATFORM_LINUX
#	define rv_only_windows(code)	
#	define rv_only_apple(code)		
#	define rv_only_linux(code)		code
#	define rv_only_android(code)	
#endif
#ifdef __ANDROID__
#	define RV_PLATFORM_ANDROID
#	define rv_only_windows(code)	
#	define rv_only_apple(code)		
#	define rv_only_linux(code)		
#	define rv_only_android(code)	code
#endif

#ifdef RV_RENDERER_D3D11
#	define rv_only_d3d11(code)		code
#endif

#ifdef RV_TARGET_32
#	define RV_TARGET_x32
#	define RV_TARGET_X32
#	define RV_TARGET_x86
#	define RV_TARGET_X86
#	define RV_TARGET_86
#endif
#ifdef RV_TARGET_64
#	define RV_TARGET_x64
#	define RV_TARGET_X64
#endif

namespace rv
{
	static constexpr struct CompileTimeInfo
	{
		struct Build
		{
#		ifdef RV_DEBUG
			static constexpr bool debug		= true;
			static constexpr bool release	= false;
#		else
			static constexpr bool debug		= false;
			static constexpr bool release	= true;
#		endif
		} build;

		struct Platform
		{
#		ifdef RV_PLATFORM_WINDOWS
			static constexpr bool windows	= true;
			static constexpr bool apple		= false;
			static constexpr bool linux		= false;
			static constexpr bool android	= false;
#		endif
#		ifdef RV_PLATFORM_APPLE
			static constexpr bool windows	= false;
			static constexpr bool apple		= true;
			static constexpr bool linux		= false;
			static constexpr bool android	= false;
#		endif
#		ifdef RV_PLATFORM_LINUX
			static constexpr bool windows	= false;
			static constexpr bool apple		= false;
			static constexpr bool linux		= true;
#		ifdef RV_PLATFORM_ANDROID
			static constexpr bool android	= true;
#		else
			static constexpr bool android	= false;
#		endif
#		endif
		} platform;

		struct Target
		{
#		ifdef RV_TARGET_32
			static constexpr bool x32 = true;
			static constexpr bool x86 = true;
			static constexpr bool x64 = false;
			static constexpr int bit = 32;
#		endif
#		ifdef RV_TARGET_64
			static constexpr bool x32 = false;
			static constexpr bool x86 = false;
			static constexpr bool x64 = true;
			static constexpr int bit = 64;
#		endif
		} target;

		struct Renderer
		{
#		ifdef RV_RENDERER_D3D11
			static constexpr bool d3d11 = true;
#		endif
		} renderer;
	} cti;
}