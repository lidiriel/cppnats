#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include "test_helpers.h"

static std::string g_natsUrl;

std::string natsTestUrl() {
    return g_natsUrl;
}

int main(int argc, char** argv) {
    NatsServer server(14222);
    g_natsUrl = server.url();

    doctest::Context ctx;
    ctx.applyCommandLine(argc, argv);
    return ctx.run();
}
