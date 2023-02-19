#pragma once

#ifndef LOG_INCLUDE_LOG_LOG_HPP
#define LOG_INCLUDE_LOG_LOG_HPP

#include <iostream>
#include <memory>
#include <ostream>

namespace __log {

/// Thin wrapper class for `std::ostream` with filter.
class LogStream : public std::ostream {
  public:
    LogStream() = default;
    inline LogStream(bool ignore, std::ostream& os) { (ignore) ? this->os = 0 : this->os = &os; };

    template <class T>
    inline std::ostream& operator<<(const T& t) {
        return (os) ? (*os) << t : *this;
    }

  private:
    /// if set the stream will ignore input
    std::ostream* os;
};

/// Singleton logger/tracing class
class Logger {
  public:
    /// Enum class containing possible log levels.
    /// Note: The value of the level defines the importance.
    enum class LogLevel { None = 0, Error = 1, Warn = 2, Info = 3, Debug = 4, Trace = 5, Default = Debug };

    /// Returns the singleton `Logger` instance.
    static Logger& get_instance() {
        static Logger instance; // ensures instanciation on first use and destruction
        return instance;
    }

    /// Configures the `Logger` with the provided stream and level. All levels
    /// below (see `LogLevel` enum) the set level, will be ignored.
    inline void config(LogLevel level, std::ostream& out) {
        this->level = level;
        this->out = &out;
    }

    /// Note: Sould not be used by end user. Provided `LOG` macro should be used.
    inline std::unique_ptr<LogStream> log(const std::string& file, unsigned int line, const std::string& function,
                                          LogLevel level) const {
        const bool ignore = (level > this->level || this->level == LogLevel::None);
        auto logstream = std::make_unique<__log::LogStream>(ignore, *this->out);

        const char* level_string =
            (const char*[]){"", "ERROR", "WARN", "INFO", "DEBUG", "TRACE"}[static_cast<int>(level)];

        *(logstream) << '[' << level_string << "][" << file << ':' << line << "][" << function << "] ";

        return logstream;
    }

  private:
    Logger() {}

    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;

  private:
    /// Log stream used to write to.
    std::ostream* out = &std::cerr;

    /// Configured log level.
    LogLevel level = LogLevel::Default;
};

} // namespace __log

#define LOG(level) *__log::Logger::get_instance().log(__FILE__, __LINE__, __func__, __log::Logger::LogLevel::level)

#define LOG_CONFIGURE(level, out) __log::Logger::get_instance().config(__log::Logger::LogLevel::level, out)

#endif // LOG_INCLUDE_LOG_LOG_HPP
