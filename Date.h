#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iterator>
#include <ctime>

const int DATE_LENGTH = 11;

const char UNKNOWN[] = "Unknown";
const char DELIMITER = ' ';
const char DELIMITER2 = '-';
const char DELIMITER3 = '.';
const char DELIMITER4 = '$';
const char DELIMITER5 = '|';
const char DELIMITER_ = '_';


class Date
{
private:
	unsigned int year;
	unsigned int month;
	unsigned int day;
	bool validDate;
public:

	//the default values are 1, because I set the date to today's date if no parameters are given
	//and they are not zero because of the second if-function, which would set validDate to false
	Date(unsigned y = 1, unsigned m = 1, unsigned d = 1);
	Date(const Date&);
	Date& operator=(const Date&);
	~Date() {};

	const unsigned getYear() const { return this->year; }
	const unsigned getMonth() const { return this->month; }
	const unsigned getDay() const { return this->day; }
	bool isValidDate() const { return this->validDate; };

	void setYear(const unsigned);
	void setMonth(const unsigned);
	void setDay(const unsigned);

	unsigned daysInMonth(unsigned, unsigned) const;
	Date& nextDate() const;

	bool operator==(const Date&) const;
	bool operator<(const Date&) const;
	bool operator>(const Date&) const;

	friend std::ostream& operator<<(std::ostream&, const Date&);
	friend std::istream& operator>>(std::istream&, Date&);
	friend std::vector<Date> getDatesBetween(const Date&, const Date&);
};
