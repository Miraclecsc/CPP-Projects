#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node;
    typedef node* pNode;

    class node {
    public:
        T* pVal;
        pNode prev, next;

        node() : pVal(nullptr), prev(nullptr), next(nullptr) { }
        node(const T& _val, pNode _prev = nullptr, pNode _next = nullptr) :
                pVal(new T(_val)), prev(_prev), next(_next) { }
        ~node() {
            if (pVal) delete pVal;
        }
    };

protected:
    /**
     * add data members for linked list as protected members
     */
    pNode head, tail;
    size_t m_size;
    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        m_size++;
        return cur;
    }
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        if (!m_size) throw container_is_empty();
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        m_size--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        const list* fa;
        pNode pointer;
    public:
        explicit iterator(const list* _fa = nullptr, pNode _pt = nullptr) :
                fa(_fa), pointer(_pt) { }
        explicit iterator(const const_iterator& other) : fa(other.getfa()), pointer(other.get()) { }
        iterator(const iterator& other) = default;
        ~iterator() = default;

        pNode get() const { return pointer; }
        const list* getfa() const { return fa; }
        /**
         * iter++
         */
        iterator operator++(int) {
            if (!pointer->next) throw invalid_iterator();
            iterator ret = *this;
            pointer = pointer->next;
            return ret;
        }
        /**
         * ++iter
         */
        iterator & operator++() {
            if (!pointer->next) throw invalid_iterator();
            pointer = pointer->next;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            if (!pointer->prev->prev) throw invalid_iterator();
            iterator ret = *this;
            pointer = pointer->prev;
            return ret;
        }
        /**
         * --iter
         */
        iterator & operator--() {
            if (!pointer->prev->prev) throw invalid_iterator();
            pointer = pointer->prev;
            return *this;
        }
        /**
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (!pointer->prev || !pointer->next) throw invalid_iterator();
            return *pointer->pVal;
        }
        /**
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (!pointer->prev || !pointer->next) throw invalid_iterator();
            return pointer->pVal;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return pointer == rhs.pointer;
        }
        bool operator==(const const_iterator &rhs) const {
            return pointer == rhs.get();
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return pointer != rhs.pointer;
        }
        bool operator!=(const const_iterator &rhs) const {
            return pointer != rhs.get();
        }
    };
    /**
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        const list* fa;
        const node* pointer;
    public:
        explicit const_iterator(const list* _fa = nullptr, pNode _pt = nullptr) :
                fa(_fa), pointer(_pt) { }
        explicit const_iterator(const iterator& other) : fa(other.getfa()), pointer(other.get()) { }
        const_iterator(const const_iterator& other) = default;
        ~const_iterator() = default;

        const node* get() const { return pointer; }
        const list* getfa() const { return fa; }
        /**
         * iter++
         */
        const_iterator operator++(int) {
            if (!pointer->next) throw invalid_iterator();
            const_iterator ret = *this;
            pointer = pointer->next;
            return ret;
        }
        /**
         * ++iter
         */
        const_iterator & operator++() {
            if (!pointer->next) throw invalid_iterator();
            pointer = pointer->next;
            return *this;
        }
        /**
         * iter--
         */
        const_iterator operator--(int) {
            if (!pointer->prev->prev) throw invalid_iterator();
            const_iterator ret = *this;
            pointer = pointer->prev;
            return ret;
        }
        /**
         * --iter
         */
        const_iterator & operator--() {
            if (!pointer->prev->prev) throw invalid_iterator();
            pointer = pointer->prev;
            return *this;
        }
        /**
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (!pointer->prev || !pointer->next) throw invalid_iterator();
            return *pointer->pVal;
        }
        /**
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (!pointer->prev || !pointer->next) throw invalid_iterator();
            return pointer->pVal;
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return pointer == rhs.get();
        }
        bool operator==(const const_iterator &rhs) const {
            return pointer == rhs.pointer;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return pointer != rhs.get();
        }
        bool operator!=(const const_iterator &rhs) const {
            return pointer != rhs.pointer;
        }
    };
    /**
     * Atleast two: default constructor, copy constructor
     */
    list() : head(new node), tail(new node), m_size(0) {
        head->next = tail;
        tail->prev = head;
    }
    list(const list &other) : head(new node), tail(new node), m_size(0) {
        head->next = tail;
        tail->prev = head;
        for (pNode q = other.head->next; q != other.tail; q = q->next) {
            push_back(*q->pVal);
        }
    }
    virtual ~list() {
        clear();
        delete head;
        delete tail;
    }
    list &operator=(const list &other) {
        if (&other != this) {
            clear();
            for (pNode q = other.head->next; q != other.tail; q = q->next) {
                push_back(*q->pVal);
            }
        }
        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (!m_size) throw container_is_empty();
        return *head->next->pVal;
    }
    const T & back() const {
        if (!m_size) throw container_is_empty();
        return *tail->prev->pVal;
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(this, head->next);
    }
    const_iterator cbegin() const {
        return const_iterator(this, head->next);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(this, tail);
    }
    const_iterator cend() const {
        return const_iterator(this, tail);
    }
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const { return !m_size; }
    /**
     * returns the number of elements
     */
    virtual size_t size() const { return m_size; }

    /**
     * clears the contents
     */
    virtual void clear() {
        int rec = m_size;
        for (int i = 0; i < rec; i++) {
            pop_back();
        }
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        pNode tmp = pos.get();
        if (!tmp->prev || pos.getfa() != this) throw invalid_iterator();
        pNode p = new node(value, tmp->prev, tmp);
        tmp->prev->next = p;
        tmp->prev = p;
        m_size++;
        return iterator(this, p);
    }
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (!m_size) throw container_is_empty();
        if (pos.get() == tail || pos.getfa() != this) throw invalid_iterator();
        pNode p = pos.get();
        p->prev->next = p->next;
        p->next->prev = p->prev;
        iterator ret = iterator(this, p->next);
        delete p;
        m_size--;
        return ret;
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        pNode p = new node(value, tail->prev, tail);
        tail->prev->next = p;
        tail->prev = p;
        m_size++;
    }
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (!m_size) throw container_is_empty();
        pNode p = tail->prev;
        p->prev->next = tail;
        tail->prev = p->prev;
        delete p;
        m_size--;
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        pNode p = new node(value, head, head->next);
        head->next->prev = p;
        head->next = p;
        m_size++;
    }
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (!m_size) throw container_is_empty();
        pNode p = head->next;
        p->next->prev = head;
        head->next = p->next;
        delete p;
        m_size--;
    }
    /**
     * sort the values in ascending order with operator
     * < of T
     */
    void sort() {
        auto cmp = [](const T& a, const T& b)->bool { return a < b; };
        T* tmp = static_cast<T*>(operator new[](m_size * sizeof(T)));
        pNode p = head->next;
        for (int i = 0; i < m_size; p = p->next, i++) {
            new(tmp + i) T(*p->pVal);
            p->pVal->~T();
        }
        sjtu::sort<T>(tmp, tmp + m_size, cmp);
        p = head->next;
        for (int i = 0; i < m_size; p = p->next, i++) {
            new(p->pVal) T(tmp[i]);
        }
        for (int i = 0; i < m_size; i++) tmp[i].~T();
        operator delete[](tmp);
    }
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        for (pNode p = head->next, q = other.head->next; q != other.tail;) {
            if (p == tail || *q->pVal < *p->pVal) {
                pNode tmp = other.erase(q);
                q = tmp->next;
                insert(p, tmp);
            } else {
                p = p->next;
            }
        }
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        for (pNode p = head; p; p = p->prev) {
            pNode tmp = p->next;
            p->next = p->prev;
            p->prev = tmp;
        }
        std::swap(head, tail);
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (!m_size) return;
        T* last = head->next->pVal;
        for (pNode p = head->next->next; p != tail;) {
            if (*p->pVal == *last) {
                pNode tmp = erase(p);
                p = p->next;
                delete tmp;
            } else {
                last = p->pVal;
                p = p->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP