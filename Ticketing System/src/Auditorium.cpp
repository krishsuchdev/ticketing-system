//============================================================================
// Name        : Krish Suchdev
// Course      : CS 2336.004
// Term        : Fall 2018
// Instructor  : Jason Smith
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include "Auditorium.h"
#include "TheaterSeat.h"

// Reads file to auditorium
Auditorium::Auditorium(int id, std::string filename) {
	this->id = id;
	std::ifstream input(filename);

	this->first = 0;
	TheaterSeat *lastTheaterSeat;

	if (input.is_open()) {
		int row = 0;
		char seat = 'A';
		char let;
		while (input.get(let)) {
			// Making sure newline characters do not throw the program out of balance
			if (let == '\n' || let == '\r') {
				if (this->columns == -1)
					this->columns = lastTheaterSeat->getSeat() - 'A' + 1;
				row++;
				seat = 'A';
			} else {
				TheaterSeat *newTheaterSeat = new TheaterSeat(row + 1, seat, let);
				if (this->first == 0)
					first = newTheaterSeat;
				// Sets right and left if not first column
				else if (seat != 'A') {
					newTheaterSeat->setLeft(lastTheaterSeat);
					lastTheaterSeat->setRight(newTheaterSeat);
				}
				// Sets above and below if not first row
				if (row > 0) {
					TheaterSeat *aboveTheaterSeat;
					if (seat == 'A') {
						aboveTheaterSeat = lastTheaterSeat;
					} else {
						aboveTheaterSeat = lastTheaterSeat->getUp()->getRight();
					}
					while (aboveTheaterSeat->getSeat() != newTheaterSeat->getSeat()) {
						aboveTheaterSeat = aboveTheaterSeat->getLeft();
					}
					aboveTheaterSeat->setDown(newTheaterSeat);
					newTheaterSeat->setUp(aboveTheaterSeat);
				}
				lastTheaterSeat = newTheaterSeat;
				seat++;
			}
		}
		this->rows = lastTheaterSeat->getRow();
	} else {
		// Message outputted if file cannot be found or read for whatever reason
		std::cout << "ERROR: Unable to Open Input File" << std::endl;
	}
}

// Gets the theater seat node pointer at specified row and seat
TheaterSeat* Auditorium::getSeat(int row, char seat) {
	TheaterSeat *node = this->first;
	while (node->getRow() != row) { node = node->getDown(); }
	while (node->getSeat() != seat) { node = node->getRight(); }
	return node;
}
