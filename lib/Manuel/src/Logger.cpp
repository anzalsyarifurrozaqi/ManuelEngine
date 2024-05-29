#include <Manuel/Log.hpp>

#include <mutex>
#include <string>
#include <vector>

namespace {
    class ManuelLogger {
    private:
        std::mutex mSync;

        struct SinkSet {
            void *pContext;
            PFN_ManuelLogMessage logMessage;
            PFN_ManuelLogException logException;
        };
        std::vector<SinkSet> mSinks;
        
    public:
        ManuelLogger() = default;

        void Log(char const *pCategoryName, ManuelLogLevel level, char const *pMessage);

        bool RegisterSink(
            void *pContext,
            PFN_ManuelLogMessage logMessage,
            PFN_ManuelLogException logException
        );
        
        bool DeregisterSink(
            void *pContext,
            PFN_ManuelLogMessage logMessage,
            PFN_ManuelLogException logException
        );
    };    

    void ManuelLogger::Log(char const *pCategoryName, ManuelLogLevel level, char const *pMessage) {
        std::scoped_lock lock{mSync};

        for (auto const &it : mSinks) {
            if (it.logMessage)
                it.logMessage(it.pContext, pCategoryName, level, pMessage);
        }

        [[unlikely]] if (level == MANUEL_LOG_LEVEL_FATAL) {
            for (auto const &it : mSinks) {
                if (it.logException) {
                    it.logException(it.pContext);
                }
            }
        }
    }

    bool ManuelLogger::RegisterSink(
        void *pContext,
        PFN_ManuelLogMessage logMessage,
        PFN_ManuelLogException logException
    ) {
        std::scoped_lock lock{mSync};

        for (auto it = mSinks.begin(); it != mSinks.end(); ++it) {
            if ((pContext != nullptr && it->pContext == pContext) ||
                (pContext == nullptr && it->logMessage == logMessage &&
                 it->logException == logException)) {
                
                return false;
            }
        }

        mSinks.emplace_back(SinkSet{
            .pContext = pContext,
            .logMessage = logMessage,
            .logException = logException
        });
        return true;
    }

    bool ManuelLogger::DeregisterSink(
        void *pContext,
        PFN_ManuelLogMessage logMessage,
        PFN_ManuelLogException logException
    ) {
        std::scoped_lock lock{mSync};

        for (auto it = mSinks.begin(); it != mSinks.end(); ++it) {
            if ((pContext != nullptr && it->pContext == pContext) ||
                (pContext == nullptr && it->logMessage == logMessage) ||
                 it->logException == logException) {
                mSinks.erase(it);
                return true;
            }
        }

        return false;
    }

    ManuelLogger logger;
} // namespace


char const *ManuelLogLevel_To_String(ManuelLogLevel logLevel) {
    switch (logLevel) {
        case MANUEL_LOG_LEVEL_FATAL:        return "Fatal";
        case MANUEL_LOG_LEVEL_ERROR:        return "Error";
        case MANUEL_LOG_LEVEL_WARNING:      return "Warning";
        case MANUEL_LOG_LEVEL_INFO:         return "Info";
        case MANUEL_LOG_LEVEL_VERBOSE:      return "Verbose";
        default:                            return "Unknown";
    }
}

void ManuelLogMessage(char const *pCategoryName, ManuelLogLevel level, char const *pMessage) {
    logger.Log(pCategoryName, level, pMessage);
}

bool ManuelLogRegisterSink(
    void *pContext,
    PFN_ManuelLogMessage logMessage,
    PFN_ManuelLogException logException
) {
    return logger.RegisterSink(pContext, logMessage, logException);
}
bool ManuelLogDeregisterSink(
    void *pContext,
    PFN_ManuelLogMessage logMessage,
    PFN_ManuelLogException logException
) {
    return logger.DeregisterSink(pContext, logMessage, logException);
}                            
