#pragma once

#include <thread>

unsigned int get_dop() {
	auto processor_count = std::thread::hardware_concurrency();
	return processor_count == 0 ? 1 : processor_count;
}
