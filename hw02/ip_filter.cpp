
#include "ip_filter.h"
#include "ip.h"

#include <algorithm>

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
std::vector<std::string> split(std::string_view str, char d) {
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos) {
        r.emplace_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.emplace_back(str.substr(start));

    return r;
}


/**
 * @brief Вывести все значения IP-адресов в стандартный поток вывода
 * @param ip_pool - пул IP-адресов
 */
void print_ip_pool(const Ip_Pool &ip_pool) noexcept {
    for (const auto &ip: ip_pool) {
        std::cout << ip.getIP() << std::endl;
    }
}


/**
 * @brief Сортировать в обратном лексикографическом порядке
 * @param ip_pool - пул IP-адресов
 * @return
 */
Ip_Pool rsort(const Ip_Pool &ip_pool) {
    Ip_Pool result{ip_pool};
    std::sort(result.rbegin(), result.rend());
    return result;
}


/**
 * @brief Получить подмножество IP-адресов с первым байтом равным заданному
 * @param ip_pool - пул IP-адресов
 * @param firstByte - первый байт
 * @return
 */
Ip_Pool ip_filter(const Ip_Pool &ip_pool, int32_t firstByte) {
    Ip_Pool result;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(result),
                 [firstByte](const IP &ip) {
                     return ip.getFirstByte() == firstByte;
                 });
    return result;
}


/**
 * @brief Получить подмножество IP-адресов с первым и вторым байтами равными заданным
 * @param ip_pool - пул IP-адресов
 * @param firstByte - первый байт
 * @param secondByte - второй байт
 * @return
 */
Ip_Pool ip_filter(const Ip_Pool &ip_pool, int32_t firstByte, int32_t secondByte) {
    Ip_Pool result;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(result),
                 [firstByte, secondByte](const IP &ip) {
                     return ((ip.getFirstByte() == firstByte) && (ip.getSecondByte() == secondByte));
                 });
    return result;
}


/**
 * @brief Получить подмножество IP-адресов в которых на любой позиции встречается заданный байт
 * @param ip_pool - пул IP-адресов
 * @param anyByte - любой байт
 * @return
 */
Ip_Pool ip_any_filter(const Ip_Pool &ip_pool, int32_t anyByte) {
    Ip_Pool result;
    std::copy_if(ip_pool.begin(), ip_pool.end(), std::back_inserter(result),
                 [anyByte](const IP& ip) { return ip.isByteInIP(anyByte); });
    return result;
}