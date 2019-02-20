//============================================================================
// Name        : Krish Suchdev
// Course      : CS 2336.004
// Term        : Fall 2018
// Instructor  : Jason Smith
//============================================================================

#include "Order.h"

Order::Order(int auditoriumID) {
	this->auditoriumID = auditoriumID;

	this->adults = 0;
	this->children = 0;
	this->seniors = 0;
}

void Order::addSeat(TheaterSeat *seat) {
	this->seats.push_back(seat);
	if (seat->getTicketType() == 'A') adults++;
	if (seat->getTicketType() == 'C') children++;
	if (seat->getTicketType() == 'S') seniors++;
}


void Order::removeSeat(TheaterSeat *seat) {
	this->seats.erase(find(seats.begin(), seats.end(), seat));
	if (seat->getTicketType() == 'A') adults--;
	if (seat->getTicketType() == 'C') children--;
	if (seat->getTicketType() == 'S') seniors--;
}

void Order::removeAllSeats() {
	for (TheaterSeat *seat : this->seats) {
		removeSeat(seat);
	}
}

TheaterSeat * Order::getSeat(int index) {
	return this->seats[index];
}

bool Order::containsSeat(TheaterSeat *seat) {
	return find(seats.begin(), seats.end(), seat) != seats.end();
}

bool Order::isEmpty() {
	return this->seats.empty();
}

int Order::seatsSize() {
	return static_cast<int>(this->seats.size());
}
