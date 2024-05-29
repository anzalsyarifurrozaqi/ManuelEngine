#ifndef MANUEL_RESULT_H
#define MANUEL_RESULT_H

#include <Manuel/Export.hpp>

/// The minimum possible result code (not quite the full rangle to allow conversion to the absolute value)
#define MANUEL_RESULT_MIN_ENUM -0x7FFFFFFF
/// The maximum possible result code
#define MANUEL_RESULT_MAX_ENUM 0x7FFFFFFF
/// The maximum length that a result code name that can be returned from a <result>ToString(<result>) function
#define MANUEL_MAX_RESULT_STRING_SIZE 128

/// This is the form of the function that can convert a given Result value and turn it into a human-readable string
typedef void (*PFN_ManuelResultToString)(int value, char buffer[MANUEL_MAX_RESULT_STRING_SIZE]);

typedef struct ManuelResultSet {
    int value;
    PFN_ManuelResultToString toString;
} ManuelResultSet;

typedef enum ManuelResult {
    MANUEL_SUCCESS                                      = 0,
    MANUEL_ERROR_OUT_OF_MEMORY                          = -1000000001,
    MANUEL_ERROR_FAILED_TO_OPEN_FILE                    = -1000000002,
    MANUEL_ERROR_FAILED_TO_STAT_FILE                    = -1000000003,
    MANUEL_ERROR_ATTEMPTED_TO_MAP_ZERO_SIZED_FILE       = -1000000004,
    MANUEL_ERROR_FAILED_TO_MAP_FILE                     = -1000000005,
    MANUEL_ERROR_FAILED_TO_UNMAP_FILE                   = -1000000006,
    MANUEL_ERROR_FAILED_TO_CLOSE_FILE                   = -1000000007,
    MANUEL_ERROR_MEMORY_SUBSET_OVERRUNS_PARENT          = -1000000008,
    MANUEL_ERROR_ZERO_SYNC_THREADS                      = -1000000009,
    MANUEL_ERROR_ZERO_ASYNC_THREADS                     = -1000000010,
    MANUEL_ERROR_DESTINATION_BUFFER_TOO_SMALL           = -1000000011,
    MANUEL_ERROR_INVALID_HEX_DATA_SIZE                  = -1000000012,
    MANUEL_ERROR_MALFORMED_HEX_DATA                     = -1000000013,
} ManuelResult;

MANUEL_EXPORT
void ManuelResultToString(ManuelResult value, char buffer[MANUEL_MAX_RESULT_STRING_SIZE]);

#endif // MANUEL_RESULT_H