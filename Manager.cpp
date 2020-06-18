#include "Manager.h"

Manager::Manager(const char* f)
{
	if (f == nullptr)
	{
		strcpy_s(fileName, FILENAME_MAX, ROOMS_FILE);
	}
	else
	{
		strcpy_s(fileName, FILENAME_MAX, f);
	}

	strcpy_s(tempName, FILENAME_MAX, TEMPORARY_FILE);
}

Manager::~Manager()
{
	for (auto& x : rooms)
	{
		delete x;
	}
}

void Manager::readRooms()
{
	if (!rooms.empty())
	{
		rooms.clear();
	}

	temp.open(tempName);

	//check if empty
	temp.seekp(0, std::ios::end);
	int size = (int)temp.tellg();
	if (size == 0)
	{
		std::cout << "File is empty.\n";
		temp.close();
		return;
	}

	temp.clear();
	temp.seekg(0, std::ios::beg);
	Room* r = new Room;
	while (temp)
	{
		while (temp >> *r)
		{
			rooms.push_back(r); 
			r = new Room;
		}
	}

	temp.close();
}

void Manager::writeRooms()
{
	temp.open(tempName, std::ios::out | std::ios::trunc);

	for (auto x : rooms)
	{
		temp << *x;
	}

	temp.close();
}

void Manager::callManager()
{

	std::cout << "Choose what you want to do:\nopen, close, save, saveas, help, exit.\n";

	char choice[20];
	std::cin >> choice;
	std::cin.clear();
	std::cin.ignore();

	if (strcmp(choice, "open") == 0)
	{
		open();
		workwithfile();
	}
	else if (strcmp(choice, "close") == 0)
	{
		close();
	}
	else if (strcmp(choice, "save") == 0)
	{
		save();
		callManager();
	}
	else if (strcmp(choice, "saveas") == 0)
	{
		saveas();
		callManager();
	}
	else if (strcmp(choice, "help") == 0)
	{
		help();
		callManager();
	}
	else if (strcmp(choice, "exit") == 0)
	{
		exitFunction();
	}

	else callManager();
}


void Manager::open()
{
	file.open(fileName);
	if (!file)
	{
		std::cerr << "File couldn’t be opened!\n";
		callManager();
	}

	std::ofstream temp(tempName);
	if (!temp.is_open())
	{
		std::cerr << "Temporary file couldn’t be opened!\n";
		callManager();
	}

	file.seekg(0);
	temp << file.rdbuf();

	file.close();
	temp.close();

	readRooms();
}

void Manager::close()
{
	if (file.is_open())
	{
		file.close(); // file contains the original text
	}

	if (temp.is_open())
	{
		temp.close();
	}

	remove(tempName);
	system("pause");
	system("cls");

	open();
}

void Manager::save()
{
	std::ofstream file(fileName, std::ios::trunc);
	temp.open(tempName);
	file << temp.rdbuf();

	file.close();
	temp.close();

	remove(tempName);
	system("pause");
	system("cls");
}

void Manager::saveas()
{
	std::cout << "How do you want to be the name of the new file?:\n";
	char newName[FILENAME_MAX];
	std::cin >> newName;
	std::cin.clear();
	std::cin.ignore();

	std::ofstream newFile(newName, std::ios::out | std::ios::trunc);

	if (!newFile)
	{
		std::cout << "New file can't be opened, try again!\n\n";
		saveas();
	}

	temp.open(tempName);
	newFile << temp.rdbuf();
	newFile.close();
	temp.close();

	if (file.is_open())
	{
		file.close();
	}

	remove(tempName);
}

void Manager::help()
{
	std::cout << "The following commands are supported:\n"
		<< "-------------------------------------\n"
		<< "open <file>\topens <file>\n"
		<< "close\t\tcloses currently opened file\n"
		<< "save\t\tsaves the currently open file\n"
		<< "saveas <file>\tsaves the currently open file in <file>\n"
		<< "help\t\tprints this information\n"
		<< "exit\t\texists the program\n"
		<< "exit\t\texists the program\n"
		<< "----------------------------------\n";
	system("pause");
	system("cls");
}

void Manager::exitFunction()
{
	if (file.is_open())
	{
		std::cout << "You haven't closed the opened files, so i will!\n";
		file.close();
	}

	if (temp.is_open())
	{
		std::cout << "You haven't closed the opened files, so i will!\n";
		temp.close();
	}

	remove(tempName);
	exit(0);
}

void Manager::workwithfile()
{
	std::cout << "\nPlease, enter function with parameters:";
	char function[1000];
	std::cin.getline(function, 1000);
	std::stringstream ss(function);

	char word[MAX_NOTE_LENGTH];
	int wordCount = 0;
	while (ss >> word)
	{
		wordCount++;
	}
	if (wordCount == 0)
	{
		std::cerr << "Please write something!\n";
		workwithfile();
	}

	ss.clear();
	ss.seekg(0, std::ios::beg);
	char functionName[20];
	ss.getline(functionName, 20, DELIMITER);

	if (strcmp(functionName, "checkin") == 0)
	{
		unsigned rn;
		Date d1, d2;
		char note[MAX_NOTE_LENGTH];

		if (wordCount < 5)
		{
			std::cout << "Wrong parameters!\n";
			workwithfile();
		}

		ss >> rn >> d1 >> d2;
		ss.get();

		char digits[11] = "0123456789";
		char c = ss.str().back();
		if (strchr(digits,c) != NULL)
		{
			ss.getline(note, MAX_NOTE_LENGTH, DELIMITER);
			wordCount -= 5;

			char temp[MAX_NOTE_LENGTH];
			while (wordCount != 1)
			{
				ss.getline(temp, MAX_NOTE_LENGTH - strlen(note), DELIMITER);
				strncat_s(note, &DELIMITER, 1);
				strncat_s(note, temp, strlen(temp));
				wordCount--;
			}

			unsigned guests;
			ss >> guests;
			checkin(rn, d1, d2, note, guests);
		}
		else
		{	
			ss.getline(note, MAX_NOTE_LENGTH);
			checkin(rn, d1, d2, note);
		}	

		writeRooms();
	}
	else if (strcmp(functionName, "availability") == 0)
	{
		if (wordCount == 1)
		{
			availabilty();
		}
		else if (wordCount == 2)
		{
			Date d;
			ss >> d;
			availabilty(d);
		}
		else
		{
			std::cout << "Wrong parameters!\n";
			workwithfile();
		}
	}
	else if (strcmp(functionName, "checkout") == 0)
	{
		if (wordCount != 2)
		{
			std::cerr << "Wrong parameters!\n";
			workwithfile();
		}

		unsigned rn;
		ss >> rn;

		checkout(rn);
		writeRooms();
	}
	else if (strcmp(functionName, "report") == 0)
	{
		if (wordCount != 3)
		{
			std::cerr << "Wrong parameters!\n";
			workwithfile();
		}

		Date d1, d2;
		ss >> d1 >> d2;
		report(d1, d2);
	}
	else if (strcmp(functionName, "find") == 0)
	{
		if (wordCount != 4)
		{
			std::cerr << "Wrong parameters!\n";
			workwithfile();
		}

		unsigned short b;
		Date d1, d2;
		ss >> b >> d1 >> d2;
		find(b, d1, d2);
	}
	else if (strcmp(functionName, "find!") == 0)
	{
		if (wordCount != 4)
		{
			std::cerr << "Wrong parameters!\n";
			workwithfile();
		}

		unsigned short b;
		Date d1, d2;
		ss >> b >> d1 >> d2;
		
		find_(b, d1, d2);
		writeRooms();
	}
	else if (strcmp(functionName, "unavailable") == 0)
	{
		if (wordCount < 5)
		{
			std::cout << "Wrong parameters!\n";
			workwithfile();
		}

		unsigned int rn;
		Date d1, d2;
		char note[MAX_NOTE_LENGTH];
		ss >> rn >> d1 >> d2;
		ss.getline(note, MAX_NOTE_LENGTH);

		unavailable(rn, d1, d2, note);
		writeRooms();
	}
	else
	{
		std::cout << "Wrong input!\n\n";
		workwithfile();
	}

	std::cout << "\nDo you want to work with the same file more(y/n)\n";
	char answer;
	std::cin >> answer;
	std::cin.clear();
	std::cin.ignore();
	if (answer == 'y')
	{
		workwithfile();
	}
	else callManager();
}

void Manager::checkin(unsigned rn, const Date& d1, const Date& d2, const char* note, int guests)
{
	if (!d1.isValidDate() || d1 < Date() || !d2.isValidDate() || d2 < Date() || d1 > d2)
	{
		std::cerr << "Enter valid future dates, from which the first one is before the second one!\n";
		return;
	}

	if (guests == 0 || guests < -1)
	{
		std::cerr << "I can't check in 0 guests!\n";
		return;
	}

	for (auto& x : rooms)
	{
		if (x->getRoomNumber() == rn )
		{
			if (!x->addReservation(d1, d2, note, guests)) 
			{
				std::cout << "Try finding another room with the function 'find'.\n";
				return;
			}

			//if 'add reservation' was true, this reservation will be saved in the memory
			if (guests == -1)
			{
				std::cout << "Reservation successfully added from " << d1 << " to " << d2 << " with all the beds taken.\n";
			}
			else
			{
				std::cout << "Reservation successfully added from " << d1 << " to " << d2 << " for " << guests << " guests.\n";
			}
			
			return;
		}
	}
	std::cerr << "Room not found, choose another one!\n";
}

void Manager::availabilty(const Date& d)
{
	if (!d.isValidDate())
	{
		std::cerr << "Enter valid future dates, from which the first one is before the second one!\n";
		return;
	}

	for (auto x : rooms)
	{
		if (x->getAvailabilityAt(d))
		{
			std::cout <<x->getRoomNumber() << " is free at this date.\n";
		}
	}
}

void Manager::checkout(unsigned rn)
{
	for (auto x : rooms)
	{
		if (x->getRoomNumber() == rn)
		{
			if (x->checkout())
			{
				std::cout << "Successfully checked out client.\n";
			}
			return;
		}
	}
	std::cerr << "Room not found, choose another one!\n";
}

void Manager::report(const Date& d1, const Date& d2)
{
	if (!d1.isValidDate() || d1 < Date() || !d2.isValidDate() || d2 < Date() || d1 > d2)
	{
		std::cerr << "Enter valid future dates, from which the first one is before the second one!\n";
		return;
	}

	bool foundReserved = false;
	for (auto x : rooms)
	{
		int count = x->report(d1, d2);
		if (count != 0)
		{
			foundReserved = true;
			std::cout << x->getRoomNumber() << " is reserved/unavailable for " << count << " days in this period.\n";
		}
	}
	if (!foundReserved)
	{
		std::cout << "No rooms were busy these days!\n";
	}
}

bool Manager::find(short unsigned g, const Date& d1, const Date& d2)
{
	if (!d1.isValidDate() || d1 < Date() || !d2.isValidDate() || d2 < Date() || d1 > d2)
	{
		std::cerr << "Enter valid future dates, from which the first one is before the second one!\n";
		return false;
	}

	std::vector<Date> interval = getDatesBetween(d1, d2);
	std::vector<Room> good;
	bool foundEqual = false;
	bool free = true;
	for (auto x : rooms)
	{
		free = true;
		for (auto y : interval)
		{
			if (!x->getAvailabilityAt(y))
			{
				free = false;
				break;
			}
		}

		if (g == x->getBeds() && free)
		{
			foundEqual = true;
			good.push_back(*x);
		}
		else if (g < x->getBeds() && free)
		{
			good.push_back(*x);
		}
	}
	
	if (good.size() == 0) return false;

	std::cout << "The rooms I can check in for " << g << " guests are:";
	for (auto x : good)
	{
		if (foundEqual && x.getBeds() == g)
		{
			std::cout<<DELIMITER << x.getRoomNumber();
		}
		else if (!foundEqual)
		{
			std::cout <<DELIMITER<< x.getRoomNumber();
		}
	}
	std::cout << "\n\n";

	return true;
}

void Manager::find_(short unsigned g, const Date& d1, const Date& d2)
{
	if (!d1.isValidDate() || d1 < Date() || !d2.isValidDate() || d2 < Date() || d1 > d2)
	{
		std::cerr << "Enter valid future dates, from which the first one is before the second one!\n";
		return;
	}

	std::vector<Room* > roomsForTheVIP;
	
	//check which rooms have enough beds
	for (auto x : rooms)
	{
		if (g <= x->getBeds())
		{
			roomsForTheVIP.push_back(x);
		}
	}

	//check details about the reservations from d1 to d2 for each room
	bool good = true;
	for (std::vector<Room*>::iterator it = roomsForTheVIP.begin(); it != roomsForTheVIP.end();) {

		good = true;
		std::vector<std::stringstream*> reservations = (*it)->getReservations(d1, d2);
		
		//if the room has more than 2 reservations in the wanted period
		if (reservations.size() > 2)
		{
			it =roomsForTheVIP.erase(it);
			good = false;
			continue;
		}
		
		//if the room is set for unavailable in this period
		for (auto x : reservations)
		{
			if (strcmp("unavailable", x->str().c_str()) == 0)
			{
				it =roomsForTheVIP.erase(it);
				good = false;
				break;
			}
		}
		if (!good) continue;
		
		//check if we can find another room for the people, who reserved the wanted room in this period
		for (auto x : reservations)
		{
			
			Date db, de;
			int g = 0;
			*x >> db >> de >> g;
			std::cout << "Checking if a room is available for the guests in #" << (*it)->getRoomNumber() << " from " <<d1 << " to " <<d2<< "...\n";
			if (!find(g, d1, d2))
			{
				it =roomsForTheVIP.erase(it);
				good = false;
				std::cout << "Can't find a room!\n\n";
				break;
			}			
		}
		if (good) ++it;
	}

	if (roomsForTheVIP.size() == 0)
	{
		std::cout << "Can't free a room in this period.\n\n" ;
	}

	std::cout << "\nThe room(s) the VIP can be checked in is/are:";
	for (auto x : roomsForTheVIP)
	{
		std::cout <<DELIMITER<< (*x).getRoomNumber();
	}
	std::cout << "\n\n";

	std::cout << "Do you want to add a reservation(y/n)\n";
	char answer;
	std::cin >> answer;
	std::cin.clear();
	std::cin.ignore();
	if (answer == 'y')
	{
		std::cout << "Which room to reserve?\n";
		unsigned int choice;
		std::cin >> choice;
		std::cin.clear();
		std::cin.ignore();
		bool found = false;
		for (auto x : roomsForTheVIP)
		{
			if (x->getRoomNumber() == choice)
			{
				found = true;
				std::vector<std::stringstream*> reservations = x->getReservations(d1, d2);
				for (auto y : reservations)
				{
					Date d;
					*y >> d;
					x->checkout(d);
				}
				char* note = new char[MAX_NOTE_LENGTH];
				std::cout << "Enter a note: ";
				std::cin.getline(note, MAX_NOTE_LENGTH);
				std::cout << "\nDon't forget to change the old reservations!\n";
				x->addReservation(d1, d2, note, g);
				break;
			}
		}

		if (!found) std::cout << "Not an available room!\n";
	}

	std::cout << '\n';
}

void Manager::unavailable(unsigned int rn, const Date& d1, const Date& d2, const char* note)
{
	if (!d1.isValidDate() || d1 < Date() || !d2.isValidDate() || d2 < Date() || d1 > d2)
	{
		std::cerr << "Enter valid future dates, from which the first one is before the second one!\n";
		return;
	}

	for (auto x : rooms)
	{
		if (x->getRoomNumber() == rn)
		{
			x->unavailable(d1,d2,note);
			return;
		}
	}

	std::cout << "This room doesn't exist!\n";
}
