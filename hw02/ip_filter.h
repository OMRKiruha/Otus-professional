//
// Created by Professional on 15.01.2024.
//

#pragma once

#include <string>
#include <vector>
#include <iostream>

using Ip_Pool = std::vector<std::vector<std::string> >;


std::vector<std::string> split(std::string_view str, char d);


void print_ip(const Ip_Pool& ip_pool) noexcept;