#include <iostream>
#include <cstddef>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/strand.hpp>

int main(int argc, char* argv[]) {
    constexpr std::uint32_t threads = 4;

    boost::asio::io_context sigioc(threads);
    boost::asio::io_context ioc(threads);
    boost::asio::signal_set signals(sigioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](const boost::system::error_code& ec, int sig) {
        ioc.stop();
    });

    boost::asio::io_context::strand strand0(ioc);
    boost::asio::io_context::strand strand1(ioc);
    boost::asio::io_context::strand strand2(ioc);
    boost::asio::io_context::strand strand3(ioc);
    boost::asio::post(strand0, []() {
        std::cout << "post0_0: " << std::this_thread::get_id() << std::endl;
    });
    boost::asio::post(strand0, []() {
        std::cout << "post0_1: " << std::this_thread::get_id() << std::endl;
    });

    boost::asio::post(strand1, []() {
        std::cout << "post1_0: " << std::this_thread::get_id() << std::endl;
    });
    boost::asio::post(strand1, []() {
        std::cout << "post1_1: " << std::this_thread::get_id() << std::endl;
    });

    boost::asio::post(strand2, []() {
        std::cout << "post2_0: " << std::this_thread::get_id() << std::endl;
    });
    boost::asio::post(strand2, []() {
        std::cout << "post2_1: " << std::this_thread::get_id() << std::endl;
    });

    boost::asio::post(strand3, []() {
        std::cout << "post3_0: " << std::this_thread::get_id() << std::endl;
    });
    boost::asio::post(strand3, []() {
        std::cout << "post3_1: " << std::this_thread::get_id() << std::endl;
    });

    std::thread sigth([&sigioc]() {
        sigioc.run();
    });

    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for ( std::uint32_t i = 0; i < threads; i++ ) {
        v.emplace_back([&ioc]() {
            ioc.run();
        });
    }
    ioc.run();
    for ( auto& t : v ) {
        t.join();
    }

    sigioc.stop();
    sigth.join();
    return 0;
}
