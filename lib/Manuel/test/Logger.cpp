#include <catch2/catch_test_macros.hpp>
#include <Manuel/Log.hpp>

namespace {
    ManuelLogLevel lastLogLevel;

    void log(void *, char const *, ManuelLogLevel level, char const *) { lastLogLevel = level; }
    void exception(void *) {}
} // namespace

TEST_CASE("ManuelLogLevel - to_string checks", "[Manuel][Log]") {
    CHECK(std::string{ManuelLogLevel_To_String(MANUEL_LOG_LEVEL_FATAL)} == "Fatal");    
    CHECK(std::string{ManuelLogLevel_To_String(MANUEL_LOG_LEVEL_ERROR)} == "Error");    
    CHECK(std::string{ManuelLogLevel_To_String(MANUEL_LOG_LEVEL_WARNING)} == "Warning");    
    CHECK(std::string{ManuelLogLevel_To_String(MANUEL_LOG_LEVEL_INFO)} == "Info");    
    CHECK(std::string{ManuelLogLevel_To_String(MANUEL_LOG_LEVEL_VERBOSE)} == "Verbose");    
}

TEST_CASE("ManuelLogger - Cannot register the same sink multiple times") {
    CHECK(ManuelLogRegisterSink(nullptr, log, exception));
    CHECK_FALSE(ManuelLogRegisterSink(nullptr, log, exception));

    REQUIRE(ManuelLogDeregisterSink(nullptr, log, exception));
}

TEST_CASE("ManuelLogger - cannot deregister sink that was not registered") {
    CHECK_FALSE(ManuelLogDeregisterSink(nullptr, log, exception));
}

TEST_CASE("ManuelLogger - Caan deregister is different orders than registered") {
    CHECK(ManuelLogRegisterSink(nullptr, log, exception));
    CHECK(ManuelLogRegisterSink(&lastLogLevel, log, exception));

    SECTION("Same Order") {
        REQUIRE(ManuelLogDeregisterSink(nullptr, log, exception));
        REQUIRE(ManuelLogDeregisterSink(&lastLogLevel, log, exception));
    }

    SECTION("Reverse Order") {
        REQUIRE(ManuelLogDeregisterSink(&lastLogLevel, log, exception));
        REQUIRE(ManuelLogDeregisterSink(nullptr, log, exception));
    }
}
