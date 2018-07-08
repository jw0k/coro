#include "RandomHttpGetter.hpp"

#include <iostream>

#include <boost/asio/connect.hpp>
#include <boost/beast/version.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

RandomHttpGetter::RandomHttpGetter(boost::asio::io_context& ioc):
    ioc_(ioc), resolver_(ioc), socket_(ioc)
{



    req_.version(11);
    req_.method(http::verb::post);
    req_.target("/json-rpc/1/invoke");
    req_.set(http::field::host, "api.random.org");
    req_.set(http::field::content_type, "application/json");

    req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req_.body() = "{\"jsonrpc\":\"2.0\",\"method\":\"generateIntegers\"}";
    req_.set(http::field::content_length, req_.body().size());
/*
    req_.body() = R"(
{
    "jsonrpc": "2.0",
    "method": "generateIntegers",
    "params": {
        "apiKey": "10b75fb9-52b8-49f4-b9ab-8ba3001bcf26",
        "n": 1,
        "min": 1,
        "max": 6,
        "replacement": true
    },
    "id": 42
})";
*/
}

void RandomHttpGetter::get()
{
    resolver_.async_resolve("api.random.org", "http",
        [this](auto ec, auto results) { this->onResolve(ec, results); });
}

void RandomHttpGetter::onResolve(boost::system::error_code ec,
    tcp::resolver::results_type results)
{
    if (ec)
    {
        std::cout << "NOT RESOLVED\n";
    }
    else
    {
        std::cout << "RESOLVED\n";

        for (auto& result : results)
        {
            std::cout << result.host_name() << "\n";
            std::cout << result.endpoint() << "\n";
        }

        boost::asio::async_connect(
            socket_,
            results.begin(),
            results.end(),
            [this](auto ec, auto it) { this->onConnect(ec, it); });
    }
}

void RandomHttpGetter::onConnect(boost::system::error_code ec,
    tcp::resolver::iterator it)
{
    if (ec)
    {
        std::cout << "NOT CONNECTED\n";
    }
    else
    {
        std::cout << "CONNECTED\n";
        http::async_write(socket_, req_,
            [this](auto ec, auto bt) { this->onWrite(ec, bt); });
    }
}

void RandomHttpGetter::onWrite(boost::system::error_code ec,
    std::size_t bytesTransferred)
{
    if (ec)
    {
        std::cout << "NOT WRITTEN\n";
    }
    else
    {
        std::cout << "WRITTEN\n";
        http::async_read(socket_, buffer_, res_,
            [this](auto ec, auto bt) { this->onRead(ec, bt); });
    }
}

void RandomHttpGetter::onRead(boost::system::error_code ec,
    std::size_t bytesTransferred)
{
    if (ec)
    {
        std::cout << "NOT READ\n";
    }
    else
    {
        std::cout << "READ\n";
        std::cout << res_ << "\n";

        socket_.shutdown(tcp::socket::shutdown_send);
    }
}
