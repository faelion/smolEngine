#pragma once
#pragma warning(disable : 4251)

#include "imgui.h"
#include "spdlog/spdlog.h"
#include "Utils/ImguiLog.h"

namespace smol {
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		static const char* GetCoreLastLog();
		static const char* GetClientLastLog();

		static void ImGuiLogTrace(const char* log);
		static void ImGuiLogInfo(const char* log);
		static void ImGuiLogWarn(const char* log);
		static void ImGuiLogError(const char* log);
		static void ImGuiLogCritical(const char* log);

		static void ImGuiConsoleTrace(const char* log);
		static void ImGuiConsoleInfo(const char* log);
		static void ImGuiConsoleWarn(const char* log);
		static void ImGuiConsoleError(const char* log);
		static void ImGuiConsoleCritical(const char* log);
	private:
		Log() = default;

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}
#ifndef SMOL_DIST
//Core log macros
#define SMOL_CORE_TRACE(...)      ::smol::Log::GetCoreLogger()->trace(__VA_ARGS__); ::smol::Log::ImGuiLogTrace(::smol::Log::GetCoreLastLog())
#define SMOL_CORE_INFO(...)       ::smol::Log::GetCoreLogger()->info(__VA_ARGS__);  ::smol::Log::ImGuiLogInfo(::smol::Log::GetCoreLastLog())
#define SMOL_CORE_WARN(...)       ::smol::Log::GetCoreLogger()->warn(__VA_ARGS__); ::smol::Log::ImGuiLogWarn(::smol::Log::GetCoreLastLog())
#define SMOL_CORE_ERROR(...)      ::smol::Log::GetCoreLogger()->error(__VA_ARGS__);  ::smol::Log::ImGuiLogError(::smol::Log::GetCoreLastLog())
#define SMOL_CORE_CRITICAL(...)      ::smol::Log::GetCoreLogger()->critical(__VA_ARGS__); ::smol::Log::ImGuiLogCritical(::smol::Log::GetCoreLastLog())
#else
#define SMOL_CORE_TRACE(...)   
#define SMOL_CORE_INFO(...)    
#define SMOL_CORE_WARN(...)    
#define SMOL_CORE_ERROR(...)   
#define SMOL_CORE_CRITICAL(...)
#endif


//Client log macros
#define SMOL_TRACE(...)           ::smol::Log::GetClientLogger()->trace(__VA_ARGS__); ::smol::Log::ImGuiConsoleTrace(::smol::Log::GetClientLastLog())
#define SMOL_INFO(...)            ::smol::Log::GetClientLogger()->info(__VA_ARGS__); ::smol::Log::ImGuiConsoleInfo(::smol::Log::GetClientLastLog())
#define SMOL_WARN(...)            ::smol::Log::GetClientLogger()->warn(__VA_ARGS__); ::smol::Log::ImGuiConsoleWarn(::smol::Log::GetClientLastLog())
#define SMOL_ERROR(...)           ::smol::Log::GetClientLogger()->error(__VA_ARGS__); ::smol::Log::ImGuiConsoleError(::smol::Log::GetClientLastLog())
#define SMOL_CRITICAL(...)           ::smol::Log::GetClientLogger()->critical(__VA_ARGS__); ::smol::Log::ImGuiConsoleCritical(::smol::Log::GetClientLastLog())