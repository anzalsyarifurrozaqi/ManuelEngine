#include <Manuel/Result.hpp>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESULT_CASE(X)                                  \
    case X:                                             \
        pResultStr = #X;                                \
        break;                                          

void ManuelResultToString(ManuelResult value, char buffer[MANUEL_MAX_RESULT_STRING_SIZE]) {
    char const *pResultStr = NULL;

    switch (value) {
        RESULT_CASE(MANUEL_SUCCESS)
        RESULT_CASE(MANUEL_ERROR_OUT_OF_MEMORY)
        RESULT_CASE(MANUEL_ERROR_FAILED_TO_OPEN_FILE)
        RESULT_CASE(MANUEL_ERROR_FAILED_TO_STAT_FILE)
        RESULT_CASE(MANUEL_ERROR_ATTEMPTED_TO_MAP_ZERO_SIZED_FILE)
        RESULT_CASE(MANUEL_ERROR_FAILED_TO_MAP_FILE)
        RESULT_CASE(MANUEL_ERROR_FAILED_TO_UNMAP_FILE)
        RESULT_CASE(MANUEL_ERROR_FAILED_TO_CLOSE_FILE);
        RESULT_CASE(MANUEL_ERROR_MEMORY_SUBSET_OVERRUNS_PARENT)
        RESULT_CASE(MANUEL_ERROR_ZERO_SYNC_THREADS)
        RESULT_CASE(MANUEL_ERROR_ZERO_ASYNC_THREADS)
        RESULT_CASE(MANUEL_ERROR_DESTINATION_BUFFER_TOO_SMALL)
        RESULT_CASE(MANUEL_ERROR_INVALID_HEX_DATA_SIZE)
        RESULT_CASE(MANUEL_ERROR_MALFORMED_HEX_DATA)

        default:
            if (value > 0) {
                sprintf(buffer, "MANUEL_UNKNOWN_SUCCESS_%i", value);
            } else {
                sprintf(buffer, "MANUEL_UNKNOWN_ERROR_%i", value);
            }
    }

    if (pResultStr != NULL) {
        assert(strlen(pResultStr) <= MANUEL_MAX_RESULT_STRING_SIZE);

        strcpy(buffer, pResultStr);
    }
}