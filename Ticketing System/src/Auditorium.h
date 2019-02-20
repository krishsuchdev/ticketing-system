//============================================================================
// Name        : Krish Suchdev
// Course      : CS 2336.004
// Term        : Fall 2018
// Instructor  : Jason Smith
//============================================================================

#ifndef AUDITORIUM_H_
#define AUDITORIUM_H_

#include "TheaterSeat.h"

class Auditorium {
private:
	TheaterSeat *first;
	int rows = -1, columns = -1;

public:
	Auditorium(int id, std::string filename);
	~Auditorium();

	int id;

	TheaterSeat* getSeat(int row, char seat);

	TheaterSeat*& getFirst() {
		return first;
	}

	void setFirst(TheaterSeat*& first) {
		this->first = first;
	}

	int getColumns() {
		return columns;
	}

	void setColumns(int columns) {
		this->columns = columns;
	}

	int getRows() {
		return rows;
	}

	void setRows(int rows) {
		this->rows = rows;
	}
};


#endif /* AUDITORIUM_H_ */
