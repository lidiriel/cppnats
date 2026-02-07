/**
 * This file is part of CppNats, a C++ client for NATS.
 *
 * Copyright (C) 2026 Ludovic Leau Mercier
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http ://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 */
#pragma once
#include <nats.h>
#include <string>
#include <list>

/*
    nc, err := nats.Connect(url, nats.UserCredentials("user.creds"))
    Client c;
    c.connect("nats://user:password@localhost:4222", Option.UserCredentials("user.creds"));
*/

namespace CppNats {

    enum class Status : short
    {
        Ok = NATS_OK,
        Err = NATS_ERR,
        ProtocolError = NATS_PROTOCOL_ERROR,
        IoError = NATS_IO_ERROR,
        LineTooLong = NATS_LINE_TOO_LONG,
        ConnectionClosed = NATS_CONNECTION_CLOSED,
        NoServer = NATS_NO_SERVER,
        StaleConnection = NATS_STALE_CONNECTION,
        SecureConnectionWanted = NATS_SECURE_CONNECTION_WANTED,
        SecureConnectionRequired = NATS_SECURE_CONNECTION_REQUIRED,
        ConnectionDisconnected = NATS_CONNECTION_DISCONNECTED,
        ConnectionAuthFailed = NATS_CONNECTION_AUTH_FAILED,
        NotPermitted = NATS_NOT_PERMITTED,
        NotFound = NATS_NOT_FOUND,
        AddressMissing = NATS_ADDRESS_MISSING,
        InvalidSubject = NATS_INVALID_SUBJECT,
        InvalidArg = NATS_INVALID_ARG,
        InvalidSubscription = NATS_INVALID_SUBSCRIPTION,
        InvalidTimeout = NATS_INVALID_TIMEOUT,
        IllegalState = NATS_ILLEGAL_STATE,
        SlowConsumer = NATS_SLOW_CONSUMER,
        MaxPayload = NATS_MAX_PAYLOAD,
        MaxDeliveredMsgs = NATS_MAX_DELIVERED_MSGS,
        InsufficientBuffer = NATS_INSUFFICIENT_BUFFER,
        NoMemory = NATS_NO_MEMORY,
        SysError = NATS_SYS_ERROR,
        Timeout = NATS_TIMEOUT,
        FailedToInitialize = NATS_FAILED_TO_INITIALIZE,
        NotInitialized = NATS_NOT_INITIALIZED,
        SslError = NATS_SSL_ERROR,
        NoServerSupport = NATS_NO_SERVER_SUPPORT,
        NotYetConnected = NATS_NOT_YET_CONNECTED
    };

    enum class ConnectionStatus : short
    {
        Disconnected = NATS_CONN_STATUS_DISCONNECTED,
        Connecting = NATS_CONN_STATUS_CONNECTING,
        Connected = NATS_CONN_STATUS_CONNECTED,
        Closed = NATS_CONN_STATUS_CLOSED,
        Reconnecting = NATS_CONN_STATUS_RECONNECTING,
        DrainingSubs = NATS_CONN_STATUS_DRAINING_SUBS,
        DrainingPubs = NATS_CONN_STATUS_DRAINING_PUBS
    };

    class Exception : public std::exception
    {
    public:
        Exception(natsStatus s) : errorCode(s) {}
        natsStatus errorCode;
        const char* what() const noexcept override { return natsStatus_GetText(errorCode); }
    };
    
    /* For advanced configuration, create a Options object before connecting:
        Options opts;
        opts.timeout(5000);
        opts.userInfo("user", "password");
        Client c;
        c.connect(opts);
     */
    class Options
    {
        private:
            natsOptions* natsOpts;
            friend class Client;

        public:
            Options();
            ~Options();
            
            enum class IPResolutionOrder : short
            {
                SystemDefault = 0,
                V4Only = 4,
                V6Only = 6,
                V4First = 46,
                V6First = 64
            };

            // ----------- Core Connection Configuration ---------------
            // Single URL or server pool for failover
            // default server : nats://localhost:4222
            void addServer(const std::string& url);
            // Disable server randomization
            void dontRandomize(bool randomize);

            // ----------- Timeout and Network Configuration -----------
            void timeout(int timeout);
            // PingInterval: Time interval (in milliseconds) between client pings to the server to keep the connection alive (default: 2 minutes).
            void setPingInterval(int interval);
            // WriteDeadline: Time (in milliseconds) to wait for a write operation to complete before considering it failed (default: 5 seconds).
            void setWriteDeadline(int deadline);
            // IPResolutionOrder: Order in which to resolve IP addresses when connecting to a server (default: SystemDefault).
            void setIPResolutionOrder(IPResolutionOrder order);

            // ----------- Reconnect Configuration ---------------
            // Reconnection attempts and delays are configured using the following options:
            // - ReconnectWait: Time to wait between reconnection attempts (default: 2 seconds).
            void setReconnectWait(int wait);
            // - MaxReconnects: Maximum number of reconnection attempts before giving up (default: 60). Set to -1 for infinite attempts.
            void setMaxReconnects(int maxReconnects);
            // - ReconnectBufSize: Size of the buffer used to store messages while reconnecting (default: 8MB). Set to 0 for unlimited buffering.
            // - ReconnectJitter: Maximum random delay added to the ReconnectWait time to prevent reconnection storms (default: 1 second).
            // - ReconnectJitterTLS: Maximum random delay added to the ReconnectWait time for TLS connections (default: 1 second).
            // - ReconnectJitterTLS: Maximum random delay added to the ReconnectWait time

            // ----------- Authentication Configuration -----------
            // JWT and NKey Authentication
            void setNKeyFromSeed(const std::string& nkey, const std::string& userCreds);
            // Username/Password Authentication
            void userInfo(const std::string& user, const std::string& password);
            // Token Authentication
            void token(const std::string& token);
            
            #ifdef CPPNATS_ENABLE_TLS
            // ----------- TLS/SSL Configuration -----------
            // Enable encrypted connections
            void setSecure(bool secure);
            // Load client certificates and CA for TLS connections
            void loadCertificates(const std::string& certFile, const std::string& keyFile, const std::string& caFile);
            #endif

            // ----------- Callback and Event Configuration -----------
            // Set callback functions for connection events (e.g., disconnect, reconnect, error)    
            //void setDisconnectHandler(void (*handler)(natsConnection* nc, void* closure), void* closure);
            //void setReconnectHandler(void (*handler)(natsConnection* nc, void* closure), void* closure);
            //void setErrorHandler(void (*handler)(natsConnection* nc, natsStatus err, const char* errTxt, void* closure), void* closure);
            //void setClosedHandler(void (*handler)(natsConnection* nc, void* closure), void* closure);
     }; 

    class Client
    {
    private:
        natsConnection* m_conn;

    public:
        Client();
        ~Client() noexcept;

        void connect(const Options& options);
        void connect(const std::string& address);
        void close() noexcept;

    };
    
    
}
