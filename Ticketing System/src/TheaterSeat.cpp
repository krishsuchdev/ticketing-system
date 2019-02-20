//============================================================================
// Name        : Krish Suchdev
// Course      : CS 2336.004
// Term        : Fall 2018
// Instructor  : Jason Smith
//============================================================================

#include "BaseNode.h"
#include "TheaterSeat.h"

TheaterSeat::TheaterSeat() : BaseNode() {
	this->up = 0;
	this->down = 0;
	this->left = 0;
	this->right = 0;
}

TheaterSeat::TheaterSeat(int row, char seat) : BaseNode(row, seat) {
	this->up = 0;
	this->down = 0;
	this->left = 0;
	this->right = 0;
}

TheaterSeat::TheaterSeat(int row, char seat, char ticketType) : BaseNode(row, seat, ticketType) {
	this->up = 0;
	this->down = 0;
	this->left = 0;
	this->right = 0;
}
