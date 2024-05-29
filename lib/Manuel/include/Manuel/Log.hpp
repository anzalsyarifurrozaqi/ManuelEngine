#ifndef MANUEL_LOG_H
#define MANUEL_LOG_H

#include <Manuel/Export.hpp>
#include <fmt/core.h>

/// Logs a compile-time message to the global logger with the given parameters
#define MANUEL_LOG(CATEGORY, LOG_LEVEL, MESSAGE, ...)                                                   \
    if (static_cast<int>(LOG_LEVEL) <= static_cast<int>(CATEGORY##LogCategoryGetLogLevel())) {          \
        ManuelLogMessage(CATEGORY##LogCategoryGetName(), LOG_LEVEL, MESSAGE, ##__VA_ARGS__);            \
    }                                                                                                   \

/** Declares a log category message to the global logger with the given parameters
 * @param CATEGORY Name of the category, which is both how it appears in logs and used in the
 * function names
*/
#define MANUEL_DECLARE_LOG_CATEGORY(CATEGORY)                                                           \
                                                                                                        \
    char const *CATEGORY##LogCategoryGetName();                                                         \
                                                                                                        \
    ManuelLogLevel CATEGORY##LogCategoryGetLogLevel();                                                  \
                                                                                                        \
    void CATEGORY##LogCategorySetLogLevel(ManuelLogLevel logLevel);                                     \

/** Declares a log category for shared environments
 * @param EXPORT Export macro to use for categories that are in shared binaries and shared
 * externally
 * @param CATEGORY Name of the category, which is both how it appears in logs and used in the function names
*/
#define MANUEL_DECLARE_SHARED_LOG_CATEGORY(EXPORT)                                                      \
    char const *CATEGORY##LogCategoryGetName();                                                         \
                                                                                                        \
    ManuelLogLevel CATEGORY##LogCategoryGetLogLevel();                                                  \
                                                                                                        \
    void CATEGORY##LogCategorySetLogLevel(ManuelLogLevel logLevel);                                     \

/** Definition of a give log category, must appear in only a single compile unit
 * @param CATEGORY Name of the category, which is both how it appears in logs and used in the
 * function names
 * @param RUNTIME_DEFAULT_LEVEL Maximum log type accepted by category at runtime by default
*/
#define MANUEL_DEFINE_LOG_CATEGORY(CATEGORY, RUNTIME_DEFAULT_LEVEL)                                     \
                                                                                                        \
    static ManuelLogLevel g_##CATEGORY##_log_level = RUNTIME_DEFAULT_LEVEL;                             \
                                                                                                        \
    char const *CATEGORY##LogCategoryGetName() { return #CATEGORY; }                                    \
                                                                                                        \
    ManuelLogLevel CATEGORY##LogCategoryGetLogLevel() { return g_##CATEGORY##_log_level; }              \
                                                                                                        \
    void CATEGORY##LogCategorySetLogLevel(ManuelLogLevel logLevel) {                                     \
        g_##CATEGORY##_log_level = logLevel;                                                            \
    }                                                                                                   \

enum ManuelLogLevel {
    MANUEL_LOG_LEVEL_FATAL = 0,
    MANUEL_LOG_LEVEL_ERROR,
    MANUEL_LOG_LEVEL_WARNING,
    MANUEL_LOG_LEVEL_INFO,
    MANUEL_LOG_LEVEL_VERBOSE,
    MANUEL_LOG_LEVEL_ALL = MANUEL_LOG_LEVEL_VERBOSE,
};

typedef void (*PFN_ManuelLogMessage)(
    void *pContext,
    char const *pCategoryName,
    ManuelLogLevel level,
    char const *pMessage
);

typedef void (*PFN_ManuelLogException)(void *pContext);

MANUEL_EXPORT
char const *ManuelLogLevel_To_String(ManuelLogLevel logLevel);

MANUEL_EXPORT
void ManuelLogMessage(char const *pCategoryName, ManuelLogLevel level, char const *pMessage);

MANUEL_EXPORT
bool ManuelLogRegisterSink(
    void *pContext,
    PFN_ManuelLogMessage logMessage,
    PFN_ManuelLogException logException
);

MANUEL_EXPORT
bool ManuelLogDeregisterSink(
    void *pContext,
    PFN_ManuelLogMessage logMessage,
    PFN_ManuelLogException logException
);

template <typename... Args>
inline void ManuelLogMessage(
    char const *pCategoryName,
    ManuelLogLevel level,
    fmt::format_string<Args...> message,
    Args &&...args
) {
    ManuelLogMessage(pCategoryName, level, fmt::format(message, std::forward<Args>(args)...).c_str());
}
#endif // MANUEL_LOG_H