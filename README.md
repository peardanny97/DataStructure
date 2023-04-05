# DataStructure

## LAB-01 Deque

### Deque?

Double ended Queue, can be pushed and removed from both front and back

TODO: Implementation of array deque, list deque, and palindrome

Array deque: resizable array, when deque size get bigger than capacity, resize deque
<br>List deque: linked list style, watch out for memory leak.
<br>Palindrome: check whether string is palindrome or not by using deque structure

## LAB-02 BST

### BST?

Binary Search Tree, a rooted binary tree data structure with the key of each internal node being greater than all the keys in the respective node's left subtree and less than the ones in its right subtree. Time complexity becomes O(h). Worst case O(n), generally O(logn)

TODO: Implementation of BST using unique_ptr

### Unique_ptr?
[std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) is a smart pointer that owns and manages another object through a pointer and disposes of that object when the unique_ptr goes out of scope.

## LAB-03 B-Tree

### B-Tree?
B-Trees maintain balance by ensuring that each node has a minimum number of keys, so the tree is always balanced, and all leaves are at same level.
Every opreations(insertion, deletion, search) takes O(logn) time always.

TODO: Implementation of B-Tree, to maintain tree constructure you have to implement function such as try_borrow_from_sibling, split_child, etc.
