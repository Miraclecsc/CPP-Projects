#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"
namespace sjtu {
/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
	struct treeNode
	{
		T val;
		int npl;
		treeNode *left;
		treeNode *right;
		treeNode()=default;
		treeNode(const T &_val,treeNode *_lef=nullptr,treeNode *_rig=nullptr,int _npl=0):val(_val),left(_lef),right(_rig),npl(_npl){}
	};
	treeNode *root;
	size_t treeSize;
	void Clear(treeNode *rt)
	{
		if(!rt)
			return;
		Clear(rt->left);
		Clear(rt->right);
		delete rt;
	}
	treeNode *newcpy(treeNode *other)
	{
		if(!other)
			return nullptr;
		treeNode *tmp=new treeNode(other->val,newcpy(other->left),newcpy(other->right),other->npl);
		return tmp;
	}
	void swap(treeNode *x)
	{
		treeNode *tmp;
		tmp=x->left;
		x->left=x->right;
		x->right=tmp;
	}
	void bigger(treeNode *x,treeNode *y)
	{
		treeNode *tmp;
		Compare cmp;
		if(cmp(x->val,y->val)){
            tmp=x;
            x=y;
            y=tmp;
        }
	}
	treeNode *mergeNode(treeNode *x,treeNode *y)
	{
		if(!x||!y) return x?x:y;
		//bigger(x,y);
		treeNode *tmp;
		Compare cmp;
		if(cmp(x->val,y->val))
		{
			tmp=x;
			x=y;
			y=tmp;
		}
		//
		x->right=mergeNode(x->right,y);
		if(!x->left||x->left->npl<x->right->npl)
			//swap(x);
			{
				tmp=x->left;
				x->left=x->right;
				x->right=tmp;
			}
			//
		if(x->right)
			x->npl=x->right->npl+1;
		else
			x->npl=0;
		return x;
	}
public:
	/**
	 * TODO constructors
	 */
	priority_queue():root(nullptr),treeSize(0) {}
	priority_queue(const priority_queue &other) {
		root=newcpy(other.root);
		treeSize=other.treeSize;
	}
	/**
	 * TODO deconstructor
	 */
	~priority_queue() {
		Clear(root);
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other) {
		if(&other==this)
            return *this;
        Clear(root);
        root=newcpy(other.root);
        treeSize=other.treeSize;
        return *this;
	}
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const {
		if(!treeSize)
			throw container_is_empty();
		return root->val;
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e) {
		treeSize++;
		treeNode *tmp=new treeNode(e);
		root=mergeNode(root,tmp);
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
		if(!treeSize)
			throw container_is_empty();
		treeSize--;
		treeNode *tmp=root;
        root=mergeNode(root->left,root->right);
        delete tmp;
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
		return treeSize;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
		return !treeSize;
	}
	/**
	 * merge two priority_queues with at least O(logn) complexity.
	 * clear the other priority_queue.
	 */
	void merge(priority_queue &other) {
		root=mergeNode(root,other.root);
		treeSize+=other.treeSize;
		other.root=nullptr;
		other.treeSize=0;
	}
};

}

#endif