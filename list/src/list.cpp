#include <iostream>
#include <cstdlib>
#include "list.h"

namespace task {
template <class T, class Alloc>
list<T, Alloc>::iterator::iterator() : _node(nullptr) { }

template <class T, class Alloc>
list<T, Alloc>::iterator::iterator(const iterator& other) : _node(other._node) { }

template <class T, class Alloc>
list<T, Alloc>::iterator::iterator(Node* node) : _node(node) { }

template <class T, class Alloc>
typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator=(const iterator& other) {
    _node = other._node;
    return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator++() {
    _node = _node->next;
    return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator++(int) {
    iterator temp = *this;
    ++*this;
    return temp;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator::reference list<T, Alloc>::iterator::operator*() const {
    return *_node->value;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator::pointer list<T, Alloc>::iterator::operator->() const {
    return _node->value;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator--() {
    _node = _node->prev;
    return *this;
}

template <class T, class Alloc>
bool list<T, Alloc>::iterator::operator==(iterator other) const {
    return _node == other._node;
}

template <class T, class Alloc>
bool list<T, Alloc>::iterator::operator!=(iterator other) const {
    return _node != other._node;
}

template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator() : _node(nullptr) { }

template <class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(Node* node) : _node(node) { }

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator& list<T, Alloc>::const_iterator::operator++() {
    _node = _node->next;
    return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator::reference list<T, Alloc>::const_iterator::operator*() const {
    return *_node->value;
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator::pointer list<T, Alloc>::const_iterator::operator->() const {
    return _node->value;
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator& list<T, Alloc>::const_iterator::operator--() {
    _node = _node->prev;
    return *this;
}

template <class T, class Alloc>
bool list<T, Alloc>::const_iterator::operator==(const_iterator other) const {
    return _node == other._node;
}

template <class T, class Alloc>
bool list<T, Alloc>::const_iterator::operator!=(const_iterator other) const {
    return _node != other._node;
}

template <class T, class Alloc>
list<T, Alloc>::list() : _size(0) {
    _allocator = Alloc();
    _head = new Node();
    _tail = new Node(_head, nullptr);
    _head->next = _tail;
}

template <class T, class Alloc>
list<T, Alloc>::list(const Alloc& alloc) : list() {
    _allocator = alloc;
}

template <class T, class Alloc>
list<T, Alloc>::list(size_t count, const T& value, const Alloc& alloc) : list(alloc) {
    _size = count;
    for (size_type i = 0; i < count; ++i) {
        push_back(value);
    }
}

template <class T, class Alloc>
list<T, Alloc>::list(size_t count, const Alloc& alloc) : list(alloc) {
    for (size_type i = 0; i < count; ++i) {
        _size++;
        emplace_back();
    }
}

template <class T, class Alloc>
list<T, Alloc>::~list() { }

template <class T, class Alloc>
list<T, Alloc>::list(const list& other) : list(other._allocator) {
    _allocator = other._allocator;

    for (auto i = other.cbegin(); i != other.cend(); ++i) {
        push_back(*i);
    }
}

template <class T, class Alloc>
list<T, Alloc>::list(list&& other) {
    _allocator = std::move(other._allocator);
    _head = std::move(other._head);
    _tail = std::move(other._tail);
    _size = std::move(other._size);
}

template <class T, class Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(const list& other) {
    _allocator = other._allocator;
    _head = other._head;
    _tail = other._tail;
    _size = other._size;

    return *this;
}

template <class T, class Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(list&& other) {
    _allocator = std::move(other._allocator);
    _head = std::move(other._head);
    _tail = std::move(other._tail);
    _size = std::move(other._size);

    return *this;
}

template <class T, class Alloc>
typename list<T, Alloc>::allocator_type list<T, Alloc>::get_allocator() const {
    return _allocator;
}

template <class T, class Alloc>
typename list<T, Alloc>::reference list<T, Alloc>::front() {
    return *begin();
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reference list<T, Alloc>::front() const {
    return *cbegin();
}

template <class T, class Alloc>
typename list<T, Alloc>::reference list<T, Alloc>::back() {
    return *std::prev(end());
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reference list<T, Alloc>::back() const {
    return *std::prev(cend());
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
    return iterator(_head->next);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::end() {
    return iterator(_tail);
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cbegin() const {
    return const_iterator(_head->next);
}

template <class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cend() const {
    return const_iterator(_tail);
}

template <class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rbegin() {
    return reverse_iterator(end());
}

template <class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rend() {
    return reverse_iterator(begin());
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crbegin() const {
return const_reverse_iterator(cend());
}

template <class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crend() const {
return const_reverse_iterator(cbegin());
}

template <class T, class Alloc>
bool list<T, Alloc>::empty() const {
    return _size == 0;
}

template <class T, class Alloc>
size_t list<T, Alloc>::size() const {
    return _size;
}

template <class T, class Alloc>
size_t list<T, Alloc>::max_size() const {
    return std::min<size_type>(
                           pointer::max_size(_allocator),
                           std::numeric_limits<difference_type>::max());
}

template <class T, class Alloc>
void list<T, Alloc>::clear() {
    erase(cbegin(), cend());
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, const T& value) {
    pointer node_value_pointer = _allocator.allocate(1);

    _allocator.construct(node_value_pointer, value);
    ++_size;

    Node* currentNode = pos._node;

    Node* node = new Node(node_value_pointer, currentNode->prev, currentNode);
    node->prev->next = node;
    currentNode->prev = node;

    return iterator(node);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, T&& value) {
    pointer node_value_pointer = _allocator.allocate(1);

    _allocator.construct(node_value_pointer, std::move(value));
    ++_size;

    Node* currentNode = pos._node;

    Node* node = new Node(node_value_pointer, currentNode->prev, currentNode);
    node->prev->next = node;
    currentNode->prev = node;

    return iterator(node);
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, size_t count, const T& value) {
    if (count == 0){
        return iterator(pos._node);
    }

    iterator result = nullptr;

    for (size_type i = 0; i < count; ++i) {
        iterator new_result = insert(pos, value);
        result = result == nullptr ? new_result : result;
    }

    return result;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator pos) {
    if (pos == cend()) {
        return iterator(pos._node);
    }

    Node* currentNode = pos._node;
    auto result = iterator(currentNode->next);
    currentNode->prev->next = currentNode->next;
    currentNode->next->prev = currentNode->prev;
    --_size;

    _allocator.destroy(currentNode->value);
    _allocator.deallocate(currentNode->value, 1);

    delete currentNode;

    return result;
}

template <class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator first, const_iterator last) {
    if (first == last) {
        return iterator(last._node);
    }

    const_iterator current = first;

    while (current != last) {
        erase(current);
        current = std::next(current);
    }

    return iterator(last._node);
}

template <class T, class Alloc>
void list<T, Alloc>::push_back(const T& value) {
    insert(cend(), value);
}

template <class T, class Alloc>
void list<T, Alloc>::push_back(T&& value) {
    insert(cend(), std::move(value));
}

template <class T, class Alloc>
void list<T, Alloc>::pop_back() {
    erase(std::prev(cend()));
}

template <class T, class Alloc>
void list<T, Alloc>::push_front(const T& value) {
    insert(cbegin(), value);
}

template <class T, class Alloc>
void list<T, Alloc>::push_front(T&& value) {
    insert(cbegin(), std::move(value));
}

template <class T, class Alloc>
void list<T, Alloc>::pop_front() {
    erase(cbegin());
}

template <class T, class Alloc>
template <class... Args>
typename list<T, Alloc>::iterator list<T, Alloc>::emplace(const_iterator pos, Args&&... args) {
    pointer node_value_pointer = _allocator.allocate(1);
    _allocator.construct(node_value_pointer, std::forward<Args>(args)...);
    ++_size;

    Node* currentNode = pos._node;

    Node* node = new Node(node_value_pointer, currentNode->prev, currentNode);
    node->prev->next = node;
    currentNode->prev = node;

    return iterator(node);
}

template <class T, class Alloc>
template <class... Args>
void list<T, Alloc>::emplace_back(Args&&... args) {
    emplace(end(), std::forward<Args>(args)...);
}

template <class T, class Alloc>
template <class... Args>
void list<T, Alloc>::emplace_front(Args&&... args) {
    emplace(begin(), std::forward<Args>(args)...);
}

template <class T, class Alloc>
void list<T, Alloc>::resize(size_t count) {
    size_type list_size = size();
    if (count < size()) {
        for (size_type i = count; i < list_size; ++i) {
            pop_back();
        }
    } else if (count > size()) {
        for (size_type i = count; i > list_size; --i) {
            emplace_back();
        }
    }
}

template <class T, class Alloc>
void list<T, Alloc>::swap(list& other) {
    auto temp_head = std::move(_head);
    auto temp_tail = std::move(_tail);
    auto temp_allocator = std::move(_allocator);
    auto temp_size = std::move(_size);
    _head = std::move(other._head);
    _tail = std::move(other._tail);
    _allocator = std::move(other._allocator);
    _size = std::move(other._size);
    other._head = std::move(temp_head);
    other._tail = std::move(temp_tail);
    other._allocator = std::move(temp_allocator);
    other._size = std::move(temp_size);
}


template <class T, class Alloc>
void list<T, Alloc>::merge(list& other) {
    auto current = begin();

    while (!other.empty()) {
        if (current == end() || *other.begin() < *current) {
            auto currentNode = current._node;
            auto otherNode = other.begin()._node;

            otherNode->prev->next = otherNode->next;
            if (otherNode->next != nullptr) {
                otherNode->next->prev = otherNode->prev;
            }
            otherNode->prev = currentNode->prev;
            otherNode->next = currentNode;
            --other._size;

            currentNode->prev->next = otherNode;
            currentNode->prev = otherNode;
            ++_size;
        } else {
            ++current;
        }
    }
}

template <class T, class Alloc>
void list<T, Alloc>::splice(const_iterator pos, list& other) {
    _size += other._size;
    auto last_element_iterator = --other.end();
    last_element_iterator._node->next = pos._node;
    pos._node->prev->next = other.begin()._node;
    other.begin()._node->prev = pos._node->prev;
    pos._node->prev = last_element_iterator._node;
    other._head->next = other._tail;
    other._tail->prev = other._head;
    other._size = 0;
}

template <class T, class Alloc>
void list<T, Alloc>::remove(const T& value) {
    auto current = begin();

    while (current != end()) {
        if (*current == value) {
            current = erase(current);
        } else {
            ++current;
        }
    }
}

template <class T, class Alloc>
void list<T, Alloc>::reverse() {
    auto left = begin();
    auto right = --end();

    for (size_type i = 0; i < _size / 2; ++i) {
        swap(left._node, right._node);
        ++left;
        --right;
    }
}

template <class T, class Alloc>
void list<T, Alloc>::unique() {
    if (_size < 2) {
        return;
    }

    auto current = begin();
    auto prev = current++;

    while (current != end()) {
        if (*prev == *current) {
            current = erase(current);
        } else {
            prev = current++;
        }
    }
}

template <class T, class Alloc>
void list<T, Alloc>::sort() {
    quick_sort(0, _size);
}

template <class T, class Alloc>
void list<T, Alloc>::swap(Node* a, Node* b) {
    T* temp = a->value;
    a->value = b->value;
    b->value = temp;
}

template <class T, class Alloc>
typename list<T, Alloc>::size_type list<T, Alloc>::split(size_type l, size_type r, T x) {
    size_type m = l;

    if (r < l) {
        return m;
    }

    auto l_iterator = std::next(begin(), l);
    auto r_iterator = std::next(begin(), r);
    auto m_iterator = std::next(begin(), m);

    while (l_iterator != r_iterator) {
        if (*l_iterator < x) {
            swap(l_iterator._node, m_iterator._node);
            ++m;
            ++m_iterator;
        }
        ++l_iterator;
    }

    return m;
}

template <class T, class Alloc>
typename list<T, Alloc>::size_type list<T, Alloc>::split_equal(size_type l, size_type r, T x) {
    size_type m = l;

    if (r < l) {
        return m;
    }

    auto l_iterator = std::next(begin(), l);
    auto r_iterator = std::next(begin(), r);
    auto m_iterator = std::next(begin(), m);

    while (l_iterator != r_iterator) {
        if (*l_iterator == x) {
            swap(l_iterator._node, m_iterator._node);
             ++m;
             ++m_iterator;
        }
        ++l_iterator;
    }

    return m;
}
template <class T, class Alloc>
void list<T, Alloc>::quick_sort(size_type l, size_type r) {
    if (r - l <= 1){
        return;
    }

    size_type x_index = rand() % (r - l) + l;
    Node* x = std::next(begin(), x_index)._node;
    size_type m = split(l, r, *x->value);
    size_type e = split_equal(m, r, *x->value);
    quick_sort(l, m);
    quick_sort(e, r);
}

}
