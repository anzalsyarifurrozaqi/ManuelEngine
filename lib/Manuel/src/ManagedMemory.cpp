#include <Manuel/ManagedMemory.hpp>

#include "Result.hpp"

#include <atomic>
#include <new>

#include <stdlib.h>
#include <string>

#include <cstring>

namespace {

    struct ManagedMemory {
        void *pData;
        size_t dataSize;

        std::atomic_uint32_t useCount;
        PFN_ManuelManagedMemoryCleanup cleanupFn;

        ManagedMemory(void *pData, size_t dataSize, PFN_ManuelManagedMemoryCleanup cleanupFn) : pData{pData}, dataSize{dataSize}, useCount{1}, cleanupFn{cleanupFn} {}
    };

    MANUEL_DEFINE_HANDLE_CASTS(managed_memory, ManagedMemory, ManuelManagedMemory)

    void *ManuelResourceCreateInfoGetMetaData(ManagedMemory const *pManagedMemory) {
        return (char *)pManagedMemory + sizeof(ManagedMemory);
    }

} // namespace

ManuelResultSet ManuelCreateManagedMemory(
    void *pData,
    size_t dataSize,
    PFN_ManuelManagedMemoryCleanup cleanupFn,
    void *pMetaData,
    size_t metadataSize,
    ManuelManagedMemory *pManagedMemory
) {
    ManagedMemory *pNewManagedMemory = (ManagedMemory *) malloc(sizeof(ManagedMemory) + metadataSize);
    if (pNewManagedMemory == nullptr)
        return to_ManuelResult(MANUEL_ERROR_OUT_OF_MEMORY);
    
    new (pNewManagedMemory) ManagedMemory(pData, dataSize, cleanupFn);

    memcpy(ManuelResourceCreateInfoGetMetaData(pNewManagedMemory), pMetaData, metadataSize);

    *pManagedMemory = managed_memory_to_handle(pNewManagedMemory);

    return to_ManuelResult((ManuelResult)MANUEL_SUCCESS);
}

void ManuelManagedMemoryGetData(ManuelManagedMemory managerMemory, void **ppData, uint32_t *pDataSize) {
    ManagedMemory *pManagedMemory = managed_memory_from_handle(managerMemory);

    *ppData = pManagedMemory->pData;
    if (pDataSize)
        *pDataSize = pManagedMemory->dataSize;
}

uint32_t ManuelManagedMemoryGetUse(ManuelManagedMemory managedMemory) {
    ManagedMemory *pManagedMemory = managed_memory_from_handle(managedMemory);

    return pManagedMemory->useCount;
}

uint32_t ManuelManagedMemoryIncrementUse(ManuelManagedMemory managedMemory) {
    ManagedMemory *pManagedMemory = managed_memory_from_handle(managedMemory);

    return ++pManagedMemory->useCount;
}


uint32_t ManuelManagedMemoryDecrementUse(ManuelManagedMemory managedMemory) {
    ManagedMemory *pManagedMemory = managed_memory_from_handle(managedMemory);

    uint32_t count = --pManagedMemory->useCount;
    if (count == 0) {
        if (pManagedMemory->cleanupFn) {
            pManagedMemory->cleanupFn(pManagedMemory->pData, pManagedMemory->dataSize, ManuelResourceCreateInfoGetMetaData(pManagedMemory));
        }

        pManagedMemory->~ManagedMemory();
        free(pManagedMemory);
    }

    return count;
}