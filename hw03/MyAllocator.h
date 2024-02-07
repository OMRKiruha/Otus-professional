//
// Created by Professional on 03.02.2024.
//

#pragma once

#include <cstddef>
#include <list>
#include <vector>
#include <memory>


// #define DEBUG

template<std::size_t CHUNK_SIZE = 1024>
class Chunk {
    static_assert(CHUNK_SIZE > 64, "CHUNK_SIZE must be more than 64 bytes");

    struct Header {
        uint8_t *m_start{nullptr};
        uint8_t *m_end{nullptr};
        Header *m_prev{nullptr};
        Header *m_next{nullptr};
    };

    static constexpr int s = sizeof(Header);
public:
    Chunk() {
        m_data = reinterpret_cast<uint8_t *>(std::malloc(m_size));
#ifdef DEBUG
        std::cout << "Реальное выделение памяти из кучи" << std::endl;
#endif
        if (!m_data) throw std::bad_alloc();
        // Указатель на ячейку памяти следующую за выделенным нам чанком
        m_end = m_data + m_size;
        // Самый первый заголовок создаётся одновременно с чанком
        m_chunkHeader = new(m_data) Header;
        // Начальная позиция размещения полезных данных
        m_chunkHeader->m_start = m_data + sizeof(Header);
    }

    ~Chunk() noexcept {
        std::free(m_data);
#ifdef DEBUG
        std::cout << "Реальное освобождение памяти на куче" << std::endl;
#endif
    }

    uint8_t *getFreeBlock(size_t blockSize) {
        // Проходим по цепочке заголовков
        auto current = m_chunkHeader;
        ptrdiff_t freeSpace{0};
        while (current) {
            // Если m_end == nullptr то за блоком следует пустое место
            if (!current->m_end) {
                if (current->m_next) {// Если есть следующий заголовок
                    freeSpace = reinterpret_cast<uint8_t *>(current->m_next) - current->m_start;
                } else {
                    freeSpace = (m_end - current->m_start) - sizeof(Header);
                }
            }
            if (freeSpace > 0 && blockSize <= static_cast<size_t>(freeSpace)) {
                // Если это конец, то создаём следующий заголовок
                if (!current->m_next) {
                    auto newHeader = reinterpret_cast<uint8_t *>(current) + sizeof(Header) + blockSize;
                    auto lastHeader = new(newHeader) Header;
                    lastHeader->m_start = newHeader + sizeof(Header);
                    lastHeader->m_prev = current;
                    current->m_next = lastHeader;
                }
                // Отмечаем текущий блок как занятый
                current->m_end = current->m_start + blockSize;
                // Возвращаем указатель на начало пользовательских данных
                return current->m_start;
            }
            current = current->m_next;
        }
        return nullptr;
    }

    void freeBlock(void *pointerToDelete) {
        // Ищем среди выделенных блоков указатель соответствующий переданному
        auto current = m_chunkHeader;
        while (current) {
            if (current->m_start == pointerToDelete) {
                // Помечаем блок как пустой
                current->m_end = nullptr;
                break;
            }
            current = current->m_next;
        }

        // Если следующий заголовок тоже пустой, то удаляем следующий заголовок
        if (!current->m_next->m_end) {
            // Если есть элемент за следующим, то отмечаем в нём удаление следующего
            if (current->m_next->m_next) current->m_next->m_next->m_prev = current;
            // В текущем заголовке присваиваем следующий за следующим элемент
            current->m_next = current->m_next->m_next;
        }

        // Если предыдущий заголовок пустой, то удаляем текущий заголовок
        if (current->m_prev && !current->m_prev->m_end) {
            // Если есть следующий элемент, то в нём отмечаем удаление текущего
            if (current->m_next) current->m_next->m_prev = current->m_prev;
            // В предыдущем заголовке присваиваем следующий после текущего
            current->m_prev->m_next = current->m_next;
        }
    }

    void *getMaxAdress() { return static_cast<void *>(m_end); }

    void *getMinAdress() { return static_cast<void *>(m_chunkHeader->m_start); }

private:
    const size_t m_size = CHUNK_SIZE;
    uint8_t *m_data{nullptr};
    uint8_t *m_end{nullptr};
    Header *m_chunkHeader{nullptr};
};


template<typename T, size_t pool = 10>
struct MyAllocator {
    using value_type = T;

    MyAllocator() {
        memoryStoradge.reserve(5);
        memoryStoradge.emplace_back(std::make_shared<Chunk<chunkSize>>());
    };

    ~MyAllocator() {
        memoryStoradge.clear();
    };

    template<class U>
    explicit MyAllocator(const MyAllocator<U, pool> &) noexcept {}

    T *allocate(size_t n) {
#ifdef DEBUG
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
#endif
        auto blockSize = n * sizeof(T);
        // Если мы запрашиваем больше чем помещается в чанк
        if (blockSize > (chunkSize)) {
            std::cerr << "Size of chunk is to small" << std::endl;
            throw std::bad_alloc();
        }

        auto newPointer = searchFreeSpace(blockSize); //Ищем по всем чанкам свободное место
        if (!newPointer) {   // Если запрошено больше чем есть свободного пространства
            // Создаём новый чанк
            memoryStoradge.emplace_back(std::make_shared<Chunk<chunkSize>>());
            // выделяем место в новом чанке
            newPointer = memoryStoradge.back()->getFreeBlock(blockSize);
        }
        // Если есть достаточно свободного места для выделения запрошенного пространства
        return reinterpret_cast<T *>(newPointer);
    }

    void deallocate(T *p, [[maybe_unused]]size_t n) {
#ifdef DEBUG
        std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
#endif
        if (!p) return; // Если это nullptr то ничего не делаем
        //Перебираем чанки, ищем тот к которому относится данный указатель
        for (auto &chunk: memoryStoradge) {
            if (p >= chunk->getMinAdress() && p < chunk->getMaxAdress()) chunk->freeBlock(p); // Вызываем деаллокатор
        }
    }

    template<typename U, typename... Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
#ifdef DEBUG
        std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    }

    template<typename U>
    void destroy(U *p) {
#ifdef DEBUG
        std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
        p->~U();
    }

    template<typename U>
    struct rebind {
        using other = MyAllocator<U, pool>;
    };

private:
    static constexpr size_t chunkSize = pool * (sizeof(T) + 64);
    std::vector<std::shared_ptr<Chunk<chunkSize>>> memoryStoradge;

private:
    /**
     * @brief Для всех выделенных чанков ищем в них непрерывный объём
     * @param blockSize - размер блока в байтах
     * @return - указатель на начало блока
     */
    uint8_t *searchFreeSpace(size_t blockSize) {
        for (auto &chunk: memoryStoradge) {
            if (auto newBlockPointer = chunk->getFreeBlock(blockSize); newBlockPointer) {
                return newBlockPointer;
            }
        }
        return nullptr;
    }
};

template<class T, class U, size_t pool>
constexpr bool operator==(const MyAllocator<T, pool> &a1, const MyAllocator<U, pool> &a2) noexcept {
    return true;
}

template<class T, class U, size_t pool>
constexpr bool operator!=(const MyAllocator<T, pool> &a1, const MyAllocator<U, pool> &a2) noexcept {
    return false;
}