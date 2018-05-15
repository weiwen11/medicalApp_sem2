#ifndef PERSON_H
#define PERSON_H

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class Admin
{
private:
	char username[20];
	char password[20];
public:
	string getUsername();
	string getPW();
	void init();
	bool readRecord();
	void writeRecord();
};

class Contact
{
private:
	string phoneNum;
	string email;
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
	string name;
	string ic;
	string gender;
	bool isAssigned;
	Contact contact;
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
class TicketMaster
{
	int ticket;
	static int tix_NUM;
public:
	TicketMaster();
	static int getTix_NUM();
	int getTicket() const;

	void giveTicket();
	void setTicket(int t);
	void reset();
	static void initTicketNum(int a);
};
class Patient : public Person
{
	string condition;
	double height;
	double weight;
	bool isAdmit;
	static int pat_NUM;
	TicketMaster ticket;
public:
	Patient();
	bool addPat(Patient *);
	double getWeight() const;
	double getHeight() const;
	double getBMI() const;
	string getBMIStatus(double) const;
	string getCondition() const;
	int getTicket() const;
	static int getPatNum();
	bool getIsAdmit() const;

	static void incPatNum();
	void setIsAdmit(bool);
	void readRecord(ifstream &);
	void writeRecord(ofstream &, int) const;
	void display() const;
};
class Doctor : public Person
{
private:
	string position;
	double payRate;
	static int doc_NUM;
public:
	Doctor();
	double findSalary() const;
	string getPosName() const;
	static int getDocNum();

	static void incDocNum();
	static void decDocNum();
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
	bool getIsStationed() const;
	Doctor *getDoctor() const;
	Patient *getPatient() const;
	string getPatientName() const;
	string getDoctorIC() const;
	string getDoctorName() const;
	string getPatientIC() const;
	string getDoctorNameInBracket() const;
	string getAvail() const;
	bool getIsOccupied() const;
	double getRate() const;

	void releasePat();
	void releaseDoc();
	void setPatient(Patient *);
	void setIsOccupied(bool);
	void setIsStationed(bool);
	void setDoctor(Doctor *);
	void setRate(double);
	void doctorPtrAdjust();
};
#endif // !PERSON_H