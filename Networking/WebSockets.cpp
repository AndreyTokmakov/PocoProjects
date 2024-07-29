/**============================================================================
Name        : WebSockets.cpp
Created on  : 29.07.2024
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : WebSockets.cpp
============================================================================**/

#include "WebSockets.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/HTTPClientSession.h"

#include <iostream>
#include <memory>


/// https://websocket.org/tools/websocket-echo-server

namespace WebSockets {

    using Poco::Net::HTTPClientSession;
    using Poco::Net::HTTPRequest;
    using Poco::Net::HTTPResponse;
    using Poco::Net::HTTPMessage;
    using Poco::Net::WebSocket;

    struct WebSocketDeleter
    {
        void operator()(WebSocket* ptr) {
            ptr->close();
            delete ptr;
        }
    };


    void SimpleTest()
    {
        // HTTPClientSession session("echo.websocket.org",80);
        HTTPClientSession session("localhost", 6789);
        HTTPRequest request(HTTPRequest::HTTP_GET, "/?encoding=text",HTTPMessage::HTTP_1_1);
        request.set("origin", "http://www.websocket.org");
        HTTPResponse response;

        try {
            // std::cout << "Session connected: " << std::boolalpha << session.connected() << std::endl;
            std::unique_ptr<WebSocket, WebSocketDeleter> webSocket { std::unique_ptr<WebSocket, WebSocketDeleter>(
                    new WebSocket(session, request, response),WebSocketDeleter{})};

            std::string testStr { "Hello echo websocket!!!" };
            const int32_t bytesSend = webSocket->sendFrame(testStr.data(), testStr.size(),WebSocket::FRAME_TEXT);
            std::cout << "Sent bytes " << bytesSend << std::endl;
            int flags = 0;

            char receiveBuff[256];
            const int32_t bytesReceived = webSocket->receiveFrame(receiveBuff,256,flags);
            std::cout << "Received bytes " << bytesReceived << std::endl;
            std::cout << receiveBuff << std::endl;

        } catch (std::exception &e) {
            std::cout << "Exception " << e.what();
        }
    }
}

void WebSockets::TestAll()
{
    SimpleTest();
}