#include <iostream>

template <typename T>
class Stack {
public:
	Stack(int size) : nr_elem(0), size(size) {
		data = new T[size];
	}

	T pop() {
		if (nr_elem == 0) {
			throw std::runtime_error("No more elements.");
		}
		return data[--nr_elem];
	}
	
	void push(const T &element) {
		if(nr_elem >= size) {
			throw std::runtime_error("Max elements reached.");		
		}
		data[nr_elem++] = element;
	}
		

private:
	int nr_elem;
	int size;
	T *data;
};



int main() {
	Stack<int> s(10);
	
	s.push(10);
	s.push(20);
	std::cout << s.pop() << std::endl;
	std::cout << s.pop() << std::endl;

	Stack<std::string> s2(20);
	s2.push("Test");
	

	return 0;
}
