#pragma once
#include "Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Mama {

	class MAMA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		 
	};

}

// CORE MACROS USED IN MAMA_API
#define MAMA_CORE_TRACE(...)    ::Mama::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MAMA_CORE_INFO(...)     ::Mama::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MAMA_CORE_WARN(...)     ::Mama::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MAMA_CORE_ERROR(...)    ::Mama::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MAMA_CORE_FATAL(...)    ::Mama::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client MACROS USED IN SANDBOX
#define MAMA_TRACE(...)	      ::Mama::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MAMA_INFO(...)	      ::Mama::Log::GetClientLogger()->info(__VA_ARGS__)
#define MAMA_WARN(...)	      ::Mama::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MAMA_ERROR(...)	      ::Mama::Log::GetClientLogger()->error(__VA_ARGS__)
#define MAMA_FATAL(...)	      ::Mama::Log::GetClientLogger()->fatal(__VA_ARGS__)