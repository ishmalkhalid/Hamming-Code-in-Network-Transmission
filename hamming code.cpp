/*-------------------------------------------------*/
/* Name: Ishmal Khalid, Net ID: ik1299 */
/* Date: October 13, 2019. */
/* Program: assignment.cpp */
/* Description: This program ensures error-free communication using Hamming Code.*/
/*-------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//Function Prototypes
int printMenu();
void encodeMessageStream(string);
void generateHammingCode(bool[], int);
void decodeMessageStream(string);
int checkMessageparity(bool[], int);

int main() {

	//Declare variables
	string encodeFile, decodeFile;
	int select;
	do {
		//Presents different outputs depending on the user input
		switch (select = printMenu()) {

			//Encodes file
		case 1:
			cout << "Input file name containing bitstreams to be encoded." << endl;
			cin >> encodeFile;
			encodeMessageStream(encodeFile);
			break;

			//Decodes, verifies and corrects encoded file
		case 2:
			cout << "Input file name containing encoded bitstreams to be verified for Hamming code." << endl;
			cin >> decodeFile;
			decodeMessageStream(decodeFile);
			break;

			//Exits the program
		case 3:
			exit(2);
			break;

		}
	} while (select != 3);
	system("pause");
	return 0;

}

//Function to present the user with a menu
int printMenu() {
	int choice;
	do {

		cout << "Select from the following menu:" << endl;
		cout << "1) Encode a message" << endl;
		cout << "2) Decode a message" << endl;
		cout << "3) Exit" << endl;
		cout << "Please make a selection: ";
		cin >> choice;

		//Check if the user input in valid
		if (choice < 1 || choice > 3) {
			cout << "Invalid Selection. Please try again." << endl;
		}
	} while (choice < 1 || choice > 3); //Repeat if input is invalid

	return choice; //Returns user input to the main function
}

//Function for encoding
void encodeMessageStream(string encodeFile) {

	const int size(7);
	bool message[size];
	char x;

	ifstream theFile(encodeFile, ios::in); //Open file to be read

	//Check if file opens successfully
	if (theFile.fail()) {
		cerr << "Error opening file.\n";
		exit(1);
	}

	ofstream newFile("encodeMessages.txt", ios::app); //Create file to write in

	//Check if file is created successfully
	if (newFile.fail()) {
		cerr << "Error creating file.\n";
		exit(3);
	}

	//Loop until end of file is reached
	while (!theFile.eof()) {

		//Read values from the file and store in the correct position in the array
		for (int i = 0; i < 5; i++) {
			if (i == 3) {
				message[i] = 0;
			}
			else {
				theFile.get(x);
				message[i] = x - 48;
			}
		}
		message[5] = 0;
		message[6] = 0;

		generateHammingCode(message, size); //Function to calculate parity bits

		cout << "Encoded Message: ";
		newFile << "Encoded Message: ";
		for (int i = 0; i < size; i++) {
			cout << message[i];
			newFile << message[i];
		}
		cout << endl;
		newFile << endl;

		theFile.get(x);

	}
	cout << "Messages are successfully encoded, check encoded messages file." << endl;

	//close files
	theFile.close();
	newFile.close();
}

//Function to add parity bits
void generateHammingCode(bool message[], const int size = 7) {


	message[6] = message[0] ^ message[2] ^ message[4];

	message[5] = message[0] ^ message[1] ^ message[4];

	message[3] = message[0] ^ message[1] ^ message[2];

}

//Function for decoding and correction
void decodeMessageStream(string decodeFile) {

	char x;
	int const size = 7;
	bool encodedmessage[size];
	string message_status;

	ifstream file(decodeFile, ios::in); // Open file to be read

	//Check if file opens successfully
	if (file.fail()) {
		cerr << "Error opening file.\n";
		exit(1);
	}

	ofstream newFile("encodeMessages.txt", ios::app); //Create file to write in

	//Check if file create successfully
	if (newFile.fail()) {
		cerr << "Error creating file.\n";
		exit(3);
	}

	cout << "Encoded|Corrected|Decoded|Status" << endl;
	newFile << "Encoded|Corrected|Decoded|Status" << endl;

	//Loop until end of file is reached
	while (!file.eof()) {


		//Read values from the file and store in the correct position in the array
		for (int i = 0; i < size; i++) {
			file.get(x);
			encodedmessage[i] = x - 48;
		}

		//Print encoded messages
		if (!file.eof())
		{
			for (int i = 0; i < size; i++) {
				cout << encodedmessage[i];
				newFile << encodedmessage[i];
			}

			checkMessageparity(encodedmessage, size); //Function to correct error

			cout << '|';
			newFile << '|';

			//Print corrected messages
			for (int i = 0; i < size; i++) {
				cout << encodedmessage[i];
				newFile << encodedmessage[i];
			}

			//Print decoded messages
			cout << '|' << encodedmessage[0] << encodedmessage[1] << encodedmessage[2] << encodedmessage[4];
			newFile << '|' << encodedmessage[0] << encodedmessage[1] << encodedmessage[2] << encodedmessage[4];

			//Check if error found in encoded messages and in what location
			if (checkMessageparity(encodedmessage, size) == -1) {
				cout << "|NO_ERROR" << endl;
				newFile << "|NO_ERROR" << endl;
			}
			else if (checkMessageparity(encodedmessage, size) > 0) {
				cout << "|ERROR_" << checkMessageparity(encodedmessage, size) << endl;
				newFile << "|ERROR_" << checkMessageparity(encodedmessage, size) << endl;
			}
			file.get(x);
		}

	}
	cout << "Messages are successfully decoded, check decoded messages file." << endl;

	//close files
	file.close();
	newFile.close();
}

//Function to calculate the paritty for errors
int checkMessageparity(bool encodedmessage[], const int size = 7) {

	int dec_val;
	int p1, p2, p4;

	p1 = encodedmessage[6] ^ encodedmessage[0] ^ encodedmessage[2] ^ encodedmessage[4];

	p2 = encodedmessage[5] ^ encodedmessage[0] ^ encodedmessage[1] ^ encodedmessage[4];

	p4 = encodedmessage[3] ^ encodedmessage[0] ^ encodedmessage[1] ^ encodedmessage[2];

	dec_val = (p1 * 1) + (p2 * 2) + (p4 * 4);

	//Return appropriate value to the decodeMessageStream function
	if (dec_val > 0) {
		return dec_val;
		if (encodedmessage[abs(7 - dec_val)] == 0) {
			encodedmessage[abs(7 - dec_val)] = 1;
		}
		else {
			encodedmessage[abs(7 - dec_val)] = 0;
		}

	}
	else {
		return -1;
	}

}



