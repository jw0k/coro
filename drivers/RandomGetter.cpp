#include "RandomGetter.hpp"

#include <iostream>

#include <boost/asio/connect.hpp>
#include <boost/beast/version.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace http = boost::beast::http;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

RandomGetter::RandomGetter(boost::asio::io_context& ioc,
    coro_t::push_type& yield, coro_t::pull_type& cont):
    ioc_(ioc), sslCtx_(ssl::context::sslv23_client),
    resolver_(ioc), stream_(ioc, sslCtx_), result_(-1),
    yield_(yield), cont_(cont)
{
    //SSL_set_tlsext_host_name(stream.native_handle(), "api.random.org");

    req_.version(11);
    req_.method(http::verb::post);
    req_.target("/json-rpc/1/invoke");
    req_.set(http::field::host, "api.random.org");
    req_.set(http::field::content_type, "application/json");
    req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

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

    req_.set(http::field::content_length, req_.body().size());
}

int RandomGetter::get()
{
    resolver_.async_resolve("api.random.org", "https",
        [this](auto ec, auto results) { this->onResolve(ec, results); });
    yield_();
    return result_;
}

void RandomGetter::onResolve(boost::system::error_code ec,
    tcp::resolver::results_type results)
{
    if (ec)
    {
        std::cout << "NOT RESOLVED\n";
        cont_();
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
            stream_.next_layer(),
            results.begin(),
            results.end(),
            [this](auto ec, auto it) { this->onConnect(ec, it); });
    }
}

void RandomGetter::onConnect(boost::system::error_code ec,
    tcp::resolver::iterator it)
{
    if (ec)
    {
        std::cout << "NOT CONNECTED\n";
        cont_();
    }
    else
    {
        std::cout << "CONNECTED\n";
        stream_.async_handshake(ssl::stream_base::client,
            [this](auto ec) { this->onHandshake(ec); });
    }
}

void RandomGetter::onHandshake(boost::system::error_code ec)
{
    if (ec)
    {
        std::cout << "NOT HANDSHAKEN\n";
        cont_();
    }
    else
    {
        std::cout << "HANDSHAKEN\n";
        http::async_write(stream_, req_,
            [this](auto ec, auto bt) { this->onWrite(ec, bt); });
    }
}

void RandomGetter::onWrite(boost::system::error_code ec,
    std::size_t bytesTransferred)
{
    if (ec)
    {
        std::cout << "NOT WRITTEN\n";
        cont_();
    }
    else
    {
        std::cout << "WRITTEN\n";
        http::async_read(stream_, buffer_, res_,
            [this](auto ec, auto bt) { this->onRead(ec, bt); });
    }
}

void RandomGetter::onRead(boost::system::error_code ec,
    std::size_t bytesTransferred)
{
    if (ec)
    {
        std::cout << "NOT READ\n";
        cont_();
    }
    else
    {
        std::cout << "READ\n";
        std::cout << res_ << "\n";

        std::stringstream s(res_.body());
        boost::property_tree::ptree root;
        boost::property_tree::read_json(s, root);
        for (auto& rnd : root.get_child("result.random.data"))
        {
            std::cout << rnd.second.data() << "\n";
            result_ = rnd.second.get<int>("", 0);
            break;
        }

        stream_.async_shutdown(
            [this](auto ec) { this->onShutdown(ec); });
    }
}

void RandomGetter::onShutdown(boost::system::error_code ec)
{
    if (ec == boost::asio::error::eof)
    {
        std::cout << "EOF\n";
        ec.assign(0, ec.category());
    }
    if (ec)
    {
        std::cout << ec << " NOT SHUT DOWN\n";
    }
    else
    {
        std::cout << "SHUT DOWN\n";
    }

    cont_();
}
