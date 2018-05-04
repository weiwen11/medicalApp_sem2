#ifndef PERSON_H
#define PERSON_H

#include <fstream>
#include <iostream>
#include <string>
using namespace std;
class Contact
{
private:
	string phoneNum, email;
public:
	Contact();
	void setPhone(string);
	void setEmail(string);
	string getPhoneNum() const;
	string getEmail() const;
};
class Person
{
protected:
	string name, ic, gender;
	Contact contact;
public:
	Person();
	string getName() const;
	string getIc() const;
	string getGender() const;
	string getContactNum() const;
	string getContactEmail() const;
	void printPerson() const;
	virtual void readRecord(ifstream &);
	void readBasic();
	void readContact();
};
class Employee : public Person
{
protected:
	string position;
	double payRate;
public:
	Employee();
	double findSalary() const;
	string getPosName() const;
};
class Patient : public Person
{
	string condition;
	double height, weight;
	bool assigned;
public:
	Patient();
	static int pat_NUM;
	void addPat();
	double getWeight() const;
	double getHeight() const;
	double getBMI() const;
	string getBMIStatus(double) const;
	string getCondition() const;
	bool getAssigned() const;
	void setAssigned(bool);
	void readRecord(ifstream &);
	void writeRecord(fstream &, int) const;
	void display() const;
};
class Doctor : public Employee
{

public:
	Doctor();
	Patient *patient[10];
	int noPat;
	int patIndex[10];
	static int doc_NUM;
	void setPatient(Patient *, int);
	Patient *getPatient(int) const;
	int getNoPat() const;
	int getPatIndex(int) const;
	void addDoc();
	void initPatient(Patient *, int);
	void readRecord(ifstream &);
	void writeRecord(fstream &, int) const;
	void display(int) const;
	friend void deleteDoc(Doctor *);
	friend void unbindPatient(Doctor *, int, int, int);
};
#endif // !PERSON_H