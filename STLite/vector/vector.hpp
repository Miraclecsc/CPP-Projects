#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

#define INITSIZE 100

namespace sjtu
{
    template<typename T>
    class vector
    {
    private:
        T *arr;
        long long nowsize, maxsize;

        void deletespace()
        {
            for(int i = 0; i < nowsize; ++i)
                arr[i].~T();
            free(arr);
        }
        void doublespace()
        {
            maxsize *= 2;
            T *temp = (T *)malloc(maxsize * sizeof(T));
            for(long long i = 0; i < nowsize; ++i)
            {
                new(temp+i) T(arr[i]);
                arr[i].~T();
            }
            free(arr);
            arr = temp;
        }

    public:
        class const_iterator;
        class iterator
        {
        private:
            vector<T> *ptr;
            int index;

        public:
            iterator() = default;
            iterator(const iterator& other) = default;
            iterator(vector<T> *_ptr, int _index) :ptr(_ptr), index(_index) {}

            iterator operator+(const int &n) const {return iterator(ptr, index+n);}
            iterator operator-(const int &n) const {return iterator(ptr, index-n);}
            int operator-(const iterator &rhs) const
            {
                if(ptr != rhs.ptr)
                    throw invalid_iterator();
                return index - rhs.index;
            }
            iterator& operator+=(const int &n)
            {
                index += n;
                return *this;
            }
            iterator& operator-=(const int &n)
            {
                if(index-n < 0)
                    throw runtime_error();
                index -= n;
                return *this;
            }
            iterator operator++(int)
            {
                iterator temp = *this;
                ++index;
                return temp;
            }
            iterator& operator++()
            {
                ++index;
                return *this;
            }
            iterator operator--(int)
            {
                if(!index)
                    throw runtime_error();
                iterator temp = *this;
                --index;
                return temp;
            }
            iterator& operator--()
            {
                if(!index)
                    throw runtime_error();
                --index;
                return *this;
            }

            T& operator*() const {return ptr->arr[index];}
            bool operator==(const iterator &rhs) const {return ptr == rhs.ptr && index == rhs.index;}
            bool operator==(const const_iterator &rhs) const {return ptr == rhs.ptr && index == rhs.index;}
            bool operator!=(const iterator &rhs) const {return !(*this == rhs);}
            bool operator!=(const const_iterator &rhs) const {return !(*this == rhs);}
        };
        class const_iterator
        {
        private:
            const vector *ptr;
            int index;

        public:
            const_iterator(const vector *p, int idx) :ptr(p),index(idx) {}
            const_iterator operator+(const int &n) const {return const_iterator(ptr, index+n);}
            const_iterator operator-(const int &n) const {return const_iterator(ptr, index-n);}

            int operator-(const iterator &rhs) const
            {
                if(ptr != rhs.ptr)
                    throw invalid_iterator();
                int temp = index - rhs.index;
                return temp >= 0 ? temp : -temp;
            }
            const_iterator& operator+=(const int &n)
            {
                index += n;
                return *this;
            }
            const_iterator& operator-=(const int &n)
            {
                if(index-n < 0)
                    throw runtime_error();
                index -= n;
                return *this;
            }
            const_iterator operator++(int)
            {
                const_iterator temp = *this;
                ++index;
                return temp;
            }
            const_iterator& operator++()
            {
                ++index;
                return *this;
            }
            const_iterator operator--(int)
            {
                if(!index)
                    throw runtime_error();
                const_iterator temp = *this;
                --index;
                return temp;
            }
            const_iterator& operator--()
            {
                if(!index)
                    throw runtime_error();
                --index;
                return *this;
            }

            T& operator*() const {return ptr->arr[index];}
            bool operator==(const iterator &rhs) const {return ptr == rhs.ptr && index == rhs.index;}
            bool operator==(const const_iterator &rhs) const {return ptr == rhs.ptr && index == rhs.index;}
            bool operator!=(const iterator &rhs) const {return !(*this == rhs);}
            bool operator!=(const const_iterator &rhs) const {return !(*this == rhs);}
        };

        vector() :nowsize(0), maxsize(INITSIZE) {arr = (T *)malloc(maxsize * sizeof(T));}
        vector(const vector &other) :nowsize(other.nowsize),maxsize(other.maxsize)
        {
            arr = (T *)malloc(maxsize * sizeof(T));
            for(long long i = 0; i < nowsize; ++i)
                new(arr+i) T(other.arr[i]);
        }
        ~vector() {deletespace();}

        vector &operator=(const vector &other)
        {
            if(this == &other) return *this;
            deletespace();
            nowsize = other.nowsize;
            maxsize = other.maxsize;
            arr = (T *)malloc(maxsize * sizeof(T));
            for(int i = 0; i < nowsize; ++i)
                new(arr+i) T(other.arr[i]);
            return *this;
        }
        T & at(const size_t &pos)
        {
            if(pos < 0 || pos >= nowsize)
                throw index_out_of_bound();
            return arr[pos];
        }
        const T & at(const size_t &pos) const
        {
            if(pos < 0 || pos >= nowsize)
                throw index_out_of_bound();
            return arr[pos];
        }
        T & operator[](const size_t &pos)
        {
            if(pos < 0 || pos >= nowsize)
                throw index_out_of_bound();
            return arr[pos];
        }
        const T & operator[](const size_t &pos) const
        {
            if(pos < 0 || pos >= nowsize)
                throw index_out_of_bound();
            return arr[pos];
        }
        const T & front() const
        {
            if(!nowsize)
                throw container_is_empty();
            return arr[0];
        }
        const T & back() const
        {
            if(!nowsize)
                throw container_is_empty();
            return arr[nowsize-1];
        }

        iterator begin() {return iterator(this, 0);}
        const_iterator cbegin() const {return const_iterator(this, 0);}
        iterator end() {return iterator(this, nowsize);}
        const_iterator cend() const {return const_iterator(this, nowsize);}

        bool empty() const {return !nowsize;}
        size_t size() const {return nowsize;}
        void clear()
        {
            for (int i = 0; i < nowsize; ++i)
                arr[i].~T();
            nowsize = 0;
        }
        iterator insert(iterator pos, const T &value)
        {
            if(nowsize == maxsize)
                doublespace();
            new(arr + nowsize) T(arr[nowsize-1]);
            for(long long i = nowsize-1; i > pos-begin(); --i)
                arr[i] = arr[i-1];
            arr[pos-begin()] = value;
            ++nowsize;
            return pos;
        }
        iterator insert(const size_t &ind, const T &value)
        {
            if(ind > nowsize)
                throw index_out_of_bound();
            if(nowsize == maxsize)
                doublespace();
            new(arr + nowsize) T(arr[nowsize-1]);
            for(long long i = nowsize-1; i > ind; --i)
                arr[i] = arr[i-1];
            arr[ind] = value;
            ++nowsize;
            return iterator(this, ind);
        }
        iterator erase(iterator pos)
        {
            for(long long i = pos-begin(); i < nowsize-1; ++i)
                arr[i] = arr[i+1];
            arr[--nowsize].~T();
            return iterator(this, pos-begin());
        }
        iterator erase(const size_t &ind)
        {
            if(ind >= nowsize)
                throw index_out_of_bound();
            for(long long i = ind; i < nowsize-1; ++i)
                arr[i] = arr[i+1];
            arr[--nowsize].~T();
            return iterator(this, ind);
        }
        void push_back(const T &value)
        {
            if(nowsize == maxsize)
                doublespace();
            new(arr + nowsize++) T(value);
        }
        void pop_back()
        {
            if(!nowsize)
                throw container_is_empty();
            arr[--nowsize].~T();
        }
    };
}

#endif