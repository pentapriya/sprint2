#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include <mutex>
#include <ctime>


class Sunil
{
public:
	enum LogPriority
	{
		TracePriority, DebugPriority, InfoPriority, WarnPriority, ErrorPriority, CriticalPriority
	};

private:
	LogPriority priority = InfoPriority;
	std::mutex log_mutex;
	
	const char* filepath = 0;
	std::FILE* file = 0;
	
	// for timestamp formatting
	char buffer[80];
	const char* timestamp_format = "%T  %d-%m-%Y";

public:
	// Set desired priority for the logger (messages with lower priority will not be recorded)
	// The default priority is Yellog::InfoPriority
	static void SetPriority(LogPriority new_priority)
	{
		get_instance().priority = new_priority;
	}

	// Get the current logger priority (messages with lower priority will not be recorded)
	// The default priority is Yellog::InfoPriority
	static LogPriority GetPriority()
	{
		return get_instance().priority;
	}

	// Enable file output
	// Logs will be written to /log.txt
	// If the file doesn't exist, it will create it automatically
	// File will be closed when program stops
	// Returns true if a file was successfully opened, false otherwise
	static bool EnableFileOutput()
	{
		Sunil& Sunil_instance = get_instance();
		Sunil_instance.filepath = "log.txt";
		return Sunil_instance.enable_file_output();
	}

	// Enable file output
	// Logs will be written to /filepath, provided the filepath is valid
	// If the file doesn't exist, it will create it automatically
	// File will be closed when program stops
	// Returns true if a file was successfully opened, false otherwise
	static bool EnableFileOutput(const char* new_filepath)
	{
		Sunil& Sunil_instance = get_instance();
		Sunil_instance.filepath = new_filepath;
		return Sunil_instance.enable_file_output();
	}

	// Returns the current filepath for file logging
	// if Yellog::EnableFileOutput was called without specifying a filepath, the filepath will be "log.txt"
	// if file output was not enabled, the filepath will contain NULL
	static const char* GetFilepath()
	{
		return get_instance().filepath;
	}

	// Returns true is file output was enabled and file was successfully opened, false if it wasn't
	static bool IsFileOutputEnabled()
	{
		return get_instance().file != 0;
	}

	// Set a log timestamp format
	// Format follows <ctime> strftime format specification
	// Default format is "%T  %d-%m-%Y" (e.g. 13:20:25  14-02-2021)
	// 4 spaces are added automatically to the end of timestamp each time the message is logged
	static void SetTimestampFormat(const char* new_timestamp_format)
	{
		get_instance().timestamp_format = new_timestamp_format;
	}

	// Get the current log timestamp format
	// Format follows <ctime> strftime format specification
	// Default format is "%T  %d-%m-%Y" (e.g. 13:20:25  14-02-2021)
	static const char* GetTimestampFormat()
	{
		return get_instance().timestamp_format;
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::TracePriority
	template<typename... Args>
	static void Trace(const char* message, Args... args)
	{
		get_instance().log("[Trace]    ", TracePriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::DebugPriority
	template<typename... Args>
	static void Debug(const char* message, Args... args)
	{
		get_instance().log("[Debug]    ", DebugPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::InfoPriority
	template<typename... Args>
	static void Info(const char* message, Args... args)
	{
		get_instance().log("[Info]     ", InfoPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::WarnPriority
	template<typename... Args>
	static void Warn(const char* message, Args... args)
	{
		get_instance().log("[Warn]     ", WarnPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::ErrorPriority
	template<typename... Args>
	static void Error(const char* message, Args... args)
	{
		get_instance().log("[Error]    ", ErrorPriority, message, args...);
	}

	// Log a message (format + optional args, follow printf specification)
	// with log priority level Yellog::CriticalPriority
	template<typename... Args>
	static void Critical(const char* message, Args... args)
	{
		get_instance().log("[Critical]     ", CriticalPriority, message, args...);
	}

private:
	Sunil() {}

	Sunil(const Sunil&) = delete;
	Sunil& operator= (const Sunil&) = delete;

	~Sunil()
	{
		free_file();
	}

	static Sunil& get_instance()
	{
		static Sunil instance;
		
		return instance;
	}

	template<typename... Args>
	void log(const char* message_priority_str, LogPriority message_priority, const char* message, Args... args)
	{
		if (priority <= message_priority)
		{
			std::time_t current_time = std::time(0);
			std::tm* timestamp = std::localtime(&current_time);

			std::scoped_lock lock(log_mutex);
			std::strftime(buffer, 80, timestamp_format, timestamp);
			std::printf("%s    ", buffer);
			std::printf(message_priority_str);
			std::printf(message, args...);
			std::printf("\n");

			if (file)
			{
				std::fprintf(file, "%s    ", buffer);
				std::fprintf(file, message_priority_str);
				std::fprintf(file, message, args...);
				std::fprintf(file, "\n");
			}
		}
	}

	bool enable_file_output()
	{
		free_file();

		file = std::fopen(filepath, "a");

		if (file == 0)
		{
			return false;
		}

		return true;
	}

	void free_file()
	{
		if (file)
		{
			std::fclose(file);
			file = 0;
		}
	}
};
