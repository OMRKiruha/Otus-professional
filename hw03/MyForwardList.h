
#ifndef CONTEINERS_MYFORWARDLIST_H
#define CONTEINERS_MYFORWARDLIST_H

#include <cstddef>
#include <iostream>

//#define DEBUG

template<typename T>
class MyForwardList {

    template<typename U>
    struct Node {

        U data{}; // пользовательские данные (хранимый объект)
        Node<U> *next = nullptr; // указатель на следующий элемент Node

        explicit Node(const U &value, Node<U> *next = nullptr) :
                data(value), next(next) {}
    };

public:
    template<typename V>
    class MyForwardListIterator {
        friend class MyForwardList<V>;

    public:
        MyForwardListIterator() : pNode(new Node<T>{}) {}

        explicit MyForwardListIterator(Node<T> *p) : pNode(p) {}

        V &operator*() { return static_cast<Node<T> *>(pNode)->data; }

        V *operator->() { return static_cast<Node<T> *>(pNode); }

        V &get() { return *static_cast<Node<T> *>(pNode)->data; }

        bool operator!=(MyForwardListIterator const &other) const { return pNode != other.pNode; }

        bool operator!=(std::nullptr_t const &) const { return pNode != nullptr; }

        bool operator==(MyForwardListIterator const &other) const { return pNode == other.pNode; }

        MyForwardListIterator<V> &operator++() {
            pNode = pNode->next;
            return *this;
        }

        MyForwardListIterator<V> &operator+(int i) {
            while (i != 0) {
                if( pNode->next != nullptr ) pNode = pNode->next;
                --i;
            }
            return *this;
        }

    private:
        Node<T> *pNode;

        Node<T> *node() {
            return pNode;
        }
    };

    typedef MyForwardListIterator<T> iterator;
    typedef MyForwardListIterator<const T> const_iterator;

private:
    Node<T> *m_begin = nullptr;
    size_t m_size{};

public:
    // Конструктор без параметров
    MyForwardList() {
#ifdef DEBUG
        std::cout << "\tКонструктор без параметров (по умолчанию)\n";
#endif
        m_begin = nullptr;
        m_size = 0;
    }


    // Конструктор перемещения
    MyForwardList(const MyForwardList<T> &&other) noexcept {
#ifdef DEBUG
        std::cout << "\tКонструктор перемещения\n";
#endif
        // Очищаем текущий объект
        delete_elements();
        // Присваиваем чужие указатели
        this->m_begin = other.m_begin;
        this->m_size = other.m_size;
    }


    // Конструктор копирования
    MyForwardList(const MyForwardList<T> &other) {
#ifdef DEBUG
        std::cout << "\tКонструктор копирования\n";
#endif
        copy_elements_from(other);
    }


    //Деструктор
    ~MyForwardList() {
#ifdef DEBUG
        std::cout << "\tДеструктор\n";
#endif
        delete_elements();
    }


    // Оператор присваивания копированием
    MyForwardList<T> &operator=(const MyForwardList<T> &other) {
#ifdef DEBUG
        std::cout << "\tОператор присваивания копированием\n";
#endif
        //Проверка на присваивание самому себе
        if(this == &other) { return *this; }
        // Удаляем наш лист
        delete_elements();
        // и повторяем конструктор копирования
        copy_elements_from(other);
        return *this;
    }


    // Оператор присваивания перемещением
    MyForwardList<T> &operator=(MyForwardList<T> &&other) noexcept {
#ifdef DEBUG
        std::cout << "\tОператор присваивания перемещением\n";
#endif
        //Проверка на присваивание самому себе
        if( other.m_begin == this->m_begin ) { return *this; }
        // Очищаем текущий объект
        delete_elements();
        // Присваиваем чужие указатели и поля
        this->m_begin = other.m_begin;
        this->m_size = other.m_size;
        other.m_begin = nullptr;
        other.m_size = 0;
        return *this;
    }


    [[nodiscard]] size_t size() const { return m_size; }


    // Оператор добавления элемента в конец вектора
    void push_back(const T &value) {
        auto *newNode = new Node<T>(value);    // Создаём новую ноду
        if( m_size == 0 ) {
            this->m_begin = newNode;
            auto *endNode = new Node<T>({});    // Создаём концевую ноду
            this->m_begin->next = endNode;
            this->m_size++;
        } else {
            Node<T> *current = this->m_begin;
            Node<T> *last = this->m_begin;
            while (current->next != nullptr) {
                last = current;
                current = current->next;
            }
            newNode->next = current;
            last->next = newNode;      // В предыдущий конец присваиваем текущую ноду
            this->m_size++;
        }
    }


    // Оператор добавления элемента в конец вектора
    void push_front(const T &value) {
        auto *newNode = new Node<T>(value);    // Создаём новую ноду
        if( m_size == 0 ) {
            this->m_begin = newNode;
            auto *endNode = new Node<T>({});    // Создаём концевую ноду
            this->m_begin->next = endNode;
            this->m_size++;
        } else {
            newNode->next = m_begin;      // В первую ячейку присваиваем предыдущее начало
            this->m_begin = newNode;      // В начало листа присваиваем адрес новой ноды
            this->m_size++;
        }
    }


    // Оператор удаления элемента из конца списка
    void pop_back() {
        Node<T> *current = this->m_begin;
        Node<T> *prev = this->m_begin;
        Node<T> *pprev = this->m_begin;
        while (current->next != nullptr) {
            pprev = prev;
            prev = current;
            current = current->next;
        }

        if( prev != m_begin ) { // Если удаляем не последний элемент
            delete prev;
            pprev->next = current;    // Присваиваем новые указатели
            --m_size;
        } else {    // Если удаляем последний элемент
            delete prev;
            delete current;
            this->m_begin = nullptr;
            --m_size;
        }
    }

    // Оператор удаления элемента из начала списка
    void pop_front() {
        if( m_begin != nullptr ) { // Если удаляем не последний элемент
            Node<T> *new_begin = m_begin->next;
            delete m_begin;
            m_begin = new_begin;    // Присваиваем новые указатели
            --m_size;
        }
    }

    T &operator[](size_t index) {
        // Проверка на корректность переданного индекса элемента
        if( index < m_size ) {
            size_t i = 0;
            Node<T> *it = m_begin;
            while (i != index) {
                it = it->next;
                i++;
            }
            return it->data;
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта MyForwardList"));
    }


    T operator[](size_t index) const {
        // Проверка на корректность переданного индекса элемента
        if( index < m_size ) {
            size_t i = 0;
            Node<T> *it = m_begin;
            while (i != index) {
                it = it->next;
                i++;
            }
            return it->data;
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта MyForwardList"));
    }

    T & at(size_t index) {
        // Проверка на корректность переданного индекса элемента
        if( index < m_size ) {
            size_t i = 0;
            Node<T> *it = m_begin;
            while (i != index) {
                it = it->next;
                i++;
            }
            return it->data;
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта MyForwardList"));
    }


    T at(size_t index) const {
        // Проверка на корректность переданного индекса элемента
        if( index < m_size ) {
            size_t i = 0;
            Node<T> *it = m_begin;
            while (i != index) {
                it = it->next;
                i++;
            }
            return it->data;
        }
        throw (std::out_of_range("Проверьте правильность индекса объекта MyForwardList"));
    }

    iterator begin() { return iterator(m_begin); }

    iterator end() {
        Node<T> *current = this->m_begin;
        if( current ) {
            while (current->next != nullptr) { current = current->next; }
            return iterator(current);
        }
        return iterator(nullptr);
    }

    const_iterator begin() const { return const_iterator(m_begin); }

    const_iterator end() const {
        Node<T> *current = this->m_begin;
        if( current ) {
            while (current->next != nullptr) { current = current->next; }
            return const_iterator(current);
        }
        return const_iterator(nullptr);
    }

    iterator erase(iterator pos) {

        // Удаление из начала
        if( pos == begin()) {
            pop_front();
            return begin();
        } else if( pos == end()) { // Удаление из конца
            pop_back();
            return end();
        } else {        // Удаление из середины
            // Находим предыдущий и следующий элементы
            Node<T> *current = this->m_begin;
            Node<T> *prev = this->m_begin;
            while (current != pos.node()) {
                prev = current;
                current = current->next;
            }
            // Присваиваем новые указатели в обход удаляемого элемента
            Node<T> *next = pos.node()->next;
            prev->next = next;
            --m_size;
            // Удаляем старый элемент
            delete pos.node();
            return iterator(next);
        }
    }

    iterator insert(iterator pos, const T &value) {
        // Вставка в начало
        if( pos == begin()) {
            push_front(value);
        } else if( pos == end()) { // Вставка в конец
            push_back(value);
        } else {        // Вставка в произвольное место
            // Находим предыдущий элемент
            Node<T> *current = this->m_begin;
            Node<T> *prev = this->m_begin;
            while (current != pos.node()) {
                prev = current;
                current = current->next;
            }
            Node<T> *next = pos.node();
            // Присваиваем новые указатели включая добавляемый элемент
            auto temp = new Node<T>(value);
            prev->next = temp;
            temp->next = next;
            ++m_size;
        }
        return pos;
    }

private:
    void copy_elements_from(const MyForwardList<T> &other) {
#ifdef DEBUG
        std::cout << "\tКопируем элементы из другого объекта\n";
#endif
        for (auto otherElem: other) {
            push_back(otherElem);
        }
    }

    // Удаляем элементы
    void delete_elements() {
#ifdef DEBUG
        std::cout << "\tУдаляем элементы. Size = " << m_size << "\n";
#endif
        while (m_size > 0) {
            pop_front();
        }
    }

};

template<typename T>
inline void print(const MyForwardList<T> &list) {
    for (auto elem: list) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

#endif //CONTEINERS_MYFORWARDLIST_H
