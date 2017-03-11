#ifndef COURSE_H
#define COURSE_H
#include <queue>
class person;
using namespace std;

class course {
public:
	course* next;
	person* root;
	string name;
	string term;
	
	course();
	~course();
	course(const course&);
	void deletetrie(person*);
	void add(person*);
	void printroster();
	void searchforstudent(string,string,course&);
	void recursiveenqueue(person*,string,course&,
			      queue<person*>&,queue<string>&);
	person* findstudent(string);
private:
	void copyenqueue(person*,queue<person*>&);
	void recursiveprintstudents(person*);
	void recursivesearchtrie(person*,string,string,course);
	void searchnextclasses(person*,string,string,course&);
};

#endif
