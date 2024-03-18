#ifndef SJTU_LINKED_HASHMAP_HPP_STD
#define SJTU_LINKED_HASHMAP_HPP_STD

#include <cstddef>
#include <functional>
#include "utility.hpp"
#include "algorithm.hpp"
#include "exceptions.hpp"
#include "list.hpp"

const double LoadFactor = 0.75;
const size_t InitSize = 16;
namespace sjtu {
	/**
	 *  Maintains key-value pairs just like MAP
	 *  Dynamically sized hash table who handles collision with linked lists
	 *  Iterators arrange in order of insertion (maintained by base class LIST)
	 */

template <
        class Key,
        class Value,
        class Hash = std::hash<Key>,
        class Equal = std::equal_to<Key>
>
class linked_hashmap : public list<pair<const Key, Value> > {
public:
    using value_type = pair<const Key, Value>;

private:
    class Node : public list<value_type>::node {
    public:
        /**
         * add data members in addition to class node in LIST
         */
        Node *after;
        Node():list<value_type>::node(),after(nullptr){}
        explicit Node(const value_type &_val,Node *_after=nullptr):list<value_type>::node(_val),after(_after){}
        ~Node()=default;
    };

    /**
     * singly-linked list used for hash collision
     */
    class BucketList {
    public:
        /**
         * data members, constructors and destructor
         */
        Node *_head;
        BucketList():_head(new Node){}
        ~BucketList()
        {
            delete _head;
        }
        /**
         *  TODO find corresponding Node with key o
         */
        Node * find(const Key &o) {
            Node *p=_head->after;
            while(p!=nullptr)
            {
                if(Equal()(p->val->first,o))
                    break;
                p=p->after;
            }
            return p;
        }
        /**
         * TODO insert key-value pair(k, v) into this BucketList
         * return this new Node
         */
        Node * insert(const Key &k, const Value &v) {
            Node *tmp=new Node(value_type(k,v),_head->after);
            _head->after=tmp;
            return tmp;
        }
        Node * insert(const value_type &kv) {
            Node *tmp=new Node(kv,_head->after);
            _head->after=tmp;
            return tmp;
        }
        /**
         * TODO remove the Node with key k from this BucketList (no need to delete)
         * return the removed Node
         */
        Node * erase(const Key &k) {
            Node *p=_head;
            Node *q;
            while(p!=nullptr)
            {
                q=p->after;
                if(Equal()(q->val->first,k))
                {
                    p->after=q->after;
                    return q;
                }
                p=q;
            }
            return nullptr;
        }
    };
    BucketList *table;
    size_t Capacity;
    void resize(size_t _Capacity)
    {
        Capacity=_Capacity;
        delete[] table;
        table=new BucketList[Capacity];
        for(typename list<value_type>::node* p=list<value_type>::head->nxt;p!=list<value_type>::nil;p=p->nxt)
        {
            size_t _hash=Hash()(p->val->first)%Capacity;
            Node *q=dynamic_cast<Node*>(p),*hashHead=table[_hash]._head;
            q->after = hashHead->after;
            hashHead->after = q;
        }
    }
    /**
     * add data members as needed and necessary private function such as resize()
     */

public:
    /**
     * iterator is the same as LIST
     */
    using iterator = typename list<value_type>::iterator;
    using const_iterator = typename list<value_type>::const_iterator;

    /**
    * TODO two constructors
    */
    linked_hashmap():list<value_type>(),Capacity(InitSize) {
        table=new BucketList[Capacity];
    }
    linked_hashmap(const linked_hashmap &other) {
        list<value_type>::num=0;
        Capacity=other.Capacity;
        table=new BucketList[Capacity];
        for(typename list<value_type>::node* p=other.head->nxt;p!=other.nil;p=p->nxt)
        {
            size_t _hash=Hash()(p->val->first)%Capacity;
            Node* tmp=table[_hash].insert(*p->val);
            list<value_type>::insert(list<value_type>::nil,tmp);
        }
    }
    /**
	 * TODO assignment operator
	 */
    linked_hashmap &operator=(const linked_hashmap &other) {
        if(this==&other)
            return *this;
        list<value_type>::clear();
        delete[] table;
        Capacity=other.Capacity;
        table=new BucketList[Capacity];
        list<value_type>::num=0;
        for(typename list<value_type>::node *p=other.head->nxt;p!=other.nil;p=p->nxt){
            size_t _hash=Hash()(p->val->first)%Capacity;
            Node *tmp=table[_hash].insert(*p->val);
            list<value_type>::insert(list<value_type>::nil,tmp);
        }
        return *this;
    }   
    /**
	 * TODO Destructors
	 */
    ~linked_hashmap() {
        delete[] table;
    }
    /**
	 * TODO access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
    Value &at(const Key &key) {
        size_t _hash=Hash()(key)%Capacity;
        Node *p=table[_hash].find(key);
        if(p==nullptr)
            throw index_out_of_bound();
        return p->val->second;
    }
    const Value &at(const Key &key) const {
        size_t _hash=Hash()(key)%Capacity;
        Node *p=table[_hash].find(key);
        if(p==nullptr)
            throw index_out_of_bound();
        return p->val->second;
    }
    /**
	 * TODO access specified element
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
    Value &operator[](const Key &key) {
        size_t _hash=Hash()(key)%Capacity;
        Node *p=table[_hash].find(key);
        if(p==nullptr)
            return insert(value_type(key,Value())).first->second;
        else
            return p->val->second;
    }
    /**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
    const Value &operator[](const Key &key) const {
        size_t _hash=Hash()(key)%Capacity;
        Node *p=table[_hash].find(key);
        if(p==nullptr)
            throw index_out_of_bound();
        return p->val->second;
    }
    /**
	 * TODO override clear() in LIST
	 */
    void clear() override{
        list<value_type>::clear();
        delete[] table;
        Capacity=InitSize;
        table=new BucketList[Capacity];
    }
    /**
	 * TODO insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion),
	 *   the second one is true if insert successfully, or false.
	 */
    pair<iterator, bool> insert(const value_type &value) {
        size_t _hash=Hash()(value.first)%Capacity;
        Node *p=table[_hash].find(value.first);
        if(p!=nullptr)
            return pair<iterator,bool>(iterator(p,this),false);
        p=table[_hash].insert(value);
        list<value_type>::insert(list<value_type>::nil,p);
        if(list<value_type>::num>Capacity*LoadFactor)
            resize(Capacity<<1);
        return pair<iterator,bool>(iterator(p,this),true);
    }
    /**
	 * TODO erase the element at pos.
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
     * return anything, it doesn't matter
	 */
    iterator erase(iterator pos) override{
        if(pos.invalid(this)) throw invalid_iterator();
        size_t _hash = Hash()((*pos).first) % Capacity;
        Node* p = table[_hash].erase(pos->first);
        if(p == nullptr) throw invalid_iterator();
        list<value_type>::erase(pos);
        if(Capacity << 1 >= InitSize && list<value_type>::num < (Capacity << 1) * LoadFactor)
            resize(Capacity >> 1);
        return this->begin();
    }
    /**
	 * TODO Returns the number of elements with key
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0
	 *     since this container does not allow duplicates.
	 */
    size_t count(const Key &key) const {
        size_t _hash = Hash()(key) % Capacity;
        return table[_hash].find(key) != nullptr ? 1 : 0;
    }
    /**
	 * TODO Finds an element with key equivalent to key.
	 * return iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
    iterator find(const Key &key) {
        size_t _hash = Hash()(key) % Capacity;
        Node* p = table[_hash].find(key);
        if(p != nullptr) return iterator(p, this);
        else return list<value_type>::end();
    }
    const_iterator find(const Key &key) const {
        size_t _hash = Hash()(key) % Capacity;
        Node* p = table[_hash].find(key);
        if(p != nullptr) return const_iterator(p, this);
        else return list<value_type>::cend();
    }
};

}

#endif