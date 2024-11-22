
// Naive Vector. New delete operators call Constructor too many times;
namespace nv {

    template <typename T>
    class vector final {
    private:
        T* buf = nullptr;
        size_t cp = 0;
        size_t sz = 0;

    public:
        
        // �������
        size_t capacity() const {
            return cp;
        }
        size_t size() const {
            return sz;
        }
        T* data() const {
            return buf;
        }
        //������������
        vector() = default;

        // ����������� � ��������
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
       
        
        
        // ����������

        ~vector() {
            operator delete(buf);
        }
        // ��������� 
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

        // �������� �������
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