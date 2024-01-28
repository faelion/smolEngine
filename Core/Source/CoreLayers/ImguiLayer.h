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

		void Start() override { SMOL_CORE_TRACE("ImGui Layer Started"); }
		void Update() override {}
		void CleanUp() override { SMOL_CORE_TRACE("ImGui Layer Cleaned Up"); }

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
		inline ImGuiContext* GetContext() { return m_ImguiEnabler->GetImguiContext(); }
	private:
		inline static ImGuiLog s_Log;
	private:
		ImGuiLog m_Console;

		// Modules
		std::unique_ptr<ImguiEnabler> m_ImguiEnabler;
	};
}
