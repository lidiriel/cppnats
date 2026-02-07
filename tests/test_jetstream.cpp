#include <doctest/doctest.h>

#include "test_helpers.h"

// Lazily starts a NatsServer with JetStream on port 14223.
// Shared across all TEST_CASEs in this file via the fixture.
static NatsServer& jsServer() {
    static NatsServer srv(14223, true);
    return srv;
}

struct JetStreamFixture {
    std::string jsUrl;

    JetStreamFixture() : jsUrl(jsServer().url()) {}
};

TEST_SUITE("jetstream") {

TEST_CASE_FIXTURE(JetStreamFixture, "connect to jetstream server") {
    CppNats::Client c;
    CHECK_NOTHROW(c.connect(jsUrl));
    c.close();
}

// TODO: add publish, subscribe, pull consumer tests

} // TEST_SUITE("jetstream")
