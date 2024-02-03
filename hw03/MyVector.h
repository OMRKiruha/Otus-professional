
#ifndef CONTEINERS_MYVECTOR_H
#define CONTEINERS_MYVECTOR_H

#include <cstddef>
#include <iostream>


template<typename T>
class MyVecIterator;

//#define DEBUG

template<typename T>
class MyVector {

    typedef MyVecIterator<T> iterator;
    typedef MyVecIterator<const T> const_iterator;

public:
    // Конструктор без параметров
    MyVector() {
#ifdef DEBUG
        std::cout << "\tКонструктор без параметров (по умолчанию)\n";
#endif
        m_capacity = 0;
        m_size = 0;
        m_end = nullptr;
        m_begin = nullptr;
        m_end_capacity = nullptr;
    }


    // Конструктор копирования
    MyVector(const MyVector<T> &other) {
#ifdef DEBUG
        std::cout << "\tКонструктор копирования\n";
#endif
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_begin = new T[m_capacity]();
        copy_elements_from(other);
        m_end = m_begin + m_size;
        m_end_capacity = m_begin + m_capacity;
    }


    // Конструктор перемещения
    MyVector(const MyVector<T> &&other) noexcept {
#ifdef DEBUG
        std::cout << "\tКонструктор перемещения\n";
#endif
        // Очищаем текущий объект
        delete_elements();
        // Присваиваем чужие указатели и поля
        this->m_begin = other.m_begin;
        this->m_end = other.m_end;
        this->m_end_capacity = other.m_end_capacity;
        this->m_size = other.m_size;
        this->m_capacity = other.m_capacity;
    }


    //Деструктор
    ~MyVector() {
#ifdef DEBUG
        std::cout << "\tДеструктор\n";
#endif
        delete_elements();
    }


    // Оператор присваивания копированием
    MyVector<T> &operator=(const MyVector<T> &other) {
#ifdef DEBUG
        std::cout << "\tОператор присваивания копированием\n";
#endif
        //Проверка на присваивание самому себе
        if (this == &other) { return *this; }
        // Если другой вектор содержит меньше информации чем ёмкость текущего
        // вектора, то избегаем аллокаций
        if (this->m_capacity >= other.m_size) {
            copy_elements_from(other);
            m_size = other.m_size;
            return *this;
        } else {
            // Если другой вектор не помещается в наш, то удаляем наш
            delete_elements();
            // и повторяем конструктор копирования
            this->m_capacity = other.m_capacity;
            m_begin = new T[m_capacity]();
            copy_elements_from(other);
            m_size = other.m_size;
            return *this;
        }
    }


    // Оператор присваивания перемещением
    MyVector<T> &operator=(MyVector<T> &&other) noexcept {
#ifdef DEBUG
        std::cout << "\tОператор присваивания перемещением\n";
#endif
        //Проверка на присваивание самому себе
        if (other.m_begin == this->m_begin) { return *this; }
        // Очищаем текущий объект
        delete_elements();
        // Присваиваем чужие указатели и поля
        this->m_begin = other.m_begin;
        this->m_end = other.m_end;
        this->m_end_capacity = other.m_end_capacity;
        this->m_size = other.m_size;
        this->m_capacity = other.m_capacity;
        other.m_begin = nullptr;
        return *this;
    };


    [[nodiscard]] size_t size() const { return m_size; }


    // Оператор добавления элемента в конец вектора
    void push_back(const T &value) {
        // Если память под массив ещё не выделена, то выделяем и инициализируем
        // соответствующие переменные
        if (m_begin == nullptr) {
            m_capacity = m_min_elements;
            m_begin = new T[m_capacity];
            m_end = m_begin + m_size;
            m_end_capacity = m_begin + m_capacity;
        }
        // Если есть ещё место в векторе
        if (m_end != m_end_capacity) {
            *m_end = value;
            ++m_end;
            ++m_size;
        } else {
            // Если места для добавления нового элемента уже нет, то
            m_capacity *= m_multiplicator;  // Увеличиваем размер capacity
            T *temp = new T[m_capacity];    // Выделяем новый увеличенный массив
            copy_elements_to(temp);         // Копируем элементы в новый массив
            *(temp + m_size) = value;
            ++m_size;
            delete_elements();              // Удаляем старый массив
            this->m_begin = temp;           // Присваиваем новые указатели
            this->m_end = temp + m_size;
            this->m_end_capacity = temp + m_capacity;
        }
    }


    iterator insert(iterator pos, const T &value) {
        // Проверка на корректность переданного итератора
        if (pos.get() >= m_begin && pos.get() <= m_end) {
            // Вставка почти всегда вызывает реаллокацию всего массива
            // Только если это не вставка в конец и есть свободное место
            if (pos.get() == m_end && m_end != m_end_capacity) {
                *m_end = value;
                T *new_pos = m_end;
                ++m_end;
                ++m_size;
                return iterator(new_pos);
            } else {
                // Проверяем необходимость увеличить массив
                if (m_size == m_capacity) {
                    m_capacity *= m_multiplicator;
                }
                T *temp = new T[m_capacity]; // Выделяем новый массив
                T *thisElem = this->m_begin;
                T *tempElem = temp;
                T *new_pos = nullptr;
                while (thisElem != m_end) {
                    if (pos.get() == thisElem) {
                        *tempElem = value;
                        new_pos = tempElem;
                        ++tempElem;
                    }
                    *tempElem = *thisElem;
                    ++tempElem;
                    ++thisElem;
                }
                delete_elements();              // Удаляем старый массив
                ++m_size;
                this->m_begin = temp;           // Присваиваем новые указатели
                this->m_end = temp + m_size;
                this->m_end_capacity = temp + m_capacity;
                return iterator(new_pos);
            }
        }
        return iterator(nullptr);
    }


    iterator erase(iterator pos) {
        // Проверка на корректность переданного итератора
        if (pos.get() >= m_begin && pos.get() < m_end) {
            // Удаление почти всегда вызывает реаллокацию всего массива
            // Только если это не удаление из конца
            if (pos.get() == (m_end - 1)) {
                --m_end;
                --m_size;
                return iterator(m_end);
            } else {
                T *temp = new T[m_capacity]; // Выделяем новый массив
                T *thisElem = this->m_begin;
                T *tempElem = temp;
                T *new_pos = nullptr;
                while (thisElem != m_end) {
                    if (thisElem == pos.get()) {
                        ++thisElem;
                        *tempElem = *thisElem;
                        new_pos = tempElem;
                    } else {
                        *tempElem = *thisElem;
                    }
                    ++tempElem;
                    ++thisElem;
                }
                delete_elements();              // Удаляем старый массив
                --m_size;
                this->m_begin = temp;           // Присваиваем новые указатели
                this->m_end = temp + m_size;
                this->m_end_capacity = temp + m_capacity;
                return iterator(new_pos);
            }
        }
        return iterator(nullptr);
    }


    T &operator[](size_t index) {
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

    iterator begin() { return iterator(m_begin); }

    iterator end() { return iterator(m_end); }

    const_iterator begin() const { return const_iterator(m_begin); }

    const_iterator end() const { return const_iterator(m_end); }

private:
    const size_t m_min_elements = 4;
    const double m_multiplicator = 1.5;
    T *m_begin = nullptr;
    T *m_end = nullptr;
    T *m_end_capacity = nullptr;
    size_t m_size{};
    size_t m_capacity{};

private:
    void copy_elements_from(const MyVector<T> &other) {
#ifdef DEBUG
        std::cout << "\tКопируем элементы из другого объекта\n";
#endif
        T *otherElem = other.m_begin;
        T *thisElem = this->m_begin;
        this->m_size = 0;
        while (otherElem != other.m_end) {
            *thisElem = *otherElem;
            ++otherElem;
            ++thisElem;
            ++(this->m_size);
        }
        this->m_end = thisElem;
        this->m_end_capacity = m_begin + m_capacity;
    }

    void copy_elements_to(T *temp) {
#ifdef DEBUG
        std::cout << "\tКопируем элементы в temp. Capacity = " << m_capacity << "\n";
#endif
        T *thisElem = this->m_begin;
        while (thisElem != m_end) {
            *temp = *thisElem;
            ++temp;
            ++thisElem;
        }
    }

    // Удаляем элементы
    void delete_elements() {
#ifdef DEBUG
        std::cout << "\tУдаляем элементы. Size = " << m_size << " Capacity = " << m_capacity << "\n";
#endif
        delete[] m_begin;
        m_begin = nullptr;
        m_end = nullptr;
        m_end_capacity = nullptr;
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

#endif //CONTEINERS_MYVECTOR_H
