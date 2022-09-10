#include <stdarg.h>
#include "logger.h"
#include "usart.h"

// program log level (disable by default)
static enum log_level _level = LOG_DISABLE;

/**
 * @brief Get the string representation of log level enum
 * 
 * @param level log level enum
 * @return char* 
 */
static char *_get_level_str(enum log_level level) {
  switch (level) {
    case LOG_TRACE: return "TRACE";
    case LOG_INFO: return "INFO";
    case LOG_WARNING: return "WARN";
    case LOG_ERROR: return "ERROR";
    default: return "CRIT";
  }
}

static void _out(const struct __std_log *stdl) {
  char buffer[1000];
  sprintf(buffer, "[%s]\t %s:%i ", stdl->repr, stdl->func, stdl->line);
  usart_print(buffer);
  vsprintf(buffer, stdl->fmt, *stdl->argp);
  usart_print(buffer);
  usart_print("\n\r");
}

/**
 * @brief Set the program log level
 * 
 * @param level target logging level
 */
void logger_set_level(enum log_level level) {
  _level = level;
}


/**
 * @brief Get the program log level
 * 
 * @return enum Level 
 */
enum log_level logger_get_level(void) {
  return _level;
}

/**
 * @brief Perform log filtration and standard log construction parsing variable arguments
 * 
 * @param level log level enum
 * @param func macro expanded log containing function name
 * @param line macro expanded log line
 * @param fmt log string formatter
 * @param ... variable arguments for string formatter
 */
void logger_out(enum log_level level, const char* func, int line, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  // filter output by log level
  if (logger_get_level() > level) {
    return;
  }
  struct __std_log stdl;
  stdl.repr = _get_level_str(level);
  stdl.argp = &args;
  stdl.fmt = fmt;
  stdl.func = func;
  stdl.line = line;
  _out(&stdl);
  va_end(args);
}
