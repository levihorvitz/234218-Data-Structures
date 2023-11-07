//
// Created by osama on 11/16/2021.
//

#ifndef HW1_WET_AVL_TREE_H
#define HW1_WET_AVL_TREE_H

#include <exception>
#include <iostream>
#include "utils.h"

template <class T, class Less>
class AvlTree{
private:
    class AvlNode{
        int bf;
        int height;
        void doLL();
        void doLR();
        void doRL();
        void doRR();
    public:
        T& node;
        AvlNode* father;
        AvlNode* left;
        AvlNode* right;
        explicit AvlNode(T& node);
        ~AvlNode() = default;
        void updateBFAndH();
        void shuffleIfNeeded(); //yet to be implemented
        bool lFather();
        bool rFather();
        bool fatherExists();
        static void setLeftFather(AvlNode* father, AvlNode* son);
        static void setRightFather(AvlNode* father, AvlNode* son);

    };
    AvlNode* head;
    AvlNode* lowest;
    AvlNode* highest;
    Less less;

    static void deleteNodeAndChildren(AvlNode* toDelete);
    AvlNode& findClosestAvlNode(T& node);
    AvlNode& findAvlNode(T& node);
    void addToHead(T& node);
    AvlNode* getSuccessor(AvlNode& cur);
    AvlNode* getPredecessor(AvlNode& cur);
    void fixUntilRootAndSetHead(AvlNode& start);
    void setLowest();
    void setHighest();
    static AvlNode** getSortedArrayFromTree(const AvlTree& tree, int size);
    AvlNode** mergeSortedArraysFromTrees(AvlNode** arr1, int size1, AvlNode** arr2, int size2);
    static AvlNode* buildTreeFromArrayAndGetHead(AvlNode const * const  arr[], int size);
    static int recSortArray(const AvlNode** arr,const AvlNode* cur_node, int cur_i);
    bool equals(const T& obj1, const T& obj2);
public:
    T& getHighest();
    int getSize() const;
    class Iterator;
    Iterator beginLowest() const;
    Iterator beginHighest() const;
    Iterator end() const;
    Iterator findClosest(T& node);
    class Iterator{
    private:
        AvlNode* cur;
        explicit Iterator(AvlNode* start);
        AvlNode& getNode() const;
    public:
        Iterator();
        ~Iterator() = default;
        Iterator(const Iterator&) = default;
        Iterator& operator=(const Iterator&) = default;
        T& operator*() const;
        Iterator& operator++();
        Iterator& operator--();
        bool operator==(const Iterator& i) const;
        bool operator!=(const Iterator& i) const;
        static Iterator end();
        friend Iterator AvlTree::beginLowest() const;
        friend Iterator AvlTree::beginHighest() const;
        friend AvlNode& AvlTree::iteratorGetNode(Iterator& i);
        friend Iterator AvlTree::getNodeIterator(AvlNode& node);
        friend Iterator AvlTree::findClosest(T& node);
    };

    explicit AvlTree(Less less);
    ~AvlTree();
    bool isEmpty();
    void add(T& node);
    void remove(T& node);
    T& find(T& node);

    void merge(const AvlTree& copied);



private:
    AvlNode& iteratorGetNode(Iterator& i);
    Iterator getNodeIterator(AvlNode& node);
};

/**************************************************************************
 Implementation:
**************************************************************************/

#include "avl_tree.h"
#include <cassert>

#define AVL_TREE AvlTree<T,Less>


/**************************************************************************
 Class AvlTree
**************************************************************************/


template<class T,class Less>
bool AVL_TREE::equals(const T& obj1, const T& obj2){
    return (not less(obj1, obj2)) && (not less(obj2, obj1));
}



template<class T, class Less>
typename AVL_TREE::Iterator AVL_TREE::beginLowest() const {
    if (lowest == nullptr){
        return AvlTree::Iterator::end();
    }else{
        return AvlTree::Iterator(lowest);
    }
}

template<class T, class Less>
typename AVL_TREE::Iterator AVL_TREE::beginHighest() const {
    if (highest == nullptr){
        return AvlTree::Iterator::end();
    }else{
        return AvlTree::Iterator(highest);
    }
}

template<class T, class Less>
typename AVL_TREE::Iterator AVL_TREE::end() const {
    return AvlTree<T, Less>::Iterator::end();
}

template<class T, class Less>
AVL_TREE::AvlTree(Less less)
        :head(nullptr),
         lowest(nullptr),
         highest(nullptr),
         less(less)
{}

#define AVL_NODE typename AvlTree<T, Less>::AvlNode

template<class T, class Less>
void AVL_TREE::addToHead(T& toAdd){
    AvlNode& addedTo = findClosestAvlNode(toAdd);
    AvlNode* newAvlNodePtr;
    if (less(addedTo.node, toAdd)){
        newAvlNodePtr = new AvlNode(toAdd);
        AvlNode::setRightFather(&addedTo, newAvlNodePtr);
    }else if (less(toAdd, addedTo.node)){
        newAvlNodePtr = new AvlNode(toAdd);
        AvlNode::setLeftFather(&addedTo, newAvlNodePtr);
    }else{
        throw NodeAlreadyExists();
    }
    if (less(highest->node, toAdd)){
        highest = newAvlNodePtr;
    }
    if (less(toAdd, lowest->node)){
        lowest = newAvlNodePtr;
    }

    fixUntilRootAndSetHead(addedTo);
}


template<class T, class Less>
void AVL_TREE::add(T& node) {
    if (head == nullptr){
        head = new AvlNode(node);
        highest = head;
        lowest = head;
    }else{
        addToHead(node);
    }
}

//returns null if there is no successor
template<class T, class Less>
AVL_NODE* AVL_TREE::getSuccessor(AvlNode& cur){
    Iterator i = getNodeIterator(cur);
    ++i;
    if (i != end()){
        return &iteratorGetNode(i);
    }else{
        return nullptr;
    }
}

//returns null if there is no successor
template<class T, class Less>
AVL_NODE* AVL_TREE::getPredecessor(AvlNode& cur){
    Iterator i = getNodeIterator(cur);
    --i;
    if (i != end()){
        return &iteratorGetNode(i);
    }else{
        return nullptr;
    }
}

template<class T, class Less>
void AVL_TREE::remove(T& node){
    AvlNode* toRemove = &findAvlNode(node);
    AvlNode* toRemoveFather = toRemove->father;
    if (toRemove == highest){
        highest = getPredecessor(*highest);
    }
    if(toRemove == lowest){
        lowest = getSuccessor(*lowest);
    }
    if (toRemove->right == nullptr){
        if (toRemove->left == nullptr){
            if(toRemove->lFather()){
                AvlNode::setLeftFather(toRemove->father, nullptr);
                fixUntilRootAndSetHead(*toRemoveFather);
            }else if (toRemove->rFather()){
                AvlNode::setRightFather(toRemove->father, nullptr);
                fixUntilRootAndSetHead(*toRemoveFather);
            }else{
                head = nullptr;
            }
        }else{ //toRemove->right == nullptr && toRemove->left != nullptr
            if (toRemove->lFather()){
                AvlNode::setLeftFather(toRemove->father, toRemove->left);
                fixUntilRootAndSetHead(*toRemoveFather);
            }else if (toRemove->rFather()) {
                AvlNode::setRightFather(toRemove->father, toRemove->left);
                fixUntilRootAndSetHead(*toRemoveFather);
            }else{
                AvlNode::setRightFather(nullptr, toRemove->left);
                head = toRemove->left;
            }
        }
    }else{ //toRemove->right != nullptr
        //it is guaranteed that there exists a successor
        AvlNode* successor = getSuccessor(*toRemove);
        AvlNode* fixingPoint;
        if(successor->father == toRemove){
            fixingPoint = successor;
        }else{
            fixingPoint = successor->father;
        }
        if (successor->lFather()) {
            AvlNode::setLeftFather(successor->father, successor->right);
        }else{
            AvlNode::setRightFather(successor->father, successor->right);
        }
        if(toRemove->lFather()){
            AvlNode::setLeftFather(toRemove->father, successor);
        }else if (toRemove->rFather()) {
            AvlNode::setRightFather(toRemove->father, successor);
        }else{ //successor is head
            AvlNode::setRightFather(nullptr, successor);
        }
        AvlNode::setLeftFather(successor, toRemove->left);
        AvlNode::setRightFather(successor, toRemove->right);
        fixUntilRootAndSetHead(*fixingPoint);
    }
    delete toRemove;
}

//updates bf and height and shuffles until it reaches head
template<class T, class Less>
void AVL_TREE::fixUntilRootAndSetHead(AvlNode& start){
    AvlNode* prev = nullptr;
    AvlNode* cur = &start;
    while(cur != nullptr){
        cur->updateBFAndH();
        cur->shuffleIfNeeded();
        prev = cur;
        cur = cur->father;
    }
    head = prev;
}

template<class T, class Less>
void AVL_TREE::merge(const AvlTree& toCopy){
    int size1 = toCopy.getSize();
    int size2 = this->getSize();
    AvlNode** arr1 = getSortedArrayFromTree(toCopy, size1);
    AvlNode** arr2;
    try{
        arr2 = getSortedArrayFromTree(*this, size2);
    }catch(std::bad_alloc&){
        delete[] arr1;
        
        throw std::bad_alloc();
    }
    AvlNode** arr;
    try{
        arr = mergeSortedArraysFromTrees(arr1, size1, arr2, size2);
    }catch(std::bad_alloc&){
        delete[] arr1;

        delete[] arr2;
        
        throw std::bad_alloc();
    }

    delete[] arr1;

    delete[] arr2;
    
    AvlNode* newHead;
    try{
        newHead = buildTreeFromArrayAndGetHead(arr, size1 + size2);
    }catch(std::bad_alloc&){
        delete[] arr;
        
        throw std::bad_alloc();
    }

    delete[] arr;
    
    deleteNodeAndChildren(head);
    head = newHead;
    setLowest();
    setHighest();
}

template<class T, class Less>
void AVL_TREE::setLowest(){
    AvlNode* prev = nullptr;
    AvlNode* cur = head;
    while(cur != nullptr){
        prev = cur;
        cur = cur->left;
    }
    lowest = prev;
}

template<class T, class Less>
void AVL_TREE::setHighest(){
    AvlNode* prev = nullptr;
    AvlNode* cur = head;
    while(cur != nullptr){
        prev = cur;
        cur = cur->right;
    }
    highest = prev;
}

template<class T, class Less>
AVL_NODE** AVL_TREE::getSortedArrayFromTree(const AvlTree& tree, int size){
    AvlNode** arr = new AvlNode*[size];
    
    recSortArray((const AvlNode**)arr, tree.head, 0);
    return arr;
}

template<class T, class Less>
AVL_NODE** AVL_TREE::mergeSortedArraysFromTrees(AvlNode** arr1, int size1, AvlNode** arr2, int size2){
    AvlNode** arr = new AvlNode*[size1 + size2];

    int i1 = 0;
    int i2 = 0;
    int i = 0;
    while(i < size1 + size2 && i1 < size1 && i2 < size2){
        if (less((arr1[i1])->node, (arr2[i2])->node)){
            arr[i] = arr1[i1];
            i1++;
        }else{
            arr[i] = arr2[i2];
            i2++;
        }
        i++;
    }
    if (i1 == size1){
        while(i2 < size2){
            arr[i] = arr2[i2];
            i2++;
            i++;
        }
    }
    if (i2 == size2){
        while(i1 < size1){
            arr[i] = arr1[i1];
            i1++;
            i++;
        }
    }
    return arr;
}

template<class T, class Less>
AVL_NODE* AVL_TREE::buildTreeFromArrayAndGetHead(AvlNode const * const  arr[], int size){
    if (size == 0){
        return nullptr;
    }
    int midIndex = (size - 1) / 2;
    int leftSize = midIndex ;
    int rightSize = size - midIndex - 1;
    AvlNode* mid = new AvlNode(arr[midIndex]->node);
    
    AvlNode* left = nullptr;
    AvlNode* right;
    try{
        left = buildTreeFromArrayAndGetHead(arr,leftSize);
        right = buildTreeFromArrayAndGetHead(arr + midIndex + 1, rightSize);
    }catch(std::bad_alloc&){
        if (left != nullptr){
            delete left;
            
        }
        //no need to delete right
        delete mid;
        
        throw std::bad_alloc();
    }

    AvlNode::setLeftFather(mid, left);
    AvlNode::setRightFather(mid, right);
    mid->updateBFAndH();
    return mid;
}

template<class T, class Less>
int AVL_TREE::recSortArray(const AvlNode** arr,const AvlNode* cur_node, int cur_i) {
    if (cur_node == nullptr){
        return cur_i;
    }
    cur_i = recSortArray(arr, cur_node->left,cur_i);
    arr[cur_i] = cur_node;
    cur_i = recSortArray(arr, cur_node->right, cur_i + 1);
    return cur_i;
}

template<class T, class Less>
T &AvlTree<T, Less>::find(T &node) {
    return findAvlNode(node).node;
}

template<class T, class Less>
void AvlTree<T, Less>::deleteNodeAndChildren(AvlNode* toDelete){
    if (toDelete == nullptr){
        return;
    }
    deleteNodeAndChildren(toDelete->left);
    deleteNodeAndChildren(toDelete->right);
    delete toDelete;
    
}

template<class T, class Less>
AvlTree<T, Less>::~AvlTree() {
    if (head != nullptr){
        assert(head->father == nullptr);
    }
    deleteNodeAndChildren(head);
}

template<class T, class Less>
typename AVL_TREE::AvlNode &AVL_TREE::iteratorGetNode(AvlTree::Iterator &i) {
    return i.getNode();
}

template<class T, class Less>
AVL_NODE& AVL_TREE::findClosestAvlNode(T &node) {
    assert(head != nullptr);
    AVL_NODE* cur = head;
    AVL_NODE* prev = nullptr;
    while(cur != nullptr){
        if (less(cur->node, node)){
            prev = cur;
            cur = cur->right;
        }else if (less(node, cur->node)){
            prev = cur;
            cur = cur->left;
        }else{
            return *cur;
        }
    }
    return *prev;
}


template<class T, class Less>
AVL_NODE& AVL_TREE::findAvlNode(T &node){
    if (head == nullptr){
        throw NodeNotExists();
    }
    AvlNode& potential = findClosestAvlNode(node);
    if (equals(node, potential.node)) {
        return potential;
    }else{
        throw NodeNotExists();
    }
}

template<class T, class Less>
T &AvlTree<T, Less>::getHighest() {
    if (highest != nullptr){
        return highest->node;
    }else{
        throw TreeEmpty();
    }
}

template<class T, class Less>
bool AvlTree<T, Less>::isEmpty() {
    return head == nullptr;
}

template<class T, class Less>
typename AVL_TREE::Iterator AVL_TREE::getNodeIterator(AvlTree::AvlNode &node) {
    return Iterator(&node);
}

template<class T, class Less>
int AvlTree<T, Less>::getSize() const{
    auto i = beginHighest();
    int arr_size = 0;
    while(i != end()){
        arr_size++;
        --i;
    }
    return arr_size;
}
template<class T, class Less>
typename AVL_TREE::Iterator AVL_TREE::findClosest(T &node) {
    return Iterator(&findClosestAvlNode(node));
}

/**************************************************************************
 Class AvlTree::AvlNode
**************************************************************************/

template<class T, class Less>
AVL_TREE::AvlNode::AvlNode(T &node)
        :bf(0),
         height(0),
         node(node),
         father(nullptr),
         left(nullptr),
         right(nullptr)
{}


template<class T, class Less>
void AVL_TREE::AvlNode::updateBFAndH() {
    int left_height;
    int right_height;

    if (left == nullptr){
        left_height = -1;
    }else{
        left_height = left->height;
    }

    if (right == nullptr){
        right_height = -1;
    }else{
        right_height = right->height;
    }

    bf = left_height - right_height;


    if (left_height > right_height){
        height = left_height + 1;
    }else{
        height = right_height + 1;
    }

}

template<class T, class Less>
void AVL_TREE::AvlNode::setRightFather(AvlNode* father, AvlNode* son){
    if (father != nullptr) {
        father->right = son;
    }
    if (son != nullptr){
        son->father = father;
    }
}

template<class T, class Less>
void AVL_TREE::AvlNode::setLeftFather(AvlNode* father, AvlNode* son){
    if (father != nullptr) {
        father->left = son;
    }
    if (son != nullptr){
        son->father = father;
    }
}

template<class T, class Less>
void AVL_TREE::AvlNode::shuffleIfNeeded() {
    if (bf == 2){
        if(left->bf >= 0){
            doLL();
        }else{ //left->bf == -1
            doLR();
        }
    }else if (bf == -2){
        if (right->bf <= 0){
            doRR();
        }else{ //right->bf == 1
            doRL();
        }
    }
}

template<class T, class Less>
void AVL_TREE::AvlNode::doLL() {
    AvlNode* b = this;
    AvlNode* a = b->left;
    AvlNode* b_father = b->father;
    AvlNode* ar = a->right;

    if (b->lFather()){
        setLeftFather(b_father, a);
    }else{
        setRightFather(b_father, a);
    }
    setRightFather(a, b);
    setLeftFather(b, ar);

    b->updateBFAndH();
    a->updateBFAndH();
}

template<class T, class Less>
void AvlTree<T, Less>::AvlNode::doLR() {
    AvlNode* c = this;
    AvlNode* a = c->left;
    AvlNode* b = a->right;
    AvlNode* bl = b->left;
    AvlNode* br = b->right;
    AvlNode* c_father = c->father;

    if (c->lFather()){
        setLeftFather(c_father, b);
    }else{
        setRightFather(c_father, b);
    }

    setLeftFather(b, a);
    setRightFather(b, c);
    setRightFather(a, bl);
    setLeftFather(c, br);

    a->updateBFAndH();
    c->updateBFAndH();
    b->updateBFAndH();
}

template<class T, class Less>
void AVL_TREE::AvlNode::doRR() {
    AvlNode* a = this;
    AvlNode* b = a->right;
    AvlNode* a_father = a->father;
    AvlNode* bl = b->left;

    if (a->lFather()){
        setLeftFather(a_father, b);
    }else{
        setRightFather(a_father, b);
    }

    setLeftFather(b, a);
    setRightFather(a, bl);

    a->updateBFAndH();
    b->updateBFAndH();
}

template<class T, class Less>
void AVL_TREE::AvlNode::doRL() {
    AvlNode* a = this;
    AvlNode* b = a->right;
    AvlNode* c = b->left;
    AvlNode* cl = c->left;
    AvlNode* cr = c->right;
    AvlNode* a_father = a->father;

    if (a->lFather()){
        setLeftFather(a_father, c);
    }else{
        setRightFather(a_father, c);
    }

    setLeftFather(c, a);
    setRightFather(c, b);
    setRightFather(a, cl);
    setLeftFather(b, cr);

    a->updateBFAndH();
    b->updateBFAndH();
    c->updateBFAndH();
}
template<class T, class Less>
bool AVL_TREE::AvlNode::lFather() {
    if (not fatherExists()){
        return false;
    }
    return (father->left == this);
}

template<class T, class Less>
bool AVL_TREE::AvlNode::rFather() {
    if (not fatherExists()){
        return false;
    }
    return (father->right == this);
}

template<class T, class Less>
bool AVL_TREE::AvlNode::fatherExists() {
    return (this->father != nullptr);
}

/**************************************************************************
 Class AvlTree::Iterator
**************************************************************************/
#define AVL_ITERATOR AvlTree<T, Less>::Iterator
template<class T, class Less>
AVL_ITERATOR::Iterator(AvlTree::AvlNode* start)
        :cur(start)
{}

template<class T, class Less>
AVL_ITERATOR::Iterator()
{
    *this = AvlTree::Iterator::end();
}

template<class T, class Less>
typename AVL_ITERATOR AVL_ITERATOR::end(){
    return Iterator(nullptr);
}

template<class T, class Less>
T& AVL_ITERATOR::operator*() const {
    return cur->node;
}

template<class T, class Less>
typename AVL_ITERATOR& AVL_ITERATOR::operator++(){
    assert(*this != end());
    AvlNode* temp;
    if (cur->right != nullptr){
        temp = cur->right;
        while(temp->left != nullptr){
            temp = temp->left;
        }
        cur = temp;
    }else{
        temp = cur;
        while(temp->rFather()){
            temp = temp->father;
        }
        if (temp->father == nullptr){
            *this = end();
        }else{
            cur = temp->father;
        }
    }
    return *this;
}

template<class T, class Less>
typename AVL_ITERATOR& AVL_ITERATOR::operator--() {
    assert(*this != end());
    AvlNode* temp;
    if (cur->left != nullptr){
        temp = cur->left;
        while(temp->right != nullptr){
            temp = temp->right;
        }
        cur = temp;
    }else{
        temp = cur;
        while(temp->lFather()){
            temp = temp->father;
        }
        if (temp->father == nullptr){
            *this = end();
        }else{
            cur = temp->father;
        }
    }
    return *this;
}

template<class T, class Less>
AVL_NODE& AVL_ITERATOR::getNode() const {
    return *cur;
}

template<class T, class Less>
bool AvlTree<T, Less>::Iterator::operator==(const AvlTree::Iterator &i) const {
    return this->cur == i.cur;
}

template<class T, class Less>
bool AvlTree<T, Less>::Iterator::operator!=(const AvlTree::Iterator &i) const {
    return !(*this == i);
}

#endif
//HW1_WET_AVL_TREE_H


