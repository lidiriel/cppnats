#include <doctest/doctest.h>
#include <string>
#include <list>

#include "test_helpers.h"

TEST_SUITE("options") {

TEST_CASE("creating options") {
    CHECK_NOTHROW(CppNats::Options opts);
}

TEST_CASE("setting options") {
    CppNats::Options opts;
    CHECK_NOTHROW(opts.addServer("nats://localhost:4222"));
    CHECK_NOTHROW(opts.addServer("nats://127.0.0.1:4222"));
    CHECK_NOTHROW(opts.addServer("nats-tls://my-server.com:4222"));
    CHECK_NOTHROW(opts.addServer("nats-tls://user:pass@localhost:4222"));
    CHECK_NOTHROW(opts.addServer("nats://localhost:"));
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

TEST_CASE("setting multiple servers") {
    CppNats::Options opts;
    std::list<std::string> urls = {
        "nats://localhost:4222",
        "nats://127.0.0.1:4222",
        "nats-tls://my-server.com:4222"
    };
    CHECK_NOTHROW(opts.addServers(urls));
}   

TEST_CASE("setting invalid options") {
    CppNats::Options opts;
    CHECK_THROWS_AS(opts.addServer("invalid_url"), CppNats::Exception);
    CHECK_THROWS_AS(opts.addServer("nats://invalid_url"), CppNats::Exception);
    CHECK_THROWS_AS(opts.addServer("http://localhost"), CppNats::Exception);
    CHECK_THROWS_AS(opts.addServer("tls://localhost:4222"), CppNats::Exception);
    CHECK_THROWS_AS(opts.timeout(-1), CppNats::Exception);
    CHECK_THROWS_AS(opts.setReconnectWait(-1), CppNats::Exception);
    CHECK_THROWS_AS(opts.setIPResolutionOrder(static_cast<CppNats::Options::IPResolutionOrder>(33)), CppNats::Exception);
    CHECK_THROWS_AS(opts.setReconnectBufSize(-1), CppNats::Exception);
    CHECK_THROWS_AS(opts.setReconnectJitter(-1, 0), CppNats::Exception);
    CHECK_THROWS_AS(opts.setReconnectJitter(0, -1), CppNats::Exception);
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

TEST_SUITE("message") {
    TEST_CASE("creating message") {
        CHECK_NOTHROW(CppNats::Message msg("subject", "data", "reply"));
    }

    TEST_CASE("creating message without reply") {
        CHECK_NOTHROW(CppNats::Message msg("subject", "data"));
    }   

    TEST_CASE("getting message fields") {
        CppNats::Message msg("subject", "data", "reply");
        CHECK(msg.subject() == "subject");
        CHECK(msg.data() == "data");
        CHECK(msg.reply() == "reply");
    }
    
    TEST_CASE("setting message fields") {
        CppNats::Message msg;
        CHECK_NOTHROW(msg.setSubject("new_subject"));
        CHECK_NOTHROW(msg.setData("new_data"));
        CHECK_NOTHROW(msg.setReply("new_reply"));
        CHECK(msg.subject() == "new_subject");
        CHECK(msg.data() == "new_data");
        CHECK(msg.reply() == "new_reply");
    }
} // TEST_SUITE("message")

TEST_SUITE("publish") {
    TEST_CASE("publishing message") {
        CppNats::Client c;
        c.connect(natsTestUrl());
        CppNats::Message msg("subject", "data", "reply");
        CHECK_NOTHROW(c.publish(msg));
        c.close();
    }
}   // TEST_SUITE("publish")