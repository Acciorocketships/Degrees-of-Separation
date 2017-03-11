#ifndef PEOPLE_H
#define PEOPLE_H
#include "person.h"
#include "course.h"
using namespace std;

class people {
public:
	people();
	~people();
	void addstudent(string);
	void addta(string);
	void printallcourses();
	void printallstudents();
	void taed(string);
	void roster(string);
	void paths(string,string);
	void shortestpath(string,string);
private:
	person* root;
	course* courses;
	
	void deletetrie(person*);
	void deletelinkedlist(course*);
	void recursiveprintstudents(person*);
	person* findstudent(string);
	person* traversetrie(persondata);
	course* addcourse(csclass);
	course* findcourse(csclass);
	persondata parserawdata(string);
};

#endif
