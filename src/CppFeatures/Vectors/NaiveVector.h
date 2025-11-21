
// Naive Vector. New delete operators call Constructor too many times;
namespace nv {

    template <typename T>
    class vector final {
    private:
        T* buf = nullptr;
        size_t cp = 0;
        size_t sz = 0;

    public:
        void swap(vector& other) noexcept {

            std::swap(buf, other.buf);
            std::swap(cp, other.cp);
            std::swap(sz, other.sz);

        }
        // геттеры
        size_t capacity() const {
            return cp;
        }
        size_t size() const {
            return sz;
        }
        T* data() const {
            return buf;
        }
        //Конструкторы
        vector() = default;
        // Конструктор от списка инициализации
        vector(std::initializer_list<T> data) {
            reserve(data.size());
            std::copy(data.begin(), data.end(), buf);
            sz = data.size();
        }

        // Копирующие конструкторы
        vector(const vector &  other) {
            reserve(other.size());
            std::copy(other.data(), other.data() + other.size(), buf);
            sz = other.sz;

        }
        vector( vector && other) {
            swap(other);
        }
        // Перемещающий оператор присваивания
        vector& operator = (vector&& other) noexcept {
            swap(other);
            return *this;
        }

        vector& operator = (const vector& other) noexcept {
            reserve(other.size());
            std::copy(other.data(), other.data() + other.size(), buf);
            sz = other.sz;
            return *this;
        }
        

        // Манипуляции с размером
        void reserve(size_t n) {
            if (n > cp) {
                T* new_data = new T[n]();
                for (int i = 0; i < sz; ++i) {
                    new_data[i] = std::move(buf[i]);
                }
                delete[]buf;
                buf = new_data;
                cp = n;
            }
        }
        void resize(size_t n) {
           
            if (sz < n) {
                reserve(n);
            }
            else if (sz > n) {
                T* new_data = new T[cp]();
                for (int i = 0; i < n; ++i) {
                    new_data[i] = std::move(buf[i]);
                }
                delete[]buf;
                buf = new_data;
            }
            sz = n;
        }

        void push_back(const T& elem) {
            if (sz ==cp) {
                reserve(sz == 0 ? 1 : sz * 2);
            }
            
            buf[sz] = elem;
            ++sz;
        }

        void push_back(T&& elem) {
            if (sz == cp) {
                reserve(sz == 0 ? 1 : sz * 2);
            }
            buf[sz] = std::move( elem);
            ++sz;
        }
       
        
        
        // Деструктор

        ~vector() {
            delete[]buf;
        }
        // итераторы 
        using iterator = T*;
        using const_iterator = const T*;

        iterator begin() noexcept {
            return buf;
        }
        iterator end() noexcept {
            return buf + sz;
        }
        const_iterator begin() const noexcept {
            return buf;
        }
        const_iterator end() const noexcept {
            return buf + sz;
        }
        const_iterator cbegin() const noexcept {
            return buf;
        }
        const_iterator cend() const noexcept {
            return buf + sz;
        }

        // Оператор доступа
        const T& operator[](size_t i) const {
            return buf[i];
        }
        T& operator[](size_t i) {
            return buf[i];
        }

        const T& back() const {
            return buf[sz - 1];
        }
        const T& front() const {
            return buf[0];
        }
       
        T& back()  {
            return buf[sz - 1];
        }
        T& front()  {
            return buf[0];
        }

    };
}