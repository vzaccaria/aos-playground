#include <memory>

void test_function_2(std::shared_ptr<Sensor> sensor) {
	// DO something with sensor
}

void test_function() {
	std::shared_ptr<Sensor> s = std::make_shared<Sensor>(123, "test");	// Dynamically allocated
	test_function_2(s);
	s->get_id();
}	// Destroyed here

int main() {

	Sensor *test = new Sensor(123, "test");
	
	// ...
	
	delete test;

	test_function();

	return 0;
}
