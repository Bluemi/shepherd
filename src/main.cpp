#include <iostream>

#include <netsi/util/blocking_queue.hpp>

int main() {
	netsi::blocking_queue<std::string> my_queue;
	my_queue.push("hello");
	std::cout << my_queue.pop() << std::endl;
	return 0;
}
