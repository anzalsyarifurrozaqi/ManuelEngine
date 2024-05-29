#ifndef MANUEL_MANAGED_MEMORY_H
#define MANUEL_MANAGER_MEMORY_H

#include <Manuel/Export.hpp>
#include <Manuel/Handle.hpp>
#include <Manuel/Result.hpp>

#include <stddef.h>
#include <stdint.h>

MANUEL_DEFINE_HANDLE(ManuelManagedMemory)

typedef void (*PFN_ManuelManagedMemoryCleanup)(void *pData, uint32_t dataSize, void *pMetadata);

MANUEL_EXPORT
ManuelResultSet ManuelCreateManagedMemory(
    void *pData,
    size_t dataSize,
    PFN_ManuelManagedMemoryCleanup cleanupFn,
    void *pMetaData,
    size_t metadataSize,
    ManuelManagedMemory *pManagedMemory
);

MANUEL_EXPORT
ManuelResultSet ManuelCreateManagedMemorySubSet(
    ManuelManagedMemory parentMemory,
    size_t dataOffset,
    size_t datasize,
    ManuelManagedMemory *pManagedMemory
);

MANUEL_EXPORT
void ManuelManagedMemoryGetData(ManuelManagedMemory managerMemory, void **ppData, uint32_t *pDataSize);

MANUEL_EXPORT
uint32_t ManuelManagedMemoryGetUse(ManuelManagedMemory managedMemory);

MANUEL_EXPORT
uint32_t ManuelManagedMemoryIncrementUse(ManuelManagedMemory managedMemory);

MANUEL_EXPORT
uint32_t ManuelManagedMemoryDecrementUse(ManuelManagedMemory managedMemory);


#endif // MANUEL_MANAGED_MEMORY_H