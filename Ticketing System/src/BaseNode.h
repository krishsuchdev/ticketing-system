//============================================================================
// Name        : Krish Suchdev
// Course      : CS 2336.004
// Term        : Fall 2018
// Instructor  : Jason Smith
//============================================================================

#ifndef BASENODE_H_
#define BASENODE_H_

class BaseNode {
private:
	int row;
	char seat;
	bool reserved;
	char ticketType;

public:
	BaseNode();
	BaseNode(int row, char seat);
	BaseNode(int row, char seat, char ticketType);

	bool isReserved() {
		return reserved;
	}

	int getRow() {
		return row;
	}

	void setRow(int row) {
		this->row = row;
	}

	char getSeat() {
		return seat;
	}

	void setSeat(char seat) {
		this->seat = seat;
	}

	char getTicketType() {
		return ticketType;
	}

	void setTicketType(char ticketType) {
		this->reserved = ticketType != '.';
		this->ticketType = ticketType;
	}
};

#endif /* BASENODE_H_ */
