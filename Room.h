#pragma once
#include "Date.h"

const unsigned MAX_NOTE_LENGTH = 200;

class Room
{
private:
	short unsigned beds;
	unsigned roomNumber;
	int key;
	std::map< int,std::vector<Date> > reservations; //reservations + unavailable
	std::map<int,short unsigned> guests;
	std::map<int, const char*> notes;

public:
	Room(short unsigned number = 0, unsigned b = 0);
	Room(const Room&);
	Room& operator=(const Room&);
	~Room();

	void setBeds(short unsigned);
	void setRoomNumber(unsigned);

	short unsigned getBeds() const { return this->beds; };
	unsigned getRoomNumber() const { return this->roomNumber; };
	bool getAvailabilityAt(const Date&) const;

	bool addReservation(const Date&, const Date&,  const char*, int g = -1);
	bool checkout(const Date& d = Date());
	void unavailable(const Date&, const Date&, const char*);

	std::vector<std::stringstream*> getReservations(const Date&, const Date&) const;
	int report(const Date&, const Date&);

	friend std::ostream& operator<<(std::ostream&, const Room&);
	friend std::istream& operator>>(std::istream&, Room&);
};
