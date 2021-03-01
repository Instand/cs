#ifndef CS_LIST_HPP
#define CS_LIST_HPP

#include <cstdlib>
#include <iterator>
#include <type_traits>
#include <utility>

namespace cs {
// classic linked list with fictive head and tail links
template<typename T>
class List {
    struct Node;
    class Iterator;
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator;
    using const_iterator = const Iterator;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    size_t size() const;
    bool isEmpty() const;

    List();
    ~List();

    void clear();
    void pushBack(const T& element);
    void pushFront(const T& element);

    void insert(iterator iterator, const T& value);

    void popFront();
    void popBack();

    void erase(iterator iterator);
    void reverse();

private:
    Node* head_ = new Node();
    Node* tail_ = new Node();
    size_t size_ = 0;

    void pushFront(Node* node);
    void pushBack(Node* node);
    void firstAdd(Node* node);

    void defaultState();

    struct Node {
        Node* next = nullptr;
        Node* previous = nullptr;
        T data;
    };

    class Iterator : public std::iterator<std::bidirectional_iterator_tag, std::remove_cv<T>> {  // deprecated in C++17
    public:
        Iterator() = default;
        explicit Iterator(Node* node);

        T& operator*();

        bool operator==(const Iterator& iterator) const;
        bool operator!=(const Iterator& iterator) const;

        Iterator& operator++();
        Iterator& operator--();

    private:
        mutable Node* node_ = nullptr;

        template<typename K>
        friend class List;
    };
};

template <typename T>
typename List<T>::iterator List<T>::begin() {
    return Iterator(head_->next);
}

template <typename T>
typename List<T>::iterator List<T>::end() {
    return Iterator(tail_);
}

template <typename T>
typename List<T>::const_iterator List<T>::begin() const {
    return (const_cast<List*>(this))->begin();
}

template <typename T>
typename List<T>::const_iterator List<T>::end() const {
    return (const_cast<List*>(this))->end();
}

template <typename T>
size_t List<T>::size() const {
    return size_;
}

template <typename T>
bool List<T>::isEmpty() const {
    return size() == 0;
}

template <typename T>
List<T>::List() {
    defaultState();
}

template <typename T>
List<T>::~List() {
    clear();

    delete head_;
    delete tail_;
}

template <typename T>
void List<T>::clear() {
    auto head = head_->next;
    while (head->next && head != tail_) {
        auto next = head->next;
        delete head;
        head = next;
    }

    defaultState();
    size_ = 0;
}

template <typename T>
void List<T>::pushBack(const T& element) {
    Node* node = new Node();
    node->data = element;

    pushBack(node);
}

template <typename T>
void List<T>::pushFront(const T& element) {
    Node* node = new Node();
    node->data = element;

    pushFront(node);
}

template <typename T>
void List<T>::insert(iterator iterator, const T& value) {
    Node* node = new Node();
    node->data = value;

    if (size_ != 0) {
        if (iterator == begin()) {
            pushFront(node);
            return;
        }
        else if (iterator == end()) {
            pushBack(node);
            return;
        }

        Node* currentNode = iterator.node_;

        if (head_->next == currentNode) {
            head_->next = currentNode;
        }
        else if (tail_->previous == currentNode) {
            tail_->previous = currentNode;
        }

        node->next = currentNode;
        node->previous = currentNode->previous;

        currentNode->previous = node;
    }
    else {
        firstAdd(node);
    }

    size_++;
}

template <typename T>
void List<T>::popFront() {
    if (size() == 0 ) {
        return;
    }
    else if (size() == 1) {
        delete head_->next;
        defaultState();
    }
    else {
        auto next = head_->next->next;
        delete head_->next;

        head_->next = next;
        next->previous = head_;
    }

    size_--;
}

template <typename T>
void List<T>::popBack() {
    if (size() == 0) {
        return;
    }
    else if (size() == 1) {
        delete tail_->previous;
        defaultState();
    }
    else {
        auto previous = tail_->previous->previous;
        delete tail_->previous;
        tail_->previous = previous;
        previous->next = tail_;
    }

    size_--;
}

template <typename T>
void List<T>::erase(iterator iterator) {
    if (size_ == 0) {
        return;
    }

    if (iterator == begin()) {
        popFront();
        return;
    }

    if (iterator == (--end())) {
        popBack();
        return;
    }

    Node* node = iterator.node_;

    if (head_->next == tail_->previous) {
        defaultState();
    }
    else {
        auto next = node->next;
        auto previous = node->previous;

        previous->next = next;
        next->previous = previous;
    }

    delete node;
    --size_;
}

template <typename T>
void List<T>::reverse() {
    if (size() < 2) {
        return;
    }

    auto next = tail_->previous;
    while (next != head_) {
        std::swap(next->previous, next->next);
        next = next->next;
    }

    std::swap(head_, tail_);

    head_->next = head_->previous;
    head_->previous = nullptr;

    tail_->previous = tail_->next;
    tail_->next = nullptr;
}

template <typename T>
void List<T>::pushFront(Node* node) {
    if (size_ != 0) {
        node->next = head_->next;
        node->previous = head_;

        head_->next->previous = node;
        head_->next = node;
    }
    else {
        firstAdd(node);
    }

    size_++;
}

template <typename T>
void List<T>::pushBack(Node* node) {
    if (size_ != 0) {
        node->next = tail_;
        node->previous = tail_->previous;

        tail_->previous->next = node;
        tail_->previous = node;
    }
    else {
        firstAdd(node);
    }

    size_++;
}

template <typename T>
void List<T>::firstAdd(Node* node) {
    node->next = tail_;
    node->previous = tail_->previous;

    tail_->previous->next = node;
    tail_->previous = node;
}

template <typename T>
void List<T>::defaultState() {
    head_->next = tail_;
    tail_->previous = head_;
}

// iterator
template <typename T>
List<T>::Iterator::Iterator(Node* node): node_(node) {}

template <typename T>
T& List<T>::Iterator::operator*() {
    return node_->data;
}

template <typename T>
bool List<T>::Iterator::operator==(const Iterator& iterator) const {
    return iterator.node_ == node_;
}

template <typename T>
bool List<T>::Iterator::operator!=(const Iterator& iterator) const {
    return !((*this) == iterator);
}

template <typename T>
typename List<T>::Iterator& List<T>::Iterator::operator++() {
    if (node_) {
        node_ = node_->next;
    }

    return *this;
}

template <typename T>
typename List<T>::Iterator& List<T>::Iterator::operator--() {
    if (node_) {
        node_ = node_->previous;
    }

    return *this;
}

}

#endif // CS_LIST_HPP
