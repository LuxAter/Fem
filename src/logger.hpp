#ifndef ARTA_LOGGER_HPP_
#define ARTA_LOGGER_HPP_

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

#include <array>
#include <cstdarg>
#include <cstdlib>
#include <ctime>
#include <string>

namespace arta {
namespace log {
#define Default() Logger::get()
#define Console() Logger::get()->get_logger(arta::log::Logger::CONSOLE)
#define OutFile() Logger::get()->get_logger(arta::log::Logger::OUT_FILE)
#define fatal(msg, ...)                                                    \
  Logger::get()->_log(arta::log::FATAL, msg, __FILE__, __func__, __LINE__, \
                      ##__VA_ARGS__)
#define error(msg, ...)                                                    \
  Logger::get()->_log(arta::log::ERROR, msg, __FILE__, __func__, __LINE__, \
                      ##__VA_ARGS__)
#define warning(msg, ...)                                                    \
  Logger::get()->_log(arta::log::WARNING, msg, __FILE__, __func__, __LINE__, \
                      ##__VA_ARGS__)
#define status(msg, ...)                                                    \
  Logger::get()->_log(arta::log::STATUS, msg, __FILE__, __func__, __LINE__, \
                      ##__VA_ARGS__)
#define success(msg, ...)                                                    \
  Logger::get()->_log(arta::log::SUCCESS, msg, __FILE__, __func__, __LINE__, \
                      ##__VA_ARGS__)
#define debug(msg, ...)                                                    \
  Logger::get()->_log(arta::log::DEBUG, msg, __FILE__, __func__, __LINE__, \
                      ##__VA_ARGS__)
#define trace(msg, ...)                                                    \
  Logger::get()->_log(arta::log::TRACE, msg, __FILE__, __func__, __LINE__, \
                      ##__VA_ARGS__)
#define info(msg, ...)                                                    \
  Logger::get()->_log(arta::log::INFO, msg, __FILE__, __func__, __LINE__, \
                      ##__VA_ARGS__)
#define version(msg, ...)                                                    \
  Logger::get()->_log(arta::log::VERSION, msg, __FILE__, __func__, __LINE__, \
                      ##__VA_ARGS__)

  enum LogType {
    FATAL = (1u << 0),
    ERROR = (1u << 1),
    WARNING = (1u << 2),
    SUCCESS = (1u << 3),
    DEBUG = (1u << 4),
    TRACE = (1u << 5),
    INFO = (1u << 6),
    VERSION = (1u << 7),
    STATUS = (1u << 8)
  };

  class LoggerBase {
   public:
    LoggerBase() {}
    virtual ~LoggerBase() {}
    void _log(LogType type, std::string_view message, std::string_view file,
              std::string_view func, unsigned long line, va_list args) {
      if ((activation_mask_ & type) != type) return;
      char fmt[255];
      std::time_t raw_time;
      std::time(&raw_time);
      std::tm* current_tm = std::localtime(&raw_time);
      vsnprintf(fmt, 255, message.data(), args);
      std::string body_str(fmt);
      if (body_str.size() < padding_[3])
        body_str = std::string(padding_[3] - body_str.size(), ' ') + body_str;
      std::string file_str(file);
      if (file_str.size() < padding_[0])
        file_str = std::string(padding_[0] - file_str.size(), ' ') + file_str;
      std::string func_str(func);
      if (func_str.size() < padding_[1])
        func_str = std::string(padding_[1] - func_str.size(), ' ') + func_str;
      std::string type_str = type_string(type);
      std::string fmt_str = log_fmt_[get_index(type)];
      if (line_pad_zero_)
        snprintf(fmt, 255, "%0*lu", padding_[2], line);
      else
        snprintf(fmt, 255, "%*lu", padding_[2], line);
      std::string line_str(fmt);
      std::strftime(fmt, 255, "%Y-%m-%d", current_tm);
      std::string date_str(fmt);
      std::strftime(fmt, 255, "%H:%M:%S", current_tm);
      std::string time_str(fmt);
      std::strftime(fmt, 255, "%Y-%m-%d %H:%M:%S", current_tm);
      std::string date_time_str(fmt);
      replace_substr(fmt_str, "__TYPE__", type_str);
      replace_substr(fmt_str, "__FILE__", file_str);
      replace_substr(fmt_str, "__FUNC__", func_str);
      replace_substr(fmt_str, "__LINE__", line_str);
      replace_substr(fmt_str, "__BODY__", body_str);
      replace_substr(fmt_str, "__DATE__", date_str);
      replace_substr(fmt_str, "__TIME__", time_str);
      replace_substr(fmt_str, "__DATE_TIME__", date_time_str);
      handle_log(type, fmt_str);
    }
    void _log(LogType type, std::string_view message, std::string_view file,
              std::string_view func, unsigned long line, ...) {
      if ((activation_mask_ & type) != type) return;
      va_list args;
      va_start(args, line);
      char fmt[255];
      std::time_t raw_time;
      std::time(&raw_time);
      std::tm* current_tm = std::localtime(&raw_time);
      vsnprintf(fmt, 255, message.data(), args);
      std::string body_str(fmt);
      if (body_str.size() < padding_[3])
        body_str = std::string(padding_[3] - body_str.size(), ' ') + body_str;
      std::string file_str(file);
      if (file_str.size() < padding_[0])
        file_str = std::string(padding_[0] - file_str.size(), ' ') + file_str;
      std::string func_str(func);
      if (func_str.size() < padding_[1])
        func_str = std::string(padding_[1] - func_str.size(), ' ') + func_str;
      std::string type_str = type_string(type);
      std::string fmt_str = log_fmt_[get_index(type)];
      if (line_pad_zero_)
        snprintf(fmt, 255, "%0*lu", padding_[2], line);
      else
        snprintf(fmt, 255, "%*lu", padding_[2], line);
      std::string line_str(fmt);
      std::strftime(fmt, 255, "%Y-%m-%d", current_tm);
      std::string date_str(fmt);
      std::strftime(fmt, 255, "%H:%M:%S", current_tm);
      std::string time_str(fmt);
      std::strftime(fmt, 255, "%Y-%m-%d %H:%M:%S", current_tm);
      std::string date_time_str(fmt);
      replace_substr(fmt_str, "__TYPE__", type_str);
      replace_substr(fmt_str, "__FILE__", file_str);
      replace_substr(fmt_str, "__FUNC__", func_str);
      replace_substr(fmt_str, "__LINE__", line_str);
      replace_substr(fmt_str, "__BODY__", body_str);
      replace_substr(fmt_str, "__DATE__", date_str);
      replace_substr(fmt_str, "__TIME__", time_str);
      replace_substr(fmt_str, "__DATE_TIME__", date_time_str);
      handle_log(type, fmt_str);
      va_end(args);
    }

    inline void format(LogType type, std::string fmt) {
      log_fmt_[get_index(type)] = fmt;
    }
    inline void format_all(std::string fmt) {
      log_fmt_ = {{fmt, fmt, fmt, fmt, fmt, fmt, fmt, fmt, fmt}};
    }
    inline void format_fatal(std::string fmt) { log_fmt_[0] = fmt; }
    inline void format_error(std::string fmt) { log_fmt_[1] = fmt; }
    inline void format_warning(std::string fmt) { log_fmt_[2] = fmt; }
    inline void format_success(std::string fmt) { log_fmt_[3] = fmt; }
    inline void format_debug(std::string fmt) { log_fmt_[4] = fmt; }
    inline void format_trace(std::string fmt) { log_fmt_[5] = fmt; }
    inline void format_info(std::string fmt) { log_fmt_[6] = fmt; }
    inline void format_version(std::string fmt) { log_fmt_[7] = fmt; }
    inline void format_status(std::string fmt) { log_fmt_[8] = fmt; }

    inline void set_fatal(bool val) {
      val ? activation_mask_ |= FATAL : activation_mask_ &= ~FATAL;
    }
    inline void set_error(bool val) {
      val ? activation_mask_ |= ERROR : activation_mask_ &= ~ERROR;
    }
    inline void set_warning(bool val) {
      val ? activation_mask_ |= WARNING : activation_mask_ &= ~WARNING;
    }
    inline void set_success(bool val) {
      val ? activation_mask_ |= SUCCESS : activation_mask_ &= ~SUCCESS;
    }
    inline void set_debug(bool val) {
      val ? activation_mask_ |= DEBUG : activation_mask_ &= ~DEBUG;
    }
    inline void set_trace(bool val) {
      val ? activation_mask_ |= TRACE : activation_mask_ &= ~TRACE;
    }
    inline void set_info(bool val) {
      val ? activation_mask_ |= INFO : activation_mask_ &= ~INFO;
    }
    inline void set_version(bool val) {
      val ? activation_mask_ |= VERSION : activation_mask_ &= ~VERSION;
    }
    inline void set_status(bool val) {
      val ? activation_mask_ |= STATUS : activation_mask_ &= ~STATUS;
    }
    inline void set_activation(unsigned mask) { activation_mask_ = mask; }
    inline void set_activation_mask(LogType type, bool val) {
      val ? activation_mask_ |= type : activation_mask_ &= ~type;
    }

    inline void set_line_zero_pad(bool val) { line_pad_zero_ = val; }
    inline void set_file_pad(unsigned val) { padding_[0] = val; }
    inline void set_func_pad(unsigned val) { padding_[1] = val; }
    inline void set_line_pad(unsigned val) { padding_[2] = val; }
    inline void set_body_pad(unsigned val) { padding_[3] = val; }
    inline void set_pad(unsigned id, unsigned val) { padding_[id] = val; }

    virtual void set_color(bool val){}

   protected:
    virtual void handle_log(const LogType&, const std::string&) {}
    unsigned activation_mask_ = FATAL | ERROR | WARNING | SUCCESS | DEBUG |
                                TRACE | INFO | VERSION | STATUS;
    bool line_pad_zero_ = false;
    std::array<unsigned, 4> padding_ = {{0, 0, 0, 0}};
    std::array<std::string, 9> log_fmt_ = {
        {"[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) __BODY__",
         "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) __BODY__",
         "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) __BODY__",
         "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) __BODY__",
         "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) __BODY__",
         "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) __BODY__",
         "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) __BODY__",
         "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) __BODY__",
         "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) __BODY__"}};

   private:
    inline unsigned get_index(const LogType& type) const {
      switch (type) {
        case FATAL:
          return 0;
        case ERROR:
          return 1;
        case WARNING:
          return 2;
        case SUCCESS:
          return 3;
        case DEBUG:
          return 4;
        case TRACE:
          return 5;
        case INFO:
          return 6;
        case VERSION:
          return 7;
        case STATUS:
          return 8;
        default:
          return 0;
      };
    }
    std::string& replace_substr(std::string& src, std::string pat,
                                std::string_view rep) {
      while (true) {
        std::size_t index = 0;
        index = src.find(pat);
        if (index == std::string::npos) break;
        src.replace(index, pat.size(), rep);
      }
      return src;
    }

    std::string type_string(LogType type) {
      switch (type) {
        case FATAL:
          return "FATAL  ";
        case ERROR:
          return "ERROR  ";
        case WARNING:
          return "WARNING";
        case SUCCESS:
          return "SUCCESS";
        case DEBUG:
          return "DEBUG  ";
        case TRACE:
          return "TRACE  ";
        case INFO:
          return "INFO   ";
        case VERSION:
          return "VESION ";
        case STATUS:
          return "STATUS ";
      };
    }
  };

  class ConsoleLogger : public LoggerBase {
   public:
    ConsoleLogger() : LoggerBase() { set_color(true); }
    virtual ~ConsoleLogger() {}

    inline static ConsoleLogger* get() {
      static ConsoleLogger instance;
      return &instance;
    }

    void set_color(bool val) {
      if (val) {
        this->log_fmt_ = {
          {
            "\033[1;31m" + this->log_fmt_[0] + "\033[0m",
            "\033[1;31m" + this->log_fmt_[1] + "\033[0m",
            "\033[1;33m" + this->log_fmt_[2] + "\033[0m",
            "\033[1;32m" + this->log_fmt_[3] + "\033[0m",
            "\033[1;34m" + this->log_fmt_[4] + "\033[0m",
            "\033[1;35m" + this->log_fmt_[5] + "\033[0m",
            "\033[1;37m" + this->log_fmt_[6] + "\033[0m",
            "\033[1;36m" + this->log_fmt_[7] + "\033[0m",
            "\033[1;33m" + this->log_fmt_[8] + "\033[0m",
          }
        };
        // this->log_fmt_ = {
        //     {"\033[1;31m[__TYPE__] <__DATE_TIME__> "
        //      "(__FILE__:__FUNC__:__LINE__) __BODY__\033[0m",
        //      "\033[1;31m[__TYPE__] <__DATE_TIME__> "
        //      "(__FILE__:__FUNC__:__LINE__) "
        //      "__BODY__\033[0m",
        //      "\033[1;33m[__TYPE__] <__DATE_TIME__> "
        //      "(__FILE__:__FUNC__:__LINE__) "
        //      "__BODY__\033[0m",
        //      "\033[1;32m[__TYPE__] <__DATE_TIME__> "
        //      "(__FILE__:__FUNC__:__LINE__) "
        //      "__BODY__\033[0m",
        //      "\033[1;34m[__TYPE__] <__DATE_TIME__> "
        //      "(__FILE__:__FUNC__:__LINE__) "
        //      "__BODY__\033[0m",
        //      "\033[1;35m[__TYPE__] <__DATE_TIME__> "
        //      "(__FILE__:__FUNC__:__LINE__) "
        //      "__BODY__\033[0m",
        //      "\033[1;37m[__TYPE__] <__DATE_TIME__> "
        //      "(__FILE__:__FUNC__:__LINE__) "
        //      "__BODY__\033[0m",
        //      "\033[1;36m[__TYPE__] <__DATE_TIME__> "
        //      "(__FILE__:__FUNC__:__LINE__) "
        //      "__BODY__\033[0m",
        //      "\033[1;33m[__TYPE__] <__DATE_TIME__> "
        //      "(__FILE__:__FUNC__:__LINE__) __BODY__\033[0m"}};
      } else {
        this->log_fmt_ = {
            {"[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) "
             "__BODY__",
             "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) "
             "__BODY__",
             "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) "
             "__BODY__",
             "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) "
             "__BODY__",
             "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) "
             "__BODY__",
             "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) "
             "__BODY__",
             "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) "
             "__BODY__",
             "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) "
             "__BODY__",
             "[__TYPE__] <__DATE_TIME__> (__FILE__:__FUNC__:__LINE__) "
             "__BODY__"}};
      }
    }

   protected:
    virtual void handle_log(const LogType& type, const std::string& log_msg) {
      printf("%s\n", log_msg.c_str());
    }
  };

  class OutLogger : public LoggerBase {
   public:
    OutLogger(const char* file_path = "out.log") : LoggerBase() {
      file = std::fopen(file_path, "w");
    }
    virtual ~OutLogger() { std::fclose(file); }

    inline static OutLogger* get() {
      static OutLogger instance;
      return &instance;
    }

    void set_flush(unsigned mask) { flush_types = mask; }

   protected:
    virtual void handle_log(const LogType& type, const std::string& log_msg) {
      fprintf(file, "%s\n", log_msg.c_str());
      if ((flush_types & type) == type) fflush(file);
    }
    unsigned flush_types = FATAL | ERROR | WARNING;
    FILE* file = nullptr;
  };

  class Logger {
   public:
    enum LoggerType { CONSOLE = (1u << 0), OUT_FILE = (1u << 1) };
    Logger() {}
    virtual ~Logger() {}

    inline void _log(LogType type, std::string_view message,
                     std::string_view file, std::string_view func,
                     unsigned long line, ...) {
      if ((logger_ & CONSOLE) == CONSOLE) {
        va_list args;
        va_start(args, line);
        ConsoleLogger::get()->_log(type, message, file, func, line, args);
        va_end(args);
      }
      if ((logger_ & OUT_FILE) == OUT_FILE) {
        va_list args;
        va_start(args, line);
        OutLogger::get()->_log(type, message, file, func, line, args);
        va_end(args);
      }
    }

    inline static Logger* get() {
      static Logger instance;
      return &instance;
    }
    inline LoggerBase* get_logger(LoggerType logger) {
      switch (logger) {
        case CONSOLE:
          return ConsoleLogger::get();
        case OUT_FILE:
          return OutLogger::get();
      }
      return NULL;
    }

    inline void set_console(bool val) {
      val ? logger_ |= CONSOLE : logger_ &= ~CONSOLE;
    }
    inline void set_out_file(bool val) {
      val ? logger_ |= OUT_FILE : logger_ &= ~OUT_FILE;
    }

    inline void format(LogType type, std::string fmt) {
      if ((logger_ & CONSOLE) == CONSOLE)
        ConsoleLogger::get()->format(type, fmt);
      if ((logger_ & OUT_FILE) == OUT_FILE) OutLogger::get()->format(type, fmt);
    }
    inline void format_all(std::string fmt) {
      if ((logger_ & CONSOLE) == CONSOLE) ConsoleLogger::get()->format_all(fmt);
      if ((logger_ & OUT_FILE) == OUT_FILE) OutLogger::get()->format_all(fmt);
    }

    inline void format_fatal(std::string fmt) { format(FATAL, fmt); }
    inline void format_error(std::string fmt) { format(ERROR, fmt); }
    inline void format_warning(std::string fmt) { format(WARNING, fmt); }
    inline void format_success(std::string fmt) { format(SUCCESS, fmt); }
    inline void format_debug(std::string fmt) { format(DEBUG, fmt); }
    inline void format_trace(std::string fmt) { format(TRACE, fmt); }
    inline void format_info(std::string fmt) { format(INFO, fmt); }
    inline void format_version(std::string fmt) { format(VERSION, fmt); }
    inline void format_status(std::string fmt) { format(STATUS, fmt); }

    inline void set_activation_mask(LogType type, bool val) {
      if ((logger_ & CONSOLE) == CONSOLE)
        ConsoleLogger::get()->set_activation_mask(type, val);
      if ((logger_ & OUT_FILE) == OUT_FILE)
        OutLogger::get()->set_activation_mask(type, val);
    }
    inline void set_activation(unsigned mask) {
      if ((logger_ & CONSOLE) == CONSOLE)
        ConsoleLogger::get()->set_activation(mask);
      if ((logger_ & OUT_FILE) == OUT_FILE)
        OutLogger::get()->set_activation(mask);
    }

    inline void set_fatal(bool val) { set_activation_mask(FATAL, val); }
    inline void set_error(bool val) { set_activation_mask(ERROR, val); }
    inline void set_warning(bool val) { set_activation_mask(WARNING, val); }
    inline void set_success(bool val) { set_activation_mask(SUCCESS, val); }
    inline void set_debug(bool val) { set_activation_mask(DEBUG, val); }
    inline void set_trace(bool val) { set_activation_mask(TRACE, val); }
    inline void set_info(bool val) { set_activation_mask(INFO, val); }
    inline void set_version(bool val) { set_activation_mask(VERSION, val); }
    inline void set_status(bool val) { set_activation_mask(STATUS, val); }

    inline void set_line_zero_pad(bool val) {
      if ((logger_ & CONSOLE) == CONSOLE)
        ConsoleLogger::get()->set_line_zero_pad(val);
      if ((logger_ & OUT_FILE) == OUT_FILE)
        OutLogger::get()->set_line_zero_pad(val);
    }
    inline void set_pad(unsigned id, unsigned val) {
      if ((logger_ & CONSOLE) == CONSOLE)
        ConsoleLogger::get()->set_pad(id, val);
      if ((logger_ & OUT_FILE) == OUT_FILE) OutLogger::get()->set_pad(id, val);
    }
    inline void set_file_pad(unsigned val) { set_pad(0, val); }
    inline void set_func_pad(unsigned val) { set_pad(1, val); }
    inline void set_line_pad(unsigned val) { set_pad(2, val); }
    inline void set_body_pad(unsigned val) { set_pad(3, val); }

   protected:
    unsigned logger_ = CONSOLE | OUT_FILE;
  };

  inline void error_log(const std::string file, const std::string msg, ...) {
    va_list args;
    va_start(args, msg);
    Logger::get()->_log(arta::log::FATAL, msg, file, 0, 0, args);
    va_end(args);
  }

}  // namespace log
}  // namespace arta

#pragma clang diagnostic pop

#endif  // ARTA_LOGGER_HPP_
