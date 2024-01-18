
#include "ip_filter.h"


// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
/**
 * @brief Разделить переданную строку на подстроки используя в качестве разделителя символ d
 * @param str - исходная строка
 * @param d - символ разделитель
 * @return вектор строк
 */
std::vector<std::string> split(std::string_view str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.emplace_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.emplace_back(str.substr(start));

    return r;
}

/**
 * @brief Выводим все значения IP-адресов в стандартный поток вывода
 * @param ip_pool
 */
void print_ip(const Ip_Pool& ip_pool) noexcept{
    for (const auto &ip: ip_pool) {
        for (auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part) {
            if (ip_part != ip.cbegin()) {
                std::cout << ".";
            }
            std::cout << *ip_part;
        }
        std::cout << std::endl;
    }
}