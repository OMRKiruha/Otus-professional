//
// Created by Professional on 03.02.2024.
//

#pragma once

#include <cstddef>


template<typename T>
struct MyAllocator {
    using value_type = T;

    MyAllocator() noexcept = default;

    ~MyAllocator() = default;

    template<class U>
    explicit MyAllocator(const MyAllocator<U> &) noexcept {}

    T *allocate(size_t n) {
        return static_cast<T *>(::operator new(n * sizeof(T)));
    }

    void deallocate(T *p, [[maybe_unused]]size_t n) {
        ::operator delete(p);
    }

};

template<class T, class U>
constexpr bool operator==(const MyAllocator<T> &a1, const MyAllocator<U> &a2) noexcept {
    return true;
}

template<class T, class U>
constexpr bool operator!=(const MyAllocator<T> &a1, const MyAllocator<U> &a2) noexcept {
    return false;
}