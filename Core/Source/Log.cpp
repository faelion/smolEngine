#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "CoreLayers/ImguiLayer.h"
#include "Application.h"

namespace smol {
	std::shared_ptr<spdlog::logger>Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger>Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		s_CoreLogger = spdlog::stdout_color_mt("WIWA");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);		
	}
	const char* Log::GetCoreLastLog()
	{
		const std::vector<spdlog::sink_ptr>& sinks = s_CoreLogger.get()->sinks();

		return sinks[0].get()->get_last();
	}
	const char* Log::GetClientLastLog()
	{
		const std::vector<spdlog::sink_ptr>& sinks = s_ClientLogger.get()->sinks();

		return sinks[0].get()->get_last();
	}

	void Log::ImGuiLogTrace(const char* log)
	{
		ImguiLayer::CoreLogTrace(log);
	}

	void Log::ImGuiLogInfo(const char* log)
	{
		ImguiLayer::CoreLogInfo(log);
	}
	void Log::ImGuiLogWarn(const char* log)
	{
		ImguiLayer::CoreLogWarn(log);
	}
	void Log::ImGuiLogError(const char* log)
	{
		ImguiLayer::CoreLogError(log);
	}
	void Log::ImGuiLogCritical(const char* log)
	{
		ImguiLayer::CoreLogCritical(log);
	}
	void Log::ImGuiConsoleTrace(const char* log)
	{
		App::Get().GetImguiLayer().LogTrace(log);
	}
	void Log::ImGuiConsoleInfo(const char* log)
	{
		App::Get().GetImguiLayer().LogInfo(log);
	}
	void Log::ImGuiConsoleWarn(const char* log)
	{
		App::Get().GetImguiLayer().LogWarn(log);
	}
	void Log::ImGuiConsoleError(const char* log)
	{
		App::Get().GetImguiLayer().LogError(log);
	}
	void Log::ImGuiConsoleCritical(const char* log)
	{
		App::Get().GetImguiLayer().LogCritical(log);
	}
}