#include <processLib/core/processLib.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_contains.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#ifdef _WIN32
auto ping_forever_arguments = processLib::Arguments{"localhost", "-t"};
#else
auto ping_forever_arguments = processLib::Arguments{"localhost"};
#endif

TEST_CASE("Interface", "[ConfigTests]")
{
    processLib::Process process(std::string("ping"), processLib::Arguments{"localhost"});
    REQUIRE(process.isRunning() == false);
    REQUIRE(process.getExitCode() == std::nullopt);
    REQUIRE(process.getName() == "ping");
    REQUIRE(process.getArguments() == processLib::Arguments{"localhost"});
}

TEST_CASE("RunningState", "[RunTests]")
{
    processLib::Process process(std::string("ping"), processLib::Arguments{"localhost"});
    REQUIRE(process.isRunning() == false);
    process.start();
    REQUIRE(process.isRunning() == true);
    auto exit_code = process.stop();
    REQUIRE(process.isRunning() == false);
    REQUIRE(exit_code.has_value());
    REQUIRE(exit_code == 0);
}

TEST_CASE("ExceptionOnStop", "[RunTests]")
{
    processLib::Process process(std::string("pingpong"),
                                "",
                                processLib::Arguments{"localhost"},
                                processLib::Policies{}.withThrowIfAlreadyStopped(true));
    REQUIRE(process.isRunning() == false);

    REQUIRE_THROWS_MATCHES(
        process.stop(),
        std::runtime_error,
        Catch::Matchers::MessageMatches(Catch::Matchers::StartsWith("Stop process failed: The handle is invalid. ")));
}

TEST_CASE("NoExceptionOnStopConfig", "[RunTests]")
{
    processLib::Process process(std::string("pingpong"), processLib::Arguments{"localhost"});
    REQUIRE(process.isRunning() == false);

    REQUIRE(!process.stop().has_value());
}

TEST_CASE("FailureStates", "[RunTests]")
{
    processLib::Process process(std::string("pingpong"),
                                "",
                                processLib::Arguments{"localhost"},
                                processLib::Policies{}.withThrowIfAlreadyStopped(true));
    REQUIRE(process.isRunning() == false);

    REQUIRE_THROWS_MATCHES(
        process.stop(),
        std::runtime_error,
        Catch::Matchers::MessageMatches(Catch::Matchers::StartsWith("Stop process failed: The handle is invalid. ")));
}

TEST_CASE("StartNonExisting", "[RunTests]")
{
    using namespace std::chrono_literals;
    processLib::Process process(std::string("xf1221lap"), processLib::Arguments{"localhost"});
    REQUIRE_THROWS_MATCHES(process.start(),
                           std::runtime_error,
                           Catch::Matchers::MessageMatches(Catch::Matchers::StartsWith("Create process failed:")));
}

TEST_CASE("ExitWithinWaitPeriod", "[RunTests]")
{
    using namespace std::chrono_literals;
    processLib::Process process(std::string("ping"), processLib::Arguments{"localhost"});
    process.start();
    auto result = process.wait(10s).get();
    process.stop();
    REQUIRE(result.has_value());
    REQUIRE(result.value() == 0);
}

TEST_CASE("ExitAfterWaitPeriod", "[RunTests]")
{
    using namespace std::chrono_literals;
    processLib::Process process(std::string("ping"), ping_forever_arguments);
    process.start();
    auto result = process.wait(5s).get();

    REQUIRE(!result.has_value());
    process.stop();
    REQUIRE(process.getExitCode().has_value());
}
