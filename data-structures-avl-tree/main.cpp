#include "avl-tree.h"

#include <string>
#include <random>

#include <iostream>

#define NUMBER_OF_RANDOM_NUMBERS 50

int main(int argc, char* argv[])
{
	std::random_device random_device;
	std::mt19937 mt19937(random_device());
	const std::uniform_int_distribution<std::int32_t> int8_distribution(INT8_MIN, INT8_MAX);

	std::vector<std::int32_t> random_numbers(NUMBER_OF_RANDOM_NUMBERS);
	for (auto& random_number : random_numbers)
		random_number = int8_distribution(mt19937);

	// const std::vector<std::int32_t> random_numbers = { 47, -20, -35, -108, 110, -49, 64, -101, 24, 23 };
	ds::avl_tree<std::int32_t, std::string> avl_tree;
	for (const auto& random_number : random_numbers)
		avl_tree.insert(random_number, std::to_string(random_number));

	for (const auto& random_number : random_numbers)
	{
		if (avl_tree.search(random_number) != nullptr)
			continue;

		std::cout << "ERROR";
		return EXIT_FAILURE;
	}



	return EXIT_SUCCESS;
}