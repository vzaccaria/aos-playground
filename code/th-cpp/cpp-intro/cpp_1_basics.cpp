#include <iostream>
#include <string>


void pass_by_copy_function(std::string str) {
	str = str + " world!";	// This is a copy, doesn't have any effect
}

void pass_by_pointer_function(std::string *str) {
	*str = *str + " world!";
}

void pass_by_reference_function(std::string &str) {
	str = str + " world!";
}

int main() {

	std::cout << "Hello world!" << std::endl;
		
	// using namespace std;	// Not the best practice
	// cout << "Hello world v2" << endl;


	// In C++ we have:
	// - pass-by-value (copy)
	// - pass-by-pointer (C-style)
	// - pass-by-reference (new in C++)
	
	std::string my_str = "Hello";	// This is a new object
	
	pass_by_copy_function(my_str);
	std::cout << my_str << std::endl;

	my_str = "Hello";
	
	pass_by_pointer_function(&my_str);
	std::cout << my_str << std::endl;

	my_str = "Hello";

	pass_by_reference_function(my_str);
	std::cout << my_str << std::endl;

	// Pass-by-reference is always preferred instead of pass-by-pointer:
	// - Safe
	// - Compiler optimizations (avoid pointer aliasing)
	
	// ---------------------------------------------------------------------
	
	// Dynamic memory allocation in C++
	// malloc/free becomes new/delete
	
	int *my_int = new int;
	*my_int = 10;
	std::cout << "My integer is: " << *my_int << std::endl;
	delete my_int;
	
	int N;
	std::cin >> N;
	
	int *my_vector = new int[N];
	// Do something...
	delete [] my_vector;
	
	// NEVER mix malloc/free with new/delete
	// Usually, you have no reasons to use the old malloc/free in C++
	
	// ... and, usually, you have no reasons to use new/delete in C++11 (see later)
	
	return 0;
}

