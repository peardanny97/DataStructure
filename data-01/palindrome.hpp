#include <string>

#include "deque.hpp"

template<typename Deque>
class Palindrome {
public:
    bool is_palindrome(const std::string&);
    void reset_deque();

private:
    Deque deque;
};

template<typename Deque>
bool Palindrome<Deque>::is_palindrome(const std::string& s1) {

	
	for(int i=0;i<s1.size();i++){
		deque.push_front(s1[i]);
	}
	size_t i = s1.size();
	i/= 2;
	while(i>0){
		if(deque.remove_front() != deque.remove_back()) {
			reset_deque();
			return false;
		}
		i --;
	}
	reset_deque();
	return true;

	// TODO
    return false;
}

template<typename Deque>
void Palindrome<Deque>::reset_deque() {
    while (!deque.empty())
        deque.remove_front();
}
