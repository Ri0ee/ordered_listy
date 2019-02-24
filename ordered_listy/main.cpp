#include <iostream>
#include "ordered_listy.h"

int main() {
	ordered_list<int> list_of_ints;

	for (int i = 1000000; i > 0; i--) {
		list_of_ints.add(i);
		if (i % 10000 == 0) {
			std::cout << i << "\n";
		}
	}
	std::cout << "added\n";


	//for (auto it = list_of_ints.begin(); it != list_of_ints.end(); it++) {
	//	std::cout << *it << " ";
	//}
	//std::cout << "\n";

	std::cout << list_of_ints.distance(list_of_ints.begin(), list_of_ints.find_first(10)) << "\n";

	system("PAUSE");
	return 0;
}