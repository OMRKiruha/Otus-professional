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


/**
 * @brief Рекурсивный шаблон проверки одинаковости типов всех элементов std::tuple<...>
 * @tparam TupleLike - тип std::tuple<...>
 * @tparam I - текущий номер проверяемого элемента
 * @return true - если все элементы одинаковые, false (а точнее ломает выполнение компиляции static_assert) - если типы отличаются
 */
template<class TupleLike, std::size_t I>
constexpr bool isSameTypeAtIndex() {
    if constexpr (I + 1 < std::tuple_size_v<TupleLike>) {       // Проверили на конец тапла
        if constexpr (not std::is_same_v<std::tuple_element_t<I, TupleLike>(), std::tuple_element_t<
                I + 1, TupleLike>()>) {
            static_assert(false, "\033[1;31m ----------!!!--- In tuple all elements must be same type ---!!!---------------------- \033[0m");
            return false;
        }
        return isSameTypeAtIndex<TupleLike, I + 1>();
    }
    return true;
}

/**
 * @brief Начало проверки того что в std::tuple<...> типы всех элементов одинаковые
 * @tparam TupleLike - тип std::tuple<...>
 * @return
 */
template<typename TupleLike>
constexpr bool isSameTypes() {
    static_assert(std::tuple_size_v<TupleLike> != 0, "\033[1;31m ----------!!!--- Tuple size must be not NULL ---!!!---------------------- \033[0m");
    if constexpr (std::tuple_size_v<TupleLike> == 1) { return true; }

    return isSameTypeAtIndex<TupleLike, 0>();
}

/**
 * @brief Собственно реализация вывода элементов std::tuple
 * @tparam TupleT - тип std::tuple<...>
 * @tparam Is - Набор индексов, перебирая которые мы имитируем цикл
 * @param tp - собственно объект std::tuple<...>
 */
template <typename TupleT, std::size_t... Is>
void printTupleImp(const TupleT& tp, std::index_sequence<Is...>) {
    size_t index = 0;
    auto printElem = [&index](const auto& x) {
        if (index++ > 0)
            std::cout << ".";
        std::cout << x;
    };

    (printElem(std::get<Is>(tp)), ...);
    std::cout << std::endl;
}

/**
 * @brief Шаблон для стандартных туплов
 * @param tuple - ссылка на контейнер
 */
template<typename ... Args, std::enable_if_t<isSameTypes<std::tuple<Args...>>(), bool> = true>
void print_ip(const std::tuple<Args...>& tuple) {
    printTupleImp(tuple, std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>>{});
}


int main() {

    print_ip(int8_t{-1}); // 255
    print_ip(int16_t{0}); // 0.0
    print_ip(int32_t{2130706433}); // 127.0.0.1
    print_ip(int64_t{8875824491850138409});// 123.45.67.89.101.112.131.41
    print_ip(std::string{"Hello, World!"}); // Hello, World!
    print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
    print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100
    print_ip(std::make_tuple(123, 456, 789, 0)); // 123.456.789.0

    return EXIT_SUCCESS;
}