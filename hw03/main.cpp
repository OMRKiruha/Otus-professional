//
// Created by Professional on 03.02.2024.
//
#include <cstdint>
#include <iostream>
#include <map>
#include <forward_list>

#include "MyVector.h"
#include "MyAllocator.h"


template<class Container>
void fillByFactorial(Container &container, int32_t size) {
    container[0] = 1;
    for (int32_t i = 1; i < size; ++i) {
        container[i] = container[i - 1] * i;
    }
}

template<typename Container>
void printContainer(const Container &container, std::string_view header){
    std::cout << header << "\n";
    for(size_t i = 0; i < container.size(); ++i){
        std::cout << i << "  " << container.at(i) << "\n";
    }
    std::cout << "\n";
}

template<class Container>
void fillBySequence(Container &container, int32_t from, int32_t to){
    for(int32_t i = from; i <= to; ++i){
        container.push_back(i);
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {

    // создание экземпляра std::map<int, int>
    std::map<int32_t, int32_t> std_allocator_map;

    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    fillByFactorial(std_allocator_map, 10);

    // создание  экземпляра  std::map<int, int> с новым аллокатором, ограниченным 10 элементами
    std::map<int32_t, int32_t, std::less<>, MyAllocator<std::pair<int32_t, int32_t>>> my_allocator_map;

    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    fillByFactorial(my_allocator_map, 10);

    // вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
    printContainer(std_allocator_map, "std::map with std::allocator");
    printContainer(std_allocator_map, "std::map with custom allocator");

    // создание экземпляра своего контейнера для хранения значений типа int
    MyVector<int32_t> std_allocator_vector;

    // заполнение 10 элементами от 0 до 9
    fillBySequence(std_allocator_vector, 0, 9);

    // создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором, ограниченным 10 элементами
    MyVector<int32_t> my_allocator_vector;

    // заполнение 10 элементами от 0 до 9
    fillBySequence(my_allocator_vector, 0, 9);

    // вывод на экран всех значений, хранящихся в контейнере
    printContainer(std_allocator_vector, "MyForwardList with std::allocator");
    printContainer(my_allocator_vector, "MyForwardList with std::allocator");

    std::forward_list<int> fl;

    return 0;
}