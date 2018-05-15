#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <ctime>
#include "Person.h"
using namespace std;

// General function
void printLine(int);
void pressEnter(bool);
void displayMenu();

bool readAllData(ifstream &, Doctor *, Patient *, Ward *, int &);
int checkNum(int, int);
int promptInput(string, int, int);
int waitingList(Patient *, string *, string *);
string repeat(string, int);
string allignMid(string , int , int );

// Doctor's standalone function
void doctorMenu();
void docList(Doctor *);
void printAllDocInfo(Doctor  *);
void assignDoctor(Ward * ward, Doctor * doc);
void deleteDoctor(Doctor * doc, int docNo, Ward * ward, int wIndex);
void initError(ofstream &out);
void saveDoctor(ofstream &, Doctor *);

// Patient's standalone function
void patList(Patient *);
void patMenu();
void patMenu(int);
void savePatient(ofstream &, Patient *);
void assignPatient(Patient *, Ward *);

// Ward's standalone function
void printWardAvail(Ward *);
void printWard(Ward *, Doctor *, Patient *, int);

const int MAX = 1000;
int main()
{
	string username;
	string password;
	Admin admin;
	bool login = false;
	if (!admin.readRecord())
	{
		admin.init();
		username = admin.getUsername();
		admin.writeRecord();
	}
	else
	{
		cout << "Please enter username and password to login." << endl << endl;
		while (!login)
		{
			cout << "username: ";
			getline(cin, username);
			cout << "password: ";
			getline(cin, password);
			cout << endl;
			if (username == admin.getUsername() && password == admin.getPW())
			{
				login = true;
			}
		}
	}
	cout << fixed << setprecision(2);

	Doctor doc[20];
	Patient pat[MAX];
	Patient *admit[MAX];
	Ward ward[9];

	ifstream inp;
	ofstream out;
	int currentTix = 0;

	// input doctor and patient and ward data
	bool error = readAllData(inp, doc, pat, ward, currentTix);
	if (error)
	{
		initError(out);
	}
	TicketMaster::initTicketNum(currentTix);
	
	int admitCount = 0;
	int waitingCount = 0;
	int availableWard = 0;
	int choice = 0;
	int availWardIndex[8];
	srand(time(NULL));
	int random = 0;
	while (choice == 0)
	{
		for (int i = 0; i < 8; i++)
			availWardIndex[i] = 0;
		//cout << currentTix << pat[1].getTicket() << endl;
		waitingCount = 0;
		availableWard = 0;
		admitCount = 0;
		for (int i = 0; i < MAX; i++)
			admit[i] = NULL;
		for (int i = 0; i < Patient::getPatNum(); i++)
		{
			if (pat[i].getIsAdmit())
			{
				admit[admitCount] = &pat[i];
				admitCount++;
			}
			if (pat[i].getIsAdmit() && !pat[i].getIsAssigned())
				waitingCount++;
		}
		int availCount = 0;
		for (int i = 0; i < 8; i++)
		{
			if (!ward[i].getIsOccupied() && ward[i].getIsStationed())
			{
				availWardIndex[availCount] = i;
				availCount++;
				availableWard++;
			}
		}
		if (availableWard > 0)
		{
			random = rand() % availableWard;
			//cout << random << endl;
		}
		if (admitCount > 0)
		{
			for (int i = 0; i < admitCount; i++)
			{
				if (!admit[i]->getIsAssigned())
				{
					if (admit[i]->getTicket() == currentTix)
					{
						ward[availWardIndex[random]].setPatient(admit[i]);
						waitingCount--;
						availableWard--;
						break;
					}
				}
			}
		}
		printWard(ward, doc, pat, currentTix);
		displayMenu();
		choice = promptInput(username, 1, 5);
		while (choice == 1)
		{
			docList(doc);
			doctorMenu();
			choice = promptInput(username, 1, 4);

			if (choice == 1)  // Expand information
			{
				printAllDocInfo(doc);
			}
			else if (choice == 2) // Add doctor
			{
				bool isFull = true;
				for (int i = 0; i < 8; i++)
				{
					if (ward[i].getIsStationed() == false)
						isFull = false;
				}
				if (isFull)
				{
					cout << "All rooms are stationed. Unable to add new doctors"
						<< endl << endl;
				}
				else
				{
					doc[Doctor::getDocNum()].addDoc();
					assignDoctor(ward, doc);
					Doctor::incDocNum();
				}
				pressEnter(1);
			}
			else if (choice == 3) // remove doctor
			{
				if (Doctor::getDocNum() != 0)
				{
					cout << "Remove doctor number => ";
					int docNo = checkNum(0, Doctor::getDocNum());
					docNo--;
					int wIndex;
					for (int i = 0; i < 8; i++)
					{
						if (ward[i].getDoctor() == &doc[docNo])
						{
							wIndex = i;
						}

					}
					if (ward[wIndex].getIsOccupied() == true)
					{
						cout << "Error removing doctor." << endl;
						cout << "Discharge patient " 
							<< ward[wIndex].getPatientName() 
							<< " first before removing doctor." << endl
							<< endl;
					}
					else
					{
						deleteDoctor(doc, docNo, ward, wIndex);
					}
				}
				else
				{
					cout << "No record found. ";
					pressEnter(1);
				}
				pressEnter(1);
			}
			else if (choice == 4) // go back
			{
				saveDoctor(out, doc);
				choice = 0;
				break;
			}
			saveDoctor(out, doc);
			choice = 1;
		}
		while (choice == 2)
		{
			patList(pat);

			patMenu();
			admitCount = 0;
			for (int i = 0; i < MAX; i++)
				admit[i] = NULL;
			for (int i = 0; i < Patient::getPatNum(); i++)
			{
				if (pat[i].getIsAdmit())
				{
					admit[admitCount] = &pat[i];
					admitCount++;
				}
			}
			choice = promptInput(username, 1, 4);
			if (choice == 1)  // selected patient
			{
				if (admitCount == 0)
					cout << "No patients admitted." << endl << endl;
				else
				{
					cout << "Enter patient's number => ";
					int patNo = checkNum(0, admitCount);
					patNo--;
					int wIndex;

					for (int i = 0; i < 8; i++)
						if (ward[i].getPatient() == admit[patNo])
							wIndex = i;
					cout << "Patient's information:" << endl;
					printLine(1);
					cout << endl;
					admit[patNo]->display();
					printLine(2);
					if (admit[patNo]->getIsAssigned())
					{
						cout << "\tAssigned to room " << (char)('A' + wIndex)
							<< endl << endl;
					}
					else
						cout << "\tNot assigned to any room." << endl << endl;

					patMenu(1);

					choice = promptInput(username, 1, 4);
					if (choice == 1)  // set / change room
					{
						if (admit[patNo]->getIsAssigned() == false)
						{
							cout << "Patient " << admit[patNo]->getName() 
								<< " is not assigned to any room." 
								<< endl << endl;
						}
						else
						{
							cout << "Patient " << admit[patNo]->getName() 
								<< " is in room "
								<< (char)('A' + wIndex) 
								<< endl << endl;
							ward[wIndex].releasePat();
							assignPatient(admit[patNo], ward);
						}
						
						pressEnter(1);

					}
					else if (choice == 2) // discharge patient
					{
						cout << "Confirm to discharge '" 
							<< admit[patNo]->getName() << "'? (Y / n): ";
						string yesno;
						cin >> yesno;
						if (yesno == "Y")
						{
							admit[patNo]->setIsAssigned(false);
							admit[patNo]->setIsAdmit(false);
							for (int i = 0; i < 8; i++) 
							{
								if (ward[i].getPatient() == admit[patNo])  
								{
									ward[i].releasePat();
								}
							}
							cout << "Patient discharged." << endl;
						}
						else
						{
							cout << "Patient not discharged." << endl;
						}
						cout << endl;
						pressEnter(1);
					}
				}
			}
			else if (choice == 2)  // add patient
			{
				cout << endl 
					<< "      Please select an option" << endl
					<< "      1. Add new patient information" << endl
					<< "      2. Search from records" << endl
					<< "      3. Go back." << endl;
				cout << endl;
				choice = promptInput(username, 1, 3);
				if (choice == 1)
				{
					if (pat[Patient::getPatNum()].addPat(pat))
					{
						pat[Patient::getPatNum()].setIsAdmit(true);
						cout << endl;
						//assignPatient(&pat[Patient::getPatNum()], ward);
						cout << pat[Patient::getPatNum()].getName() 
							<< " added to waiting list." << endl;
						Patient::incPatNum();
					}
					else 
					{
						cout << "Record contains similar IC" 
							<< " Please check again." << endl;
					}

				}
				else if (choice == 2)
				{
					cout << "Enter patient's IC (without '-') => ";
					string tmpIC;
					bool found = false;
					cin >> tmpIC;
					for (int i = 0; i < Patient::getPatNum(); i++)
					{
						if (pat[i].getIC() == tmpIC)
						{
							found = true;
							if (pat[i].getIsAssigned() || pat[i].getIsAdmit())
							{
								cout << "Patient " << pat[i].getName() 
									<< " already admitted." << endl;
							}
							else
							{
								cout << "Match found. Confirm to add " 
									<< pat[i].getName() << " ? (Y / n) => ";
								string yesno;
								cin >> yesno;
								if (yesno == "Y")
								{
									cout << "Patient added." << endl;
									pat[i].setIsAdmit(true);
									//assignPatient(&pat[i], ward);
									cout << pat[i].getName() 
										<< " added to waiting list." 
										<< endl << endl;

									break;
								}
								else
								{
									cout << "Patient not added." << endl;
									break;
								}
							}
						}

					}
					if (!found)
						cout << "Record not found." << endl;
				}
				pressEnter(1);
			}
			else if (choice == 3)  // view patient record
			{
				cout << "Patient's information:" << endl;
				printLine(1);
				cout << endl;
				for (int i = 0; i < Patient::getPatNum(); i++)
				{
					pat[i].display();
					printLine(2);
				}
			}
			else if (choice == 4)  // return
			{
				savePatient(out, pat);
				choice = 0;
				break;
			}
			savePatient(out, pat);
			choice = 2;
		}
		if (choice == 3)
		{
			if (waitingCount != 0 && availableWard != 0)
				currentTix++;
			else
			{
				cout << "No patients in waiting list or no rooms available." 
					<< endl;
				pressEnter(1);
			}
			choice = 0;

		}
		if (choice == 4)
		{
			if (admitCount != 0)
			{
				cout << "Please discharge all patients before resetting." 
					<< endl;
				pressEnter(1);
			}
			else
			{
				currentTix = 0;
				TicketMaster::initTicketNum(0);
			}
			choice = 0;
		}
		if (choice == 5)
		{
			cout << "Thank you for using this program" << endl;
			break;
		}
	}
	saveDoctor(out, doc);
	savePatient(out, pat);
	out.open("ward.txt", ios::out);
	for (int i = 0; i < 8; i++)
		out << ward[i].getDoctorIC() << "," << ward[i].getPatientIC() << endl;
	out << currentTix;
	out.close();
	return 0;
}

void initError(ofstream &out)
{
	cout << "Program initialization error." << endl
		<< "Press 'Y' to delete previous data, 'n' to exit program. :";
	char del;
	cin >> del;
	if (del == 'Y')
	{
		out.open("doctor.txt");
		out.close();
		out.open("patient.txt");
		out.close();
		out.open("ward.txt");
		out.close();
		cout << "Previous data deleted. Please restart the program now." 
			<< endl;
		exit(1);
	}
	else
		exit(1);
}

void deleteDoctor(Doctor* doc, int docNo, Ward * ward, int wIndex)
{
	cout << "Confirm to remove '" << doc[docNo].getName() << "'? (Y / n): ";
	string yesno;
	cin >> yesno;
	if (yesno == "Y")
	{
		for (int i = 0; i < 8; i++)
		{
			if (ward[i].getDoctor() > &doc[docNo])
			{
				ward[i].doctorPtrAdjust();
			}
		}

		ward[wIndex].setIsStationed(false);
		ward[wIndex].releaseDoc();
		for (int i = docNo; i < Doctor::getDocNum(); i++)
		{
			doc[i] = doc[i + 1];
		}
		cout << "Record removed." << endl << endl;
		Doctor::decDocNum();
	}
	else
	{
		cout << "Doctor " << doc[docNo].getName() << " not removed." 
			<< endl << endl;
	}
}

void assignDoctor(Ward*  ward, Doctor*  doc)
{
	printWardAvail(ward);

	cout << "Assign " << doc[Doctor::getDocNum()].getName() 
		<< " to room (A - H) => ";
	char chr;
	int wardNo;
	cin >> chr;
	chr = toupper(chr);
	while (cin)
	{
		if (chr != 'A' && chr != 'B' && chr != 'C' && chr != 'D' &&
			chr != 'E' && chr != 'F' && chr != 'G' && chr != 'H')
		{
			cout << "Please enter correct room => ";
			cin >> chr;
			chr = toupper(chr);
		}
		else
		{
			wardNo = chr - 'A';
			if (ward[wardNo].getIsStationed() == 0)
			{
				ward[wardNo].setDoctor(&doc[Doctor::getDocNum()]);
				cout << doc[Doctor::getDocNum()].getName() 
					<< " stationed to room " << chr << endl;
				break;
			}
			else
			{
				cout << "Room " << chr << " is stationed by " 
					<< ward[wardNo].getDoctorName() << endl;
				cout << "Please enter another room => ";
				cin >> chr;
				chr = toupper(chr);
			}
		}
	}
}

void printAllDocInfo(Doctor  *doc)
{
	printLine(2);
	cout << "Doctors' basic information / contact information." << endl << endl;
	if (Doctor::getDocNum() == 0)
		cout << "No doctors record" << endl;
	else
	{
		for (int i = 0; i < Doctor::getDocNum(); i++)
		{
			doc[i].display(i);
		}
	}
	pressEnter(1);
}


void displayMenu()
{
	cout << "  Ward 3 information center" << endl;
	printLine(1);
	cout << "  Please select an option" << endl
		<< "  1. Doctor" << endl
		<< "  2. Patient" << endl
		<< "  3. Add ticket number" << endl
		<< "  4. Reset ticket number" << endl
		<< "  5. Exit" << endl
		<< endl;
}
void doctorMenu()
{
	cout << "    Please select an option" << endl
		<< "    1. Expand information" << endl
		<< "    2. Add doctor" << endl
		<< "    3. Remove doctor" << endl
		<< "    4. Go back" << endl
		<< endl;
}
void patMenu()
{
	cout << "    Please select an option" << endl
		<< "    1. Select patient" << endl
		<< "    2. Add patient" << endl
		<< "    3. View patient record" << endl
		<< "    4. Go back" << endl
		<< endl;
}
void patMenu(int select)
{
	cout << "      Please select an option" << endl
		<< "      1. Change room" << endl
		<< "      2. Discharge patient" << endl
		<< "      3. Go back" << endl << endl;
}

void docList(Doctor * p)
{
	cout << "List of doctors" << endl;
	printLine(1);
	if (Doctor::getDocNum() == 0)
		cout << "No doctors found." << endl;
	for (int i = 0; i < Doctor::getDocNum(); i++)
		cout << i + 1 << ". " << p[i].getName() << endl;
	cout << endl;
}
void patList(Patient * p)
{
	cout << "List of patients admitted" << endl;
	printLine(1);
	int count = 0;
	if (Patient::getPatNum() == 0)
		cout << "No patients found." << endl;
	for (int i = 0; i < Patient::getPatNum(); i++)
	{
		if (p[i].getIsAdmit() == true)
		{
			cout << count + 1 << ". " << p[i].getName() << endl;
			count++;
		}
	}
	cout << endl;
}
void saveDoctor(ofstream &out, Doctor * d) 
{
	out.open("doctor.txt");
	for (int i = 0; i < Doctor::getDocNum(); i++) // to save person list
		d[i].writeRecord(out, i);
	out.close();
}
void savePatient(ofstream &out, Patient * p) 
{
	out.open("patient.txt");
	for (int i = 0; i < Patient::getPatNum(); i++) // to save person list
		p[i].writeRecord(out, i);
	out.close();
}
void printLine(int x)
{
	if (x == 1)
		cout << "-----------------------------" << endl;
	else if (x == 2)
		cout << "---------------------------------------------------------" 
		<< "-------------------------------------" << endl;
}
int promptInput(string username, int min, int max)
{
	int tmp;
	cout << username << ": ";
	cin >> tmp;

	while (tmp < min || tmp > max)
	{
		if (!cin)
		{
			cout << "Input error... Terminating program." << endl;
			exit(1);
		}
		cout << tmp << ": command not found." << endl
			<< username << ": ";
		cin >> tmp;

	}
	cout << endl;
	return tmp;
}

void pressEnter(bool skip)
{
	cout << "Press enter to continue. . . ";
	if (skip)
		cin.ignore();
	cin.get();
	cout << endl;
}
int checkNum(int min, int max)
{
	int x;
	cin >> x;
	while (x <= min || x > max)
	{
		if (!cin)
		{
			cout << "Input error... Terminating program." << endl;
			exit(1);
		}
		cout << "Enter correct number => ";
		cin >> x;
	}
	cout << endl;
	return x;
}
void printWardAvail(Ward *ward)
{
	cout << "List of rooms" << endl;
	printLine(1);
	cout << "Room A: " << ward[0].getAvail() << endl;
	cout << "Room B: " << ward[1].getAvail() << endl;
	cout << "Room C: " << ward[2].getAvail() << endl;
	cout << "Room D: " << ward[3].getAvail() << endl;
	cout << "Room E: " << ward[4].getAvail() << endl;
	cout << "Room F: " << ward[5].getAvail() << endl;
	cout << "Room G: " << ward[6].getAvail() << endl;
	cout << "Room H: " << ward[7].getAvail() << endl;
	cout << endl;
}
void printWard(Ward *ward, Doctor * doc, Patient * pat, int currentTix)
{
	cout << "\t" << repeat("-", 100) << endl;
	cout << "\t|" << repeat(" ", 17) << repeat(" ", 9) << "A" 
		<< repeat(" ", 18) << "B" << repeat(" ", 18) << "C" 
		<< repeat(" ", 18) << "D" << repeat(" ", 14) << "|" << endl;
	cout << "\t|" << repeat(" ", 17) << repeat("* ", 39) << "   |" << endl;
	for (int i = 0; i < 5; i++)
	{
		if (i == 2)
		{
			cout << "\t|" << repeat(" ", 17) << '*' 
				<< allignMid(ward[0].getPatientName(), 18, 0) << "*" 
				<< allignMid(ward[1].getPatientName() , 18, 0) << "*" 
				<< allignMid(ward[2].getPatientName(), 18, 0) << "*" 
				<< allignMid(ward[3].getPatientName(), 18, 0) << "*" 
				<< "    |" << endl;
		}
		if (i == 3)
		{
			cout << "\t|" << repeat(" ", 17) << '*' 
				<< allignMid(ward[0].getDoctorNameInBracket(), 18, 0) << "*" 
				<< allignMid(ward[1].getDoctorNameInBracket(), 18, 0) << "*" 
				<< allignMid(ward[2].getDoctorNameInBracket(), 18, 0) << "*" 
				<< allignMid(ward[3].getDoctorNameInBracket(), 18, 0) << "*" 
				<< "    |" << endl;
		}
		cout << "\t|" << repeat(" ", 17) << '*' << repeat(" ", 18) << "*" 
			<< repeat(" ", 18) << "*"<< repeat (" ",18) << "*" 
			<< repeat(" ", 18) << "*" << "    |" << endl;
	}
	cout << "\t|" << repeat(" ", 17) << repeat("* ", 39) << "   |" << endl;
	cout << "\t|" << allignMid("Waiting List:",17, 0) << repeat(" ", 81) 
		<< "|" << endl;
	string s[MAX];
	string sWithNum[MAX];
	int wCount;
	wCount = waitingList(pat, s, sWithNum);
	string tix[3];
	string tixTmp[3];
	int length;
	int tmp;
	int c[4];
	for (int q = 0; q < 3; q++)
	{
		length = 0;
		for (int h = 0; h < 4; h++)
			c[h] = 0;
		tmp = currentTix - q;
		if (tmp < 0)
		{
			tix[q] = "0000";
		}
		else
		{
			for (int i = 0; tmp > 0; i++)
			{
				c[i] = tmp % 10;
				tmp /= 10;
				length++;
			}
			for (int i = length - 1; i >= 0; i--)
			{
				tixTmp[q] += c[i] + '0';
			}
			for (int i = 0; i < (4 - length); i++)
			{
				tix[q] += "0";
			}
			tix[q] += tixTmp[q];
		}

	}
	string room[3];
	bool notFound = true;
	for (int q = 0; q < 3; q++)
	{
		notFound = true;
		for (int i = 0; i < Patient::getPatNum(); i++)
		{
			if ((currentTix - q) <= 0)
			{
				notFound = true;
				break;
			}
			if ((currentTix-q) == pat[i].getTicket())
			{
				notFound = false;
				for (int j = 0; j < 8; j++)
				{
					if (ward[j].getPatient() == &pat[i])
					{
						room[q] = j + 'A';
					}
				}
			}
		}
		if (notFound)
		{
			room[q] = "-";
		}
	}
	for (int i = 0; i < 5; i++)
	{
		if (i == 0)
		{
			cout << "\t| " << i + 1 << "." << allignMid(sWithNum[i], 30, 1) 
				<< repeat(" ", 10) << allignMid("NOW SERVING", 15,0) 
				<< allignMid("ROOM", 10, 0)
				<< repeat(" ", 30) << "|" << endl;
		}
		else if (i == 1)
		{
			cout << "\t| " << i + 1 << "." << allignMid(sWithNum[i], 30, 1) 
				<< repeat(" ", 10) << allignMid(tix[0], 15, 0) 
				<< allignMid(room[0], 10, 0)
				<< repeat(" ", 30) << "|" << endl;
		}
		else if (i == 2)
		{
			cout << "\t| " << i + 1 << "." << allignMid(sWithNum[i], 30, 1) 
				<< repeat(" ", 10) << allignMid(tix[1], 15, 0) 
				<< allignMid(room[1], 10, 0)
				<< repeat(" ", 30) << "|" << endl;
		}
		else if (i == 3)
		{
			cout << "\t| " << i + 1 << "." << allignMid(sWithNum[i], 30, 1) 
				<< repeat(" ", 10) << allignMid(tix[2], 15, 0) 
				<< allignMid(room[2], 10, 0)
				<< repeat(" ", 30) << "|" << endl;
		}
		else
		{
			cout << "\t| " << i + 1 << "." << allignMid(sWithNum[i], 30, 1)
				<< repeat(" ", 65) << "|" << endl;
		}
	}
	if (wCount > 5)
	{
		int tmp = wCount - 5;
		string overFlow = to_string(tmp);
		string cat = "+ ";
		cat += overFlow;
		cat += " more";
		cout << "\t|" << allignMid(cat , 17, 0) << repeat(" ", 81) << "|" 
			<< endl;
	}
	cout << "\t|" << repeat(" ", 17) << repeat("* ", 39) << "   |" << endl;
	for (int i = 0; i < 5; i++)
	{
		if (i == 2)
		{
			cout << "\t|" << repeat(" ", 17) << '*' 
				<< allignMid(ward[4].getPatientName(), 18, 0) << "*" 
				<< allignMid(ward[5].getPatientName(), 18, 0) << "*" 
				<< allignMid(ward[6].getPatientName(), 18, 0) << "*" 
				<< allignMid(ward[7].getPatientName(), 18, 0) << "*" 
				<< "    |" << endl;
		}
		if (i == 3)
		{
			cout << "\t|" << repeat(" ", 17) << '*' 
				<< allignMid(ward[4].getDoctorNameInBracket(), 18, 0) << "*" 
				<< allignMid(ward[5].getDoctorNameInBracket(), 18, 0) << "*" 
				<< allignMid(ward[6].getDoctorNameInBracket(), 18, 0) << "*" 
				<< allignMid(ward[7].getDoctorNameInBracket(), 18, 0) << "*" 
				<< "    |" << endl;
		}
		cout << "\t|" << repeat(" ", 17) << '*' << repeat(" ", 18) << "*" 
			<< repeat(" ", 18) << "*"<< repeat (" ",18) << "*" 
			<< repeat(" ", 18) << "*" << "    |" << endl;
	}
	cout << "\t|" << repeat(" ", 17) << repeat("* ", 39) << "   |" << endl;
	cout << "\t|" << repeat(" ", 17) << repeat(" ", 9) << "E" 
		<< repeat(" ", 18) << "F" << repeat(" ", 18) << "G" << repeat(" ", 18) 
		<< "H" << repeat(" ", 14) << "|" << endl;
	cout << "\t" << repeat("-", 100) << endl;
	cout << endl;
}
void dischargePat(Ward *ward, Patient *pat, int wIndex)
{
	ward[wIndex].setPatient(NULL);
	ward[wIndex].setIsOccupied(false);
	pat->setIsAssigned(false);
}
int waitingList(Patient *pat, string *s, string *sWithNum)
{
	int count = 0;
	string tixTmp;
	string tix;
	int c[5];
	int tmp;
	int length = 0;
	for (int i = 0; i < Patient::getPatNum(); i++)
	{
		tix = "";
		tixTmp = "";
		length = 0;
		for (int j = 0; j < 5; j++)
			c[j] = 0;
		if (!pat[i].getIsAssigned() && pat[i].getIsAdmit())
		{
			s[count] = pat[i].getName();
			tmp = pat[i].getTicket();
			for (int i = 0; tmp > 0; i++)
			{
				c[i] = tmp % 10;
				tmp /= 10;
				length++;
			}
			for (int i = length - 1; i >= 0; i--)
			{
				tixTmp += c[i] + '0';
			}
			for (int i = 0; i < (4 - length); i++)
			{
				tix += "0";
			}
			tix += tixTmp;
			sWithNum[count] = pat[i].getName();
			sWithNum[count] += " (";
			sWithNum[count] += tix;
			sWithNum[count] += ")";
			count++;
		}

	}
	return count;
}
void assignPatient(Patient *pat, Ward *ward)
{
	char chr;
	int wardNo;
	printWardAvail(ward);

	cout << "Assign " << pat->getName() << " to room (A - H) => ";
	cin >> chr;
	chr = toupper(chr);
	while (cin)
	{
		if (chr != 'A' && chr != 'B' && chr != 'C' && chr != 'D' &&
			chr != 'E' && chr != 'F' && chr != 'G' && chr != 'H')
		{
			cout << "Please enter correct room => ";
			cin >> chr;
			chr = toupper(chr);
		}
		else
		{
			wardNo = chr - 'A';
			if (ward[wardNo].getIsStationed() == 0)
			{
				cout << "No doctors in room " << chr << endl;
				cout << "Please enter another room => ";
				cin >> chr;
				chr = toupper(chr);
			}
			else if (ward[wardNo].getIsOccupied() == 0)
			{
				ward[wardNo].setPatient(pat);
				cout << pat->getName() << " assinged to room " << chr << endl;
				break;
			}
			else
			{
				cout << "Room " << chr << " is occupied by " 
					<< ward[wardNo].getPatient()->getName() << endl;
				cout << "Please enter another room => ";
				cin >> chr;
				chr = toupper(chr);
			}
		}
	}
}
bool readAllData(ifstream &inp, Doctor *doc, 
	             Patient *pat, Ward *ward, int &currentTix)
{
	int fileCount = 0;
	bool error = false;
	inp.open("patient.txt");
	if (!inp)
	{
		fileCount++;
	}
	else
	{
		for (int i = 0; inp.good(); i++)
			pat[i].readRecord(inp);
	}
	inp.close();

	inp.open("doctor.txt");
	if (!inp)
	{
		fileCount++;
	}
	else
	{
		for (int i = 0; inp.good(); i++)
		{
			doc[i].readRecord(inp);
		}
	}
	inp.close();
	inp.open("ward.txt");
	if (!inp)
	{
		fileCount++;
	}
	else
	{
		string docIC;
		string patIC;
		for (int i = 0; i < 8; i++)
		{
			getline(inp, docIC, ',');
			getline(inp, patIC);
			if (docIC.empty())
			{
				break;
			}
			for (int j = 0; j < Doctor::getDocNum(); j++)
			{
				if (doc[j].getIC() == docIC)
				{
					ward[i].setDoctor(&doc[j]);
				}
			}
			for (int j = 0; j < Patient::getPatNum(); j++)
			{
				if (pat[j].getIC() == patIC)
				{
					ward[i].setPatient(&pat[j]);
				}
			}
		}
		inp >> currentTix;
	}
	inp.close();
	if (fileCount != 3 && fileCount != 0)
	{
		error = true;
	}
	return error;
}
string allignMid(string s, int width, int mod)
{
	int length = s.length();
	string result = "";
	if (mod == 1)
	{
		if (length > width)
		{
			for (int i = 0; i < width - 2; i++)
			{
				result += s[i];
			}
			result += "..";
			return result;
		}
		else
		{
			int pad = width - length;
			string padding(pad , ' ');
			for (int i = 0; i < (width - length); i++)
			{
				result += s;
				result += padding;
				return result;
			}
		}
	}
	if (length < width)
	{
		int pad = width - length;
		string spaces(pad / 2, ' ');
		result = spaces + s;
		while (result.length() < (unsigned int)width)
			result += " ";
		return result;
	}
	else if (length > width)
	{
		for (int i = 0; i < width - 2; i++)
		{
			result += s[i];
		}
		result += "..";
		return result;
	}
	else 
	{
		return s;
	}
}
string repeat(string a, int max)
{
	string s = "";
	for (int i = 0; i < max; i++)
		s += a;
	return s;
}
