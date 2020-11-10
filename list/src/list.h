#pragma once
#include <iterator>


namespace task {


template<class T, class Alloc = std::allocator<T>>
class list {

private:
    class Node;

public:
    class const_iterator;

    class iterator {
    public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        friend class const_iterator;
        friend class list;

        iterator();
        iterator(const iterator&);
        iterator(Node* node);
        iterator& operator=(const iterator&);

        iterator& operator++();
        iterator operator++(int);
        iterator operator+(int);
        reference operator*() const;
        pointer operator->() const;
        iterator& operator--();
        iterator operator--(int);
        iterator operator-(int);

        bool operator==(iterator other) const;
        bool operator!=(iterator other) const;

        operator const_iterator() {
            return const_iterator(_node);
        }

    private:
        list::Node* _node;
    };

    class const_iterator {
    public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        friend class list;

        const_iterator();
        const_iterator(Node* node);
        const_iterator& operator=(const iterator&);

        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator operator+(int);
        reference operator*() const;
        pointer operator->() const;
        const_iterator& operator--();
        const_iterator operator--(int);
        const_iterator operator-(int);

        bool operator==(const_iterator other) const;
        bool operator!=(const_iterator other) const;

    private:
        list::Node* _node;
    };

    using value_type = T;
    using allocator_type = Alloc;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Alloc>::pointer;
    using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    list();
    explicit list(const Alloc& alloc);
    list(size_t count, const T& value, const Alloc& alloc = Alloc());
    explicit list(size_t count, const Alloc& alloc = Alloc());

    ~list();

    list(const list& other);
    list(list&& other);
    list& operator=(const list& other);
    list& operator=(list&& other);

    Alloc get_allocator() const;


    T& front();
    const T& front() const;

    T& back();
    const T& back() const;


    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;


    bool empty() const;
    size_t size() const;
    size_t max_size() const;
    void clear();

    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_t count, const T& value);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);


    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();

    void push_front(const T& value);
    void push_front(T&& value);
    void pop_front();

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    template <class... Args>
    void emplace_back(Args&&... args);

    template <class... Args>
    void emplace_front(Args&&... args);

    void resize(size_t count);
    void swap(list& other);


    void merge(list& other);
    void splice(const_iterator pos, list& other);
    void remove(const T& value);
    void reverse();
    void unique();
    void sort();

private:
    class Node {
    public:
        pointer value;
        Node* prev;
        Node* next;

        Node() : value(nullptr), prev(nullptr), next(nullptr) {}

        Node(Node* prev) { }

        Node(Node* prev, Node* next) {
            this->prev = prev;
            this->next = next;
        }

        Node(pointer value, Node* prev, Node* next) : Node(prev, next) {
            this->value = value;
        }
    };

    size_t _size;
    Node* _head;
    Node* _tail;
    allocator_type _allocator;

    void swap(Node*, Node*);
    size_type split(size_type, size_type, T);
    size_type split_equal(size_type, size_type, T);
    void quick_sort(size_type l, size_type r);
};

}  // namespace task
