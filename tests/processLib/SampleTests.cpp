#include <processLib/core/processLib.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_contains.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

TEST_CASE("Interface", "[ConfigTests]")
{
    processLib::Process process(std::string("ping"), processLib::Arguments{"localhost"});
    REQUIRE(process.isRunning() == false);
    REQUIRE(process.getExitCode() == std::nullopt);
    REQUIRE(process.getName() == "ping");
    REQUIRE(process.getArguments() == processLib::Arguments{"localhost"});
}

TEST_CASE("RunningState", "[ConfigTests]")
{
    processLib::Process process(std::string("ping"), processLib::Arguments{"localhost"});
    REQUIRE(process.isRunning() == false);
    process.start();
    REQUIRE(process.isRunning() == true);
    auto exit_code = process.stop();
    REQUIRE(process.isRunning() == false);
    REQUIRE(exit_code == 0);
}

TEST_CASE("FailureStates", "[ConfigTests]")
{
    processLib::Process process(std::string("pingpong"), processLib::Arguments{"localhost"});
    REQUIRE(process.isRunning() == false);

    REQUIRE_THROWS_MATCHES(
        process.stop(), std::runtime_error, Catch::Matchers::Message("Stop process failed: The handle is invalid. "));
}
