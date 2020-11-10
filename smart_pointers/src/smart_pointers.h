#pragma once


namespace task {

template <class T>
class SharedPtr;

template <class T>
class WeakPtr;

struct ControlBlock {
    long ref_count;
    long ref_weak_count;

    ControlBlock() = default;
};

template <class T>
class UniquePtr {
public:
    using pointer = T*;

private:
    pointer _ptr;

public:
    explicit UniquePtr(pointer) noexcept;
    UniquePtr(UniquePtr&&) noexcept;
    UniquePtr& operator=(UniquePtr&&) noexcept;

    typename std::add_lvalue_reference<T>::type operator*() const;
    pointer operator->() const noexcept;
    pointer get() const noexcept;

    pointer release() noexcept;
    void reset(pointer ptr = pointer()) noexcept;
    void swap(UniquePtr&) noexcept;

    ~UniquePtr();
};

template <class T>
class SharedPtr {
public:
    using pointer = T*;
    using reference = T&;
    friend WeakPtr<T>;

private:
    pointer _ptr;
    ControlBlock* _cb;

public:
    SharedPtr() noexcept;
    explicit SharedPtr(pointer);
    SharedPtr(const SharedPtr&) noexcept;
    SharedPtr(SharedPtr&&) noexcept;
    SharedPtr(const WeakPtr<T>&);

    SharedPtr& operator=(const SharedPtr&) noexcept;
    SharedPtr& operator=(SharedPtr&&) noexcept;

    reference operator*() const noexcept;
    pointer operator->() const noexcept;
    pointer get() const noexcept;

    long use_count() const noexcept;

    void reset() noexcept;
    void reset(pointer);
    void swap(SharedPtr&) noexcept;

    ~SharedPtr();
};

template <class T>
class WeakPtr {
public:
    using pointer = T*;
    using reference = T&;
    friend SharedPtr<T>;

private:
    pointer _ptr;
    ControlBlock* _cb;

public:
    WeakPtr() noexcept;
    WeakPtr(const SharedPtr<T>&) noexcept;
    WeakPtr(const WeakPtr&) noexcept;
    WeakPtr(WeakPtr&&) noexcept;

    WeakPtr& operator=(const WeakPtr&) noexcept;
    WeakPtr& operator=(WeakPtr&&) noexcept;
    WeakPtr& operator=(const SharedPtr<T>&) noexcept;

    bool expired() const noexcept;
    long use_count() const noexcept;

    SharedPtr<T> lock() const noexcept;

    void reset() noexcept;;
    void swap(WeakPtr&) noexcept;

    ~WeakPtr();
};


// Your template function definitions may go here...

}  // namespace task


#include "smart_pointers.tpp"
