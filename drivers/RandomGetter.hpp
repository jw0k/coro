#include <functional>
#include <boost/coroutine2/all.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ip/tcp.hpp>

typedef boost::coroutines2::coroutine<void> coro_t;

class RandomGetter
{
public:
    RandomGetter(boost::asio::io_context& ioc, coro_t::push_type& yield,
        coro_t::pull_type& cont);
    int get();

private:
    void onResolve(boost::system::error_code ec,
        boost::asio::ip::tcp::resolver::results_type results);

    void onConnect(boost::system::error_code ec,
        boost::asio::ip::tcp::resolver::iterator it);

    void onHandshake(boost::system::error_code ec);

    void onWrite(boost::system::error_code ec,
        std::size_t bytesTransferred);

    void onRead(boost::system::error_code ec,
        std::size_t bytesTransferred);

    void onShutdown(boost::system::error_code ec);

    boost::asio::io_context& ioc_;

    boost::asio::ssl::context sslCtx_;
    boost::asio::ip::tcp::resolver resolver_;
    //tcp::socket sockecik{ioc};
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream_;

    boost::beast::http::request<boost::beast::http::string_body> req_;
    boost::beast::flat_buffer buffer_;
    boost::beast::http::response<boost::beast::http::string_body> res_;

    int result_;
    coro_t::push_type& yield_;
    coro_t::pull_type& cont_;
};
