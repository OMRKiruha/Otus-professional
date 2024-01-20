//
// Created by Professional on 20.01.2024.
//

#pragma once

#include <vector>
#include <string>
#include <cstdint>

class IP {
public:
    IP() = default;

    explicit IP(const std::vector<std::string> &);

    ~IP() = default;

    IP &operator=(const IP &other) = default;

    bool operator<(const IP &other) const;

    [[nodiscard]] std::string getIP() const;

    [[nodiscard]] int32_t getFirstByte() const;

    [[nodiscard]] int32_t getSecondByte() const;

    [[nodiscard]] bool isByteInIP(int32_t anyByte) const;

private:
    std::vector<std::string> m_str_ip;
    uint64_t m_long_ip{};

    static int32_t fast_atoi(const char *str);
};
