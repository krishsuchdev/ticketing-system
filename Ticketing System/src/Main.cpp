//============================================================================
// Name        : Krish Suchdev
// Course      : CS 2336.004
// Term        : Fall 2018
// Instructor  : Jason Smith
//============================================================================

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <regex>
#include <vector>
#include <unordered_map>
#include "Auditorium.h"
#include "TheaterSeat.h"
#include "Order.h"
using namespace std;

const string A1_FILENAME = "A1.txt";
const string A2_FILENAME = "A2.txt";
const string A3_FILENAME = "A3.txt";

bool userSequence(Auditorium *auditoriums[], string filenames[], vector<Order *> *allOrders);
unordered_map<string, string> readUserDatabase();
string authenticate(unordered_map<string, string> database);
int selectOption(string username);
int selectAuditorium();
void outputOrders(vector<Order *> *orders);
void editOrder(Auditorium *auditorium, Order *order, vector<Order *> *orders, string username);
bool canReserveSeats(Auditorium *auditorium, int rowNumber, char seat, int people);
bool reserveSeats(Auditorium *auditorium, vector<Order *> *orders, int rowNumber, char seat, int adults, int children, int seniors, string username);
TheaterSeat* bestAvailableSeats(Auditorium *auditorium, int row, char seat, int people);
bool isBetterSeat(Auditorium *auditorium, double rm1, double cm1, double rm2, double cm2);
void outputSeats(Auditorium *auditorium);
void saveToFile(Auditorium *auditorium, string filename);
void generateReport(Auditorium *auditoriums[]);

int main() {
	// Setting up auditoriums and read from files
	Auditorium *auditorium1 = new Auditorium(1, A1_FILENAME);
	Auditorium *auditorium2 = new Auditorium(2, A2_FILENAME);
	Auditorium *auditorium3 = new Auditorium(3, A3_FILENAME);
	Auditorium *auditoriums[] = {auditorium1, auditorium2, auditorium3};
	string filenames[] = {A1_FILENAME, A2_FILENAME, A3_FILENAME};

	// Vector to store all orders of all users
	vector<Order *> *allOrders = new vector<Order *>();

	// Keep prompting login until admin closes
	bool sequence = true;
	while (sequence) sequence = userSequence(auditoriums, filenames, allOrders);

	return 0;
}

bool userSequence(Auditorium *auditoriums[], string filenames[], vector<Order *> *allOrders) {
	string username = authenticate(readUserDatabase());

	Auditorium *auditorium1 = auditoriums[0];
	Auditorium *auditorium2 = auditoriums[1];
	Auditorium *auditorium3 = auditoriums[2];

	vector<Order *> *orders = new vector<Order *>();
	for (Order *order : *allOrders) {
		if (order->user == username)
			orders->push_back(order);
	}

	int option = 0;

	do {
		option = selectOption(username);
		if (option == 1) {
			// Option 1: Reserve Seats

			Auditorium *auditorium = auditoriums[selectAuditorium() - 1];

			outputSeats(auditorium);

			// Prompts the user with a display to enter in the amount of tickets for each type they want to reserve.
			cout << "-- Reserve Seats --" << endl;
			int rowNumber;
			char seatLetter;
			int adults, children, seniors;

			// Handles input and checks if there is an error or it is out of bounds
			cout << "Row Number: ";
			do {
				cin >> rowNumber;
				if (cin.fail()) rowNumber = -1;
				cin.clear();
				cin.ignore();
			} while (rowNumber <= 0 || rowNumber > auditorium->getRows());
			cout << "Starting Seat Letter: ";
			do {
				cin >> seatLetter;
				if (cin.fail()) seatLetter = 0;
				cin.clear();
				cin.ignore();
			} while (seatLetter < 'A' || seatLetter > 'A' + (auditorium->getColumns() - 1));
			cout << "Adults: ";
			do {
				cin >> adults;
				if (cin.fail()) adults = -1;
				cin.clear();
				cin.ignore();
			} while (adults < 0);
			cout << "Children: ";
			do {
				cin >> children;
				if (cin.fail()) children = -1;
				cin.clear();
				cin.ignore();
			} while (children < 0);
			cout << "Seniors: ";
			do {
				cin >> seniors;
				if (cin.fail()) seniors = -1;
				cin.clear();
				cin.ignore();
			} while (seniors < 0);
			cout << endl;

			if (reserveSeats(auditorium, orders, rowNumber, seatLetter, adults, children, seniors, username)) {
				cout << "Your seats have been successfully reserved!" << endl;
			} else {
				// Looks for the best available seats in the row when the seats they entered cannot be reserved for whatever reason

				int people = adults + children + seniors;
				TheaterSeat *bestSeat = bestAvailableSeats(auditorium, rowNumber, seatLetter, people);
				cout << "Looks like those seats aren't available. Would you like to instead reserve seats at " << bestSeat->getRow() << bestSeat->getSeat() << " (best available seats)? (y/n)" << endl;

				char response;
				// Repeats until a response is y or n (not case sensitive)
				do {
					cin >> response;
					cout << endl;

					// If yes is selected, the closest seats to the middle is selected
					if (toupper(response) == 'Y') {
						if (bestSeat == nullptr) {
							cout << "Looks like there aren't any continuous seats of " << people << " in the auditorium.";
						} else {
							reserveSeats(auditorium, orders, bestSeat->getRow(), bestSeat->getSeat(), adults, children, seniors, username);
							cout << "Your seats have been successfully reserved." << endl;
						}
					}
					cin.clear();
					cin.ignore();
				} while (toupper(response) != 'Y' && toupper(response) != 'N');
			}

			cout << endl << endl;
		} else if (option == 2 || option == 3) {
			if (static_cast<int>(orders->size()) == 0) {
				cout << "You do not currently have any orders." << endl << endl;
				continue;
			}

			if (option == 2) {
				outputOrders(orders);
			} else if (option == 3) {
				cout << "- Select an Order -" << endl;
				outputOrders(orders);
				int selectedOrderIndex = -1;
				while (selectedOrderIndex == -1) {
					cin >> selectedOrderIndex;
					cin.clear();
					cin.ignore();

					// Checking if selection is in bounds of the options array
					if (selectedOrderIndex <= 0 || selectedOrderIndex > static_cast<int>(orders->size()))
						selectedOrderIndex = -1;
				}
				Order *selectedOrder = (*orders)[selectedOrderIndex - 1];
				Auditorium *orderAuditorium = auditoriums[selectedOrder->auditoriumID - 1];
				editOrder(orderAuditorium, selectedOrder, orders, username);
			}
		} else if (option == 4) {

			cout << "\t\tAuditorium #\tAdult\tChild\tSenior\tSeats\tCost" << endl;
			cout << "-------------------------------------------------------------------------------" << endl;
			int ta = 0, tc = 0, ts = 0;
			double cost = 0;
			int o = 0;
			for (Order *order : *orders) {
				double ct = order->adults * 10.00 + order->children * 5.00 + order->seniors * 7.50;
				cout << "Order " << ++o << ":\tAuditorium " << order->auditoriumID << "\t" << order->adults << "\t" << order->children << "\t" << order->seniors << "\t" << (order->seatsSize()) << "\t$" << ct << endl;
				ta += order->adults; tc += order->children; ts += order->seniors; cost += ct;
			}
			cout << "-------------------------------------------------------------------------------" << endl;
			cout << "[TOTAL]\t\t\t\t" << ta << "\t" << tc << "\t" << ts << "\t" << (ta + tc + ts) << "\t$" << cost << endl << endl;

		} else if (option == 5 || option == -2) {
			// Option 5: Log Out or Option -2: Log Out

			for (Order *order : *allOrders) {
				if (order->user == username) {
					allOrders->erase(find(allOrders->begin(), allOrders->end(), order));
				}
			}
			for (Order *order : *orders) {
				allOrders->push_back(order);
			}

			cout << "Logged out!" << endl << endl << endl;
			return true;
		} else if (option == -1) {
			// Option 1: Print Report

			for (int a = 0; a < 2; a++) Auditorium *auditorium = auditoriums[a];
			generateReport(auditoriums);
		} else if (option == -3) {
			// Option -3: Exit

			saveToFile(auditorium1, A1_FILENAME);
			saveToFile(auditorium2, A2_FILENAME);
			saveToFile(auditorium3, A3_FILENAME);

			return false;
		}
	} while (true);
	return true;
}

unordered_map<string, string> readUserDatabase() {
	ifstream input("userdb.dat");

	unordered_map<string, string> db;

	if (input.is_open()) {
		string username, password;
		while (input >> username && input >> password) {
			db.insert(make_pair(username, password));
		}
	}

	return db;
}

string authenticate(unordered_map<string, string> database) {
	string username = "", password = "";
	int attempts = 0;
	while (true) {
		if (username == "") {
			cout << "Username: ";
			cin >> username;
		}
		cout << "Password: ";
		cin >> password;
		if (database[username] == password) {
			cout << "Successful Login!" << endl << endl;
			return username;
		} else {
			cout << "Incorrect Password";
			if (++attempts == 3) {
				cout << "s...Resetting Session";
				username = "";
				attempts = 0;
			}
			cout << endl;
		}
	}
}

// Handling output and input for selecting an option in the menu
int selectOption(string username) {
	if (username == "admin") {
		cout << "-- Select an Admin Option --" << endl;

		string options[] = {"Print Report", "Log Out", "Exit"};

		// Iterating through all the options and placing it's appropriate option number (what the user inputs).
		int op = 0;
		for (string option : options) {
			cout << ++op << ". " << option << endl;
		}
		cout << endl;

		// -1 represents no selection yet
		int selection = -1;

		// Repeats until a selection is made
		while (selection == -1) {
			cin >> selection;
			cin.clear();
			cin.ignore();

			// Checking if selection is in bounds of the options array
			if (selection <= 0 || selection > 3)
				selection = -1;
		}
		cout << endl;

		return -selection;
	} else {
		cout << "-- Select an Option --" << endl;

		string options[] = {"Reserve Seats", "View Orders", "Update Order", "Display Receipt", "Log Out"};

		// Iterating through all the options and placing it's appropriate option number (what the user inputs).
		int op = 0;
		for (string option : options) {
			cout << ++op << ". " << option << endl;
		}
		cout << endl;

		// -1 represents no selection yet
		int selection = -1;

		// Repeats until a selection is made
		while (selection == -1) {
			cin >> selection;
			cin.clear();
			cin.ignore();

			// Checking if selection is in bounds of the options array
			if (selection <= 0 || selection > 5)
				selection = -1;
		}
		cout << endl;

		return selection;
	}
}

// Asking user which auditorium to display and reserve seats in
int selectAuditorium() {
	cout << "- Select an Update Option -" << endl;

	string options[] = {"Auditorium 1", "Auditorium 2", "Auditorium 3"};

	// Iterating through all the options and placing it's appropriate option number (what the user inputs).
	int op = 0;
	for (string option : options) {
		cout << ++op << ". " << option << endl;
	}
	cout << endl;

	// -1 represents no selection yet
	int selection = -1;

	// Repeats until a selection is made
	while (selection == -1) {
		cin >> selection;
		cin.clear();
		cin.ignore();

		// Checking if selection is in bounds of the options array
		if (selection <= 0 || selection > 3)
			selection = -1;
	}
	cout << endl;

	return selection;
}

// Outputs the current orders that the user has reserved
void outputOrders(vector<Order *> *orders) {
	int index = 0;
	for (Order *order : *orders) {
		cout << index + 1 << ". " << "Order " << index + 1 << " (" << order->adults << " Adults, " << order->children << " Children, " << order->seniors << " Seniors) [Auditorium " << order->auditoriumID << ": ";
		cout << order->getSeat(0)->getRow() << order->getSeat(0)->getSeat();
		for (int s = 1; s < order->seatsSize(); s++) { cout << ", " << order->getSeat(s)->getRow() << order->getSeat(s)->getSeat(); }
		cout << "]" << endl;
		index++;
	}
	cout << endl;
}

// Asking user how they would like to edit their order
void editOrder(Auditorium *auditorium, Order *order, vector<Order *> *orders, string username) {
	cout << "- Select an Edit Option -" << endl;

	string options[] = {"Add Tickets to Order", "Delete Tickets from Order", "Cancel Order"};

	// Iterating through all the options and placing it's appropriate option number (what the user inputs).
	int op = 0;
	for (string option : options) {
		cout << ++op << ". " << option << endl;
	}
	cout << endl;

	// -1 represents no selection yet
	int selection = -1;

	// Repeats until a selection is made
	while (selection == -1) {
		cin >> selection;
		cin.clear();
		cin.ignore();

		// Checking if selection is in bounds of the options array
		if (selection <= 0 || selection > 3)
			selection = -1;
	}
	cout << endl;

	if (selection == 1) {
		outputSeats(auditorium);

		// Prompts the user with a display to enter in the amount of tickets for each type they want to reserve.
		cout << "-- Reserve Seats --" << endl;
		int rowNumber;
		char seatLetter;
		int adults, children, seniors;

		// Handles input and checks if there is an error or it is out of bounds
		cout << "Row Number: ";
		do {
			cin >> rowNumber;
			if (cin.fail()) rowNumber = -1;
			cin.clear();
			cin.ignore();
		} while (rowNumber <= 0 || rowNumber > auditorium->getRows());
		cout << "Starting Seat Letter: ";
		do {
			cin >> seatLetter;
			if (cin.fail()) seatLetter = 0;
			cin.clear();
			cin.ignore();
		} while (seatLetter < 'A' || seatLetter > 'A' + (auditorium->getColumns() - 1));
		cout << "Adults: ";
		do {
			cin >> adults;
			if (cin.fail()) adults = -1;
			cin.clear();
			cin.ignore();
		} while (adults < 0);
		cout << "Children: ";
		do {
			cin >> children;
			if (cin.fail()) children = -1;
			cin.clear();
			cin.ignore();
		} while (children < 0);
		cout << "Seniors: ";
		do {
			cin >> seniors;
			if (cin.fail()) seniors = -1;
			cin.clear();
			cin.ignore();
		} while (seniors < 0);
		cout << endl;

		if (canReserveSeats(auditorium, rowNumber, seatLetter, adults + children + seniors)) {
			reserveSeats(auditorium, orders, rowNumber, seatLetter, adults, children, seniors, username);
			Order *lastOrder = (*orders)[static_cast<int>(orders->size()) - 1];
			for (int person = 0; person < adults + children + seniors; person++)
				order->addSeat(lastOrder->getSeat(person));
			lastOrder->removeAllSeats();
			orders->erase(find(orders->begin(), orders->end(), lastOrder));
			cout << "Order Successfully Modified!" << endl << endl;
			return;
		} else {
			cout << "At least one of the seats you entered are unavailable. Try again." << endl << endl;
			editOrder(auditorium, order, orders, username);
			return;
		}
	} else {
		if (selection == 2) {
			int rowNumber;
			char seatLetter;
			cout << "Row Number: ";
			do {
				cin >> rowNumber;
				if (cin.fail()) rowNumber = -1;
				cin.clear();
				cin.ignore();
			} while (rowNumber <= 0 || rowNumber > auditorium->getRows());
			cout << "Seat Letter: ";
			do {
				cin >> seatLetter;
				if (cin.fail()) seatLetter = 0;
				cin.clear();
				cin.ignore();
			} while (seatLetter < 'A' || seatLetter > 'A' + (auditorium->getColumns() - 1));

			TheaterSeat *deleteSeat = auditorium->getSeat(rowNumber, seatLetter);

			if (order->containsSeat(deleteSeat)) {
				order->removeSeat(deleteSeat);
				deleteSeat->setTicketType('.');
			} else {
				cout << "The order you selected does not have this seat. Try again." << endl << endl;
				editOrder(auditorium, order, orders, username);
				return;
			}
			cout << "Successfully removed seat from order." << endl;
		}
		if (selection == 3) {
			TheaterSeat *deleteSeat;
			for (int s = 0; s < order->seatsSize(); s++) {
				order->getSeat(s)->setTicketType('.');
			}
			order->removeAllSeats();
		}
		if (order->isEmpty()) {
			orders->erase(find(orders->begin(), orders->end(), order));
			cout << "Successfully cancelled order." << endl;
		}
		cout << endl;
	}
}

// Checks if the seats can be reserved
bool canReserveSeats(Auditorium *auditorium, int rowNumber, char seat, int people) {
	TheaterSeat *theaterSeat = auditorium->getSeat(rowNumber, seat);
	for (int c = 0; c < people; c++) {
		if (theaterSeat != nullptr && !theaterSeat->isReserved())
			theaterSeat = theaterSeat->getRight();
		else {
			return false;
		}
	}
	return true;
}

// Reserves the seats at a given coordinate
bool reserveSeats(Auditorium *auditorium, vector<Order *> *orders, int rowNumber, char seat, int adults, int children, int seniors, string username) {
	int people = adults + children + seniors;
	TheaterSeat *theaterSeat = auditorium->getSeat(rowNumber, seat);

	if (!canReserveSeats(auditorium, rowNumber, seat, people))
		return false;

	Order *order = new Order(auditorium->id);
	order->user = username;

	// Reserves the seats since available check has already taken place
	theaterSeat = auditorium->getSeat(rowNumber, seat);
	for (int c = 0; c < people; c++) {
		char reserveType = c < adults ? 'A' : c < adults + children ? 'C' : 'S';

		theaterSeat->setTicketType(reserveType);

		order->addSeat(theaterSeat);

		theaterSeat = theaterSeat->getRight();
	}

	orders->push_back(order);

	// Seat was successfully reserved
	return true;
}

// Locates the best available seats in the specified row
TheaterSeat* bestAvailableSeats(Auditorium *auditorium, int row, char seat, int people) {
	TheaterSeat *theaterSeat = auditorium->getFirst();
	TheaterSeat *closestSeat = nullptr;
	// Iterates through each seat to find best
	while (theaterSeat) {
		bool allFree = true;
		TheaterSeat *checkSeat = theaterSeat;
		// Checks if all people will get a seat
		for (int c = 0; c < people; c++) {
			if (checkSeat == nullptr || checkSeat->isReserved()) {
				allFree = false;
				break;
			} else checkSeat = checkSeat->getRight();
		}
		if (allFree) {
			double half = (people - 1) / 2.0;
			// Set as closest if all is available and it is the closest seats
			if (closestSeat == nullptr || isBetterSeat(auditorium, closestSeat->getRow() - 1, closestSeat->getSeat() - 'A' + half, theaterSeat->getRow() - 1, theaterSeat->getSeat() - 'A' + half))
				closestSeat = theaterSeat;
		}
		// Go right or down
		if (theaterSeat->getRight()) {
			theaterSeat = theaterSeat->getRight();
		} else {
			while (theaterSeat->getLeft()) theaterSeat = theaterSeat->getLeft();
			theaterSeat = theaterSeat->getDown();
		}
	}
	return closestSeat;
}

// Calculates whether (rm1, cm1) is in a better placement than (rm2, cm2)
bool isBetterSeat(Auditorium *auditorium, double rm1, double cm1, double rm2, double cm2) {
	// Getting middle row and columns
	double midR = (auditorium->getRows() - 1.0) / 2.0;
	double midC = (auditorium->getColumns() - 1.0) / 2.0;

	// Distance formula
	double diff = sqrt(pow(rm1 - midR, 2) + pow(cm1 - midC, 2)) - sqrt(pow(rm2 - midR, 2) + pow(cm2 - midC, 2));
	if (diff > 0.0) return true;
	else if (diff < 0.0) return false;
	else {
		// If there is a tie in distance
		diff = abs(rm1 - midR) - abs(rm2 - midR);
		if (diff > 0.0) return true;
		else if (diff < 0.0) return false;
		else {
			// If there is a tie in distance by row
			diff = rm1 - rm2;
			if (diff < 0.0) return false;
			else return false;
		}
	}
}

// Prints out the auditorium, except the type of seat reserved is hidden to the user and '#' represents a reserved seat
void outputSeats(Auditorium *auditorium) {
	// Iterates through the column letters
	printf("%3s", "");
	string columns = "";
	if (auditorium->getRows() >= 10) columns += " ";
	for (char col = 'A'; (col - 'A') < auditorium->getColumns(); col++)
		columns += col;
	cout << columns << endl;

	// Iterates through the rows
	TheaterSeat *seat = auditorium->getFirst();
	while (seat != nullptr) {
		printf("%-3d", seat->getRow());
		string row = "";
		TheaterSeat *rowSeat = seat;
		while (rowSeat != nullptr) {
			row += (rowSeat->getTicketType() == '.') ? "." : "#";
			rowSeat = rowSeat->getRight();
		}
		cout << row << endl;
		seat = seat->getDown();
	}

	cout << endl;
}

// Writes back to the file the new auditorium
void saveToFile(Auditorium *auditorium, string filename) {
	ofstream file(filename);

	TheaterSeat *seat = auditorium->getFirst();
	while (seat) {
		string row = "";
		TheaterSeat *rowSeat = seat;
		while (rowSeat) {
			row += rowSeat->getTicketType();
			rowSeat = rowSeat->getRight();
		}
		file << row << endl;
		seat = seat->getDown();
	}

	file.close();
}

// Prints this report to the screen when "exit" option is selected
void generateReport(Auditorium *auditoriums[]) {
	cout << "Auditorium #\t" << "Open Seats\t" << "Reserved Seats\t" << "Adult\t" << "Child\t" << "Senior\t" << "SALES\t" << endl;
	cout << "=================================================================================" << endl;

	int urss = 0;
	int as = 0, cs = 0, ss = 0, ts = 0;
	double smoney = 0.0;
	for (int a = 0; a < 3; a++) {
		Auditorium *auditorium = auditoriums[a];

		int urs = auditorium->getRows() * auditorium->getColumns();
		int tt = 0, at = 0, ct = 0, st = 0;
		double money = 0.0;
		for (int r = 1; r <= auditorium->getRows(); r++) {
			for (char c = 'A'; c < 'A' + auditorium->getColumns(); c++) {
				switch (auditorium->getSeat(r, c)->getTicketType()) {
				case 'A': at++; break;
				case 'C': ct++; break;
				case 'S': st++; break;
				}
			}
		}
		tt = at + ct + st;
		money = at * 10.00 + ct * 5.00 + st * 7.50;
		as += at; cs += ct; ss += st; ts += tt; smoney += money;
		urss += urs;

		cout << "Auditorium " << auditorium->id << "\t" << urs - tt << "\t\t" << tt << "\t\t" << at << "\t" << ct << "\t" << st << "\t$" << money << endl;
	}
	cout << "=================================================================================" << endl;
	cout << "[TOTAL]\t\t" << urss - ts << "\t\t" << ts << "\t\t" << as << "\t" << cs << "\t" << ss << "\t$" << smoney << endl;
	cout << endl;
}
