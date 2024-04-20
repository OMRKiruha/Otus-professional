//
// Created by Professional on 18.02.2024.
//

#include <string>
#include <vector>
#include <list>
#include <cstdint>
#include <iostream>
#include <array>


/**
 * @brief Шаблонная функция "вывода IP" для целых чисел
 * @tparam Integer
 * @param integer
 */
template<typename Integer, std::enable_if_t<std::is_integral_v<Integer>, bool> = true>
void print_ip(Integer integer) {
    constexpr size_t size = sizeof(integer);
    std::array<uint8_t, size> ip{};
    auto firstByte = ip.data();
    auto inArrayAdres = reinterpret_cast<Integer *>(firstByte);
    *inArrayAdres = integer;
    for (int i = size - 1; i >= 0; --i) {
        if (i != size - 1) std::cout << ".";
        std::cout << std::to_string(ip[i]);
    }
    std::cout << std::endl;
}

/**
 * @brief Шаблон для строк
 * @param string - строковое представление выводимых данных
 */
template<typename String, std::enable_if_t<std::is_same_v<std::remove_cv_t<String>, std::string>, bool> = true>
void print_ip(const String &string) {
    std::cout << string << std::endl;
}

/**
 * @brief Шаблон для std::vector и std::list
 * @param vec - ссылка на контейнер
 */
template<typename VectorList,
        std::enable_if_t<std::is_same_v<std::remove_cv_t<VectorList>, std::vector<typename VectorList::value_type>> ||
                         std::is_same_v<std::remove_cv_t<VectorList>, std::list<typename VectorList::value_type>>, bool> = true>
void print_ip(const VectorList &vec) {
    for (const auto &elem: vec) {
        if (elem != vec.front()) std::cout << ".";
        std::cout << elem;
    }
    std::cout << std::endl;
}

template<typename T, typename Tuple>
struct AllSame;

template<typename T>
struct AllSame<T, std::tuple<>> : std::true_type {
};

template<typename T, typename U, typename... Ts>
struct AllSame<T, std::tuple<U, Ts...>>
        : std::conditional_t<std::is_same_v<T, U>, AllSame<T, std::tuple<Ts...>>, std::false_type> {
};

/**
 *
 */
//template<class TupleLike, std::size_t I, typename PrevElement_t>
//bool isSameTypeAtIndex() {
//    if constexpr (I + 1 < std::tuple_size_v<TupleLike>) {
//        if constexpr (not std::is_same_v(PrevElement_t,std::tuple_element_t<I+1, TupleLike>) )
//        return isSameTypeAtIndex<TupleLike, I+1, std::tuple_element_t<I, TupleLike>>();
//    }
//    return true;
//    std::cout << "  The type at index " << I << " is: ";
//    using SelectedType = std::tuple_element_t<I, TupleLike>;
//    std::cout << typeid(SelectedType).name() << '\n';
//
//}

//template<typename TupleLike, std::size_t I = 0>
//bool isSameTypes() {
//    static_assert(I == 0, "Tuple size must be not NULL");
//    if constexpr (I == 1) {
//        return true;
//    }
//
//    return isSameTypeAtIndex<TupleLike, 0, std::tuple_element_t<0, TupleLike>>();
//    if constexpr (I < std::tuple_size_v<TupleLike>) {
//        isSameTypeAtIndex<TupleLike, I>();
//        isSameTypes<TupleLike, I + 1>();
//    }
//}


/**
 * @brief Шаблон для стандартных туплов
 * @param tuple - ссылка на контейнер
 */
template<typename Tuple, std::enable_if_t<std::is_same_v<std::remove_cv_t<Tuple>, std::tuple<typename Tuple::value_type>>, bool> = true>
void print_ip(Tuple tuple) {
    for (const auto &elem: tuple) {
        if (elem != tuple.front()) std::cout << ".";
        std::cout << elem;
    }
    std::cout << std::endl;
}


int main() {

    print_ip(int8_t{-1}); // 255
    print_ip(int16_t{0}); // 0.0
    print_ip(int32_t{2130706433}); // 127.0.0.1
    print_ip(int64_t{8875824491850138409});// 123.45.67.89.101.112.131.41
    print_ip(std::string{"Hello, World!"}); // Hello, World!
    print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
    print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100
//    print_ip(std::make_tuple(123, 456, 789, 0)); // 123.456.789.0

    return EXIT_SUCCESS;
}