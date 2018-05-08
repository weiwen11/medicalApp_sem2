#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "Person.h"
using namespace std;

int waitingList(Patient *, string *);
void printLine(int);
int promptInput(string, int, int);
void pressEnter(bool);
void displayMenu();
void printAllDocInfo(Doctor  *);
int checkNum(int, int);

void doctorMenu();
void docList(Doctor *);
void patList(Patient *);
void patMenu();
void patMenu(int);
void savePatient(ofstream &, Patient *);
void saveDoctor(ofstream &, Doctor *);

class Ward
{
	bool isOccupied;
	bool isStationed;
	double rate;
	Doctor *doctor;
	Patient *patient;
public:

	Ward();
	void setPatient(Patient *);
	void setIsOccupied(bool);
	void setIsStationed(bool);
	void setDoctor(Doctor *);
	void setRate(double);
	void doctorPtrAdjust();

	bool getIsStationed() const;
	Doctor *getDoctor();
	Patient *getPatient() const;
	string getPatientName() const;
	string getDoctorName() const;
	string getDocName() const;
	bool getIsOccupied() const;
	string getAvail() const;
	double getRate() const;
	void releasePat();
	void releaseDoc();
	friend void printWard();
};

string Ward::getDoctorName() const
{
	if (doctor == NULL)
		return "-";
	else
		return doctor->getName();
}
void Ward::releasePat() 
{
	isOccupied = false;
	patient->setIsAssigned(false);
	patient = NULL;
}

void Ward::releaseDoc()
{
	isStationed = false;
	doctor->setIsAssigned(false);
	doctor = NULL;
}

Ward::Ward()
{
	patient = NULL;
	doctor = NULL;
	isOccupied = false;
	isStationed = false;
	rate = 0;
}
void Ward::setPatient(Patient *p)
{
	patient = p;
	patient->setIsAssigned(true);
	isOccupied = true;
}

void Ward::setIsOccupied(bool a)
{
	isOccupied = a;
}

void Ward::setIsStationed(bool i)
{
	isStationed = i;
}

void Ward::setDoctor(Doctor *d)
{
	doctor = d;
	isStationed = true;
}

void Ward::setRate(double r)
{
	rate = r;
}

void Ward::doctorPtrAdjust()
{
	doctor--;
}

bool Ward::getIsStationed() const
{
	return isStationed;
}

Doctor * Ward::getDoctor()
{
	return doctor;
}

Patient * Ward::getPatient() const
{
	return patient;
}
string Ward::getPatientName() const
{
	if (patient != NULL)
	{
		return patient->getName();
	}
	else
		return "-";
}
string Ward::getDocName() const
{
	string s = "(";
	if (doctor != NULL)
	{
		s += doctor->getName();
		s += ")";
		return s;
	}
	else
		return "";
}

bool Ward::getIsOccupied() const
{
	return isOccupied;
}

string Ward::getAvail() const
{
	string s = "";
	if (patient == NULL && doctor != NULL)
	{
		s += "Available ";
		s += getDocName();
		s += "";
		return s;
	}
	else if (doctor == NULL)
		return "No doctor";
	else
	{
		s += getPatientName();
		s += " ";
		s += getDocName();
		s += "";
		return s;
	}
}

double Ward::getRate() const
{
	return rate;
}

void assignPatient(Patient *, Ward *);
string repeat(string a, int max)
{
	string s = "";
	for (int i = 0; i < max; i++)
		s += a;
	return s;
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
void printWardAvail(Ward *);
void printWard(Ward *, Doctor *, Patient *);
bool readAllData(ifstream &inp, Doctor *doc, Patient *pat, Ward *ward)
{
	int fileCount = 0;
	bool error = false;
	inp.open("patient.txt");
	if (!inp)
	{
		fileCount++;
		cout << "Creating patient.txt..." << endl;
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
		cout << "Creating doctor.txt..." << endl;
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
		cout << "Creating ward.txt..." << endl;
	}
	else
	{
		string docName;
		string patName;
		for (int i = 0; i < 8; i++)
		{
			getline(inp, docName, ',');
			getline(inp, patName);
			if (docName.empty())
			{
				break;
			}
			for (int j = 0; j < Doctor::doc_NUM; j++)
			{
				if (doc[j].getName() == docName)
				{
					ward[i].setDoctor(&doc[j]);
				}
			}
			for (int j = 0; j < Patient::pat_NUM; j++)
			{
				if (pat[j].getName() == patName)
				{
					ward[i].setPatient(&pat[j]);
				}
			}
		}
	}
	inp.close();
	if (fileCount != 3 && fileCount != 0)
	{
		error = true;
	}
	return error;
}
const int MAX = 1000;

void assignDoctor(Ward * ward, Doctor * doc);
void deleteDoctor(Doctor * doc, int docNo, Ward * ward, int wIndex);

int main()
{
	string username = "weiwen";
	cout << fixed << setprecision(2);
	Doctor doc[20];
	Patient pat[MAX];
	Patient *admit[MAX];
	Ward ward[9];

	// input doctor and patient data
	ifstream inp;
	ofstream out;
	bool error = readAllData(inp, doc, pat, ward);
	if (error)
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
			cout << "Previous data deleted. Please restart the program now." << endl;
			exit(1);
		}
		else
			exit(1);
	}
	

	int choice = 0;
	while (choice == 0)
	{
		printWard(ward, doc, pat);
		displayMenu();
		choice = promptInput(username, 1, 3);
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
					cout << "All rooms are stationed. Unable to add new doctors" << endl << endl;
				}
				else
				{
					doc[Doctor::doc_NUM].addDoc();
					assignDoctor(ward, doc);
					Doctor::doc_NUM++;
				}
				pressEnter(1);
			}
			else if (choice == 3) // remove doctor
			{
				if (Doctor::doc_NUM != 0)
				{
					cout << "Remove doctor number => ";
					int docNo = checkNum(0, Doctor::doc_NUM);
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
						cout << "Discharge patient " << ward[wIndex].getPatientName() << " first before removing doctor." << endl
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
			int admitCount = 0;
			for (int i = 0; i < MAX; i++)
				admit[i] = NULL;
			for (int i = 0; i < Patient::pat_NUM; i++)
			{
				if (pat[i].getIsAdmit())
				{
					admit[admitCount] = &pat[i];
					admitCount++;
				}
			}
			choice = promptInput(username, 1, 3);
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
					admit[patNo]->display();
					if (admit[patNo]->getIsAssigned())
						cout << "\tAssigned to room " << (char)('A' + wIndex) << endl << endl;
					else
						cout << "\tNot assigned to any room." << endl << endl;

					patMenu(1);

					choice = promptInput(username, 1, 4);
					if (choice == 1)  // set / change room
					{
						if (admit[patNo]->getIsAssigned() == false)
						{
							cout << "Patient " << admit[patNo]->getName() << " is not assigned to any room." << endl << endl;
						}
						else
						{
							cout << "Patient " << admit[patNo]->getName() << " is in room ";
							cout << (char)('A' + wIndex) << endl << endl;
							ward[wIndex].releasePat();
						}
						assignPatient(admit[patNo], ward);
						
						pressEnter(1);

					}
					else if (choice == 2) // discharge patient
					{
						cout << "Confirm to discharge '" << admit[patNo]->getName() << "'? (Y / n): ";
						string yesno;
						cin >> yesno;
						if (yesno == "Y")
						{
							cout << admit[patNo]->getIsAssigned();
							admit[patNo]->setIsAssigned(false);
							admit[patNo]->setIsAdmit(false);
							for (int i = 0; i < 8; i++) 
							{
								if (ward[i].getPatient() == admit[patNo])  // finds patient's room and set it NULL
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
				pat[Patient::pat_NUM].addPat();
				pat[Patient::pat_NUM].setIsAdmit(true);
				cout << endl;
				assignPatient(&pat[Patient::pat_NUM], ward);
				
				Patient::pat_NUM++;
				pressEnter(1);
			}
			else if (choice == 3)  // return
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
			cout << "Thank you for using this program" << endl;
			break;
		}
	}
	saveDoctor(out, doc);
	savePatient(out, pat);
	out.open("ward.txt", ios::out);
	for (int i = 0; i < 8; i++)
		out << ward[i].getDoctorName() << "," << ward[i].getPatientName() << endl;
	out.close();
	return 0;
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
		for (int i = docNo; i < Doctor::doc_NUM; i++)
		{
			doc[i] = doc[i + 1];
		}
		cout << "Record removed." << endl << endl;
		Doctor::doc_NUM--;
	}
	else
	{
		cout << "Doctor " << doc[docNo].getName() << " not removed." << endl << endl;
	}
}

void assignDoctor(Ward*  ward, Doctor*  doc)
{
	printWardAvail(ward);

	cout << "Assign " << doc[Doctor::doc_NUM].getName() << " to room (A - H) => ";
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
				ward[wardNo].setDoctor(&doc[Doctor::doc_NUM]);
				cout << doc[Doctor::doc_NUM].getName() << " stationed to room " << chr << endl;
				break;
			}
			else
			{
				cout << "Room " << chr << " is stationed by " << ward[wardNo].getDoctorName() << endl;
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
	if (Doctor::doc_NUM == 0)
		cout << "No doctors record" << endl;
	else
	{
		for (int i = 0; i < Doctor::doc_NUM; i++)
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
		<< "  3. Exit" << endl
		<< endl;
}
void doctorMenu()
{
	cout << "  Please select an option" << endl
		<< "  1. Expand information" << endl
		<< "  2. Add doctor" << endl
		<< "  3. Remove doctor" << endl
		<< "  4. Go back" << endl
		<< endl;
}
void patMenu()
{
	cout << "  Please select an option" << endl
		<< "  1. Select patient" << endl
		<< "  2. Add patient" << endl
		<< "  3. Go back" << endl
		<< endl;
}
void patMenu(int select)
{
	cout << "  Please select an option" << endl
		<< "  1. Set / change room" << endl
		<< "  2. Discharge patient" << endl
		<< "  3. Go back" << endl << endl;
}

void docList(Doctor * p)
{
	cout << "List of doctors" << endl;
	printLine(1);
	if (Doctor::doc_NUM == 0)
		cout << "No doctors found." << endl;
	for (int i = 0; i < Doctor::doc_NUM; i++)
		cout << i + 1 << ". " << p[i].getName() << endl;
	cout << endl;
}
void patList(Patient * p)
{
	cout << "List of patients admitted" << endl;
	printLine(1);
	int count = 0;
	if (Patient::pat_NUM == 0)
		cout << "No patients found." << endl;
	for (int i = 0; i < Patient::pat_NUM; i++)
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
	for (int i = 0; i < Doctor::doc_NUM; i++) // to save person list
		d[i].writeRecord(out, i);
	out.close();
}
void savePatient(ofstream &out, Patient * p) 
{
	out.open("patient.txt");
	for (int i = 0; i < Patient::pat_NUM; i++) // to save person list
		p[i].writeRecord(out, i);
	out.close();
}
void printLine(int x)
{
	if (x == 1)
		cout << "-----------------------------" << endl;
	else if (x == 2)
		cout << "----------------------------------------------------------------------------------------------" << endl;
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
void printWard(Ward *ward, Doctor * doc, Patient * pat)
{
	cout << "\t" << repeat("-", 100) << endl;
	cout << "\t|" << repeat(" ", 17) << repeat(" ", 9) << "A" << repeat(" ", 18) << "B" 
		 << repeat(" ", 18) << "C" << repeat(" ", 18) << "D" << repeat(" ", 14) << "|" << endl;
	cout << "\t|" << repeat(" ", 17) << repeat("* ", 39) << "   |" << endl;
	for (int i = 0; i < 5; i++)
	{
		if (i == 2)
		{
			cout << "\t|" << repeat(" ", 17) << '*' << allignMid(ward[0].getPatientName(), 18, 0) << "*" 
				<< allignMid(ward[1].getPatientName() , 18, 0) << "*" << allignMid(ward[2].getPatientName(), 18, 0) << "*" 
				<< allignMid(ward[3].getPatientName(), 18, 0) << "*" << "    |" << endl;
		}
		if (i == 3)
		{
			cout << "\t|" << repeat(" ", 17) << '*' << allignMid(ward[0].getDocName(), 18, 0) << "*" 
				<< allignMid(ward[1].getDocName(), 18, 0) << "*" << allignMid(ward[2].getDocName(), 18, 0) << "*" 
				<< allignMid(ward[3].getDocName(), 18, 0) << "*" << "    |" << endl;
		}
		cout << "\t|" << repeat(" ", 17) << '*' << repeat(" ", 18) << "*" << repeat(" ", 18)
			 << "*"<< repeat (" ",18) << "*" << repeat(" ", 18) << "*" << "    |" << endl;
	}
	cout << "\t|" << repeat(" ", 17) << repeat("* ", 39) << "   |" << endl;
	cout << "\t|" << allignMid("Waiting List:",17, 0) << repeat(" ", 81) << "|" << endl;
	string s[MAX];
	int wCount;
	wCount = waitingList(pat, s);
	for (int i = 0; i < 5; i++)
	{
		cout << "\t| " << i+1 << "." << allignMid(s[i], 15, 1) << repeat(" ", 80) << "|" << endl;
	}
	if (wCount > 5)
	{
		int tmp = wCount - 5;
		string overFlow = to_string(tmp);
		string cat = "+ ";
		cat += overFlow;
		cat += " more";
		cout << "\t|" << allignMid(cat , 17, 0) << repeat(" ", 81) << "|" << endl;
	}
	cout << "\t|" << repeat(" ", 17) << repeat("* ", 39) << "   |" << endl;
	for (int i = 0; i < 5; i++)
	{
		if (i == 2)
		{
			cout << "\t|" << repeat(" ", 17) << '*' << allignMid(ward[4].getPatientName(), 18, 0) << "*" 
				<< allignMid(ward[5].getPatientName(), 18, 0) << "*" << allignMid(ward[6].getPatientName(), 18, 0) << "*" 
				<< allignMid(ward[7].getPatientName(), 18, 0) << "*" << "    |" << endl;
		}
		if (i == 3)
		{
			cout << "\t|" << repeat(" ", 17) << '*' << allignMid(ward[4].getDocName(), 18, 0) << "*" 
				<< allignMid(ward[5].getDocName(), 18, 0) << "*" << allignMid(ward[6].getDocName(), 18, 0) << "*" 
				<< allignMid(ward[7].getDocName(), 18, 0) << "*" << "    |" << endl;
		}
		cout << "\t|" << repeat(" ", 17) << '*' << repeat(" ", 18) << "*" << repeat(" ", 18)
			 << "*"<< repeat (" ",18) << "*" << repeat(" ", 18) << "*" << "    |" << endl;
	}
	cout << "\t|" << repeat(" ", 17) << repeat("* ", 39) << "   |" << endl;
	cout << "\t|" << repeat(" ", 17) << repeat(" ", 9) << "E" << repeat(" ", 18) << "F"
		 << repeat(" ", 18) << "G" << repeat(" ", 18) << "H" << repeat(" ", 14) << "|" << endl;
	cout << "\t" << repeat("-", 100) << endl;
	cout << endl;
}
void dischargePat(Ward *ward, Patient *pat, int wIndex)
{
	ward[wIndex].setPatient(NULL);
	ward[wIndex].setIsOccupied(false);
	pat->setIsAssigned(false);
}
int waitingList(Patient *pat, string *s)
{
	int count = 0;
	for (int i = 0; i < Patient::pat_NUM; i++)
	{
		if (!pat[i].getIsAssigned() && pat[i].getIsAdmit())
		{
			s[count] = pat[i].getName();
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

	cout << "Assign " << pat->getName() << " to room (A - H)" << endl
		<< "Enter 'Z' to assign later " << " => ";
	cin >> chr;
	chr = toupper(chr);
	while (cin)
	{
		if (chr != 'A' && chr != 'B' && chr != 'C' && chr != 'D' &&
			chr != 'E' && chr != 'F' && chr != 'G' && chr != 'H' && chr != 'Z')
		{
			cout << "Please enter correct room => ";
			cin >> chr;
			chr = toupper(chr);
		}
		if (chr == 'Z')
		{
			pat->setIsAssigned(false);
			cout << pat->getName() << " added to waiting list." << endl;
			break;
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
				cout << "Room " << chr << " is occupied by " << ward[wardNo].getPatient()->getName() << endl;
				cout << "Please enter another room => ";
				cin >> chr;
				chr = toupper(chr);
			}
		}
	}
}
