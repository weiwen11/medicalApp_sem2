#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "Person.h"
using namespace std;

void printLine(int);
int promptInput(string, int, int);
void pressEnter(bool);
void displayMenu();
void assignPatToDoc(Doctor  *, Patient  *, int);
int checkNum(int, int);

void unbindPatient(Doctor *, int, int, int);
void doctorMenu();
void findPatientInDoc(Doctor *, Patient *, int, int &, int &);
void docList(Doctor *);
void patList(Patient *);
void patMenu(int);
void deleteDoc(Doctor *, Ward *);
void saveDoc(fstream &, Doctor*);
void savePat(fstream &, Patient *);

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
	void patientPtrAdjust();

	bool getIsStationed() const;
	Doctor *getDoctor();
	Patient *getPatient() const;
	string getPatientName() const;
	string getRoomDoc() const;
	bool getIsOccupied() const;
	string getAvail() const;
	double getRate() const;
	void releasePat();
	void releaseDoc();
	friend void printWard();
};


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

void Ward::patientPtrAdjust()
{
	patient--;
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
string Ward::getRoomDoc() const
{
	int index = 0;
	int docIndex, tmp;
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
	if (patient == NULL)
		return "Available";
	else
		return getPatientName();
}

double Ward::getRate() const
{
	return rate;
}
string repeat(string a, int max)
{
	string s = "";
	for (int i = 0; i < max; i++)
		s += a;
	return s;
}
string allignMid(string s, int width)
{
	int length = s.length();
	string result = "";
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
const int MAX = 1000;

int main()
{
	string username = "weiwen";
	cout << fixed << setprecision(2);
	Doctor doc[20];
	Patient pat[MAX];
	Ward ward[9];

	// input doctor and patient data
	ifstream inp;
	fstream out;

	inp.open("patient.txt");
	if (!inp)
	{
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
		cout << "Creating doctor.txt..." << endl
			<< endl;
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
		cout << "Creating ward.txt..." << endl;
	else
	{
		string inName;
		for (int i = 0; i < 8; i++)
		{
			getline(inp, inName);
			for (int j = 0; j < Patient::pat_NUM; j++)
			{
				if (pat[j].getName() == inName)
				{
					ward[i].setPatient(&pat[j]);
				}
			}
		}
	}
	inp.close();

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
					/*if (doc[Doctor::doc_NUM].getIsAssigned() == false)
					{
						cout << "Doctor " << doc[Doctor::doc_NUM].getName() << " is not stationed to any room." << endl << endl;
					}
					else
					{
						cout << "Doctor " << doc[Doctor::doc_NUM].getName() << "is stationed in room ";
						for (int i = 0; i < 8; i++)
						{
							if (ward[i].getDoctor() == &doc[Doctor::doc_NUM])
							{
								cout << 'A' + i << endl << endl;
							}
						}
					}*/
					printWardAvail(ward);

					cout << "Assign " << doc[Doctor::doc_NUM].getName() << " to room (A - H) => " << endl;
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
								cout << "gg" << wardNo << ward[wardNo].getDoctor()->getName();
								break;
							}
							else
							{
								cout << "Room " << chr << " is stationed by " << ward[wardNo].getDoctor()->getName() << endl;
								cout << "Please enter another room: ";
								cin >> chr;
								chr = toupper(chr);
							}
						}
					}
				}
				pressEnter(1);
			}
			else if (choice == 3) // remove doctor
			{
				if (Doctor::doc_NUM != 0)
				{
					deleteDoc(doc);
				}
				else
				{
					cout << "No record found. ";
					pressEnter(1);
				}
			}
			else if (choice == 4) // go back
			{
				saveDoc(out, doc);
				choice = 0;
				break;
			}
			saveDoc(out, doc);
			choice = 1;
		}
		while (choice == 2)
		{
			patList(pat);

			patMenu(1);
			choice = promptInput(username, 1, 3);
			if (choice == 1)  // selected patient
			{
				if (Patient::pat_NUM == 0)
					cout << "No patients found." << endl << endl;
				else
				{
					cout << "Enter patient's number => ";
					int patNo = checkNum(0, Patient::pat_NUM);
					patNo--;
					int wIndex;

					for (int i = 0; i < 8; i++)
						if (ward[i].getPatient() == &pat[patNo])
							wIndex = i;
					pat[patNo].display();
					if (pat[patNo].getIsAssigned())
						cout << "\tAssigned to room " << 'A' + wIndex << endl << endl;
					else
						cout << "\tNot assigned to any doctor." << endl << endl;

					patMenu(2);
					choice = promptInput(username, 1, 4);
					//if (choice == 1)  // change room
					//{

					//	int temp;
					//	if (pat[patNo].getAssigned() == false)
					//	{
					//		cout << pat[patNo].getName() << " is not assigned to any doctor." << endl << endl;
					//		docList(doc);
					//		if (Doctor::doc_NUM != 0)
					//		{
					//			cout << endl << "Assign to doctor number => ";
					//			temp = checkNum(0, Doctor::doc_NUM);
					//			temp--;
					//			doc[temp].setPatient(&pat[patNo], patNo);
					//			cout << pat[patNo].getName() << " assigned to Dr " << doc[temp].getName() << endl;
					//		}
					//	}
					//	else
					//	{
					//		docList(doc);
					//		if (Doctor::doc_NUM != 0)
					//		{
					//			cout << endl << "Assign to doctor number => ";
					//			temp = checkNum(0, Doctor::doc_NUM);
					//			temp--;
					//			if (temp == di)
					//			{
					//				cout << "Already assigned to " << doc[di].getName() << endl;
					//			}
					//			else
					//			{
					//				if (pi + 1 < doc[di].getNoPat())
					//				{
					//					doc[di].patient[pi] = NULL;
					//					doc[di].patIndex[pi] = 0;
					//					for (int i = pi; i < doc[di].getNoPat() - 1; i++)
					//					{
					//						doc[di].patient[i] = doc[di].patient[i + 1];
					//						doc[di].patIndex[i] = doc[di].patIndex[i + 1];
					//					}
					//					doc[di].noPat--;
					//				}
					//				else
					//				{
					//					doc[di].patient[pi] = NULL;
					//					doc[di].noPat--;
					//					doc[di].patIndex[pi] = 0;
					//				}
					//				doc[temp].setPatient(&pat[patNo], patNo);
					//				cout << pat[patNo].getName() << " assigned to Dr " << doc[temp].getName() << endl;
					//			}
					//		}
					//	}
					//	pressEnter(1);
					//}
					if (choice == 2)  // set / change room
					{
						if (pat[patNo].getIsAssigned() == false)
						{
							cout << "Patient " << pat[patNo].getName() << " is not assigned to any room." << endl << endl;
						}
						else
						{
							cout << "Patient " << pat[patNo].getName() << "is in room ";
							for (int i = 0; i < 8; i++)
							{
								if (ward[i].getPatient() == &pat[patNo])
								{
									cout << 'A' + i << endl << endl;
								}
							}
						}
						printWardAvail(ward);
						cout << "Assign " << pat[patNo].getName() << " to room (A - H)" << endl
							<< "Enter 'Z' to assign later " << " => ";
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
							else if (chr == 'Z')
							{
								cout << pat[patNo].getName() << " added to waiting list." << endl;
								break;
							}
							else
							{
								wardNo = chr - 'A';
								if (ward[wardNo].getIsOccupied() == 0)
								{
									ward[wardNo].setPatient(&pat[patNo]);
									cout << pat[patNo].getName() << " assinged to room " << chr << endl;
									cout << "gg" << wardNo << ward[wardNo].getPatient()->getName();
									break;
								}
								else
								{
									cout << "Room " << chr << " is occupied by " << ward[wardNo].getPatient()->getName() << endl;
									cout << "Please enter another room: ";
									cin >> chr;
									chr = toupper(chr);
								}
							}
						}

					}
					else if (choice == 3) // discharge patient
					{
						cout << "Confirm to discharge '" << pat[patNo].getName() << "'? (Y / n): ";
						string yesno;
						cin >> yesno;
						if (yesno == "Y")
						{
							cout << pat[patNo].getIsAssigned();
							pat[patNo].setIsAssigned(false);
							pat[patNo].setIsAdmit(false);
							for (int i = 0; i < 8; i++) 
							{
								if (ward[i].getPatient() == &pat[patNo])  // finds patient's room and set it NULL
								{
									ward[i].releasePat();
								}
								//if (ward[i].getPatient() > &pat[patNo])  // adjust ward ptr
								//{
								//	ward[i].patientPtrAdjust();
								//}
							}
							//for (int i = patNo; i < Patient::pat_NUM; i++)
							//{
							//	pat[i] = pat[i + 1];
							//}
							//Patient::pat_NUM--;

							for (int i = 0; i < Doctor::doc_NUM; i++)
							{
/*								for (int j = 0; j < doc[i].getNoPat(); j++)
								{
									if (doc[i].patIndex[j] > patNo && pass == false)
										doc[i].patIndex[j]--;
									cout << doc[i].getName() << (doc[i].getPatIndex(j));
									doc[i].initPatient(&pat[(doc[i].getPatIndex(j))], j);
									cout << doc[i].patient[j]->getName();

								}*/
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
				char chr;
				int wardNo;
				cout << endl;
				printWardAvail(ward);
				cout << "Assign " << pat[Patient::pat_NUM].getName() << " to room (A - H)" << endl
					<< "Enter 'Z' to assign later " << " => ";
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
					else if (chr == 'Z')
					{
						cout << pat[Patient::pat_NUM].getName() << " not assigned to any room" << endl;
						break;
					}
					else
					{
						wardNo = chr - 'A';
						if (ward[wardNo].getIsOccupied() == 0)
						{
							ward[wardNo].setPatient(&pat[Patient::pat_NUM]);
							cout << pat[Patient::pat_NUM].getName() << " assinged to room " << chr << endl;
							cout << "gg" << wardNo << ward[wardNo].getPatient()->getName();
							break;
						}
						else
						{
							cout << "Room " << chr << " is occupied by " << ward[wardNo].getPatient()->getName() << endl;
							cout << "Please enter another room: ";
							cin >> chr;
							chr = toupper(chr);
						}
					}
				}
				Patient::pat_NUM++;
				pressEnter(1);
			}
			else if (choice == 3)  // return
			{
				savePat(out, pat);
				choice = 0;
				break;
			}
			savePat(out, pat);
			choice = 2;
		}
		if (choice == 3)
		{
			cout << "Thank you for using this program" << endl;
			break;
		}
	}
	saveDoc(out, doc);
	savePat(out, pat);
	out.open("ward.txt", ios::out);
	for (int i = 0; i < 8; i++)
		out << ward[i].getPatientName() << endl;
	out.close();
	return 0;
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
void patMenu(int select)
{
	if (select == 1)
	{
		cout << "  Please select an option" << endl
			<< "  1. Select patient" << endl
			<< "  2. Add patient" << endl
			<< "  3. Go back" << endl
			<< endl;
	}
	else if (select == 2)
	{
		cout << "  Please select an option" << endl
			<< "  1. Set / change doctor" << endl
			<< "  2. Set / change room" << endl
			<< "  3. Discharge patient" << endl
			<< "  4. Go back" << endl << endl;
	}
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
	if (Patient::pat_NUM == 0)
		cout << "No patients found." << endl;
	for (int i = 0; i < Patient::pat_NUM; i++)
	{
		if (p[i].getIsAdmit() == true)
		{
			cout << i + 1 << ". " << p[i].getName() << endl;
		}
	}
	cout << endl;
}
void savePat(fstream &out, Patient * pat)
{
	out.open("patient.txt", ios::out);
	for (int i = 0; i < Patient::pat_NUM; i++) // to save patient list
		pat[i].writeRecord(out, i);
	out.close();
}

void saveDoc(fstream &out, Doctor * doc)
{
	out.open("doctor.txt", ios::out);
	for (int i = 0; i < Doctor::doc_NUM; i++) // to save doctor list
		doc[i].writeRecord(out, i);
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


void deleteDoc(Doctor *d, Ward *ward, int wIndex)
{
	cout << "Remove doctor number: ";
	int docNo = checkNum(0, Doctor::doc_NUM);
	docNo--;
	cout << "Confirm to remove '" << d[docNo].getName() << "' from system? (Y / n): ";
	string yesno;
	cin >> yesno;
	if (yesno == "Y")
	{
		for (int i = docNo; i < Doctor::doc_NUM; i++)
		{
			d[i] = d[i + 1];
		}
		Doctor::doc_NUM--;
		cout << "Record removed." << endl;
	}
	else
		cout << "Record not removed." << endl;
	cout << endl;
	pressEnter(1);
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
			cout << "\t|" << repeat(" ", 17) << '*' << allignMid(ward[0].getPatientName(), 18) << "*" 
				<< allignMid(ward[1].getPatientName() , 18) << "*" << allignMid(ward[2].getPatientName(), 18) << "*" 
				<< allignMid(ward[3].getPatientName(), 18) << "*" << "    |" << endl;
		}
		if (i == 3)
		{
			cout << "\t|" << repeat(" ", 17) << '*' << allignMid(ward[0].getRoomDoc(), 18) << "*" 
				<< allignMid(ward[1].getRoomDoc(), 18) << "*" << allignMid(ward[2].getRoomDoc(), 18) << "*" 
				<< allignMid(ward[3].getRoomDoc(), 18) << "*" << "    |" << endl;
		}
		cout << "\t|" << repeat(" ", 17) << '*' << repeat(" ", 18) << "*" << repeat(" ", 18)
			 << "*"<< repeat (" ",18) << "*" << repeat(" ", 18) << "*" << "    |" << endl;
	}
	cout << "\t|" << repeat(" ", 17) << repeat("* ", 39) << "   |" << endl;
	cout << "\t|" << " Waiting Area" << repeat(" ", 85) << "|" << endl;
	for (int i = 0; i < 6; i++)
	{
		cout << "\t|" << repeat(" ", 98) << "|" << endl;
	}
	cout << "\t|" << repeat(" ", 17) << repeat("* ", 39) << "   |" << endl;
	for (int i = 0; i < 5; i++)
	{
		if (i == 2)
		{
			cout << "\t|" << repeat(" ", 17) << '*' << allignMid(ward[4].getPatientName(), 18) << "*" 
				<< allignMid(ward[5].getPatientName(), 18) << "*" << allignMid(ward[6].getPatientName(), 18) << "*" 
				<< allignMid(ward[7].getPatientName(), 18) << "*" << "    |" << endl;
		}
		if (i == 3)
		{
			cout << "\t|" << repeat(" ", 17) << '*' << allignMid(ward[4].getRoomDoc(), 18) << "*" 
				<< allignMid(ward[5].getRoomDoc(), 18) << "*" << allignMid(ward[6].getRoomDoc(), 18) << "*" 
				<< allignMid(ward[7].getRoomDoc(), 18) << "*" << "    |" << endl;
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
