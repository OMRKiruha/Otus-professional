
#pragma once

#include <cstddef>
#include <iostream>


template<typename T>
class MyVecIterator;

// #define DEBUG

template<typename T, class Allocator = std::allocator<T> >
class MyVector {

    typedef MyVecIterator<T> iterator;
    typedef MyVecIterator<const T> const_iterator;

public:
    using value_type = T;
    using allocator_type = Allocator;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

    // Конструктор без параметров
    MyVector() : m_begin(nullptr),
                 m_end(nullptr),
                 m_end_capacity(nullptr),
                 m_size(0),
                 m_capacity(0),
                 m_alloc(Allocator()) {
#ifdef DEBUG
        std::cout << "\tКонструктор без параметров (по умолчанию)\n";
#endif
    }


    // Конструктор копирования
    MyVector(const MyVector &other) : m_begin(nullptr),
                                      m_end(nullptr),
                                      m_end_capacity(nullptr),
                                      m_size(0),
                                      m_capacity(0),
                                      m_alloc(other.m_alloc) {
#ifdef DEBUG
        std::cout << "\tКонструктор копирования\n";
#endif
        reserve(other.m_size);
        for (size_t i = 0; i < other.m_size; ++i) {
            push_back(other.m_begin[i]);
        }
    }


    // Конструктор перемещения
    MyVector(MyVector &&other) noexcept: m_begin(other.m_begin),
                                         m_end(other.m_end),
                                         m_end_capacity(other.m_end_capacity),
                                         m_size(other.m_size),
                                         m_capacity(other.m_capacity),
                                         m_alloc(other.m_alloc) {
#ifdef DEBUG
        std::cout << "\tКонструктор перемещения\n";
#endif
        // Очищаем чужие указатели и поля
        other.m_begin = nullptr;
        other.m_end = nullptr;
        other.m_end_capacity = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }


    //Деструктор
    ~MyVector() {
#ifdef DEBUG
        std::cout << "\tДеструктор\n";
#endif
        clear();
        deallocate(m_begin, m_capacity);
    }


    // Оператор присваивания копированием
    MyVector &operator=(const MyVector &other) {
#ifdef DEBUG
        std::cout << "\tОператор присваивания копированием\n";
#endif
        //Проверка на присваивание самому себе
        if (this != &other) {
            // Если другой вектор содержит меньше информации чем ёмкость текущего
            // вектора, то избегаем аллокаций
            clear();
            reserve(other.m_size);
            for (size_t i = 0; i < other.m_size; ++i) {
                push_back(other.m_data[i]);
            }
            m_alloc = other.m_alloc;
        }
        return *this;
    }


    // Оператор присваивания перемещением
    MyVector &operator=(MyVector &&other) noexcept {
#ifdef DEBUG
        std::cout << "\tОператор присваивания перемещением\n";
#endif
        //Проверка на присваивание самому себе
        if (this != &other) {
            // Очищаем текущий объект
            clear();
            deallocate(m_begin, m_capacity);
            // Присваиваем чужие указатели и поля
            this->m_begin = other.m_begin;
            this->m_end = other.m_end;
            this->m_end_capacity = other.m_end_capacity;
            this->m_size = other.m_size;
            this->m_capacity = other.m_capacity;
            // Зануляем украденный объект
            other.m_begin = nullptr;
            other.m_end = nullptr;
            other.m_end_capacity = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }
        return *this;
    };

    [[nodiscard]] bool empty() const noexcept { return m_size == 0; }

    [[nodiscard]] size_t size() const noexcept { return m_size; }

    void reserve(size_t new_capacity) {
        if (new_capacity > m_capacity) {
#ifdef DEBUG
            std::cout << "\tРезервируем " << new_capacity << " элементов. Предыдущая ёмкость "
                      << m_capacity << " элементов \n";
#endif
            pointer new_data = allocate(new_capacity);
            for (size_t i = 0; i < m_size; ++i) {
                std::allocator_traits<Allocator>::construct(m_alloc, new_data + i, std::move(m_begin[i]));
                std::allocator_traits<Allocator>::destroy(m_alloc, m_begin + i);
            }
            deallocate(m_begin, m_capacity);
            m_begin = new_data;
            m_capacity = new_capacity;
        }
    }

    void clear() noexcept {
#ifdef DEBUG
        std::cout << "\tОчищаем " << m_size << " элементов\n";
#endif
        for (size_t i = 0; i < m_size; ++i) {
            std::allocator_traits<Allocator>::destroy(m_alloc, m_begin + i);
        }
        m_size = 0;
    }

    void push_back(const T &value) {
        emplace_back(value);
    }

    void push_back(T &&value) {
        emplace_back(std::move(value));
    }

    template<typename... Args>
    T &emplace_back(Args &&... args) {
#ifdef DEBUG
        std::cout << "\tВставка в конец size = " << m_size << "\tcapacity = " << m_capacity << "\n";
#endif
        if (m_size == m_capacity) {
            reserve(std::max<int64_t>(m_size * m_multiplicator, m_min_elements));
        } else {
            reserve(m_size + 1);
        }
        std::allocator_traits<Allocator>::construct(m_alloc, m_begin + m_size, std::forward<Args>(args)...);
        ++m_size;
        return back();
    }

    iterator insert(const_iterator pos, const T &value) {
        return emplace(pos, value);
    }

    iterator insert(const_iterator pos, T &&value) {
        return emplace(pos, std::move(value));
    }

    template<typename... Args>
    iterator emplace(const_iterator pos, Args &&... args) {
        size_t index = pos - cbegin();
        if (m_size == m_capacity) {
            reserve(std::max<int64_t>(m_size * m_multiplicator, m_min_elements));
        } else {
            reserve(m_size + 1);
        }
        for (size_t i = m_size; i > index; --i) {
            std::allocator_traits<Allocator>::construct(m_alloc, m_begin + i, std::move(m_begin[i - 1]));
            std::allocator_traits<Allocator>::destroy(m_alloc, m_begin + i - 1);
        }
        std::allocator_traits<Allocator>::construct(m_alloc, m_begin + index, std::forward<Args>(args)...);
        ++m_size;
        return begin() + index;
    }

    iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }

    iterator erase(const_iterator first, const_iterator last) {
        size_t index = first - cbegin();
        size_t count = last - first;
        for (size_t i = index; i < m_size - count; ++i) {
            std::allocator_traits<Allocator>::construct(m_alloc, m_begin + i, std::move(m_begin[i + count]));
            std::allocator_traits<Allocator>::destroy(m_alloc, m_begin + i + count);
        }
        for (size_t i = m_size - count; i < m_size; ++i) {
            std::allocator_traits<Allocator>::destroy(m_alloc, m_begin + i);
        }
        m_size -= count;
        return begin() + index;
    }


    T &operator[](size_t index) {
        // Проверка на корректность переданного индекса элемента
        if (index < m_size) {
            return *(m_begin + index);
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта MyVector"));
    }

    const T &operator[](size_t index) const {
        // Проверка на корректность переданного индекса элемента
        if (index < m_size) {
            return *(m_begin + index);
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта MyVector"));
    }

    T &at(size_t index) {
        // Проверка на корректность переданного индекса элемента
        if (index < m_size) {
            return *(m_begin + index);
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта MyVector"));
    }

    const T &at(size_t index) const {
        // Проверка на корректность переданного индекса элемента
        if (index < m_size) {
            return *(m_begin + index);
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта MyVector"));
    }

    iterator begin() noexcept { return iterator(m_begin); }

    iterator end() noexcept { return iterator(m_end); }

    const_iterator cbegin() const noexcept { return const_iterator(m_begin); }

    const_iterator cend() const noexcept { return const_iterator(m_end); }

    T &front() noexcept { return *m_begin; }

    const T &front() const noexcept { return *m_begin; }

    T &back() noexcept { return *(m_begin + m_size - 1); }

    const T &back() const noexcept { return *(m_begin + m_size - 1); }

private:
    const size_t m_min_elements = 4;
    const int32_t m_multiplicator = 2;
    T *m_begin = nullptr;
    T *m_end = nullptr;
    T *m_end_capacity = nullptr;
    size_t m_size{};
    size_t m_capacity{};

    Allocator m_alloc;

private:

    pointer allocate(size_t count) {
        return std::allocator_traits<Allocator>::allocate(m_alloc, count);
    }

    void deallocate(pointer ptr, size_t count) {
        std::allocator_traits<Allocator>::deallocate(m_alloc, ptr, count);
    }
};

template<typename T>
class MyVecIterator {

public:
    explicit MyVecIterator(T *p) : p(p) {}

    MyVecIterator(const MyVecIterator &it) : p(it.p) {}

    bool operator!=(MyVecIterator const &other) const {
        return p != other.p;
    }

    bool operator==(MyVecIterator const &other) const {
        return p == other.p;
    }

    T *get() { return p; }

    T &operator*() { return *p; }

    MyVecIterator<T> &operator++() {
        ++p;
        return *this;
    }

    MyVecIterator<T> &operator--() {
        --p;
        return *this;
    }

    MyVecIterator<T> &operator+(int i) {
        p = p + i;
        return *this;
    }

    MyVecIterator<T> &operator-(int i) {
        p = p - i;
        return *this;
    }

private:
    T *p;
};

template<typename T>
inline void print(const MyVector<T> &vec) {
    for (auto elem: vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}
