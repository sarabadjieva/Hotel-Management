#include "Manager.h"


int main()
{

	std::cout << "Do you want to work with the default text file? (y/n)\n";
	char choice;
	std::cin >> choice;
	std::cin.clear();
	std::cin.ignore();
	if (choice == 'n')
	{
		std::cout << "What is the name of the file?\n";
		char filename[FILENAME_MAX];
		std::cin >> filename;
		std::cin.clear();
		std::cin.ignore();
		Manager m(filename);
		m.callManager();
	}

	Manager m;
	m.callManager();

	return 0;
}