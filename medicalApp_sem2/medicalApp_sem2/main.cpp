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
int checkNum(int, int);

void unbindPatient(Doctor *, int, int, int);
void doctorMenu();
void findPatientInDoc(Doctor *, Patient *, int, int &, int &);
void docList(Doctor *);
void patList(Patient *);
void patMenu(int);
void deleteDoc(Doctor);
void saveDoc(fstream &, Doctor*);
void savePat(fstream &, Patient *);

const int MAX = 20;

int main()
{
	string username = "weiwen";
	cout << fixed << setprecision(2);
	Doctor doc[MAX];
	Patient pat[MAX];

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
			if (doc[i].getNoPat() != 0)
			{
				for (int k = 0; k < doc[i].getNoPat(); k++)
				{
					int x = doc[i].getPatIndex(k);
					doc[i].initPatient(&pat[x], k);
				}
			}
		}
	}
	inp.close();

	int choice = 0;
	while (choice == 0)
	{
		displayMenu();
		choice = promptInput(username, 1, 6);
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
				doc[Doctor::doc_NUM].addDoc();
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
			//  Staffs

			break;
		}
		while (choice == 3)
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
					int di = 0;
					int pi = 0;

					findPatientInDoc(doc, pat, patNo, di, pi);
					pat[patNo].display();
					if (pat[patNo].getAssigned())
						cout << "\tAssigned to Dr. " << doc[di].getName() << endl << endl;
					else
						cout << "\tNot assigned to any doctor." << endl << endl;

					patMenu(2);
					choice = promptInput(username, 1, 3);
					if (choice == 1)  // change doctor
					{

						if (pat[patNo].getAssigned())
						{
							if (pi + 1 < doc[di].getNoPat())
							{
								doc[di].patient[pi] = NULL;
								doc[di].patIndex[pi] = 0;
								for (int i = pi; i < doc[di].getNoPat() - 1; i++)
								{
									doc[di].patient[i] = doc[di].patient[i + 1];
									doc[di].patIndex[i] = doc[di].patIndex[i + 1];
								}
								doc[di].noPat--;
							}
							else
							{
								doc[di].patient[pi] = NULL;
								doc[di].noPat--;
								doc[di].patIndex[pi] = 0;
							}
						}
						else
						{
							cout << pat[patNo].getName() << " is not assigned to any doctor." << endl << endl;
						}
						docList(doc);
						if (Doctor::doc_NUM != 0)
						{
							cout << "Assign to doctor number => ";
							int docNo = checkNum(0, Doctor::doc_NUM);
							doc[docNo - 1].setPatient(&pat[patNo], patNo);
							cout << pat[patNo].getName() << " is now patient of " << doc[docNo - 1].getName() << "." << endl;
						}
						cout << endl;
						pressEnter(1);
					}
					else if (choice == 2)  // discharge patient
					{
						cout << "Confirm to discharge '" << pat[patNo].getName() << "'? (Y / n): ";
						string yesno;
						cin >> yesno;
						if (yesno == "Y")
						{
							cout << pat[patNo].getAssigned();
							if (pat[patNo].getAssigned())
							{
								unbindPatient(doc, di, pi, patNo);
							}
							for (int i = patNo; i < Patient::pat_NUM; i++)
							{
								pat[i] = pat[i + 1];
							}
							Patient::pat_NUM--;

							for (int i = 0; i < Doctor::doc_NUM; i++)
							{
								for (int j = 0; j < doc[i].getNoPat(); j++)
								{
									cout << doc[i].getName() << (doc[i].getPatIndex(j));
									doc[i].initPatient(&pat[(doc[i].getPatIndex(j))], j);
									doc[i].patient[j]->getName();
								}
							}
							cout << "Patient discharged." << endl;
						}
						else
						{
							cout << "Patient not discharged." << endl;
						}
						pressEnter(1);
					}
				}
			}
			else if (choice == 2)  // add patient
			{
				pat[Patient::pat_NUM].addPat();
				docList(doc);
				if (Doctor::doc_NUM != 0)
				{
					cout << endl
						<< "Assign to doctor number => ";
					int temp = checkNum(0, Doctor::doc_NUM);
					doc[temp - 1].setPatient(&pat[Patient::pat_NUM], Patient::pat_NUM);
					pressEnter(1);
				}
				else
				{
					cout << endl;
					pressEnter(0);
				}
				Patient::pat_NUM++;
			}
			else if (choice == 3)  // return
			{
				savePat(out, pat);
				choice = 0;
				break;
			}
			savePat(out, pat);
			choice = 3;
		}
		while (choice == 4)
		{
			//  Attendance

			break;
		}
		while (choice == 5)
		{
			//  Wards

			break;
		}
		if (choice == 6)
		{
			cout << "Thank you for using this program" << endl;
			break;
		}
	}
	saveDoc(out, doc);
	savePat(out, pat);
	return 0;
}

void displayMenu()
{
	cout << "  Ward 3 information center" << endl;
	printLine(1);
	cout << "  Please select an option" << endl
		<< "  1. Doctor" << endl
		<< "  2. Staff" << endl
		<< "  3. Patient" << endl
		<< "  4. Employee's attendence" << endl
		<< "  5. Ward" << endl
		<< "  6. Exit" << endl
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
			<< "  1. Change doctor" << endl
			<< "  2. Discharge patient" << endl
			<< "  3. Go back" << endl << endl;
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
	cout << "List of patients" << endl;
	printLine(1);
	if (Patient::pat_NUM == 0)
		cout << "No patients found." << endl;
	for (int i = 0; i < Patient::pat_NUM; i++)
		cout << i + 1 << ". " << p[i].getName() << endl;
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

void unbindPatient(Doctor * doc, int di, int pi, int patNo)
{
	if (pi + 1 < doc->getNoPat())
	{
		doc[di].patIndex[pi] = 0;
		for (int i = pi; i < doc[di].getNoPat(); i++)
		{
			if (&doc[di].patient[i + 1] != NULL)
				doc[di].patIndex[i] = doc[di].patIndex[i + 1];
		}
		doc[di].noPat--;
	}
	else
	{
		doc[di].patient[pi] = NULL;
		doc[di].noPat--;
		doc[di].patIndex[pi] = 0;
	}
	for (int i = 0; i < Doctor::doc_NUM; i++)
	{
		for (int j = 0; j < doc[i].getNoPat(); i++)
		{
			if (doc[i].patIndex[j] > patNo)
				doc[i].patIndex[j]--;
		}
	}
}
void findPatientInDoc(Doctor * doc, Patient * pat, int temp, int &di, int &pi)
{
	for (int i = 0; i < Doctor::doc_NUM; i++)
	{
		for (int j = 0; j < doc[i].getNoPat(); j++)
			if (doc[i].getPatient(j) == &pat[temp])
			{
				pi = j;
				di = i;
			}
	}
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


void deleteDoc(Doctor *d)
{
	cout << "Remove doctor number: ";
	int docNo = checkNum(0, Doctor::doc_NUM);
	docNo--;
	cout << "Confirm to remove '" << d[docNo].getName() << "' from system? (Y / n): ";
	string yesno;
	cin >> yesno;
	if (yesno == "Y")
	{
		for (int i = 0; i < d[docNo].noPat; i++)
		{
			d[docNo].patient[i]->setAssigned(false);
		}
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