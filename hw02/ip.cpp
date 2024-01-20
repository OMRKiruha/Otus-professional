//
// Created by Professional on 20.01.2024.
//

#include "ip.h"

#include <algorithm>


IP::IP(const std::vector<std::string> &str_ip) {
    m_str_ip = str_ip;
    for (const auto &ip_byte: m_str_ip) {
        m_long_ip = m_long_ip * 1000 + fast_atoi(ip_byte.c_str());
    }
}

/**
 * @brief Переводим строку в число
 * @param str - C-строка
 * @return число
 */
int32_t IP::fast_atoi(const char *str) {
    int32_t val = 0;
    while (*str) {
        val = val * 10 + (*str++ - '0');
    }
    return val;
}

/**
 * @brief Оператор сравнения для сортировки
 * @param other
 * @return
 */
bool IP::operator<(const IP &other) const {
    return this->m_long_ip < other.m_long_ip;
}

/**
 * @brief Получить строку содержащую IP-адрес
 * @return
 */
std::string IP::getIP() const {
    std::string result;
    std::for_each(m_str_ip.cbegin(), m_str_ip.cend(),
                  [&result](std::string_view str) {
                      result += str;
                      result += '.';
                  });
    result.pop_back();
    return result;
}


int32_t IP::getFirstByte() const {
    return static_cast<int32_t>(m_long_ip / 1000000000);
}

int32_t IP::getSecondByte() const {
    return static_cast<int32_t>((m_long_ip % 1000000000) / 1000000);
}

bool IP::isByteInIP(int32_t anyByte) const {
    std::string s_anyByte{std::to_string(anyByte)};
    return std::find(m_str_ip.cbegin(), m_str_ip.cend(), s_anyByte) != m_str_ip.cend();
}