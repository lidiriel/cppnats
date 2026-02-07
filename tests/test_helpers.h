#pragma once

#include "cppnats.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <string>

// RAII nats-server manager.
// Forks a nats-server process and waits for it to be ready.
// The server is stopped on destruction via SIGTERM.
struct NatsServer {
    pid_t pid = -1;
    int port;

    explicit NatsServer(int port = 14222, bool jetstream = false) : port(port) {
        pid = fork();
        if (pid == 0) {
            std::string portStr = std::to_string(port);
            if (jetstream) {
                execlp(NATS_SERVER_PATH, NATS_SERVER_PATH,
                       "-p", portStr.c_str(),
                       "-a", "127.0.0.1",
                       "-js",
                       nullptr);
            } else {
                execlp(NATS_SERVER_PATH, NATS_SERVER_PATH,
                       "-p", portStr.c_str(),
                       "-a", "127.0.0.1",
                       nullptr);
            }
            _exit(1);
        }
        if (pid < 0)
            throw std::runtime_error("Failed to fork nats-server");

        waitForReady();
    }

    ~NatsServer() {
        if (pid > 0) {
            kill(pid, SIGTERM);
            int status = 0;
            waitpid(pid, &status, 0);
        }
    }

    NatsServer(const NatsServer&) = delete;
    NatsServer& operator=(const NatsServer&) = delete;

    std::string url() const {
        return "nats://127.0.0.1:" + std::to_string(port);
    }

private:
    void waitForReady() {
        for (int i = 0; i < 50; ++i) {
            natsConnection* nc = nullptr;
            if (natsConnection_ConnectTo(&nc, url().c_str()) == NATS_OK) {
                natsConnection_Destroy(nc);
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        kill(pid, SIGTERM);
        int status = 0;
        waitpid(pid, &status, 0);
        pid = -1;
        throw std::runtime_error("nats-server failed to start on port " + std::to_string(port));
    }
};

// Returns the URL of the core test server (started in main).
std::string natsTestUrl();
