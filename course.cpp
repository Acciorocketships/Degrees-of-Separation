// Ryan Kortvelesy
// File: course.cpp, Project: hw6degrees
// Purpose: Contains a trie of all the students in the roster for a
//  	    particular course. There are functions to print the roster,
//	    add a student, searching recursively for a student (by
//	    calling the same function for classes that students in the
//	    roster have TAed), and enqueuing all the students in the
//	    roster for a breadth-first search.

#include <iostream>
#include <queue>
#include <cctype>
#include "course.h"
#include "person.h"
using namespace std;

// Sets up the trie by creating a root
course::course(){
	root = new person;
	next = NULL;
	name = "";
	term = "";
}

// Copy constructor (for use when course is passed by value in the "paths"
// function)
course::course(const course& source){
	root = new person;
	next = NULL;
	name = "";
	term = "";
	queue<person*> nodes;
	copyenqueue(source.root,nodes);
	while (!nodes.empty()){
		add(nodes.front());
		nodes.pop();
	}
}

// Helper function for the copy constructor
void course::copyenqueue(person* current,queue<person*>& nodes){
	for (int i = 0; i < 27; i++){
		if (current->isperson){
			nodes.push(current);
		}
		if (current->next[i] != NULL){
			copyenqueue(current->next[i],nodes);
		}
	}
}

// Frees up the space by calling deletetrie
course::~course(){
	deletetrie(root);
}

// Returns a pointer to a student, given their name
person* course::findstudent(string name){
	person* current = root;
	for (int c = 0; c < int(name.length()); c++){
		if (!isalpha(name[c])){
			current = current->next[0];
			if (current == NULL)
				return NULL;
		}
		else {
			current = current->next[int(tolower
						  (name[c]))-96];
			if (current == NULL)
				return NULL;
		}
	}
	return current;
}

// Performs a recursive post-order delete on the trie.
void course::deletetrie(person* current){
	if (current != NULL){
		for (int i = 0; i < 27; i++){
			deletetrie(current->next[i]);
		}
	}
	delete current;
}

// Adds a copy of a student to the course roster
void course::add(person* student){
	person* current = root;
	for (int c = 0; c < int(student->name.length()); c++){
		if (!isalpha(student->name[c])){
			if (current->next[0] == NULL){
				person* newperson = new person;
				current->next[0] = newperson;
			}
			current = current->next[0];
		}
		else {
			if (current->next[int(tolower(
			    	student->name[c]))-96] == NULL){
				person* newperson = new person;
			        current->next[int(tolower(student->
					 name[c]))-96]  = newperson;
			}
			current= current->next[int(tolower(student->
				 name[c]))-96];
		}
	}
       	if (current == NULL){
		person* newstudent = new person;
		current = newstudent;
	}
	current->name = student->name;
	current->isperson = true;
}

// Calls a function that recursively prints all students on the roster
void course::printroster(){
	recursiveprintstudents(root);
}

// Recursively traverses trie, printing a student's name every time
// it encounters a person with isperson==true
void course::recursiveprintstudents(person* current){
	if (current->isperson){
		cout << current->name << endl;
	}
	for (int i = 0; i < 27; i++){
		if (current->next[i] != NULL){
			recursiveprintstudents(current->next[i]);
		}
	}
}


// Searches for a student in the course, then calls recursivesearchtrie.
void course::searchforstudent(string path, string studentname, 
					       course& visited){
	if (findstudent(studentname) != NULL){
		cout << path + " +- " + name + term + " -> " 
			     + studentname << endl;
	}
	recursivesearchtrie(root,path,studentname,visited);
}

// Calls searchnextclasses for all students in the course who have not
// yet been visited. The "visited" course is passed by value because different
// paths can include the same person.
void course::recursivesearchtrie(person* current, string path,
			   string studentname, course visited){
	for (int i = 0; i < 27; i++){
		if (current->isperson && 
		    visited.findstudent(current->name) == NULL){
			visited.add(current);
			searchnextclasses(current,path,
					  studentname,visited);
		}
		if (current->next[i] != NULL){
			recursivesearchtrie(current->next[i],path,
					    studentname,visited);
		}
	}
}

// Calls searchforstudent for all of the classes that the students in
// this course have taed
void course::searchnextclasses(person* current, string path,
		         string studentname, course& visited){
	path = path + " +- " + name + term + " -> " + current->name;
	for (int i = 0; i < current->size; i++){
		current->classestaed[i]->searchforstudent
				(path,studentname,visited);
	}
}

// Enqueues all people in this course who do not appear on the visited
// list in the nodes queue (the person pointer) and the paths queue (a
// string indicating the path so far)
void course::recursiveenqueue(person* current,string path,course& visited,
			queue<person*>& nodes,queue<string>& paths){
	for (int i = 0; i < 27; i++){
		if (current->isperson && 
		    visited.findstudent(current->name) == NULL){
			// If the node is a person in the roster and
			// they haven't been visited yet.
			visited.add(current);
			nodes.push(current);
			paths.push(path + " +- " + name + term +
				   " -> " + current->name);
		}
		if (current->next[i] != NULL){
			recursiveenqueue(current->next[i],path,
					 visited,nodes,paths);
		}
	}
}