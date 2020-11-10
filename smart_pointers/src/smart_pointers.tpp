#include "smart_pointers.h"

namespace task {

  template <class T>
  UniquePtr<T>::UniquePtr(pointer p) noexcept : _ptr(p) { }

  template <class T>
  UniquePtr<T>::UniquePtr(UniquePtr&& u) noexcept : _ptr(u.release()) { }

  template <class T>
  UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& u) noexcept {
    reset(u.release());
    return *this;
  }

  template <class T>
  typename UniquePtr<T>::pointer UniquePtr<T>::release() noexcept {
    pointer temp = _ptr;
    _ptr = pointer();
    return temp;
  }

  template <class T>
  void UniquePtr<T>::reset(pointer ptr) noexcept {
    pointer temp = _ptr;
    _ptr = ptr;
    if (temp != nullptr) {
      delete temp;
    }
  }

  template <class T>
  void UniquePtr<T>::swap(UniquePtr& u) noexcept {
    std::swap(_ptr, u._ptr);
  }

  template <class T>
  typename std::add_lvalue_reference<T>::type UniquePtr<T>::operator*() const {
    return *_ptr;
  }

  template <class T>
  typename UniquePtr<T>::pointer UniquePtr<T>::operator->() const noexcept {
    return _ptr;
  }

  template <class T>
  typename UniquePtr<T>::pointer UniquePtr<T>::get() const noexcept {
    return _ptr;
  }

  template <class T>
  UniquePtr<T>::~UniquePtr() {
    reset();
  }

  template <class T>
  SharedPtr<T>::SharedPtr() noexcept : _ptr(nullptr) {
    _cb = new ControlBlock();
    ++_cb->ref_count;
  }

  template <class T>
  SharedPtr<T>::SharedPtr(pointer p) : _ptr(p) {
    _cb = new ControlBlock();
    ++_cb->ref_count;
  }

  template <class T>
  SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept : _ptr(other._ptr) {
    _cb = other._cb;
    ++_cb->ref_count;
  }

  template <class T>
  SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept : _ptr(other._ptr) {
    _cb = other._cb;
    other._ptr = nullptr;
    other._cb = nullptr;
  }

  template <class T>
  SharedPtr<T>::SharedPtr(const WeakPtr<T>& other) : _ptr(other._ptr) {
    _cb = other._cb;
    ++_cb->ref_count;
  }

  template <class T>
  SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other) noexcept {
    SharedPtr(other).swap(*this);
    return *this;
  }

  template <class T>
  SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) noexcept {
    SharedPtr(std::move(other)).swap(*this);
    return *this;
  }

  template <class T>
  typename SharedPtr<T>::reference SharedPtr<T>::operator*() const noexcept {
    return *_ptr;
  }

  template <class T>
  typename SharedPtr<T>::pointer SharedPtr<T>::operator->() const noexcept {
    return _ptr;
  }

  template <class T>
  typename SharedPtr<T>::pointer SharedPtr<T>::get() const noexcept {
    return _ptr;
  }

  template <class T>
  long SharedPtr<T>::use_count() const noexcept {
    return _cb != nullptr ? _cb->ref_count : 0;
  }

  template <class T>
  void SharedPtr<T>::reset() noexcept {
    SharedPtr().swap(*this);
  }

  template <class T>
  void SharedPtr<T>::reset(pointer p) {
    SharedPtr(p).swap(*this);
  }

  template <class T>
  void SharedPtr<T>::swap(SharedPtr& other) noexcept {
    std::swap(_ptr, other._ptr);
    std::swap(_cb, other._cb);
  }

  template <class T>
  SharedPtr<T>::~SharedPtr() {
    if (_cb != nullptr) {
        --_cb->ref_count;
      if (_cb->ref_count == 0) {
        delete _ptr;
        delete _cb;
      }
    }
  }

  template <class T>
  WeakPtr<T>::WeakPtr() noexcept : _ptr(nullptr) {
    _cb = new ControlBlock();
    ++_cb->ref_weak_count;
  }

  template <class T>
  WeakPtr<T>::WeakPtr(const SharedPtr<T>& other) noexcept : _ptr(other._ptr) {
    _cb = other._cb;
    ++_cb->ref_weak_count;
  }

  template <class T>
  WeakPtr<T>::WeakPtr(const WeakPtr& other) noexcept : _ptr(other._ptr) {
    _cb = other._cb;
    ++_cb->ref_weak_count;
  }

  template <class T>
  WeakPtr<T>::WeakPtr(WeakPtr&& other) noexcept : _ptr(other._ptr) {
    _cb = other._cb;
    other._ptr = nullptr;
    other._cb = nullptr;
  }

  template <class T>
  WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) noexcept {
    WeakPtr(other).swap(*this);
    return *this;
  }

  template <class T>
  WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) noexcept {
    WeakPtr(std::move(other)).swap(*this);
    return *this;
  }

  template <class T>
  WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<T>& other) noexcept {
    WeakPtr(other).swap(*this);

    return *this;
  }

  template <class T>
  bool WeakPtr<T>::expired() const noexcept {
    return _cb == nullptr || use_count() == 0;
  }

  template <class T>
  long WeakPtr<T>::use_count() const noexcept {
    return _cb != nullptr ? _cb->ref_count : 0;
  }

  template <class T>
  SharedPtr<T> WeakPtr<T>::lock() const noexcept {
    return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
  }

  template <class T>
  void WeakPtr<T>::reset() noexcept {
    WeakPtr().swap(*this);
  }

  template <class T>
  void WeakPtr<T>::swap(WeakPtr& other) noexcept {
    std::swap(_ptr, other._ptr);
    std::swap(_cb, other._cb);
  }

  template <class T>
  WeakPtr<T>::~WeakPtr() {
    if (_cb != nullptr) {
      --_cb->ref_weak_count;
    }
  }
}
