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
#include <vector>
#include "cppnats.hpp"
#include "helper.hpp"


namespace CppNats {

    Options::Options()
    {
        // initialize with default options
        // opts->allowReconnect        = true;
        // opts->secure                = false;
        // opts->maxReconnect          = NATS_OPTS_DEFAULT_MAX_RECONNECT;
        // opts->reconnectWait         = NATS_OPTS_DEFAULT_RECONNECT_WAIT;
        // opts->pingInterval          = NATS_OPTS_DEFAULT_PING_INTERVAL;
        // opts->maxPingsOut           = NATS_OPTS_DEFAULT_MAX_PING_OUT;
        // opts->ioBufSize             = NATS_OPTS_DEFAULT_IO_BUF_SIZE;
        // opts->maxPendingMsgs        = NATS_OPTS_DEFAULT_MAX_PENDING_MSGS;
        // opts->maxPendingBytes       = -1;
        // opts->timeout               = NATS_OPTS_DEFAULT_TIMEOUT;
        // opts->reconnectBufSize      = NATS_OPTS_DEFAULT_RECONNECT_BUF_SIZE;
        // opts->reconnectJitter       = NATS_OPTS_DEFAULT_RECONNECT_JITTER;
        // opts->reconnectJitterTLS    = NATS_OPTS_DEFAULT_RECONNECT_JITTER_TLS;
        // opts->asyncErrCb            = natsConn_defaultErrHandler;
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

    void Options::addServers(const std::list<std::string>& urls)
    {
        std::vector<const char*> urlList;
        for (const auto& url : urls) {
            if (!Helper::urlIsValid(url)) {
                throw Exception(NATS_INVALID_ARG);
            }
            urlList.push_back(url.c_str());
        }
        natsOptions_SetServers(this->natsOpts, urlList.data(), urls.size());   
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

    void Options::setReconnectBufSize(int size)
    {
        auto err = natsOptions_SetReconnectBufSize(this->natsOpts, size);
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    void Options::setReconnectJitter(int jitter, int jitterTLS)
    {
        auto err = natsOptions_SetReconnectJitter(this->natsOpts, jitter, jitterTLS);
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

    Message::Message() : m_msg(nullptr) {}

    Message::Message(const std::string& subject, const std::string& data, const std::string& reply) : m_msg(nullptr)
    {
        auto err = natsMsg_Create(&m_msg, subject.c_str(), reply.c_str(), data.c_str(), data.size());
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    Message::~Message() noexcept
    {
        natsMsg_Destroy(m_msg);
    }   
    
    const std::string Message::subject() const
    {
        return std::string(natsMsg_GetSubject(m_msg));
    }

    const std::string Message::data() const
    {
        return std::string(natsMsg_GetData(m_msg), natsMsg_GetDataLength(m_msg));
    }

    const std::string Message::reply() const
    {        
        return std::string(natsMsg_GetReply(m_msg));
    }   

    bool Message::operator==(const Message &other) const
    {
        return subject() == other.subject()
            && data() == other.data()
            && reply() == other.reply();
    }


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

    void Client::publish(const Message& message)
    {
        auto err = natsConnection_PublishMsg(m_conn, message.getNatsMsg());
        if (err != NATS_OK) {
            throw Exception(err);
        }
    }

    Subscription Client::subscribe(const std::string& subject, const int timeout)
    {
        Subscription sub;
        auto callback = [](natsConnection* nc, natsSubscription* sub, natsMsg* msg, void* closure) {
            CppNats::QMessages* q = static_cast<CppNats::QMessages*>(closure);
            if (msg) {
                Message reply;
                reply.setMsg(msg);
                q->push(reply);
            } else {
                //p->set_exception(std::make_exception_ptr(Exception(NATS_TIMEOUT)));
            }
            // todo create a thread ? to push in queue

        };

        auto err = natsConnection_SubscribeTimeout(&sub.m_sub, m_conn, subject.data(), timeout, callback, &sub.m_queue);
        if (err != NATS_OK) {
            throw Exception(err);
        }
        return sub;
    }
    
    const Message Subscription::nextMessage(const int timeout)
    {
        // if queue empty launch wait timeout
        // sinon get value
        // futur.get()
        if(m_queue.empty()){
            // launch timeout
        }
        
        return Message();
    }

    Message Client::request(const Message& message, int timeout)
    {
        natsMsg* replyMsg = nullptr;
        auto err = natsConnection_RequestMsg(&replyMsg, m_conn, message.getNatsMsg(), timeout);
        if (err != NATS_OK) {
            throw Exception(err);
        }
        Message reply;
        reply.setMsg(replyMsg); 
        return reply;
    }

   /*  std::future<Message> Client::requestAsync(const Message& message, int timeout)
    {
        std::promise<Message> promise;
        auto future = promise.get_future();

        auto callback = [](natsConnection* nc, natsSubscription* sub, natsMsg* msg, void* closure) {
            std::promise<Message>* p = static_cast<std::promise<Message>*>(closure);
            if (msg) {
                Message reply;
                reply.setMsg(msg);
                p->set_value(reply);
            } else {
                p->set_exception(std::make_exception_ptr(Exception(NATS_ERR)));
            }
        };

        auto err = natsConnection_RequestMsg(m_conn, callback, &promise, message.getNatsMsg(), timeout);
        if (err != NATS_OK) {
            throw Exception(err);
        }

        return future;
    }   */ 


} // namespace CppNats

