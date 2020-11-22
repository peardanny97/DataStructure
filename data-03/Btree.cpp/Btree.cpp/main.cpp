
#include <cstddef>
#include <array>
#include <iostream>
#include <optional>
#include <iterator>
#include <algorithm>
#include <cstring>
#include <string>
#include <sstream>
#include <functional>
#include <vector>
#include <algorithm>
#include <random>

#include <ctime>
#include <cstdlib>

enum class NodeType { LEAF, INTERNAL };

template<typename T, size_t B = 6>
struct BTreeNode;

template<typename T, size_t B = 6>
struct BTree {
    BTreeNode<T, B>* root = nullptr;

    ~BTree() { if (root) delete root; }

    bool insert(const T&);
    bool remove(const T&);

    void for_all(std::function<void(T&)>);
    void for_all_nodes(std::function<void(const BTreeNode<T,B>&)>);

    const std::optional<T> find_rightmost_key() const;
    const std::optional<size_t> depth() const;

    std::string format(void);
};

template<typename T, size_t B>
struct BTreeNode {
    NodeType type;
    size_t n;
    std::array<T, 2 * B - 1> keys;
    std::array<BTreeNode *, 2 * B> edges;

    BTreeNode();
    BTreeNode(const T& t);
    BTreeNode(std::initializer_list<T>);

    template<typename InputIt>
    BTreeNode(InputIt begin, InputIt end);

    ~BTreeNode();

    bool insert(const T& t);
    size_t get_index(const T& t);

    void for_all(std::function<void(T&)> func);

    bool remove(const T& t);

    size_t depth(void);
    std::string format_subtree(size_t);
    std::string format_level(size_t);
    std::string format_node(void);
    std::vector<BTreeNode<T, B>*> find_nodes_at_level(size_t);

    void for_all_nodes(std::function<void(const BTreeNode&)>);

    static std::pair<BTreeNode*, size_t> search(BTreeNode<T, B>*, const T& t);
    static void split_child(BTreeNode<T, B>&, size_t);
    static bool try_borrow_from_sibling(BTreeNode<T, B>&, size_t);
    static bool borrow_from_right(BTreeNode<T, B>&, size_t);
    static bool borrow_from_left(BTreeNode<T, B>&, size_t);

    /* NOTE: If the root node has only one key, it will be empty after
      merging the children. Take care of updating the root. I guess this is
      the only way a B-tree may shrink its height. */
    static bool merge_children(BTreeNode<T, B>&, size_t);

    static T& find_rightmost_key(BTreeNode<T, B>&);
};

template<typename T,  size_t B>
bool BTree<T, B>::insert(const T& t) {
    if (!root) {
        root = new BTreeNode<T, B>(t);
        return true;
    }

    /* Make sure the root node is not full. Create an empty tree which has
       the original root as a child. Then split the original root. */
    if (root->n >= 2 * B - 1) {
        BTreeNode<T, B>* new_root = new BTreeNode<T, B>{};
        new_root->edges[0] = root;
        BTreeNode<T, B>::split_child(*new_root, 0);
        root = new_root;
    }

    return root->insert(t);
}

/* By default, use in-order traversal */
template<typename T, size_t B>
void BTree<T, B>::for_all(std::function<void(T&)> func) {
    if (root)
        root->for_all(func);
}

/* This isn't necessarily the in-order traversal */
template<typename T, size_t B>
void BTree<T, B>::for_all_nodes(std::function<void(const BTreeNode<T,B>&)> func) {
    if (root)
        root->for_all_nodes(func);
}

template<typename T, size_t B>
const std::optional<T> BTree<T, B>::find_rightmost_key() const {
    if (!root)
        return std::nullopt;

    return BTreeNode<T, B>::find_rightmost_key(*root);
}

template<typename T, size_t B>
const std::optional<size_t> BTree<T, B>::depth() const {
    if (!root)
        return std::nullopt;

    return root->depth();
}

template<typename T, size_t B>
bool BTreeNode<T, B>::insert(const T& t) {
    
    if(type == NodeType::LEAF){
        size_t idx = get_index(t);
        for(auto i = n; i>idx; i--){
            keys[i] = keys[i-1];
        }
        keys[idx] = t;
        n++;
        
        return true;
    }
    else{
        size_t idx = get_index(t);
        if(edges[idx]->n == 2*B-1){
            split_child(*this, idx);
        }
        idx = get_index(t);
        return edges[idx]->insert(t);
    }
    // TODO - DONE!
    return false;
}

/**
 * Find the desired position of t in current node.
 *
 * For example, if `n` looks like the following:
 *
 * [ 3 | 9 | 13 | 27 ]
 *
 * Then,
 *     n.get_index(2) = 0
 *     n.get_index(5) = 1
 *     n.get_index(10) = 2
 *     n.get_index(19) = 3
 *     n.get_index(31) = 4
 */
template<typename T, size_t B>
size_t BTreeNode<T, B>::get_index(const T& t) {
    for(auto i = 0; i < n;i++){
        if(keys[i]>t)
            return i;
    }
    return n;// TODO - DONE!
    return 0;
}

template<typename T, size_t B>
void BTreeNode<T, B>::for_all(std::function<void(T&)> func) {
    if (type == NodeType::LEAF) {
        for (auto j = 0; j < n; j++)
            func(keys[j]);
    } else {
        if (n < 1)
            return;

        for (auto j = 0; j < n; j++) {
            edges[j]->for_all(func);
            func(keys[j]);
        }

        /* The rightest edge */
        edges[n]->for_all(func);
    }
}

/* This isn't necessarily the in-order traversal */
template<typename T, size_t B>
void BTreeNode<T, B>::for_all_nodes(std::function<void(const BTreeNode<T,B>&)> func) {
    if (type == NodeType::LEAF) {
        func(*this);
    } else {
        if (n < 1)
            return;

        func(*this);

        for (auto j = 0; j < n + 1; j++) {
            edges[j]->for_all_nodes(func);
        }
    }
}

/* Assume this is called only when the child parent->edges[idx] is full, and
   the parent is not full. */
template<typename T, size_t B>
void BTreeNode<T, B>::split_child(BTreeNode<T, B>& parent, size_t idx) {
    
    T temp = parent.edges[idx] -> keys[B-1];
    
    //change parent first
    for(auto i = parent.n; i>idx;i--){
        parent.keys[i] = parent.keys[i-1];
    }
    for(auto i = parent.n ; i>idx;i--){
        parent.edges[i+1] = parent.edges[i];
    }
    parent.keys[idx] = temp;
    parent.type = NodeType::INTERNAL;
    parent.n++;
    
    BTreeNode<T, B>* new_R = new BTreeNode<T, B>{}; // make node for right
    
    /* we don't need node for left cause there's orignal node*/
    

    for(auto i=0;i<B-1;i++){
        new_R->keys[i] = parent.edges[idx]->keys[i+B];
        new_R->edges[i] = parent.edges[idx]->edges[i+B];

    }
    new_R->edges[B-1] = parent.edges[idx]->edges[2*B-1];
    new_R->n = B-1;
    
    //make sure type is same
    if(parent.edges[idx]->type==NodeType::LEAF){
        new_R->type = NodeType::LEAF;
    }
    else{
        new_R->type = NodeType::INTERNAL;
    }
    parent.edges[idx]->n = B-1;
    parent.edges[idx+1] = new_R;

    
    // TODO - DONE!
}

template<typename T, size_t B>
bool BTree<T, B>::remove(const T& t) {
    if (!root)
        return false;

    root->remove(t);

    /* After merging, the size of the root may become 0. */
    if (root->n == 0 && root->type == NodeType::INTERNAL) {
        auto prev_root = root;
        root = root->edges[0];
        prev_root->type = NodeType::LEAF;
        delete prev_root;
    }

    return true;
}

template<typename T, size_t B>
bool BTreeNode<T, B>::remove(const T& t) {
    size_t idx = get_index(t);
    if(type == NodeType::LEAF)
    {
        if(idx==0) return false; //out of range
        if(keys[idx-1]!=t) return false;
        else{
            for(auto i = idx-1; i<n-1;i++) {
                keys[i] = keys[i+1];
            }
            n--;
            return true;
        }
    }
    if(idx>0&&keys[idx-1]==t)//if we've found key
    {
        if(edges[idx-1]->n==B-1){
            if(idx>1&&edges[idx-2]->n>B-1)//if we can borrow from left + if our idx = 1 it's impossible
            {
                T L_max = find_rightmost_key(*edges[idx-1]);
                keys[idx-1] = L_max;
                try_borrow_from_sibling(*this, idx-1); //this will bring borrow from left
                return edges[idx-1]->remove(L_max);
            }
            // so this mean we can't borrow from left
            // if we get L_max and operate borrow form right or merge there's problem, our t will go down
            try_borrow_from_sibling(*this,idx-1);
            return edges[idx-1]->remove(t);
            
        }
        T L_max = find_rightmost_key(*edges[idx-1]);
        keys[idx-1] = L_max;
        return edges[idx-1]->remove(L_max);
    }
    if(edges[idx]->n==B-1){
        try_borrow_from_sibling(*this, idx);
        idx = get_index(t);
    }
 
    return edges[idx]->remove(t);
    // TODO -NOW
    
    return false;
}

/**
 * Try to borrow a key from sibling.
 *
 * @e: The index of the edge that are trying to borrow a key
 * @return true if borrowing succeed, false otherwise
 */
template<typename T, size_t B>
bool BTreeNode<T, B>::try_borrow_from_sibling(BTreeNode<T, B>&node, size_t e) {

    if(e>0 && node.edges[e-1]->n > B - 1)
        return borrow_from_left(node, e);
    
    if(e<node.n && node.edges[e+1]->n > B - 1)
        return borrow_from_right(node, e);
    
    return merge_children(node, e);
                
    // TODO - DONE?
    return false;
}

template<typename T, size_t B>
bool BTreeNode<T, B>::borrow_from_right(BTreeNode<T, B>& node, size_t edge) {
    node.edges[edge]->keys[B-1] = node.keys[edge];
    node.edges[edge]->n++;
    node.keys[edge] = node.edges[edge+1]->keys[0];
    for(auto i=0;i<node.edges[edge+1]->n-1;i++){
        node.edges[edge+1]->keys[i] = node.edges[edge+1]->keys[i+1];
    }
    if(node.edges[edge]->type == NodeType::INTERNAL){
        node.edges[edge]->edges[B] = node.edges[edge+1]->edges[0];
        
        for(auto i=0;i<node.edges[edge+1]->n;i++){
            node.edges[edge+1]->edges[i] = node.edges[edge+1]->edges[i+1];
        }
        
    }
    node.edges[edge+1]->n--;
    return true;
    // TODO - DONE?
    return false;
}

template<typename T, size_t B>
bool BTreeNode<T, B>::borrow_from_left(BTreeNode<T, B>& node, size_t edge) {
    node.edges[edge]->n++;
    for(auto i= B-1;i>0;i--){
        node.edges[edge]->keys[i] = node.edges[edge]->keys[i-1];
    }
    node.edges[edge]->keys[0] = node.keys[edge-1];
    node.keys[edge-1] = node.edges[edge-1]->keys[node.edges[edge-1]->n-1];
    if(node.edges[edge]->type == NodeType::INTERNAL){
        for(auto i= B;i>0;i--){
            node.edges[edge]->edges[i] = node.edges[edge]->edges[i-1];
        }
        
        node.edges[edge]->edges[0] = node.edges[edge-1]->edges[node.edges[edge-1]->n];
        
    }
    node.edges[edge-1]->n--;
    return true;
    // TODO - DONE?
    return false;
}

template<typename T, size_t B>
bool BTreeNode<T, B>::merge_children(BTreeNode<T, B> & node, size_t idx) {
    if(idx<node.n){
        node.edges[idx]->keys[B-1] = node.keys[idx];
        for(auto i=0;i<B-1;i++){
            node.edges[idx]->keys[B+i] = node.edges[idx+1]->keys[i];
        }
        node.edges[idx]->n = 2*B-1;
        if(node.edges[idx]->type==NodeType::INTERNAL){
            for(auto i=0;i<=B-1;i++){
                node.edges[idx]->edges[B+i] = node.edges[idx+1]->edges[i];
            }
        }
        for(auto i = idx; i<node.n-1; i++){
            node.keys[i] = node.keys[i+1];
        }
        for (auto i=0;i<=node.edges[idx+1]->n;i++) node.edges[idx+1]->edges[i] = nullptr;
        delete node.edges[idx+1];
        
        for(auto i = idx+1; i<node.n; i++){
                node.edges[i] = node.edges[i+1];
        }
        node.n--;
        return true;
    }
    else{
        node.edges[idx-1]->keys[B-1] = node.keys[idx-1];
        for(auto i=0;i<B-1;i++){
            node.edges[idx-1]->keys[B+i] = node.edges[idx]->keys[i];
        }
        node.edges[idx-1]->n = 2*B-1;
        if(node.edges[idx]->type==NodeType::INTERNAL){
            for(auto i=0;i<=B-1;i++){
                node.edges[idx-1]->edges[B+i] = node.edges[idx]->edges[i];
            }
        }
        
        for (auto i=0;i<=node.edges[idx]->n;i++) node.edges[idx]->edges[i] = nullptr;
        delete node.edges[idx];
        node.n--;
        return true;
    }
    // TODO
    return false;
}

template<typename T, size_t B>
T& BTreeNode<T, B>::find_rightmost_key(BTreeNode<T, B>& node) {
    if (node.type == NodeType::LEAF)
        return node.keys[node.n - 1];

    return find_rightmost_key(*node.edges[node.n]);
}

template<typename T, size_t B>
std::pair<BTreeNode<T, B>*, size_t>
BTreeNode<T, B>::search(BTreeNode<T, B>* node, const T& t) {
    if (node->type == NodeType::LEAF) {
        for (auto i = 0; i < node->keys.size(); i++)
            if (t == node->keys[i])
                return { node, i };

        return { nullptr, -1 };
    }

    size_t i;
    for (i = 0; i < node->n; i++) {
        if (t == node->keys[i])
            return { node, i };

        if (t < node->keys[i]) {
            return search(node->edges[i], t);
        }
    }

    return search(node->edges[i], t);
}

template<typename T, size_t B>
size_t BTreeNode<T, B>::depth(void) {
    if (type == NodeType::LEAF)
        return 0;

    return 1 + edges[0]->depth();
}

template <typename T, size_t B>
std::string BTree<T, B>::format(void) {
    if (!root)
        return std::string{};

    return root->format_subtree(root->depth());
}

template<typename T, size_t B>
std::string BTreeNode<T, B>::format_subtree(size_t depth) {
    std::ostringstream os;

    for (auto i = 0; i <= depth; i++)
        os << format_level(i) << '\n';

    return os.str();
}

template<typename T, size_t B>
std::string BTreeNode<T, B>::format_level(size_t level) {
    std::ostringstream os;
    auto nodes_at_level = find_nodes_at_level(level);

    for (auto node : nodes_at_level)
        os << node->format_node() << ' ';

    return os.str();
}


template<typename T, size_t B>
std::string BTreeNode<T, B>::format_node(void) {
    std::ostringstream os;

    os << '[';
    for (auto i = 0; i < n - 1; i++)
        os << keys[i] << '|';
    os << keys[n - 1];
    os << ']';

    return os.str();
}

template<typename T, size_t B>
std::vector<BTreeNode<T, B>*> BTreeNode<T, B>::find_nodes_at_level(size_t lv) {
    std::vector<BTreeNode<T, B>*> nodes;

    if (lv == 0) {
        nodes.emplace_back(this);
        return nodes;
    } else {
        std::vector<BTreeNode<T, B>*> tmp;
        for (auto i = 0; i < n + 1; i++) {
            tmp = edges[i]->find_nodes_at_level(lv - 1);
            std::copy(tmp.begin(), tmp.end(), std::back_inserter(nodes));
        }

        return nodes;
    }
}

template<typename T, size_t B>
BTreeNode<T, B>::BTreeNode() : n(0), type(NodeType::LEAF) {}

template<typename T, size_t B>
BTreeNode<T, B>::BTreeNode(const T& t) : n(1), type(NodeType::LEAF) {
    keys[0] = t;
}

/* Assume the input initializer list is sorted */
template<typename T, size_t B>
BTreeNode<T, B>::BTreeNode(std::initializer_list<T> l)
    : n(l.size()), type(NodeType::LEAF) {
    std::copy(l.begin(), l.end(), keys.begin());
}

/* Assume the input iterator is sorted. */
template<typename T, size_t B>
template<typename InputIt>
BTreeNode<T, B>::BTreeNode(InputIt begin, InputIt end)
    : n(end - begin), type(NodeType::LEAF) {
    std::copy(begin, end, keys.begin());
}

template<typename T, size_t B>
BTreeNode<T, B>::~BTreeNode() {
    if (this->type == NodeType::LEAF)
        return;

    for (auto i = 0; i < n + 1; i++)
        if (edges[i]) delete edges[i];
}

int main(){
    
    BTree<int, 3> tree;

//    srand((unsigned)time(NULL));
//    for (int i = 0; i < 10000; i++)
//        tree.insert(i);
//
//    std::cout << tree.format() << std::endl;
//
//    for (int i = 0 ; i < 9990; i++)
//        tree.remove(i);
//
//    std::cout << tree.format() << std::endl;
    
    std::vector<int> xs;
    for(int i=1; i<=100;i++){
        xs.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    
    std::shuffle(xs.begin(), xs.end(), g);
    for(int i=0;i<100;i++){
        tree.insert(xs[i]);
    }
    

    for(int i=0; i<99;i++){
        //std::cout<<i<<std::endl;
        std::cout<<" "<<xs[i]<<std::endl;
        tree.remove(xs[i]);
        
        std::cout<<tree.format()<<std::endl;
    }
    std::cout<<xs[99]<<std::endl;
    std::cout<<tree.format()<<std::endl;
    return 0;
}
