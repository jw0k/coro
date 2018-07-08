#include <functional>
#include <boost/coroutine2/all.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>

typedef boost::coroutines2::coroutine<void> coro_t;

class RandomGenerator
{
public:
    RandomGenerator(boost::asio::io_context& ioc,
        coro_t::push_type& yield, coro_t::pull_type& cont);
    int get();

private:
    boost::asio::io_context& ioc_;
    boost::asio::steady_timer timer_;
    std::function<void(int)> callback_;

    int result_;
    coro_t::push_type& yield_;
    coro_t::pull_type& cont_;
};
