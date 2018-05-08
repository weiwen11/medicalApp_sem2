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
	bool isAssigned;
public:
	Person();
	string getName() const;
	string getIc() const;
	string getGender() const;
	string getContactNum() const;
	string getContactEmail() const;
	bool getIsAssigned() const;
	void setIsAssigned(bool);
	void printPerson() const;
	virtual void readRecord(ifstream &) {};
	virtual void writeRecord(ofstream &, int) const{};
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
	bool isAdmit;
public:
	Patient();
	static int pat_NUM;
	void addPat();
	double getWeight() const;
	double getHeight() const;
	double getBMI() const;
	string getBMIStatus(double) const;
	string getCondition() const;
	
	bool getIsAdmit() const;
	void setIsAdmit(bool);
	void readRecord(ifstream &);
	void writeRecord(ofstream &, int) const;
	void display() const;
};
class Doctor : public Employee
{

public:
	Doctor();
	static int doc_NUM;
	void addDoc();
	void readRecord(ifstream &);
	void writeRecord(ofstream &, int) const;
	void display(int) const;
	friend void deleteDoc(Doctor *);
	friend void unbindPatient(Doctor *, int, int, int);
};
#endif // !PERSON_H