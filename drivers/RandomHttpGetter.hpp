#include <boost/asio/io_context.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>

class RandomHttpGetter
{
public:
    RandomHttpGetter(boost::asio::io_context& ioc);
    void get();

private:
    void onResolve(boost::system::error_code ec,
        boost::asio::ip::tcp::resolver::results_type results);

    void onConnect(boost::system::error_code ec,
        boost::asio::ip::tcp::resolver::iterator it);

    void onWrite(boost::system::error_code ec,
        std::size_t bytesTransferred);

    void onRead(boost::system::error_code ec,
        std::size_t bytesTransferred);

    boost::asio::io_context& ioc_;

    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;

    boost::beast::http::request<boost::beast::http::string_body> req_;
    boost::beast::flat_buffer buffer_;
    boost::beast::http::response<boost::beast::http::string_body> res_;
};
