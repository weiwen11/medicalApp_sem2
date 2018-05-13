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
	string getIC() const;
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
class TicketMaster
{
	int ticket;
	static int tix_NUM;
public:
	TicketMaster()
	{
		ticket = 0;
	}
	static int getTix_NUM()
	{
		return tix_NUM;
	}
	int getTicket()const
	{
		return ticket;
	}
	void giveTicket()
	{
		ticket = tix_NUM;
		tix_NUM++;
	}
	void setTicket(int t)
	{
		ticket = t;
	}
	void reset()
	{
		ticket = 0;
	}
	static void initTicketNum(int a)
	{
		tix_NUM = a+1;
	}
};
class Patient : public Person
{
	string condition;
	double height, weight;
	bool isAdmit;
	TicketMaster ticket;
public:
	Patient();
	static int pat_NUM;
	void addPat();
	double getWeight() const;
	double getHeight() const;
	double getBMI() const;
	string getBMIStatus(double) const;
	string getCondition() const;
	int getTicket() const;
	
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
};
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
	string getDoctorIC() const;
	string getDoctorName() const;
	string getPatientIC() const;
	string getDoctorNameInBracket() const;
	bool getIsOccupied() const;
	string getAvail() const;
	double getRate() const;
	void releasePat();
	void releaseDoc();
};
#endif // !PERSON_H