#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "cppnats.hpp"

TEST_CASE("connecting to server") {
    CppNats::Client c;
    CHECK_NOTHROW(c.connect("nats://localhost:4222"));
    c.close();
}

TEST_CASE("connecting to invalid server") {
    CppNats::Client c;
    CHECK_THROWS_AS(c.connect("nats://invalid:4222"), CppNats::Exception);
}
