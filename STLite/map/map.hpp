/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

 // only for std::less<T>
#include <functional>
#include <cstddef>
#include<cmath>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

    template<
        class Key,
        class T,
        class Compare = std::less<Key>
    > class map {
    public:
        typedef pair<const Key, T> value_type;
        struct AVLNode {
            Key* k = NULL;
            T* data = NULL;
            value_type* v = NULL;
            AVLNode* left = NULL;
            AVLNode* right = NULL;
            AVLNode* parent = NULL;
            int height = 0;
            bool flag = true;
            value_type* getvalue_type() {
                if (v != NULL) {
                    return v;
                    //delete v;
                    //v = NULL;
                }
                v = new value_type(*k, *data);
                return v;
            }
        };
        AVLNode* root = NULL;
        size_t total = 0;
    private:
        void dfsconstructor(const AVLNode* other, AVLNode*& t, AVLNode* p) {
            if (other == NULL || other == t || !(other->flag)) {
                return;
            }
            t = new AVLNode;
            t->data = new T(*(other->data));
            t->k = new Key(*(other->k));
            t->parent = p;
            t->height = other->height;
            total++;
            dfsconstructor(other->left, t->left, t);
            dfsconstructor(other->right, t->right, t);
        }
        void dfsdel(AVLNode*& t) {
            if (t == NULL) {
                return;
            }
            if (t->left != NULL) {
                dfsdel(t->left);
            }
            if (t->right != NULL) {
                dfsdel(t->right);
            }
            delete t->k;
            t->k = NULL;
            delete t->data;
            t->data = NULL;
            if (t->v != NULL) {
                delete t->v;
                t->v = NULL;
            }
            delete t;
            t = NULL;
        }
        void nodefind(Key key, AVLNode*& result, AVLNode* tem) const {
            Compare cmp;
            if (tem == NULL || !(tem->flag)) {
                result = NULL;
                return;
            }
            if (!(cmp(*(tem->k), key) || cmp(key, *(tem->k)))) {
                result = tem;
                return;
            }
            if (cmp(*(tem->k), key)) {
                nodefind(key, result, tem->right);
            }
            if (cmp(key, *(tem->k))) {
                nodefind(key, result, tem->left);
            }
        }
        void LL(AVLNode*& t, AVLNode* p) {
            AVLNode* t1 = t->left;
            t->left = t1->right;
            if (t1->right != NULL) {
                t1->right->parent = t;
            }
            t->parent = t1;
            t1->parent = p;
            t1->right = t;
            t->height = max(height(t->left), height(t->right)) + 1;
            t1->height = max(height(t1->left), height(t)) + 1;
            t = t1;
        }
        void RR(AVLNode*& t, AVLNode* p) {
            AVLNode* t1 = t->right;
            t->right = t1->left;
            if (t1->left != NULL) {
                t1->left->parent = t;
            }
            t->parent = t1;
            t1->parent = p;
            t1->left = t;
            t->height = max(height(t->left), height(t->right)) + 1;
            t1->height = max(height(t1->right), height(t)) + 1;
            t = t1;
        }
        void LR(AVLNode*& t, AVLNode* p) {
            RR(t->left, t);
            LL(t, p);
        }
        void RL(AVLNode*& t, AVLNode* p) {
            LL(t->right, t);
            RR(t, p);
        }
        void insert(const value_type& value, AVLNode*& t, AVLNode* p, bool& flag, AVLNode*& result) {
            Compare cmp;
            if (t == NULL || !(t->flag)) {
                if (t == NULL) {
                    t = new AVLNode;
                }
                t->k = new Key(value.first);
                t->data = new T(value.second);
                t->parent = p;
                t->flag = true;
                result = t;
            }
            else {
                if (cmp(value.first, *(t->k))) {
                    insert(value, t->left, t, flag, result);
                    if (height(t->left) - height(t->right) == 2) {
                        if (cmp(value.first, *(t->left->k))) {
                            LL(t, p);
                        }
                        else {
                            LR(t, p);
                        }
                    }
                }
                else {
                    if (cmp(*(t->k), value.first)) {
                        insert(value, t->right, t, flag, result);
                        if (height(t->right) - height(t->left) == 2) {
                            if (cmp(*(t->right->k), value.first)) {
                                RR(t, p);
                            }
                            else {
                                RL(t, p);
                            }
                        }
                    }
                    else {
                        result = t;
                        flag = false;
                    }
                }
            }
            t->height = max(height(t->left), height(t->right)) + 1;
        }
        bool adjust(AVLNode*& t, int s, AVLNode* p) {
            if (s) {
                if (height(t->left) - height(t->right) == 1) {
                    return true;
                }
                if (height(t->right) == height(t->left)) {
                    --t->height;
                    return false;
                }
                if (height(t->left->right) > height(t->left->left)) {
                    LR(t, p);
                    return false;
                }
                LL(t, p);
                if (height(t->right) == height(t->left)) {
                    return false;
                }
                else {
                    return true;
                }
            }
            else {
                if (height(t->right) - height(t->left) == 1) {
                    return true;
                }
                if (height(t->right) == height(t->left)) {
                    --t->height;
                    return false;
                }
                if (height(t->right->left) > height(t->right->right)) {
                    RL(t, p);
                    return false;
                }
                RR(t, p);
                if (height(t->right) == height(t->left)) {
                    return false;
                }
                else {
                    return true;
                }
            }
        }
        bool remove(const Key& x, AVLNode*& t, AVLNode* p) {
            if (t == NULL || !(t->flag)) {
                return true;
            }
            Compare cmp;
            if (!(cmp(x, *(t->k)) || cmp(*(t->k), x))) {
                if (t->left == NULL || t->right == NULL || !(t->right->flag)) {
                    AVLNode* oldNode = t;
                    if (t->left != NULL) {
                        t->left->parent = t->parent;
                        t->left->right = t->right;
                        t = t->left;
                    }
                    else {
                        if (t->right != NULL) {
                            t->right->parent = t->parent;
                        }
                        t = t->right;
                    }
                    delete oldNode->k;
                    oldNode->k = NULL;
                    delete oldNode->data;
                    oldNode->data = NULL;
                    if (oldNode->v != NULL) {
                        delete oldNode->v;
                        oldNode->v = NULL;
                    }
                    delete oldNode;
                    oldNode = NULL;
                    return false;
                }
                else {
                    AVLNode* tmp = t->right;
                    while (tmp->left != NULL) {
                        tmp = tmp->left;
                    }
                    if (tmp == t->right) {
                        t->left->parent = tmp;
                        tmp->left = t->left;
                        t->left = NULL;
                        t->right = tmp->right;
                        if (tmp->right != NULL) {
                            tmp->right->parent = t;
                        }
                        tmp->parent = t->parent;
                        tmp->right = t;
                        t->parent = tmp;
                        t = tmp;
                        tmp = t->right;
                        int tmpheight = t->height;
                        t->height = tmp->height;
                        tmp->height = tmpheight;
                    }
                    else {
                        t->left->parent = tmp;
                        tmp->left = t->left;
                        t->left = NULL;
                        tmp->parent->left = t;
                        AVLNode* tmpp = t->parent;
                        t->parent = tmp->parent;
                        tmp->parent = tmpp;
                        t->right->parent = tmp;
                        if (tmp->right != NULL) {
                            tmp->right->parent = t;
                        }
                        tmpp = t->right;
                        t->right = tmp->right;
                        tmp->right = tmpp;
                        tmpp = tmp;
                        tmp = t;
                        t = tmpp;
                        int tmpheight = t->height;
                        t->height = tmp->height;
                        tmp->height = tmpheight;
                    }
                    if (remove(*(tmp->k), t->right, t)) {
                        return true;
                    }
                    return adjust(t, 1, p);
                }
            }
            if (cmp(x, *(t->k))) {
                if (remove(x, t->left, t)) {
                    return true;
                }
                return adjust(t, 0, p);
            }
            else {
                if (remove(x, t->right, t)) {
                    return true;
                }
                return adjust(t, 1, p);
            }
        }
    public:

        size_t height(AVLNode* t)const {
            return t == NULL ? 0 : t->height;
        }
        size_t max(size_t a, size_t b) {
            return a > b ? a : b;
        }
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
         /**
          * see BidirectionalIterator at CppReference for help.
          *
          * if there is anything wrong throw invalid_iterator.
          *     like it = map.begin(); --it;
          *       or it = map.end(); ++end();
          */
        class const_iterator;
        class iterator {
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            map* it = NULL;
            AVLNode* node = NULL;
            void getnext() {
                AVLNode* tmp = NULL;
                if (this->node->right != NULL) {
                    tmp = this->node->right;
                    while (tmp->left != NULL) {
                        tmp = tmp->left;
                    }
                    this->node = tmp;
                    return;
                }
                if (this->node->parent != NULL && this->node->parent->left == this->node) {
                    this->node = this->node->parent;
                    return;
                }
                if (this->node->parent != NULL && this->node->parent->right == this->node) {
                    tmp = this->node->parent;
                    while (tmp != NULL) {
                        if (tmp->parent == NULL) {
                            this->node = NULL;
                            return;
                        }
                        if (tmp->parent->left == tmp) {
                            this->node = tmp->parent;
                            return;
                        }
                        tmp = tmp->parent;
                    }
                    return;
                }
            }
            void getpast() {
                AVLNode* tmp = NULL;
                if (this->node->left != NULL) {
                    tmp = this->node->left;
                    while (tmp->right != NULL) {
                        tmp = tmp->right;
                    }
                    this->node = tmp;
                    return;
                }
                if (this->node->parent != NULL && this->node->parent->right == this->node) {
                    this->node = this->node->parent;
                    return;
                }
                if (this->node->parent != NULL && this->node->parent->left == this->node) {
                    tmp = this->node->parent;
                    while (tmp != NULL) {
                        if (tmp->parent == NULL) {
                            this->node = NULL;
                            return;
                        }
                        if (tmp->parent->right == tmp) {
                            this->node = tmp->parent;
                            return;
                        }
                        tmp = tmp->parent;
                    }
                    return;
                }
            }
        public:
            AVLNode* getnode()const {
                return node;
            }
            map* getmap()const {
                return it;
            }
            iterator(map* a = NULL, AVLNode* b = NULL) {
                this->it = a;
                this->node = b;
                // TODO
            }
            iterator(const iterator& other) {
                this->it = other.getmap();
                this->node = other.getnode();
                // TODO
            }
            iterator& operator=(const iterator& other) {
                this->it = other.getmap();
                this->node = other.getnode();
                return *this;
            }
            /**
             * return a new iterator which pointer n-next elements
             *   even if there are not enough elements, just return the answer.
             * as well as operator-
             */
             /**
              * TODO iter++
              */
            iterator operator++(int) {
                iterator t(*this);
                if (*this == this->it->end()) {
                    throw invalid_iterator();
                }
                else {
                    getnext();
                }
                return t;
            }
            /**
             * TODO ++iter
             */
            iterator& operator++() {
                if (*this == this->it->end()) {
                    throw invalid_iterator();
                }
                else {
                    getnext();
                }
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator t(*this);
                if (*this == this->it->begin()) {
                    throw invalid_iterator();
                }
                else {
                    getpast();
                }
                return t;
            }
            /**
             * TODO --iter
             */
            iterator& operator--() {
                if (*this == this->it->begin()) {
                    throw invalid_iterator();
                }
                else {
                    getpast();
                }
                return *this;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type& operator*() const {
                return *node->getvalue_type();
            }
            bool operator==(const iterator& rhs) const {
                return ((this->it == rhs.getmap()) && (this->node == rhs.getnode()));
            }
            bool operator==(const const_iterator& rhs) const {
                return ((this->it == rhs.getmap()) && (this->node == rhs.getnode()));
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator& rhs) const {
                return !((this->it == rhs.getmap()) && (this->node == rhs.getnode()));
            }
            bool operator!=(const const_iterator& rhs) const {
                return !((this->it == rhs.getmap()) && (this->node == rhs.getnode()));
            }
            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {
                return node->getvalue_type();
            }
        };
        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            // data members.
            const map* it = NULL;
            AVLNode* node = NULL;
            void getnext() {
                AVLNode* tmp = NULL;
                if (this->node->right != NULL) {
                    tmp = this->node->right;
                    while (tmp->left != NULL) {
                        tmp = tmp->left;
                    }
                    this->node = tmp;
                    return;
                }
                if (this->node->parent != NULL && this->node->parent->left == this->node) {
                    this->node = this->node->parent;
                    return;
                }
                if (this->node->parent != NULL && this->node->parent->right == this->node) {
                    tmp = this->node->parent;
                    while (tmp != NULL) {
                        if (tmp->parent == NULL) {
                            this->node = NULL;
                            return;
                        }
                        if (tmp->parent->left == tmp) {
                            this->node = tmp->parent;
                            return;
                        }
                        tmp = tmp->parent;
                    }
                    return;
                }
            }
            void getpast() {
                AVLNode* tmp = NULL;
                if (this->node->left != NULL) {
                    tmp = this->node->left;
                    while (tmp->right != NULL) {
                        tmp = tmp->right;
                    }
                    this->node = tmp;
                    return;
                }
                if (this->node->parent != NULL && this->node->parent->right == this->node) {
                    this->node = this->node->parent;
                    return;
                }
                if (this->node->parent != NULL && this->node->parent->left == this->node) {
                    tmp = this->node->parent;
                    while (tmp != NULL) {
                        if (tmp->parent == NULL) {
                            this->node = NULL;
                            return;
                        }
                        if (tmp->parent->right == tmp) {
                            this->node = tmp->parent;
                            return;
                        }
                        tmp = tmp->parent;
                    }
                    return;
                }
            }
        public:
            AVLNode* getnode() const {
                return node;
            }
            const map* getmap() const {
                return it;
            }
            const_iterator(const map* a = NULL, AVLNode* b = NULL) {
                // TODO
                this->it = a;
                this->node = b;
            }
            const_iterator(const const_iterator& other) {
                // TODO
                this->it = other.getmap();
                this->node = other.getnode();
            }
            const_iterator(const iterator& other) {
                // TODO
                this->it = other.getmap();
                this->node = other.getnode();
            }
            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
            const_iterator operator++(int) {
                const_iterator t(*this);
                if (*this == this->it->cend()) {
                    throw invalid_iterator();
                }
                else {
                    getnext();
                }
                return t;
            }
            /**
             * TODO ++iter
             */
            const_iterator& operator++() {
                if (*this == this->it->cend()) {
                    throw invalid_iterator();
                }
                else {
                    getnext();
                }
                return *this;
            }
            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator t(*this);
                if (*this == this->it->cbegin()) {
                    throw invalid_iterator();
                }
                else {
                    getpast();
                }
                return t;
            }
            /**
             * TODO --iter
             */
            const_iterator& operator--() {
                if (*this == this->it->cbegin()) {
                    throw invalid_iterator();
                }
                else {
                    getpast();
                }
                return *this;
            }
            value_type& operator*() const {
                return *node->getvalue_type();
            }
            const_iterator& operator=(const const_iterator& other) {
                this->it = other.getmap();
                this->node = other.getnode();
                return *this;
            }
            bool operator==(const iterator& rhs) const {
                return ((this->it == rhs.getmap()) && (this->node == rhs.getnode()));
            }
            bool operator==(const const_iterator& rhs) const {
                return ((this->it == rhs.getmap()) && (this->node == rhs.getnode()));
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator& rhs) const {
                return!((this->it == rhs.getmap()) && (this->node == rhs.getnode()));
            }
            bool operator!=(const const_iterator& rhs) const {
                return!((this->it == rhs.getmap()) && (this->node == rhs.getnode()));
            }
            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {
                return node->getvalue_type();
            }
        };
        /**
         * TODO two constructors
         */
        map() {}
        map(const map& other) {
            dfsconstructor(other.root, root, NULL);
        }
        /**
         * TODO assignment operator
         */
        map& operator=(const map& other) {
            dfsconstructor(other.root, root, NULL);
            return *this;
        }
        /**
         * TODO Destructors
         */
        ~map() {
            dfsdel(root);
        }
        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T& at(const Key& key) {
            AVLNode* t = root;
            Compare cmp;
            while (t != NULL && (cmp(*(t->k), key) || cmp(key, *(t->k)))) {
                if (cmp(key, *(t->k))) {
                    t = t->left;
                }
                else {
                    t = t->right;
                }
            }
            if (t == NULL) {
                throw index_out_of_bound();
            }
            else {
                return *t->data;
            }
        }
        const T& at(const Key& key) const {
            AVLNode* t = root;
            Compare cmp;
            while (t != NULL && (cmp(*(t->k), key) || cmp(key, *(t->k)))) {
                if (cmp(key, *(t->k))) {
                    t = t->left;
                }
                else {
                    t = t->right;
                }
            }
            if (t == NULL) {
                throw index_out_of_bound();
            }
            else {
                return *t->data;
            }
        }
        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T& operator[](const Key& key) {
            AVLNode* t = root;
            Compare cmp;
            while (t != NULL && t->flag && (cmp(*(t->k), key) || cmp(key, *(t->k)))) {
                if (cmp(key, *(t->k))) {
                    t = t->left;
                }
                else {
                    t = t->right;
                }
            }
            if (t == NULL || !(t->flag)) {
                T m;
                value_type a(key, m);
                return *(insert(a).first.getnode()->data);
            }
            else {
                return *t->data;
            }
        }
        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T& operator[](const Key& key) const {
            return at(key);
        }
        /**
         * return a iterator to the beginning
         */
        iterator begin() {
            AVLNode* tem = root;
            if (tem != NULL) {
                while (tem->left != NULL) {
                    tem = tem->left;
                }
            }
            iterator bgn(this, tem);
            return bgn;
        }
        const_iterator cbegin() const {
            AVLNode* tem = root;
            if (tem != NULL) {
                while (tem->left != NULL) {
                    tem = tem->left;
                }
            }
            const_iterator bgn(this, tem);
            return bgn;
        }
        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {
            AVLNode* tem = root;
            if (tem != NULL && tem->flag) {
                while (tem->right != NULL && tem->right->flag) {
                    tem = tem->right;
                }
                if (tem->right == NULL) {
                    tem->right = new AVLNode;
                    tem->right->parent = tem;
                    tem->right->flag = false;
                }
                tem = tem->right;
            }
            iterator ed(this, tem);
            return ed;
        }
        const_iterator cend() const {
            AVLNode* tem = root;
            if (tem != NULL && tem->flag) {
                while (tem->right != NULL && tem->right->flag) {
                    tem = tem->right;
                }
                if (tem->right == NULL) {
                    tem->right = new AVLNode;
                    tem->right->parent = tem;
                    tem->right->flag = false;
                }
                tem = tem->right;
            }
            const_iterator ed(this, tem);
            return ed;
        }
        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {
            return total == 0;
        }
        /**
         * returns the number of elements.
         */
        size_t size() const {
            return total;
        }
        /**
         * clears the contents
         */
        void clear() {
            dfsdel(root);
            total = 0;
        }
        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type& value) {
            bool flag = true;
            AVLNode* result = NULL;
            insert(value, root, NULL, flag, result);
            if (flag) {
                total++;
            }
            iterator tem(this, result);
            return pair<iterator, bool>(tem, flag);
        }
        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            if (pos == this->end() || pos.getnode() == NULL || !pos.getnode()->flag || pos.getmap() != this) {
                throw invalid_iterator();
                return;
            }
            if (pos.getnode() == NULL || !(pos.getnode()->flag)) {
                return;
            }
            remove(pos->first, root, NULL);
            total--;
        }
        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key& key) const {
            AVLNode* result = NULL;
            nodefind(key, result, root);
            if (result == NULL) {
                return 0;
            }
            else {
                return 1;
            }
        }
        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key& key) {
            AVLNode* result = NULL;
            nodefind(key, result, root);
            if (result == NULL) {
                return end();
            }
            iterator fd(this, result);
            return fd;
        }
        const_iterator find(const Key& key) const {
            AVLNode* result = NULL;
            nodefind(key, result, root);
            if (result == NULL) {
                return cend();
            }
            const_iterator fd(this, result);
            return fd;
        }
    };
}

#endif