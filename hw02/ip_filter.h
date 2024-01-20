//
// Created by Professional on 15.01.2024.
//

#pragma once

#include "ip.h"

#include <string>
#include <vector>
#include <iostream>
#include <cstdint>


using Ip_Pool = std::vector<IP>;


std::vector<std::string> split(std::string_view str, char d);


void print_ip_pool(const Ip_Pool &ip_pool) noexcept;

Ip_Pool rsort(const Ip_Pool &ip_pool) noexcept;

Ip_Pool ip_filter(const Ip_Pool &ip_pool, int32_t firstByte) noexcept;

Ip_Pool ip_filter(const Ip_Pool &ip_pool, int32_t firstByte, int32_t secondByte) noexcept;

Ip_Pool ip_any_filter(const Ip_Pool &ip_pool, int32_t anyByte) noexcept;