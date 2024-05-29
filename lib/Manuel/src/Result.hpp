#ifndef RESULT_HPP
#define RESULT_HPP

#include <Manuel/Result.hpp>

inline ManuelResultSet to_ManuelResult(ManuelResult value) {
    ManuelResultSet result = {
        .value = value,
        .toString = (PFN_ManuelResultToString) ManuelResultToString,
    };

    return result;
}

#endif // RESULT_HPP