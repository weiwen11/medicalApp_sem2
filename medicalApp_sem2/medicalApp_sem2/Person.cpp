#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "Person.h"

using namespace std;
int Doctor::doc_NUM = 0;
int Patient::pat_NUM = 0;

void printLine(int);
Contact::Contact()
{
	email = "";
	phoneNum = "";
}
void Contact::setPhone(string p) { phoneNum = p; }
void Contact::setEmail(string e) { email = e; }
string Contact::getPhoneNum() const { return phoneNum; }
string Contact::getEmail() const { return email; }

Person::Person()
{
	name = "";
	ic = "";
	gender = "";
}
string Person::getName() const
{
	return name;
}
string Person::getIc() const
{
	return ic;
}
string Person::getGender() const
{
	return gender;
}
string Person::getContactNum() const
{
	return contact.getPhoneNum();
}
string Person::getContactEmail() const
{
	return contact.getEmail();
}
void Person::printPerson() const
{
	cout << setw(15) << "Name"
		<< ": " << setw(30) << name
		<< setw(15) << "Phone Number"
		<< ": " << contact.getPhoneNum() << endl

		<< setw(4) << "" << setw(15) << "Gender"
		<< ": " << setw(30) << gender
		<< setw(15) << "Email"
		<< ": " << contact.getEmail() << endl

		<< setw(4) << "" << setw(15) << "IC"
		<< ": " << setw(30) << ic << endl;
}
void Person::readRecord(ifstream & inp)
{
}
void Person::readBasic()
{
	cout << "Basic information" << endl;
	printLine(1);
	cout << "\tName\t\t=> ";

	cin.ignore();
	getline(cin, name);

	cout << "\tGender (M / F)\t"
		<< "=> ";
	cin >> gender;
	gender[0] = toupper(gender[0]);
	if (gender[0] == 'M')
		gender = "Male";
	else if (gender[0] == 'F')
		gender = "Female";

	cout << "\tIC\t\t=> ";
	cin >> ic;
}
void Person::readContact()
{
	cout << "Contact information" << endl;
	printLine(1);
	cout << "\tPhone number\t=> ";
	string foo;
	cin >> foo;
	contact.setPhone(foo);
	cout << "\tEmail address\t=> ";
	cin >> foo;
	contact.setEmail(foo);
}

Employee::Employee()
{
	position = "";
	payRate = 0;
}
double Employee::findSalary() const
{
	return payRate * 12.00 * 30.0;
}
string Employee::getPosName() const
{
	return position;
}

Patient::Patient()
{
	condition = "";
	height = 0;
	weight = 0;
	assigned = false;
}
void Patient::addPat()
{
	cout << "New patient" << endl << endl;
	readBasic();

	cout << "\tHeight\t\t"
		<< "=> ";
	cin >> height;
	cout << "\tWeight\t\t"
		<< "=> ";
	cin >> weight;
	cout << "\tCondition\t"
		<< "=> ";
	cin.ignore();
	getline(cin, condition);
	cout << endl << endl;
	readContact();
	cout << endl;
}
double Patient::getBMI() const
{
	return weight / (pow((height / 100), 2));
}
string Patient::getBMIStatus(double BMI) const
{
	if (BMI < 17.50)
		return "Underweight";
	else if (BMI >= 17.50 && BMI < 23.00)
		return "Normal weight";
	else if (BMI >= 23.00 && BMI < 28.00)
		return "Over weight";
	else
		return "Obese";
}
string Patient::getCondition() const
{
	return condition;
}
void Patient::readRecord(ifstream & inp)
{
	string test;
	getline(inp, test, ',');
	if (!test.empty())
	{
		name = test;
		getline(inp, gender, ',');
		getline(inp, test, ',');
		contact.setPhone(test);
		getline(inp, test, ',');
		contact.setEmail(test);
		getline(inp, ic, ',');
		getline(inp, condition, ',');
		inp >> height >> weight >> assigned;
		inp.ignore();
		pat_NUM++;
	}
}
void Patient::writeRecord(fstream & out, int i) const
{
	out << name << "," << gender << "," << contact.getPhoneNum() << ","
		<< contact.getEmail() << "," << ic << ","
		<< condition << "," << height << " " << weight << " " << assigned;
	if (i != pat_NUM - 1)
		out << endl;
}
void Patient::display() const
{
	cout << "Patient's information:" << endl;
	printLine(1);
	cout << endl;
	cout << left << setw(4) << "";
	printPerson();
	cout << setw(4) << "" << setw(15) << "Height "
		<< ": " << height << " cm" << endl
		<< setw(4) << "" << setw(15) << "Weight "
		<< ": " << weight << " kg" << endl
		<< setw(4) << "" << setw(15) << "BMI"
		<< ": " << getBMI() << " (" << getBMIStatus(getBMI()) << ") " << endl
		<< setw(4) << "" << setw(15) << "Condition "
		<< ": " << condition << endl << endl;
}
bool Patient::getAssigned() const
{
	return assigned;
}
double Patient::getWeight() const
{
	return weight;
}
double Patient::getHeight() const
{
	return height;
}
void Patient::setAssigned(bool a)
{
	assigned = a;
}

Doctor::Doctor()
{
	noPat = 0;
	for (int i = 0; i < 10; i++)
	{
		patient[i] = NULL;
		patIndex[i] = NULL;
	}

}
void Doctor::addDoc()
{
	for (int i = 0; i < 10; i++)
		patient[i] = NULL;
	noPat = 0;
	doc_NUM++;
	cout << "New employee" << endl
		<< endl;
	readBasic();

	cout << "\t(1:Intern    2:Doctor    3:Senior Doctor)\n";
	cout << "\tPosition\t=> ";
	string tmp;
	cin >> tmp;

	while (tmp != "1" && tmp != "2" && tmp != "3")
	{
		cout << "Please enter a correct position number: ";
		cin >> tmp;
	}
	if (tmp == "1")
		position = "Intern";

	else if (tmp == "2")
		position = "Doctor";

	else if (tmp == "3")
		position = "Senior Doctor";


	cout << "\tPay rate / hr\t=> ";
	cin >> payRate;
	findSalary();
	cout << endl;
	readContact();
	cout << endl;
	cout << "Record Saved. ";
}
void Doctor::setPatient(Patient *p, int i)
{
	patIndex[noPat] = i;
	patient[noPat] = p;
	noPat++;
	p->setAssigned(true);
}
int Doctor::getNoPat() const
{
	return noPat;
}
int Doctor::getPatIndex(int i) const
{
	return patIndex[i];
}
Patient * Doctor::getPatient(int index) const
{
	return patient[index];
}
void Doctor::initPatient(Patient *p, int i)
{
	patient[i] = p;
}
void Doctor::readRecord(ifstream &inp)
{
	string test;
	getline(inp, test, ',');
	if (!test.empty())
	{
		name = test;
		getline(inp, gender, ',');
		getline(inp, ic, ',');
		getline(inp, position, ',');
		getline(inp, test, ',');
		contact.setPhone(test);
		getline(inp, test, ',');
		contact.setEmail(test);
		inp >> payRate >> noPat;
		for (int i = 0; i < noPat; i++)
		{
			inp >> patIndex[i];
		}
		inp.ignore();
		doc_NUM++;
	}
}
void Doctor::writeRecord(fstream &out, int i) const
{
	out << name << "," << gender << "," << ic << "," << position << ","
		<< contact.getPhoneNum() << "," << contact.getEmail() << ","
		<< payRate << " " << noPat;
	for (int i = 0; i < noPat; i++)
	{
		out << " " << patIndex[i];
	}
	if (i != doc_NUM - 1)
		out << endl;
}
void Doctor::display(int x) const
{
	cout << x + 1 << left << setw(3) << ".";
	printPerson();
	cout << setw(4) << "" << setw(15) << "Position"
		<< ": " << getPosName() << endl
		<< setw(4) << "" << setw(15) << "Salary"
		<< ": RM" << findSalary() << endl
		<< endl
		<< "Patient list: ";
	for (int j = 0; j < noPat; j++)
	{
		if (j == noPat - 1)
			cout << patient[j]->getName() << ". ";
		else
			cout << patient[j]->getName() << ", ";
	}
	if (noPat == 0)
	{
		cout << "none";
	}
	cout << endl;
	printLine(2);
	cout << endl;
}