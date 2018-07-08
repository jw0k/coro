#include "RandomGenerator.hpp"

#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iostream>
#include <boost/asio.hpp>

RandomGenerator::RandomGenerator(boost::asio::io_context& ioc,
    coro_t::push_type& yield, coro_t::pull_type& cont):
    ioc_(ioc), timer_(ioc), result_(-1), yield_(yield), cont_(cont)
{
     std::srand(std::time(nullptr));
}

int RandomGenerator::get()
{
    timer_.expires_from_now(std::chrono::seconds(5));
    timer_.async_wait([this](auto ec)
    {
        if (!ec)
        {
            result_ = 1 + std::rand() % 6;
            cont_();
        }
        else
        {
            cont_();
        }
    });
    yield_();
    return result_;
}
