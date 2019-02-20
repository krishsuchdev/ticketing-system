//============================================================================
// Name        : Krish Suchdev
// Course      : CS 2336.004
// Term        : Fall 2018
// Instructor  : Jason Smith
//============================================================================

#ifndef ORDER_H_
#define ORDER_H_

#include <vector>
#include <string>
#include "TheaterSeat.h"

class Order {
public:
	Order(int auditoriumNumber);

	std::string user;

	int auditoriumID;

	int adults;
	int children;
	int seniors;

	void addSeat(TheaterSeat *seat);
	void removeSeat(TheaterSeat *seat);
	void removeAllSeats();
	TheaterSeat * getSeat(int index);
	bool containsSeat(TheaterSeat *seat);
	bool isEmpty();
	int seatsSize();

private:
	std::vector<TheaterSeat *> seats;
};

#endif /* ORDER_H_ */
