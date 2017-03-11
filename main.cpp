#include <iostream>
#include <cstdlib>
#include <fstream>
#include "people.h"
using namespace std;

void readfiles(int argc, char* argv[],people&);
void runcommands(people&);

int main(int argc, char* argv[]){
	people students;
	readfiles(argc,argv,students);
	runcommands(students);
}

void readfiles(int argc, char* argv[], people& students){
	if (argc < 3){
		cerr << "Please specify a TAs and a Students file " 
		     << "to read data from." << endl;
		exit(1);
	}
	ifstream studentstxt;
	studentstxt.open(argv[1]);
	if (!studentstxt.is_open()){
		cerr << "Students data file not found" << endl;
		exit(1);
	}
	string studentdata;
	for (int i = 0; getline(studentstxt,studentdata); i++){
		students.addstudent(studentdata);
	}
	studentstxt.close();
	ifstream tastxt;
	tastxt.open(argv[2]);
	if (!tastxt.is_open()){
		cerr << "TAs data file not found" << endl;
		exit(1);
	}
	string tadata;
	for (int i = 0; getline(tastxt,tadata); i++){
		students.addta(tadata);
	}
	tastxt.close();
}

void runcommands(people& students){
	string line;
	while (getline(cin,line)){
		if (line.substr(0,2) == "ls"){
			students.printallstudents();
		}
		else if (line.substr(0,2) == "lc"){
			students.printallcourses();
		}
		else if (line.substr(0,4) == "taed"){
			students.taed(line.substr(5));
		}
		else if (line.substr(0,6) == "roster"){
			students.roster(line.substr(7));
		}
		else if (line.substr(0,5) == "paths"){
			string bothnames = line.substr(6);
			int namebreak = bothnames.find(" ");
			string firstname = bothnames.substr(0,namebreak);
			string secondname = bothnames.substr(namebreak+1);
			students.paths(firstname,secondname);
		}
		else if (line.substr(0,12) == "shortestpath"){
			string bothnames = line.substr(13);
			int namebreak = bothnames.find(" ");
			string firstname = bothnames.substr(0,namebreak);
			string secondname = bothnames.substr(namebreak+1);
			students.shortestpath(firstname,secondname);
		}
	}
}
