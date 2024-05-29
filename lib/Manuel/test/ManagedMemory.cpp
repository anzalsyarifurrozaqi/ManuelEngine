#include <catch2/catch_test_macros.hpp>
#include <Manuel/ManagedMemory.hpp>
#include <Manuel/Result.hpp>

constexpr uint16_t cDataCount = 2048;

TEST_CASE("ManagedMemory - No metadata/cleanup") {
    ManuelManagedMemory managedMemory = MANUEL_NULL_HANDLE;

    ManuelResultSet result;

    uint16_t buffer[cDataCount];
    for (uint16_t i = 0; i < cDataCount; ++i) {
        buffer[i] = i;
    }

    result = ManuelCreateManagedMemory(buffer, sizeof(uint16_t) *cDataCount, nullptr, nullptr, 0, &managedMemory);

    CHECK(result.value == MANUEL_SUCCESS);
    REQUIRE(managedMemory != MANUEL_NULL_HANDLE);
    CHECK(ManuelManagedMemoryGetUse(managedMemory) == 1);

    uint16_t *ptr = nullptr;
    uint32_t size = 0;

    ManuelManagedMemoryGetData(managedMemory, (void **)&ptr, &size);
    REQUIRE(ptr == buffer);
    REQUIRE(size == cDataCount * sizeof(uint16_t));

    for (uint16_t i = 0; i < cDataCount; ++i) {
        if (ptr[i] != i)
            CHECK(ptr[i] == i);            
    }

    CHECK(ManuelManagedMemoryIncrementUse(managedMemory) == 2);
    CHECK(ManuelManagedMemoryGetUse(managedMemory) == 2);
    CHECK(ManuelManagedMemoryDecrementUse(managedMemory) == 1);
    CHECK(ManuelManagedMemoryGetUse(managedMemory) == 1);

    REQUIRE(ManuelManagedMemoryDecrementUse(managedMemory) == 0);
}

TEST_CASE("ManagedMemory - metada and cleanup") {
    ManuelManagedMemory managedMemory = MANUEL_NULL_HANDLE;
    ManuelResultSet result;

    uint16_t *buffer = new uint16_t[cDataCount];
    for (int i = 0; i < cDataCount; ++i) {
        buffer[i] = i;
    }

    auto cleanupFn = [](void *pData, uint32_t, void *pMetadata) {
        uint16_t **ppData = (uint16_t **)pMetadata;
        CHECK(*ppData == pData);

        delete[] (uint16_t *)pData;
    };

    result = ManuelCreateManagedMemory(buffer, sizeof(uint16_t) *cDataCount, cleanupFn, &buffer, sizeof(uint16_t *), &managedMemory);
    CHECK(result.value == MANUEL_SUCCESS);
    REQUIRE(managedMemory != MANUEL_NULL_HANDLE);
    CHECK(ManuelManagedMemoryGetUse(managedMemory) == 1);

    uint16_t *ptr = nullptr;
    uint32_t size = 0;

    ManuelManagedMemoryGetData(managedMemory, (void **)&ptr, &size);
    REQUIRE(ptr == buffer);
    REQUIRE(size == cDataCount * sizeof(uint16_t));

    for (uint16_t i = 00; i < cDataCount; ++i) {
        if (ptr[i] != i)
            CHECK(ptr[i] == i);
    }

    CHECK(ManuelManagedMemoryIncrementUse(managedMemory) == 2);
    CHECK(ManuelManagedMemoryGetUse(managedMemory) == 2);
    CHECK(ManuelManagedMemoryDecrementUse(managedMemory) == 1);
    CHECK(ManuelManagedMemoryGetUse(managedMemory) == 1);

    REQUIRE(ManuelManagedMemoryDecrementUse(managedMemory) == 0);
}

TEST_CASE("ManagedMemory - subset success case") {
    ManuelManagedMemory managedMemory = MANUEL_NULL_HANDLE;
    ManuelManagedMemory managedSubset = MANUEL_NULL_HANDLE;
    ManuelResultSet result;

    uint16_t *buffer = new uint16_t[cDataCount];
    for (int i = 0; i < cDataCount; ++i) {
        buffer[i] = i;
    }

    auto cleanupFn = [](void *pData, uint32_t, void *pMetadata) {
        uint16_t **ppData = (uint16_t **)pMetadata;
        CHECK(*ppData == pData);

        delete[] (uint16_t *)pData;
    };

    result = ManuelCreateManagedMemory(buffer, sizeof(uint16_t) *cDataCount, cleanupFn, &buffer, sizeof(uint16_t *), &managedMemory);
    CHECK(result.value == MANUEL_SUCCESS);
    REQUIRE(managedMemory != MANUEL_NULL_HANDLE);
    CHECK(ManuelManagedMemoryGetUse(managedMemory) == 1);

    result = ManuelCreateManagedMemorySubSet(managedMemory, (cDataCount / 2) * sizeof(uint16_t), (cDataCount / 2 ) * sizeof(uint16_t), &managedSubset);
    CHECK(result.value == MANUEL_SUCCESS);
    REQUIRE(managedSubset != MANUEL_NULL_HANDLE);
    CHECK(ManuelManagedMemoryGetUse(managedSubset) == 1);

    // Original memory has an increased use
    CHECK(ManuelManagedMemoryDecrementUse(managedMemory) > 0);

    uint16_t *ptr = nullptr;
    uint32_t size = 0;

    ManuelManagedMemoryGetData(managedSubset, (void **)&ptr, &size);
    REQUIRE(ptr == buffer + (cDataCount / 2));
    REQUIRE(size == (cDataCount / 2) * sizeof(uint16_t));

    for (uint16_t i = 0; i < cDataCount / 2; ++i) {
        if (ptr[i] != (cDataCount / 2) + i)
            CHECK(ptr[i] == (cDataCount / 2) + i);
    }

    CHECK(ManuelManagedMemoryIncrementUse(managedSubset) == 2);
    CHECK(ManuelManagedMemoryGetUse(managedSubset) == 2);
    CHECK(ManuelManagedMemoryDecrementUse(managedSubset) == 1);
    CHECK(ManuelManagedMemoryGetUse(managedSubset) == 1);

    REQUIRE(ManuelManagedMemoryDecrementUse(managedSubset) == 0);
}

TEST_CASE("ManagedMemory - subset failure case") {
    ManuelManagedMemory managedMemory = MANUEL_NULL_HANDLE;
    ManuelManagedMemory managedSubSet = MANUEL_NULL_HANDLE;
    ManuelResultSet result;

    uint16_t *buffer = new uint16_t[cDataCount];
    for (int i = 0; i < cDataCount; ++i) {
        buffer[i] = i;        
    }

    auto cleanupFn = [](void *pData, uint32_t, void *pMetadata) {
        uint16_t **ppData = (uint16_t **)pMetadata;
        CHECK(*ppData == pData);

        delete[] (uint16_t *)pData;
    };

    result = ManuelCreateManagedMemory(buffer, sizeof(uint16_t) *cDataCount, cleanupFn, &buffer, sizeof(uint16_t *), &managedMemory);
    CHECK(result.value == MANUEL_SUCCESS);
    REQUIRE(managedMemory != MANUEL_NULL_HANDLE);
    CHECK(ManuelManagedMemoryGetUse(managedMemory) == 1);

    result = ManuelCreateManagedMemorySubSet(managedMemory, (cDataCount / 2) * sizeof(uint16_t), (cDataCount / 2) * sizeof(uint16_t) + 1, &managedSubSet);
    CHECK(result.value == MANUEL_ERROR_MEMORY_SUBSET_OVERRUNS_PARENT);
    REQUIRE(managedSubSet == MANUEL_NULL_HANDLE);

    REQUIRE(ManuelManagedMemoryDecrementUse(managedMemory) == 0);
}