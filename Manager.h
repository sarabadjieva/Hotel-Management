#pragma once
#include "Room.h"

const char ROOMS_FILE[] = "rooms.txt";
const char TEMPORARY_FILE[] = "temp.txt";

class Manager
{
private:
	std::fstream file;
	std::fstream temp;
	char fileName[FILENAME_MAX];
	char tempName[FILENAME_MAX];
	std::vector<Room*> rooms; 

public:
	Manager(const char* f = nullptr);
	~Manager();

	void readRooms();
	void writeRooms();

	void callManager();

	void open();
	void close();
	void save();
	void saveas();
	void help();
	void exitFunction(); //had to rename it, because when calling exit(), the program called this function

	void workwithfile();
	void checkin(unsigned rn, const Date&, const Date&, const char*, int guests = -1); 
	void availabilty(const Date& d = Date()); //the default constructor sets the current date
	void checkout(unsigned rn);
	void report(const Date&, const Date&);
	bool find(short unsigned b, const Date&, const Date&);
	void find_(short unsigned g, const Date&, const Date&);
	void unavailable (unsigned rn, const Date&, const Date&, const char*); 
};

