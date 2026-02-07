/**
 * @file cppnats.cpp    
 * This file is part of CppNats, a C++ client for NATS.
 * 
 * Copyright (C) 2026 Ludovic Leau Mercier
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http ://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <iostream>
#include <atomic>
#include "cppnats.hpp"
#include "helper.hpp"


namespace CppNats {

    Options::Options()
    {
        natsOptions_Create(&this->natsOpts);
    }

    Options::~Options()
    {
        natsOptions_Destroy(this->natsOpts);
    }

    void Options::addServer(const std::string& url)
    {
        if (!Helper::urlIsValid(url)) {
            throw Exception(NATS_INVALID_ARG);
        }
        auto err = natsOptions_SetURL(this->natsOpts, url.c_str());
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::dontRandomize(bool randomize)
    {
        auto err = natsOptions_SetNoRandomize(this->natsOpts, randomize);
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::timeout(int timeout)
    {
        auto err = natsOptions_SetTimeout(this->natsOpts, timeout);
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::setPingInterval(int interval)
    {
        auto err = natsOptions_SetPingInterval(this->natsOpts, interval);
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::setWriteDeadline(int deadline)
    {
        auto err = natsOptions_SetWriteDeadline(this->natsOpts, deadline);
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::setIPResolutionOrder(Options::IPResolutionOrder order)
    {
        auto err = natsOptions_IPResolutionOrder(this->natsOpts, static_cast<int>(order));
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::setReconnectWait(int wait)
    {
        auto err = natsOptions_SetReconnectWait(this->natsOpts, wait);
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::setMaxReconnects(int maxReconnects)
    {
        auto err = natsOptions_SetMaxReconnect(this->natsOpts, maxReconnects);
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::setNKeyFromSeed(const std::string& nkey, const std::string& userCreds)
    {
        auto err = natsOptions_SetNKeyFromSeed(this->natsOpts, nkey.c_str(), userCreds.c_str());
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::userInfo(const std::string& user, const std::string& password)
    {
        auto err = natsOptions_SetUserInfo(this->natsOpts, user.c_str(), password.c_str());
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::token(const std::string& token)
    {
        auto err = natsOptions_SetToken(this->natsOpts, token.c_str());
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    #if defined(CPPNATS_ENABLE_TSL)
    void Options::setSecure(bool secure)
    {
        auto err = natsOptions_SetSecure(this->natsOpts, secure);
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::loadCertificates(const std::string& certFile, const std::string& keyFile, const std::string& caFile)
    {
        auto err = natsOptions_LoadCertificates(this->natsOpts, certFile.c_str(), keyFile.c_str(), caFile.c_str());
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }
    #endif

    Client::Client() : m_conn(nullptr) {}

    Client::~Client() noexcept
    {
        if (m_conn) {
            natsConnection_Destroy(m_conn);
        }
    }

    void Client::connect(const Options& opts)
    {
        auto err = natsConnection_Connect(&m_conn, opts.natsOpts);
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Client::connect(const std::string& address)
    {
        auto err = natsConnection_ConnectTo(&m_conn, address.c_str());
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Client::close() noexcept
    {
        if (m_conn) {
            natsConnection_Close(m_conn);
        }
    }
}

