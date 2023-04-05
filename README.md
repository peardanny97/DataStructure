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

## LAB-04 RB-Tree

### RB-Tree?
RB-Tree is binary search tree data structure which has properties below.

1.Every node is either red or black.
<br>2. All NIL nodes (figure 1) are considered black.
<br>3. A red node does not have a red child.
<br>4. Every path from a given node to any of its descendant NIL nodes goes through the same number of black nodes.

TODO: Implementation of RB-Tree using unique_ptr(<- this may cause many trouble)

## LAB-05 Hash

### Hash?
The process of mapping an object or a number onto an integer in a given range i called hashing, ideal hashing makes every operation tooks O(1) time.
But multiple objects may hash to the same value, which cause collision. Hash tables use a hash function together with a mechanism for dealing with collisions.

TODO: Implementation of LinearProbeHashTable, QuadProbeHashTable, and hash functions

## LAB-06 Fibonacci Heap

### Fibonacci Heap?
Fibonacci heap is a data structure for priority queue operations, consisting of a collection of heap-ordered trees. A Fibonacci heap is a collection of trees satisfying the minimum-heap property, that is, the key of a child is always greater than or equal to the key of the parent. This implies that the minimum key is always at the root of one of the trees.

TODO: Implementation of Fibonacci heap using shared_ptr

### Shared_ptr?
[std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) is a smart pointer that retains shared ownership of an object through a pointer. Several shared_ptr objects may own the same object.

## LAB-07 Graph

### Graph?
Graph is an abstract data type that is meant to implement the undirected graph and directed graph concepts from the field of graph theory within mathematics.

TODO: Implementation of Graph, topological sort, Prim's minimum spanning tree, Dijkstra's shortest path algorithm

## Topological sort?
Topological sort of a directed graph is a linear ordering of its vertices such that for every directed edge uv from vertex u to vertex v, u comes before v in the ordering. By using a property of topological sorting, we can find out a graph is a DAG(Directed Acyclic Graph) if and only if it has topolgical sorting.

## Prim's MST?
Prim's algorithm is a greedy algorithm that finds a minimum spanning tree for a weighted undirected graph. This means it finds a subset of the edges that forms a tree that includes every vertex, where the total weight of all the edges in the tree is minimized. The algorithm operates by building this tree one vertex at a time, from an arbitrary starting vertex, at each step adding the cheapest possible connection from the tree to another vertex.

## Dijkstra's shortest path?
Dijkstra's algorithm is an algorithm for finding the shortest paths between nodes in a weighted graph. Dijkstra's Algorithm basically starts at the node that you choose (the source node) and it analyzes the graph to find the shortest path between that node and all the other nodes in the graph.
The algorithm keeps track of the currently known shortest distance from each node to the source node and it updates these values if it finds a shorter path.
Once the algorithm has found the shortest path between the source node and another node, that node is marked as "visited" and added to the path.
The process continues until all the nodes in the graph have been added to the path. This way, we have a path that connects the source node to all other nodes following the shortest path possible to reach each node.
