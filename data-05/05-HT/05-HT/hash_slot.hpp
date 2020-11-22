#include <cstddef>

/* Fill in the TODO sections in the following code. */

template <typename K, typename V>
class HashSlot
{
public:
    HashSlot(): _empty(true), _removed(false) {
    }
    
    K get_key() const {
        return _key;
    }

    V get_value() const {
        return _value;
    }

    void set_key_value(K key, V value) {
        _key = key;
        _value = value;
        _empty = false;
        _removed = false;
        // TODO
    }

    bool is_empty() const {
        return _empty;
        // TODO
        return false;
    }

    void set_empty() {
        _empty = true;
        // TODO
    }

    bool is_removed() const {
        return _removed;
        // TODO
        return false;
    }

    void set_removed() {
        _removed = true;
        // TODO
    }
    
private:
    // key-value pair
    K _key;
    V _value;
    bool _empty;
    bool _removed;

    // disallow copy and assignment
    HashSlot(const HashSlot &);
    HashSlot & operator=(const HashSlot &);
};
