//============================================================================
// Name        : Krish Suchdev
// Course      : CS 2336.004
// Term        : Fall 2018
// Instructor  : Jason Smith
//============================================================================

#include "BaseNode.h"

BaseNode::BaseNode() {
	this->row = -1;
	this->seat = '0';
	this->reserved = false;
	this->ticketType = '.';
}

BaseNode::BaseNode(int row, char seat) {
	this->row = row;
	this->seat = seat;
	this->reserved = false;
	this->ticketType = '.';
}

BaseNode::BaseNode(int row, char seat, char ticketType) {
	this->row = row;
	this->seat = seat;
	this->reserved = ticketType != '.';
	this->ticketType = ticketType;
}
