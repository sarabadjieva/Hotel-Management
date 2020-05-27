#include "Room.h"

Room::Room(short unsigned number, unsigned b) :key(0)
{
	setRoomNumber(number);
	setBeds(b);
}

Room::Room(const Room& r)
{
	if (!reservations.empty())
	{
		reservations.clear();
		guests.clear();
		notes.clear();
	}
	setBeds(r.getBeds());
	setRoomNumber(r.getRoomNumber());
	reservations = r.reservations;
	guests = r.guests;
	notes = r.notes;
	key = r.key;
}

Room& Room::operator=(const Room& r)
{
	if (this != &r)
	{
		if (!reservations.empty())
		{
			reservations.clear();
			guests.clear();
			notes.clear();
		}
		setBeds(r.getBeds());
		setRoomNumber(r.getRoomNumber());
		reservations = r.reservations;
		guests = r.guests;
		notes = r.notes;
		key = r.key;
	}

	return *this;
}

Room::~Room()
{
	if (!reservations.empty())
	{
		reservations.clear();
		guests.clear();
		notes.clear();
	}
}


void Room::setBeds(short unsigned b)
{
	if (b >= 0)
	{
		this->beds = b;
	}
	else
	{
		std::cerr << "There must be minimum 0 beds in the room!\n";
		this->beds = 0;
	}
}

void Room::setRoomNumber(unsigned n)
{
	if (n >= 0)
	{
		this->roomNumber = n;
	}
	else
	{
		std::cerr << "The room number is not valid, so it is 0 now!\n";
		this->roomNumber = 0;
	}
}

bool Room::addReservation(const Date& d1, const Date& d2, const char* note, int g)
{
	if (g > getBeds())
	{
		std::cout << "Not enough beds!\n";
		return false;
	}

	std::vector<Date> reserved = getDatesBetween(d1, d2);
	if (reserved.empty() || reserved.size() == 1)
	{
		std::cout << "Enter different dates!\n";
		return false;
	}

	for (auto x : reserved)
	{
		if (!getAvailabilityAt(x))
		{
			std::cerr << "Could not add this reservation!\n";
			return false;
		}
	}


	reservations.insert(std::pair<int, std::vector<Date> >(key, reserved));
	notes.insert(std::pair < int, const char*>(key, note));
	
	if (g == -1)
	{
		guests.insert(std::pair<int, short unsigned >(key, getBeds()));
	}
	else
	{
		guests.insert(std::pair<int, short unsigned >(key, g));
	}
	
	key++;
	return true;
}

bool Room::checkout(const Date& d)
{
	if (getAvailabilityAt(d))
	{
		std::cerr << "This room is free today.\n";
		return false;
	}

	for (auto& x : reservations)
	{
		if (x.second.front() < d && d < x.second.back())
		{
			if (guests.count(x.first) == 0)
			{
				std::cerr << "This room is set for unavailable from the manager at this date! Can't do anything, sorry.\n";
				return false;
			}
			while (d < x.second.back())
			{
				x.second.pop_back();
			}
			return true;
		}
		else if (x.second.front() == d)
		{
			if (guests.count(x.first) == 0)
			{
				std::cerr << "This room is set for unavailable from the manager at this date! Can't do anything, sorry.\n";
				return false;
			}
			reservations.erase(x.first);
			guests.erase(x.first);
			notes.erase(x.first);
			return true;
		}
		else if (x.second.back() == d)
		{
			if (guests.count(x.first) == 0)
			{
				std::cerr << "This room is set for unavailable from the manager at this date! Can't do anything, sorry.\n";
				return false;
			}
			return true;
		}
	}

	return false;
}

void Room::unavailable(const Date& d1, const Date& d2, const char* note)
{
	std::vector<Date> reserved = getDatesBetween(d1, d2);
	if (reserved.empty())
	{
		std::cout << "Enter different dates!";
		return;
	}

	for (auto x : reserved)
	{
		if (!getAvailabilityAt(x))
		{
			std::cerr << "This room was reserved on date "<< x <<",tell the client to checkin another room!\n";
			checkout(x);
		}
	}

	std::cout << "Room " << roomNumber << " is set for unavailable from " << d1 << " to " << d2 << ".\n";
	reservations.insert(std::pair<int, std::vector<Date> >(key, reserved));
	notes.insert(std::pair < int, const char*>(key, note));
	key++;
}

bool Room::getAvailabilityAt(const Date& d) const
{
	for (auto x : reservations)
	{
		if ((x.second.front() < d && d < x.second.back()) || x.second.front() == d || x.second.back() == d)
		{
			return false;
		}		
	}
	return true;
}

std::vector<std::stringstream*> Room::getReservations(const Date& d1, const Date& d2) const
{
	std::vector<std::stringstream*> reserved;

	for (auto x : reservations)
	{
		std::stringstream* ss = new std::stringstream;

		if (x.second.front() == d1 || x.second.front() == d2 || (x.second.front() < d1 && d1 <x.second.back())
			|| x.second.back() == d1 || x.second.back() == d2 || (x.second.front()<d2 && d2<x.second.back())
			||d1 < x.second.front() && x.second.back() < d2)
		{
			
			if (guests.count(x.first) == 0)
			{
				*ss << "unavailable";
				reserved.push_back(ss);
				return reserved;
			}
			else
			{
				*ss << x.second.front() << DELIMITER << x.second.back() << DELIMITER << guests.at(x.first);
				reserved.push_back(ss);

			}
		}
	}

	return reserved;
}

int Room::report(const Date& d1, const Date& d2)
{
	std::vector <Date> interval = getDatesBetween(d1, d2);
	int countDays = 0;
	for (auto x : interval)
	{
		if (!getAvailabilityAt(x))
		{
			countDays++;
		}
	}
	return countDays;
}

std::ostream& operator<<(std::ostream& os, const Room& r)
{
	os << "Room" <<DELIMITER << r.getRoomNumber() << DELIMITER << "with" << DELIMITER << r.getBeds() << " beds\n";
	for (auto& x : r.reservations)
	{
		os << x.second.front() << DELIMITER_ << x.second.back() << DELIMITER;
		if (r.guests.count(x.first) == 1)
		{
			os<< r.guests.at(x.first) << DELIMITER << r.notes.at(x.first) << '\n';
		}
		else
		{
			os << r.notes.at(x.first) << '\n';
		}
	}

	return os << '\n';
}

std::istream& operator>>(std::istream& is, Room& r)
{
	char temp[10];
	unsigned rn;
	unsigned short b;

	is.getline(temp, 10, DELIMITER);
	is >> rn;
	is.get();
	is.getline(temp, 10,DELIMITER);
	is >> b;
	is.get();
	is.getline(temp, 10);
	
	r.setRoomNumber(rn);
	r.setBeds(b);

	Date d1, d2;
	short unsigned guests;
	char* note = new char[MAX_NOTE_LENGTH];
	while (is.peek() >= '0' && is.peek() <= '9') //while date
	{
		is >> d1;
		is.get(); //delimiter_
		is >> d2;
		is.get();
		
		if (is.peek() >= '0' && is.peek() <= '9') //if guests
		{
			is >> guests;
			is.get();
			is.getline(note, MAX_NOTE_LENGTH);
			r.addReservation(d1, d2, note, guests);
		}	
		else
		{
			is.getline(note, MAX_NOTE_LENGTH);
			r.unavailable(d1, d2, note);
		}
		note = new char[MAX_NOTE_LENGTH];
	}

	delete[] note;
	is.get();
	return is;
}