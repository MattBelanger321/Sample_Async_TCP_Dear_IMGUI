#include <string>

#include <iostream>

#if !defined(__DEMO__STUDENT__H__)
#define __DEMO__STUDENT__H__

class Student {
private:
    int gr {0}; //grade
    float gp {0.0};    //gpa
    std::string f {""}; //fname
    std::string l {""}; //lname
public:
    Student(int grade = 0, double gpa = 0.0, std::string fname = "", std::string lname = ""): gr(grade), gp(gpa), f(fname), l(lname){};
    Student(const Student &stud): gr(stud.gr), gp(stud.gp), f(stud.f), l(stud.l){};

    int getGrade() const;
    float getGpa() const;
    std::string getFirstName() const;
    std::string getLastName() const;

    void setGrade(const int grade);
    void setGpa(const float gpa);
    void setFirstName(const std::string fname);
    void setLastName(const std::string lname);

};

std::ostream& operator << (std::ostream &o, const Student stud);

#endif // __DEMO__STUDENT__H__