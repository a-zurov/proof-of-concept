// modification_order.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <atomic>
#include <thread>
#include <cassert>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x() {
	x.store(true);
}

void write_y() {
	y.store(true);
}

void read_x_y() {

	while (!x.load()) {
		std::this_thread::yield();
	};
	if (y.load()) ++z;
}

void read_y_x() {

	while (!y.load()) {
		std::this_thread::yield();
	};
	if (x.load()) ++z;
}

int main() {

	x = false;
	y = false;
	z = 0;

	std::thread th1(write_x);
	std::thread th2(write_y);
	std::thread th3(read_x_y);
	std::thread th4(read_y_x);
	th1.join();
	th2.join();
	th3.join();
	th4.join();

	assert(z.load() != 0);
}