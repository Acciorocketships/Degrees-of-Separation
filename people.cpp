// Ryan Kortvelesy
// File: people.cpp, Project: hw6degrees
// Purpose: Contains a trie of all the students and a linked list of all
//	    the classes. There are functions that read the information
//	    into the data structures, helper functions, and a function
//	    for each of the possible commands (ls,lc,taed,roster,paths,
//          and shortestpath).


#include <iostream>
#include <math.h>
#include <queue>
#include <cctype>
#include "people.h"
#include "course.h"
#include "person.h"
using namespace std;

// Sets up a root for the trie that contains all people and an array of
// courses.
people::people(){
	root = new person;
	for (int i = 0; i < 27; i++){
		root->next[i] = NULL;
	}
	courses = NULL;
}

// Frees up the memory by calling deletetrie
people::~people(){
	deletetrie(root);
	deletelinkedlist(courses);
}

// Performs a recursive post-order delete on the trie.
void people::deletetrie(person* current){
	if (current != NULL) {
		for (int i = 0; i < 27; i++){
			deletetrie(current->next[i]);
		}
	}
	delete current;
}

// Performs a recursive delete on the linked list
void people::deletelinkedlist(course* current){
	if (current != NULL){
		deletelinkedlist(current->next);
		delete current;
	}
}

// Adds a student to the list of all people and to the roster in the
// appropriate course.
void people::addstudent(string studentdata){
	persondata student = parserawdata(studentdata);
	person* current = traversetrie(student);
	current->name = student.name;
	current->isperson = true;
	addcourse(student.classdata)->add(current);
}

// Adds a ta to the list of all people and adds the appropriate courses to
// the list of courses that they TA.
void people::addta(string tadata){
	persondata ta = parserawdata(tadata);
	person* current = traversetrie(ta);
	course* coursetaed = addcourse(ta.classdata);
	current->name = ta.name;
	current->isperson = true;
	current->addclasstaed(coursetaed);
	course* currentcourse = courses;
	while (currentcourse != NULL){
		person* tainroster = currentcourse->findstudent(ta.name);
		if (tainroster != NULL){
			tainroster->addclasstaed(coursetaed);
		}
		currentcourse = currentcourse->next;
	}
}

// Finds the correct position in the trie to add a new ta or student,
// creating new person nodes where needed.
person* people::traversetrie(persondata taorstudent){
	person* current = root;
	for (int c = 0; c < int(taorstudent.name.length()); c++){
		if (!isalpha(taorstudent.name[c])){
		// Because of the sheer number of possible special characters,
		// anything that is not a letter goes in slot zero.
			if (current->next[0] == NULL){
				person* newperson = new person;
				current->next[0] = newperson;
			}
			current = current->next[0];
		}
		else {
			if (current->next[int(tolower(
			    taorstudent.name[c]))-96] == NULL){
				person* newperson = new person;
			        current->next[int(tolower(taorstudent.
					 name[c]))-96]  = newperson;
			}
			current= current->next[int(tolower(taorstudent.
				 name[c]))-96];
			// Converting a letter to an array index is a type
			// of hash function (albeit a simple one).
		}
	}
	return current;
}

// Takes in the string read from the file, and returns a struct with
// the separated name, course, and term.
persondata people::parserawdata(string studentdata){
        persondata student;
	csclass tempclass;
        string temp = "";
        bool part = false;
        for (int i = 0; i < int(studentdata.length()); i++){
                if (studentdata[i] == ':'){
                        if (part == false)
                                student.name = temp;
                        else if (part == true)
                                tempclass.course = temp;
                        part = true;
                        temp = "";
                }
                else {
                        temp += studentdata[i];
                }
        }
        tempclass.term = temp;
	student.classdata = tempclass;
        return student;
}

// Checks if a course already exists in the linked list
// of courses, makes a new one if it doesn't, then
// returns a pointer to it
course* people::addcourse(csclass classdata){
	course* coursepointer = findcourse(classdata);
	if (coursepointer == NULL){
		course* newcourse = new course;
		newcourse->name = classdata.course;
		newcourse->term = classdata.term;
		newcourse->next = courses;
		courses = newcourse;
		return newcourse;
	}
	else {
		return coursepointer;
	}
}

// Looks for a course in the linked list of courses. Returns
// a pointer to the course if it is found, and NULL if it isn't
course* people::findcourse(csclass classdata){
	if (courses == NULL){
		return NULL;
	}
	course* current = courses;
	while (current != NULL){
		if (current->name == classdata.course &&
		    current->term == classdata.term){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

// Prints all of the courses read in from the input files
void people::printallcourses(){
	course* current = courses;
	while (current != NULL){
		cout << current->name << current->term << endl;
		current = current->next;
	}
}

// Prints all of the students read in from the input files
void people::printallstudents(){
	recursiveprintstudents(root);
}

// Recursively prints all of the students in the trie. The students are
// printed out in alphabetical order!
void people::recursiveprintstudents(person* current){
	if (current->isperson){
		cout << current->name << endl;
	}
	for (int i = 0; i < 27; i++){
		if (current->next[i] != NULL){
			recursiveprintstudents(current->next[i]);
		}
	}
}

// Returns a pointer to a student, given their name
person* people::findstudent(string name){
	person* current = root;
	for (int c = 0; c < int(name.length()); c++){
		if (!isalpha(name[c])){
			current = current->next[0];
			if (current == NULL)
				return NULL;
		}
		else {
			current = current->next[int(tolower(name[c]))-96];
			if (current == NULL)
				return NULL;
		}
	}
	return current;
}

// Prints out all of the courses that a particular student has taed
void people::taed(string name){
	person* student = findstudent(name);
	if (student != NULL){
		student->listclassestaed();
	}
	else {
		cout << "Student not found" << endl;
	}
}

// Prints the roster for a given class. Students are printed out
// alphabetically.
void people::roster(string classtolookup){
	csclass temp;
	int cutoff = max(int(classtolookup.find("F")),
			 int(classtolookup.find("S")));
	if (cutoff == -1){
		cout << "Course not found" << endl;
		return;
	}
	temp.course = classtolookup.substr(0,cutoff);
	temp.term = classtolookup.substr(cutoff);
	course* courseptr = findcourse(temp);
	if (courseptr != NULL){
		courseptr->printroster();
	}
	else {
		cout << "Course not found" << endl;
	}
}

// Finds all of the paths from one person to another that do not contain
// the same person twice DEPTH FIRST
void people::paths(string currentperson, string soughtperson){
	person* student = findstudent(currentperson);
	if (student == NULL || findstudent(soughtperson) == NULL){
		cout << "Student not found" << endl;
		return;
	}
	else {
		for (int i = 0; i < student->size; i++){
			course visited; // Keeps track of the people who have
					// already been visited. Creates a
					// copy for every iteration so each
					// path gets a separate visited list.
			visited.add(student);
			student->classestaed[i]->searchforstudent
			         (student->name,soughtperson,visited);
		}
	}
}

// Finds the shortest path from one person to another (prints only
// one solution if multiple exist). BREADTH FIRST
void people::shortestpath(string currentperson, string soughtperson){
	person* student = findstudent(currentperson);
	if (student == NULL || findstudent(soughtperson)==NULL){
		cout << "Student not found" << endl;
		return;
	}
	else {
		queue<person*> nodes;
		queue<string> paths;
		course visited;
		visited.add(student);
		for (int i = 0; i < student->size; i++){
			student->classestaed[i]->recursiveenqueue
				(student->classestaed[i]->root,
				 currentperson,visited,nodes,paths);
		} // The initial path is the name of the starting person
		while (!nodes.empty()){
			if (nodes.front()->name == soughtperson){
				cout << paths.front() << endl;
				return;
			}
			for (int i = 0; i < nodes.front()->size; i++){
			nodes.front()->classestaed[i]->recursiveenqueue
				     (nodes.front()->classestaed[i]->root,
				       paths.front(),visited,nodes,paths);	 
			}
			nodes.pop();
			paths.pop();
		}
	}
}