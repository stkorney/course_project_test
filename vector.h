template <typename T>
class MyVector {
private:
    size_t size_;
    T* data_;
    size_t capacity_;

public:
    ////////////////////////////////////////////////////// ctors ////////////////////////////////////////////////////////

    explicit MyVector() noexcept : capacity_(0), size_(0), data_(nullptr) {
    }

    explicit MyVector(size_t size) noexcept : capacity_(size), size_(size) {
        if (size > 0) {
            data_ = new T[size_];
        } else {
            data_ = nullptr;
        }
    }

    explicit MyVector(size_t size, const T& value) noexcept : capacity_(size), size_(size) {
        if (size_ > 0) {
            data_ = new T[size_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = value;
            }
        } else {
            data_ = nullptr;
        }
    }

    MyVector(const MyVector<T>& other) : capacity_(other.size_), size_(other.size_) {
        if (other.data_ != nullptr) {
            data_ = new T[other.size_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        } else {
            data_ = nullptr;
        }
    }

    template <typename Y>
    MyVector(const MyVector<Y>& other) = delete;

    MyVector(MyVector<T>&& other) noexcept {
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    template <typename Y>
    MyVector(MyVector<Y>&& other) = delete;

    // From init list
    MyVector(std::initializer_list<T> list) : size_(list.size()),
                                              capacity_(list.size()) {
        if (list.size() == 0) {
            data_ = nullptr;
        } else {
            data_ = new T[list.size()];
        }
        std::copy(list.begin(), list.end(), data_);
    }

    ////////////////////////////////////////////////////// operators = ////////////////////////////////////////////////////////

    MyVector& operator=(const MyVector<T>& other) {
        if (&other != this) {
            delete[] data_;
            if (other.data_ != nullptr) {
                data_ = new T[other.size_];
                size_ = other.size_;
                capacity_ = other.size_;
                for (size_t i = 0; i < size_; ++i) {
                    data_[i] = other.data_[i];
                }
            } else {
                data_ = nullptr;
                size_ = 0;
                capacity_ = 0;
            }
        }
        return *this;
    }

    template <typename Y>
    MyVector& operator=(const MyVector<Y>& other) = delete;

    MyVector& operator=(MyVector<T>&& other) {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.size_ = 0;
            other.data_ = nullptr;
            other.capacity_ = 0;
        }
        return *this;
    }

    template <typename Y>
    MyVector& operator=(MyVector<Y>&& other) = delete;

    ////////////////////////////////////////////////////// destructor ////////////////////////////////////////////////////////

    ~MyVector() {
        delete[] data_;
    }

    ////////////////////////////////////////////////////// reserve ////////////////////////////////////////////////////////

    void reserve(size_t capacity) {
        if (capacity < size_) {
            capacity = size_;
        }
        capacity_ = capacity;

        T* data;
        if (capacity != 0) {
            data = new T[capacity];
        } else {
            data = nullptr;
        }
    
        if (data != nullptr) {
            for (size_t i = 0; i < size_; ++i) {
                data[i] = std::move(data_[i]);
            }
            delete[] data_;
        }
        data_ = data;
    }

    ////////////////////////////////////////////////////// modifiers ////////////////////////////////////////////////////////

    void assign(size_t size, const T& value) {
        delete[] data_;
        size_ = size;
        capacity_ = size;
        if (size_ > 0) {
            data_ = new T[size_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = value;
            }
        } else {
            data_ = nullptr;
        }
    }

    void clear() {
        size_ = 0;
        return;
    }

    void push_back(const T& value) {
        if (capacity_ == 0) {
            reserve(1);
        } else if (size_ >= capacity_) {
            reserve(capacity_ * 2);
        }
        data_[size_] = value;
        ++size_;
    }

    void push_back(T&& value) {
        if (capacity_ == 0) {
            reserve(1);
        } else if (size_ >= capacity_) {
            reserve(capacity_ * 2);
        }
        data_[size_] = std::move(value);
        ++size_;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    void resize(size_t new_size) {
        if (new_size > capacity_) {
            if (new_size > capacity_ * 2) {
                reserve(new_size);
            } else {
                reserve(capacity_ * 2);
            }
        }
        
        if (new_size > size_) {
            for (size_t i = size_; i < new_size; ++i) {
                data_[i] = T();
            }
        }
        size_ = new_size;
    }

    void resize(size_t new_size, const T& value) {
        if (new_size > capacity_) {
            if (new_size > capacity_ * 2) {
                reserve(new_size);
            } else {
                reserve(capacity_ * 2);
            }
        }
        
        if (new_size > size_) {
            for (size_t i = size_; i < new_size; ++i) {
                data_[i] = value;
            }
        }
        size_ = new_size;
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (size_ >= capacity_) {
            if (capacity_ == 0) {
                reserve(1);
            } else if (size_ >= capacity_) {
                reserve(capacity_ * 2);
            }
        }
        data_[size_] = T(std::forward<Args>(args)...);
        ++size_;
    }

    template<typename... Args>
    T* emplace(const T* position, Args&&... args) {
        // Somehow emplace lets you use it if position > end(), but less then begint() + capacity_
        if (position < begin() || position > begin() + capacity_) {
            throw std::out_of_range("Position out of reserved memory.");
        }
        
        size_t idx = position - begin();
        if (size_ >= capacity_) {
            if (capacity_ == 0) {
                reserve(1);
            } else if (size_ >= capacity_) {
                reserve(capacity_ * 2);
            }
            position = begin() + idx;
        }
        
        for (T* el = end(); el > position; --el) {
            *el = *(el - 1);
        }
        
        *position = T(std::forward<Args>(args)...);
        ++size_;
        
        return position;
    }

    void swap(MyVector& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        using std::swap;
        swap(data_, other.data_);
    }

    ////////////////////////////////////////////////////// observers ////////////////////////////////////////////////////////

    T& operator[](size_t idx) {
        return data_[idx];
    }

    const T& operator[](size_t idx) const {
        return data_[idx];
    }


    T* begin() {
        return data_;
    }

    T* end() {
        return data_ + size_;
    }

    const T* begin() const {
        return data_;
    }

    const T* end() const {
        return data_ + size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    size_t size() const {
        return size_;
    }
    
    bool empty() const {
        return size_ == 0;
    }
};
