#include <doctest/doctest.h>

#include "test_helpers.h"

TEST_SUITE("options") {

TEST_CASE("creating options") {
    CHECK_NOTHROW(CppNats::Options opts);
}

TEST_CASE("setting options") {
    CppNats::Options opts;
    CHECK_NOTHROW(opts.addServer("nats://localhost:4222"));
    CHECK_NOTHROW(opts.timeout(5000));
    CHECK_NOTHROW(opts.setPingInterval(60000));
    CHECK_NOTHROW(opts.setWriteDeadline(5000));
    CHECK_NOTHROW(opts.setIPResolutionOrder(CppNats::Options::IPResolutionOrder::V4First));
    CHECK_NOTHROW(opts.setReconnectWait(2000));
    CHECK_NOTHROW(opts.setMaxReconnects(10));
    CHECK_NOTHROW(opts.userInfo("user", "password"));
    CHECK_NOTHROW(opts.token("mytoken"));
    #ifdef CPPNATS_ENABLE_TLS
    CHECK_NOTHROW(opts.setSecure(true));
    #endif
}

TEST_CASE("setting invalid options") {
    CppNats::Options opts;
    CHECK_THROWS_AS(opts.addServer("invalid_url"), CppNats::Exception);
    CHECK_THROWS_AS(opts.timeout(-1), CppNats::Exception);
    CHECK_THROWS_AS(opts.setReconnectWait(-1), CppNats::Exception);
    CHECK_THROWS_AS(opts.setIPResolutionOrder(static_cast<CppNats::Options::IPResolutionOrder>(33)), CppNats::Exception);
}

} // TEST_SUITE("options")

TEST_SUITE("connection") {

TEST_CASE("creating client") {
    CHECK_NOTHROW(CppNats::Client c);
}

TEST_CASE("connecting to server") {
    CppNats::Client c;
    CHECK_NOTHROW(c.connect(natsTestUrl()));
    c.close();
}

TEST_CASE("connecting to invalid server") {
    CppNats::Client c;
    CHECK_THROWS_AS(c.connect("nats://invalid:4222"), CppNats::Exception);
}

} // TEST_SUITE("connection")
