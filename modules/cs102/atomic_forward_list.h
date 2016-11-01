#pragma once

#include <atomic>
#include <memory>

template <class T>
class atomic_forward_list {
    struct Node {
        Node() = default;
        Node(const T& val) : t{val} {
        }
        Node(T&& val) : t{val} {
        }

        T t;
        std::shared_ptr<Node> next; // should be atomic
    };
    std::shared_ptr<Node> m_head; // should be atomic

    class ref {
    public:
        ref(std::shared_ptr<Node> p) : m_p{p} {
        }

        T& operator*() {
            return m_p->t;
        }

        T* operator->() {
            return &m_p->t;
        }

    private:
        std::shared_ptr<Node> m_p;
    };

    template <class Type>
    class my_iterator {
    public:
        typedef Type value_type;
        typedef value_type& reference;
        typedef value_type* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;

        my_iterator() = default;
        my_iterator(std::shared_ptr<Node> data) : m_data{data} {
        }

        reference operator*() {
            return *ref(m_data);
        }

        pointer operator->() {
            return &*ref(m_data);
        }

        my_iterator& operator++() {
            if (m_data) {
                m_data = m_data->next;
            }
            return *this;
        }

        my_iterator operator++(int) {
            auto temp = *this;
            if (m_data) {
                m_data = m_data->next;
            }
            return temp;
        }

        bool operator!=(const my_iterator& it) const {
            return m_data != it.m_data;
        }

    private:
        std::shared_ptr<Node> m_data;

        friend class forward_list;
    };

    typedef my_iterator<T> iterator;
    typedef my_iterator<const T> const_iterator;

public:
    atomic_forward_list() = default;
    atomic_forward_list(const atomic_forward_list&) = delete;
    atomic_forward_list& operator=(const atomic_forward_list&) = delete;

    auto front() {
        return ref(std::atomic_load(&m_head));
    }

    const auto front() const {
        return ref(std::atomic_load(&m_head));
    }

    void push_front(const T& t) {
        // Make a new node
        auto p = std::make_shared<Node>(t);

        // Point p's next pointer at head
        p->next = std::atomic_load(&m_head);

        // Update head to new node
        std::atomic_compare_exchange_strong(&m_head, &p->next, p);
    }

    void push_front(T&& t) {
        // Make a new node
        auto p = std::make_shared<Node>(std::forward<T>(t));

        // Point p's next pointer at head
        p->next = std::atomic_load(&m_head);

        // Update head to new node
        std::atomic_compare_exchange_strong(&m_head, &p->next, p);
    }

    void clear() noexcept {
        auto p = std::atomic_load(&m_head);

        // Update head to nullptr to empty list
        while (p && !std::atomic_compare_exchange_weak(&m_head, &p, nullptr)) {
        }
    }

    void insert_after(iterator position, const T& t) {
        // Make a new node
        auto p = std::make_shared<Node>(t);

        // Point p's next pointer at position's next pointer
        p->next = std::atomic_load(&position.m_data->next);

        // Update position's next pointer to new node
        if (std::atomic_load(&m_head) == nullptr) {
            std::atomic_compare_exchange_strong(&m_head, nullptr, &p, p);
        } else {
            std::atomic_compare_exchange_strong(&position.m_data->next, &p->next,
                                                p);
        }
    }

    void erase_after(iterator position) {
        auto prev = std::atomic_load(&position.m_data);
        auto p = std::atomic_load(&prev->next);

        decltype(p) head;

        // Update previous node's next pointer to skip current node
        bool exchanged = false;
        do {
            if (p) {
                head = std::atomic_load(&m_head);
                if (p && p == head) {
                    exchanged = std::atomic_compare_exchange_weak(&m_head, &p,
                        std::atomic_load(&p->next));
                } else {
                    exchanged = std::atomic_compare_exchange_weak(&prev->next,
                        &p, std::atomic_load(&p->next));
                }
            }
        } while (p && !exchanged);
    }

    void erase_after(const_iterator position) {
        auto prev = std::atomic_load(&position.m_data);
        auto p = std::atomic_load(&prev->next);

        decltype(p) head;

        // Update previous node's next pointer to skip current node
        bool exchanged = false;
        do {
            if (p) {
                head = std::atomic_load(&m_head);
                if (p && p == head) {
                    exchanged = std::atomic_compare_exchange_weak(&m_head, &p,
                        std::atomic_load(&p->next));
                } else {
                    exchanged = std::atomic_compare_exchange_weak(&prev->next,
                        &p, std::atomic_load(&p->next));
                }
            }
        } while (p && !exchanged);
    }

    template <class... Args>
    void emplace_after(const_iterator position, Args&& ... args) {
        // Make a new node
        auto p = std::make_shared<Node>(args...);

        // Point p's next pointer at position's next pointer
        p->next = std::atomic_load(&position.m_data);

        // Update position's next pointer to new node
        std::atomic_compare_exchange_strong(&position.m_data, &p->next, p);
    }

    template <class... Args>
    void emplace_front(Args&& ... args) {
        push_front(std::move(T(args...)));
    }

    void pop_front() {
        auto p = std::atomic_load(&m_head);

        // Update head to next pointer
        while (p && !std::atomic_compare_exchange_weak(&m_head, &p, p->next)) {
        }
    }

    void remove(const T& t) {
        auto p = std::atomic_load(&m_head);
        auto prev = std::atomic_load(&p);

        // Find node to remove
        while (p) {
            if (p->t == t) {
                // Update previous node's next pointer to skip current node
                while (p && !std::atomic_compare_exchange_weak(&prev->next, &p,
                                                               std::atomic_load(
                                                                   &p->next))) {
                }
            }

            // If not at end of list
            if (p) {
                // Update pointer to previous node
                prev = std::atomic_load(&p);

                p = std::atomic_load(&p->next);
            }
        }
    }

    template <class Predicate>
    void remove_if(Predicate pred) {
        auto p = std::atomic_load(&m_head);
        auto prev = std::atomic_load(&p);

        // Find node to remove
        while (p) {
            if (pred(p->t)) {
                // Update previous node's next pointer to skip current node
                while (p &&
                       !std::atomic_compare_exchange_weak(&prev->next, &p,
                                                          std::atomic_load(&p->
                                                                           next)))
                {
                }
            }

            // If not at end of list
            if (p) {
                // Update pointer to previous node
                prev = std::atomic_load(&p);

                p = std::atomic_load(&p->next);
            }
        }
    }

    bool empty() const noexcept {
        return std::atomic_load(&m_head) == nullptr;
    }

    iterator before_begin() noexcept {
        // Make a new node
        auto p = std::make_shared<Node>();

        // Point p's next pointer at head
        p->next = std::atomic_load(&m_head);

        return p;
    }

    iterator begin() noexcept {
        return std::atomic_load(&m_head);
    }

    iterator end() noexcept {
        return {nullptr};
    }

    const_iterator before_begin() const noexcept {
        // Make a new node
        auto p = std::make_shared<Node>();

        // Point p's next pointer at head
        p->next = std::atomic_load(&m_head);

        return p;
    }

    const_iterator begin() const noexcept {
        return std::atomic_load(&m_head);
    }

    const_iterator end() const noexcept {
        return {nullptr};
    }
};

