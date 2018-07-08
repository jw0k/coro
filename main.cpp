#include <iostream>
#include <iterator>

#include "drivers/RandomGetter.hpp"
#include "drivers/RandomHttpGetter.hpp"
#include "drivers/RandomGenerator.hpp"

#include <boost/coroutine2/all.hpp>
#include <boost/asio/io_context.hpp>

//https://github.com/boostorg/beast/blob/develop/example/http/client/async-ssl/http_client_async_ssl.cpp

typedef boost::coroutines2::coroutine<void> coro_t;

int main()
{
    boost::asio::io_context ioc;

    coro_t::pull_type printRandomNumber(
        [&ioc, &printRandomNumber](coro_t::push_type& yield)
        {
            RandomGetter rg{ioc, yield, printRandomNumber};
            auto result = rg.get();
            if (result != -1)
            {
                std::cout << "RN FETCHED FROM RANDOM.ORG: " << result << "\n";
            }
            else
            {
                RandomGenerator rng{ioc, yield, printRandomNumber};
                std::cout << "FALLING BACK, RNG: " << rng.get() << "\n";
            }
        });

    std::cout << "CALLING IOC::RUN\n";
    ioc.run();
    std::cout << "GOODBYE\n";

    return 0;
}
