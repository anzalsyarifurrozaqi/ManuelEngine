#include <Manuel/ManagedMemory.hpp>

#include "Result.hpp"

typedef struct  ManagedMemorySubset {
    ManuelManagedMemory parentMemory;
} ManagedMemorySubset;

static void cleanup_ManagedMemorySubset(void *pData, uint32_t dataSize, void *pMetadata) {
    ManagedMemorySubset *pManagedMemorySubsetData = (ManagedMemorySubset *) pMetadata;

    ManuelManagedMemoryDecrementUse(pManagedMemorySubsetData->parentMemory);
}

ManuelResultSet ManuelCreateManagedMemorySubSet(
    ManuelManagedMemory parentMemory,
    size_t dataOffset,
    size_t dataSize,
    ManuelManagedMemory *pManagedMemory
) {
    uint8_t *pParentData;
    uint32_t parentDataSize;

    ManuelManagedMemoryGetData(parentMemory, (void **)&pParentData, &parentDataSize);

    if (dataOffset + dataSize > parentDataSize) {
        return to_ManuelResult(MANUEL_ERROR_MEMORY_SUBSET_OVERRUNS_PARENT);
    }

    pParentData += dataOffset;

    ManagedMemorySubset metaData = {
        .parentMemory = parentMemory,
    };

    ManuelResultSet result = ManuelCreateManagedMemory(pParentData, dataSize, cleanup_ManagedMemorySubset, &metaData, sizeof(ManagedMemorySubset), pManagedMemory);
    if (result.value == MANUEL_SUCCESS) {
        ManuelManagedMemoryIncrementUse(parentMemory);
    }

    return result;
}