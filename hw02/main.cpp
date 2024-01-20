//
// Created by Professional on 15.01.2024.
//

#include "ip_filter.h"


int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {

    Ip_Pool ip_pool;

    try {
        for (std::string line; std::getline(std::cin, line);) {
            std::vector<std::string> v{split(line, '\t')};
            ip_pool.emplace_back(split(v.at(0), '.'));
        }
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    //print_ip_pool(ip_pool);

    // reverse lexicographically sort
    Ip_Pool sorted_pool{rsort(ip_pool)};
    print_ip_pool(sorted_pool);

    // filter by first byte and output
    print_ip_pool(ip_filter(sorted_pool, 1));

    // filter by first and second bytes and output
    print_ip_pool(ip_filter(sorted_pool, 46, 70));

    // filter by any byte and output
    print_ip_pool(ip_any_filter(sorted_pool, 46));

    return 0;
}