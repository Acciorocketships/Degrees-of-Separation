// Ryan Kortvelesy
// File: person.cpp, Project: hw6degrees
// Purpose: Contains csclass and persondata structs (which are just 
//	    temporary structs for passing information about people
//	    or classes between functions) and the person class. The
//	    person class contains a name, a boolean indicating whether
//	    or not it is a person (useful for traversing the trie), and
//	    functions to add a class that the person has TAed or list
//	    all of the classes that they TAed.

#include <iostream>
#include "person.h"
#include "course.h"
using namespace std;

// Sets up the class for an individual person, including
// an array of 27 pointers to other people (for use in
// the trie)
person::person(){
        name = "";
	capacity = 1;
	size = 0;
        classestaed = new course*[capacity];
        next = new person*[27];
        isperson = false;
        for (int i = 0; i < 27; i++){
                next[i] = NULL;
        }
        for (int i = 0; i < capacity; i++){
		classestaed[i] = NULL;
	}
}

// Frees up the memory occupied by a person after it is
// done being used
person::~person(){
        delete [] classestaed;
	delete [] next;
}

// Adds a class 
void person::addclasstaed(course* newcourse){
	size += 1;
	expand();
        classestaed[size-1] = newcourse;
}

// Expands the array of classes taed by the person
void person::expand(){
	if (size == capacity){
		capacity *= 2;
		course** temp = new course*[capacity];
		for (int i = 0; i < size; i++){
			temp[i] = classestaed[i];
		}
		delete [] classestaed;
		classestaed = temp;
	}
}

// Prints all of the classes a student teaches
void person::listclassestaed(){
	for (int i = 0; i < size; i++){
		cout << classestaed[i]->name 
		     << classestaed[i]->term << endl;
	}
}