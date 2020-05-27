#include "Date.h"

Date::Date(unsigned y, unsigned m, unsigned d) : year(y), month(m), day(d), validDate(true)
{
	if (y == 1 && m == 1 && d == 1) //default values
	{
		time_t t = time(NULL);
		tm* tPtr = new tm;
		localtime_s(tPtr, &t);
		setYear(tPtr->tm_year + 1900);
		setMonth(tPtr->tm_mon + 1);
		setDay(tPtr->tm_mday);
	}

	unsigned maxDays = daysInMonth(y, m);
	if (m < 1 || m > 12 || d<1 || d> maxDays) //valid date
	{
		std::cerr << "Not valid date!\n";
		validDate = false;
	}

};

Date::Date(const Date& d)
{
	setYear(d.getYear());
	setMonth(d.getMonth());
	setDay(d.getDay());
};

Date& Date::operator=(const Date& d)
{
	if (this != &d)
	{
		setYear(d.getYear());
		setMonth(d.getMonth());
		setDay(d.getDay());
	}
	return *this;
}

void Date::setYear(const unsigned y)
{
	this->year = y;
}

void Date::setMonth(const unsigned m)
{
	this->month = m;
}

void Date::setDay(const unsigned d)
{
	this->day = d;
}

unsigned Date::daysInMonth(unsigned y, unsigned m) const
{

	if (m == 2)
	{
		if ((y % 400 == 0) || (y % 4 == 0 && y % 100 != 0))
			return 29;
		else
			return 28;
	}
	else if (m == 4 || m == 6 || m == 9 || m == 11)
	{
		return 30;
	}
	else return 31;
}

Date& Date::nextDate() const
{
	unsigned daysM = daysInMonth(this->year, this->month);
	Date* d;

	if (this->getDay() < daysM)
	{
		d = new Date(this->getYear(), this->getMonth(), this->getDay() + 1);
	}
	else if ((this->getMonth() == 12) && (this->getDay() == 31))
	{
		d = new Date(this->getYear() + 1, 1, 1);
	}
	else //if (this->getDay() == daysM)
	{
		d = new Date(this->getYear(), this->getMonth() + 1, 1);
	}

	return *d;
}

bool Date::operator==(const Date& d) const
{
	return (this->year == d.year && this->month == d.month && this->day == d.day);
};

bool Date::operator<(const Date& d) const
{
	if (this->year < d.year) return true;
	else if (this->year == d.year && this->month < d.month) return true;
	else return (this->year == d.year && this->month == d.month && this->day < d.day);
};

bool Date::operator>(const Date& d) const
{
	if (this->year > d.year) return true;
	else if (this->year == d.year && this->month > d.month) return true;
	else return (this->year == d.year && this->month == d.month && this->day > d.day);
};

std::ostream& operator<<(std::ostream& os, const Date& d)
{
	os << d.year << DELIMITER2;
	if (d.month < 10)
	{
		os << '0' << d.month << DELIMITER2;
	}
	else
	{
		os << d.month << DELIMITER2;
	}

	if (d.day < 10)
	{
		os << '0' << d.day;
	}
	else
	{
		os << d.day;
	}

	return os;
}

std::istream& operator>>(std::istream& is, Date& d)
{
	int year, month, day;
	is >> year;
	is.get();

	if (is.peek() == '0')
	{
		is.get();
		is >> month;
	}
	else
	{
		is >> month;
	}
	is.get();

	if (is.peek() == '0')
	{
		is.get();
		is >> day;
	}
	else
	{
		is >> day;
	}

	d.setYear(year);
	d.setMonth(month);
	d.setDay(day);

	unsigned maxDays = d.daysInMonth(year, month);
	if (d.month < 1 || d.month > 12 || d.day<1 || d.day> maxDays)
	{
		std::cerr << "Not valid date!\n";
		d.validDate = false;
	}

	return is;
}

std::vector<Date> getDatesBetween(const Date& d1, const Date& d2)
{
	std::vector<Date> interval;

	if (!d1.isValidDate() || !d2.isValidDate())
	{
		std::cerr << "Enter valid dates!\n";
		return interval;
	}

	if (d1 > d2)
	{
		std::cerr << "The second date must be after the first one!\n";
		return interval;
	}

	if (d1 == d2)
	{
		interval.push_back(d1);
		return interval;
	}

	interval.push_back(d1);
	Date* d = new Date;
	*d = d1.nextDate();
	while (*d < d2)
	{
		interval.push_back(*d);
		*d = d->nextDate();
	}
	interval.push_back(d2);

	return interval;
}
