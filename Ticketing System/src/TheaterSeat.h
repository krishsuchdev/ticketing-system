//============================================================================
// Name        : Krish Suchdev
// Course      : CS 2336.004
// Term        : Fall 2018
// Instructor  : Jason Smith
//============================================================================

#ifndef THEATERSEAT_H_
#define THEATERSEAT_H_

#include "BaseNode.h"

class TheaterSeat : public BaseNode {
private:
	TheaterSeat *up;
	TheaterSeat *down;
	TheaterSeat *left;
	TheaterSeat *right;

public:
	TheaterSeat();
	TheaterSeat(int row, char seat);
	TheaterSeat(int row, char seat, char ticketType);

	TheaterSeat* getDown() {
		return down;
	}

	void setDown(TheaterSeat* down) {
		this->down = down;
	}

	TheaterSeat* getLeft() {
		return left;
	}

	void setLeft(TheaterSeat* left) {
		this->left = left;
	}

	TheaterSeat* getRight() {
		return right;
	}

	void setRight(TheaterSeat* right) {
		this->right = right;
	}

	TheaterSeat* getUp() {
		return up;
	}

	void setUp(TheaterSeat* up) {
		this->up = up;
	}
};

#endif /* THEATERSEAT_H_ */
