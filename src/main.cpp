#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>

// run in separate thread
void raytrace_scene();

// run in main thread
void wait_for_input();

std::thread t;
std::mutex mut;

bool done = false;

int main()
{
	t = std::thread(raytrace_scene);
	std::string trash; // input that won't get used
	while (!done) {
		std::getline(std::cin, trash); // wait for user to hit enter
		wait_for_input();
	}
	return 0;
}

void raytrace_scene()
{
	std::chrono::seconds trace_duration(1);
	std::chrono::microseconds wait_duration(1);
	mut.lock();
	while (!done) {
		std::cout << "Ray tracing scene... (enter any input to pause)" << std::endl;
		std::this_thread::sleep_for(trace_duration);
		mut.unlock(); // possibly defer to wait_for_input
		// sleep for extremely small duration to give main thread time to register inputs
		std::this_thread::sleep_for(wait_duration);
		mut.lock(); // pick up when wait_for_input has given back control
	}
}

void wait_for_input()
{
	mut.lock();
	std::cout << "Pausing ray trace..." << std::endl;
	std::cout << "Enter 'q' to quit or anything else to continue." << std::endl;
	auto c = (char)getchar();
	if (c == 'q' || c == 'Q') {
		done = true;
		mut.unlock();
		if (t.joinable()) {
			// wait for raytrace_scene thread to finish
			t.join();
		}
		return;
	}
	std::cout << "Resuming ray trace..." << std::endl;
	mut.unlock(); // return control to raytrace_scene
}