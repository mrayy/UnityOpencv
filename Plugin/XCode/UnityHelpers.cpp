

#include "UnityHelpers.h"

#include "Unity/IUnityInterface.h"
#ifdef WIN32
#include <windows.h>
#endif

#include "PixelUtil.h"
#include "ImageInfo.h"

using namespace mray;
using namespace video;

FuncPtr Debug;
FuncFloatRetPtr GetEngineTimePtr;

void LogMessage(const std::string& msg, ELogLevel level)
{
	std::string m;
	if (level == ELL_INFO)
		m = "Info: ";
	if (level == ELL_WARNING)
		m = "Warning: ";
	if (level == ELL_SUCCESS)
		m = "Success: ";
	if (level == ELL_ERROR)
		m = "Error: ";

	m += msg;
#if UNITY_WIN
	OutputDebugStringA(m.c_str());
#else
	printf("%s", m.c_str());
#endif
	LogManager::Instance()->LogMessage(m);
// 	if (Debug)
// 		Debug(m.c_str());
}
float GetEngineTime()
{
	if (GetEngineTimePtr)
	{
		return GetEngineTimePtr();
	}
	return 0;
}
extern "C" UNITY_INTERFACE_EXPORT void mray_SetDebugFunction(FuncPtr f)
{
	Debug = f;
}
extern "C" UNITY_INTERFACE_EXPORT void mray_SetGetEngineTime(FuncFloatRetPtr f)
{
	GetEngineTimePtr = f;
}

LogManager* LogManager::s_instance=0;
LogManager* LogManager::Instance()
{
	if (!s_instance)
	{
		s_instance = new LogManager();
	}
	return s_instance;
}

LogManager::LogManager()
{
	fileName = "GStreamerLog.txt";
	m_logFile = fopen("GStreamerLog.txt", "w");
	fclose(m_logFile);
	m_logFile = 0;
}
LogManager::~LogManager()
{
	fclose(m_logFile);
}
void LogManager::LogMessage(const std::string& msg)
{
	m_logFile = fopen("GStreamerLog.txt", "a");
	fprintf(m_logFile, "%s\n", msg.c_str());
	fclose(m_logFile);
	m_logFile = 0;
}


