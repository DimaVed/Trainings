#include <cstddef>
#include <memory>
#include <new>
#include <utility>
#include <iterator>

// rawMemVector
namespace rmv {

    template <typename T>
    struct RawMemory {
    private :
        T* buf = nullptr;
        size_t cp = 0;
    public:
        static T* Allocate(size_t n) {
            return static_cast<T*>(operator new(n * sizeof(T)));
        }
        static void Deallocate(T* buf) {
            operator delete(buf);
        }

        size_t Capacity() const {
            return cp;
        }
        T* data() const {
            return buf;
        }

        void Swap(RawMemory& other) noexcept {
            std::swap(buf, other.buf);
            std::swap(cp, other.cp);
        }

        RawMemory() = default;

        RawMemory(size_t n) {
            buf = Allocate(n);
            cp = n;
        }

        RawMemory(const RawMemory&) = delete;
        RawMemory(RawMemory&& other) noexcept {
            Swap(other);
        }

        ~RawMemory() {
            Deallocate(buf);
        }

        RawMemory& operator = (const RawMemory&) = delete;
        RawMemory& operator = (RawMemory&& other) noexcept {
            Swap(other);
            return *this;
        }

        const T* operator + (size_t i) const {
            return buf + i;
        }
        T* operator + (size_t i) {
            return buf + i;
        }
        const T& operator [] (size_t i) const {
            return buf[i];
        }
        T& operator [] (size_t i) {
            return buf[i];
        }

    };


    template <typename T>
    class vector {
    private:
        RawMemory<T> data;
        size_t sz = 0;

    public:
        void swap(vector& other) noexcept {
            data.Swap(other.data);
            std::swap(sz, other.sz);
        }

        vector() = default;

        vector(size_t n) : data(n) {
            std::uninitialized_value_construct_n(
                data.data(), n
            );
            sz = n;
        }

        vector(const vector& other) : data(other.sz) {
            std::uninitialized_copy_n(
                other.data.data(), other.sz, data.data()
            );
            sz = other.sz;
        }

        vector(vector&& other) noexcept {
            swap(other);
        }

        // Конструктор от списка инициализации
        vector(std::initializer_list<T> data) {
            reserve(data.size());
            for (auto& item : data) {
                push_back(item);
            }
            sz = data.size();
        }


        ~vector() {
            std::destroy_n(data.data(), sz);
        }

        vector& operator = (const vector& other) {
            if (other.sz > data.Capacity()) {
                vector tmp(other);
                swap(tmp);
            }
            else {
                for (size_t i = 0; i < sz && i < other.sz; ++i) {
                    data[i] = other.data[i];
                }
                if (sz < other.sz) {
                    std::uninitialized_copy_n(
                        other.data.data() + sz,
                        other.sz - sz,
                        data.data() + sz
                    );
                }
                else if (sz > other.sz) {
                    std::destroy_n(
                        data.data() + other.sz,
                        sz - other.sz
                    );
                }
                sz = other.sz;
            }
            return *this;
        }

        vector& operator = (vector&& other) noexcept {
            swap(other);
            return *this;
        }

        void reserve(size_t n) {
            if (n > data.Capacity()) {
                RawMemory<T> data2(n);
                std::uninitialized_move_n(
                    data.data(), sz, data2.data()
                );
                std::destroy_n(data.data(), sz);
                data = std::move(data2);
            }
        }

        void resize(size_t n) {
            reserve(n);
            if (sz < n) {
                std::uninitialized_value_construct_n(
                    data + sz, n - sz
                );
            }
            else if (sz > n) {
                std::destroy_n(
                    data + n, sz - n
                );
            }
            sz = n;
        }

        void push_back(const T& elem) {
            if (sz == data.Capacity()) {
                reserve(sz == 0 ? 1 : sz * 2);
            }
            new (data + sz) T(elem);
            ++sz;
        }

        void push_back(T&& elem) {
            if (sz == data.Capacity()) {
                reserve(sz == 0 ? 1 : sz * 2);
            }
            new (data + sz) T(std::move(elem));
            ++sz;
        }

        template <typename ... Args>
        T& emplace_Back(Args&&... args) {
            if (sz == data.Capacity()) {
                reserve(sz == 0 ? 1 : sz * 2);
            }
            auto elem = new (data + sz) T(std::forward<Args>(args)...);
            ++sz;
            return *elem;
        }

        void pop_back() {
            std::destroy_at(data + sz - 1);
            --sz;
        }

        using iterator = T*;
        using const_iterator = const T*;

        iterator begin() noexcept {
            return data.data();
        }
        iterator end() noexcept {
            return data.data() + sz;
        }
        const_iterator begin() const noexcept {
            return data.buf;
        }
        const_iterator end() const noexcept {
            return data.data() + sz;
        }
        const_iterator cbegin() const noexcept {
            return data.data();
        }
        const_iterator cend() const noexcept {
            return data.data() + sz;
        }

        iterator insert(const_iterator pos, const T& elem) {
            size_t i = pos - cbegin();
            PushBack(elem);
            std::rotate(data + i, data + sz - 1, data + sz);
            return begin() + i;
        }
        iterator insert(const_iterator pos, T&& elem) {
            size_t i = pos - cbegin();
            PushBack(std::move(elem));
            std::rotate(data + i, data + sz - 1, data + sz);
            return begin() + i;
        }

        template <typename ... Args>
        iterator emplace(const_iterator pos, Args&&... args) {
            size_t i = pos - cbegin();
            EmplaceBack(std::forward<Args>(args)...);
            std::rotate(data + i, data + sz - 1, data + sz);
            return begin() + i;
        }

        iterator erase(const_iterator pos) {
            size_t i = pos - cbegin();
            if (i + 1 < sz) {
                std::move(data + i + 1, data + sz, data + i);
            }
            pop_back();
            return begin() + i;
        }

        size_t size() const noexcept {
            return sz;
        }
        size_t capacity() const noexcept {
            return data.Capacity();
        }
        // Доступ
        const T& operator[](size_t i) const {
            return data[i];
        }
        T& operator[](size_t i) {
            return data[i];
        }
        const T& back() const {
            return data[sz-1];
        }
        const T& front() const {
            return data[0];
        }
        T& back()  {
            return data[sz - 1];
        }
        T& front()  {
            return data[0];
        }

    };

}