#pragma once
#include "Layer.h"
#include "Utils/ImguiLog.h"
#include "Modules/ImguiEnabler.h"

struct ImGuiContext;

namespace smol {
	class ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();

		void Begin();
		void End();

		inline void LogTrace(const char* log) { m_Console.AddLog(ImGuiLog::LogLevel::TRACE, log); }
		inline void LogInfo(const char* log) { m_Console.AddLog(ImGuiLog::LogLevel::INFO, log); }
		inline void LogWarn(const char* log) { m_Console.AddLog(ImGuiLog::LogLevel::WARN, log); }
		inline void LogError(const char* log) { m_Console.AddLog(ImGuiLog::LogLevel::ERR, log); }
		inline void LogCritical(const char* log) { m_Console.AddLog(ImGuiLog::LogLevel::CRITICAL, log); }

		inline static void CoreLogTrace(const char* log) { s_Log.AddLog(ImGuiLog::LogLevel::TRACE, log); }
		inline static void CoreLogInfo(const char* log) { s_Log.AddLog(ImGuiLog::LogLevel::INFO, log); }
		inline static void CoreLogWarn(const char* log) { s_Log.AddLog(ImGuiLog::LogLevel::WARN, log); }
		inline static void CoreLogError(const char* log) { s_Log.AddLog(ImGuiLog::LogLevel::ERR, log); }
		inline static void CoreLogCritical(const char* log) { s_Log.AddLog(ImGuiLog::LogLevel::CRITICAL, log); }

		inline static ImGuiLog& GetLog() { return s_Log; }
		inline ImGuiLog& GetConsole() { return m_Console; }
		inline ImGuiContext* GetContext() { return imgui->GetImguiContext(); }
	private:
		inline static ImGuiLog s_Log;
	private:

		// Modules
		std::unique_ptr<ImguiEnabler> imgui;

		ImGuiLog m_Console;
		float m_Time = 0.0f;
	};
}
