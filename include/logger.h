
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>

// logging levels
enum log_level {
  LOG_TRACE,
  LOG_INFO,
  LOG_WARNING,
  LOG_ERROR,
  LOG_CRITICAL,
  LOG_DISABLE
};

enum log_level logger_get_level(void);
void logger_set_level(enum log_level level);
void logger_out(enum log_level level, const char *func, int line,
                const char *fmt, ...);

#ifndef critical
#define critical(...) __CRITICAL(__VA_ARGS__, "")
#define __CRITICAL(fmt, ...)                                                   \
  logger_out(LOG_CRITICAL, __func__, __LINE__, fmt, __VA_ARGS__)
#endif

#ifndef error
#define error(...) __ERROR(__VA_ARGS__, "")
#define __ERROR(fmt, ...)                                                      \
  logger_out(LOG_ERROR, __func__, __LINE__, fmt, __VA_ARGS__)
#endif

#ifndef warning
#define warning(...) __WARNING(__VA_ARGS__, "")
#define __WARNING(fmt, ...)                                                    \
  logger_out(LOG_WARNING, __func__, __LINE__, fmt, __VA_ARGS__)
#endif

#ifndef info
#define info(...) __INFO(__VA_ARGS__, "")
#define __INFO(fmt, ...)                                                       \
  logger_out(LOG_INFO, __func__, __LINE__, fmt, __VA_ARGS__)
#endif

#ifndef trace
#define trace(...) __TRACE(__VA_ARGS__, "")
#define __TRACE(fmt, ...)                                                      \
  logger_out(LOG_TRACE, __func__, __LINE__, fmt, __VA_ARGS__)
#endif

#endif // __LOGGER_H__