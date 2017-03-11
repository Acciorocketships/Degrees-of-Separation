#ifndef PERSON_H
#define PERSON_H
#include "course.h"
using namespace std;

// Holds information about a class (temporary)
struct csclass {
        string course;
        string term;
};

// Holds information about a person (temporary)
struct persondata {
         string name;
         csclass classdata;
};

// The object for an individual person
class person {
public:
        string name;
	bool isperson;
	int size;
        course** classestaed;
        person** next;
	
	person();
        ~person();
        void addclasstaed(course*);
	void listclassestaed();
private:
	int capacity;
	
	void expand();
};

#endif
