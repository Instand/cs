#ifndef CS_FORWARD_LIST_HPP
#define CS_FORWARD_LIST_HPP

#include <iterator>

namespace cs {
template <typename T>
class ForwardList {
    class Iterator;
    using Pointer = ForwardList*;

public:
    using value_type = T;
    using iterator = Iterator;
    using const_iterator = const iterator;
    using reference = T&;
    using const_reference = const T&;

    ~ForwardList();

    void clear();

    void pushFront(const T& value);
    void popFront();

    void reverse();

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    size_t size() const;
    bool isEmpty() const;

    reference front();
    const_reference front() const;

private:
    void defaultState();

    struct Node {
        Node* next = nullptr;
        T data;
    };

    class Iterator : public std::iterator<std::forward_iterator_tag, std::remove_cv_t<T>> {
    public:
        Iterator() = default;
        Iterator(Node* node);

        reference operator*();
        const_reference operator*() const;

        Iterator& operator++();

        bool operator==(const Iterator& iter) const;
        bool operator!=(const Iterator& iter) const;

    private:
        Node* node_ = nullptr;
    };

    Node* head_ = nullptr;
    size_t size_ = 0;
};

template <typename T>
ForwardList<T>::~ForwardList() {
    clear();
}

template <typename T>
void ForwardList<T>::clear() {
    Node* current = head_;

    while (current != nullptr) {
        Node* next = current->next;
        delete current;

        current = next;
    }

    defaultState();
}

template <typename T>
void ForwardList<T>::pushFront(const T& value) {
    Node* node = new Node { head_, value };

    head_ = node;
    ++size_;
}

template <typename T>
void ForwardList<T>::popFront() {
    if (isEmpty()) {
        return;
    }

    auto temp = head_;
    head_ = temp->next;

    delete temp;
    --size_;
}

template <typename T>
void ForwardList<T>::reverse() {
    if (size() < 2) {
        return;
    }

    auto current = head_->next;
    head_->next = nullptr;

    while (current != nullptr) {
        auto next = current->next;

        current->next = head_;
        head_ = current;

        current = next;
    }
}

template <typename T>
typename ForwardList<T>::iterator ForwardList<T>::begin() {
    return iterator(head_);
}

template <typename T>
const typename ForwardList<T>::Iterator ForwardList<T>::begin() const {
    return (const_cast<Pointer>(this))->begin();
}

template <typename T>
typename ForwardList<T>::iterator ForwardList<T>::end() {
    return iterator(nullptr);
}

template <typename T>
const typename ForwardList<T>::Iterator ForwardList<T>::end() const {
    return (const_cast<Pointer>(this))->end();
}

template <typename T>
size_t ForwardList<T>::size() const {
    return size_;
}

template <typename T>
bool ForwardList<T>::isEmpty() const {
    return size() == 0;
}

template <typename T>
typename ForwardList<T>::reference ForwardList<T>::front() {
    return head_->data;
}

template <typename T>
typename ForwardList<T>::const_reference ForwardList<T>::front() const {
    return (const_cast<Pointer>(this))->front();
}

template <typename T>
void ForwardList<T>::defaultState() {
    head_ = nullptr;
    size_ = 0;
}

// iterator
template <typename T>
ForwardList<T>::Iterator::Iterator(Node* node):node_(node) {}

template <typename T>
typename ForwardList<T>::reference ForwardList<T>::Iterator::operator*() {
    return node_->data;
}

template <typename T>
typename ForwardList<T>::const_reference ForwardList<T>::Iterator::operator*() const {
    return node_->data;
}

template <typename T>
typename ForwardList<T>::Iterator& ForwardList<T>::Iterator::operator++() {
    if (node_) {
        node_ = node_->next;
    }

    return *this;
}

template <typename T>
bool ForwardList<T>::Iterator::operator==(const Iterator& iter) const {
    return iter.node_ == node_;
}

template <typename T>
bool ForwardList<T>::Iterator::operator!=(const Iterator& iter) const {
    return !((*this) == iter);
}
}

#endif // CS_FORWARD_LIST_HPP
