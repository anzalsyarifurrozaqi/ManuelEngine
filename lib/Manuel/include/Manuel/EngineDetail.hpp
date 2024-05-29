#ifndef MANUEL_ENGINE_DETAIL_H
#define MANUEL_ENGINE_DETAIL_H

/// Basic string macro represeting the canonical name of engine
#define MANUEL_ENGINE_NAME "Manuel-Engine"

#define MANUEL_MAKE_VERSION(major, minor, patch)                                                    \
    ((((uint32_t)(major)) << 22) | (((uint32_t)(minor)) << 12) | ((uint32_t)(patch)))               \

/// 32-bit balue represting the 'version' of the engine
#define MANUEL_ENGINE_VERSION MANUEL_MAKE_VERSION(0, 1, 0)

#endif