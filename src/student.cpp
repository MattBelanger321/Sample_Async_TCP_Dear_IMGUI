#include "student.h"
#include <iostream>
#include <iomanip>

int Student::getGrade() const{
    return this->gr;
}

float Student::getGpa() const{
    return this->gp;
}

std::string Student::getFirstName() const{
    return this->f;
}

std::string Student::getLastName() const{
    return this->l;
}

void Student::setGrade(const int grade){
    this->gr = grade;
}

void Student::setGpa(const float gpa){
    this->gp = gpa;
}

void Student::setFirstName(const std::string fname){
    this->f = fname;
}

void Student::setLastName(const std::string lname){
    this->l = lname;
}

std::ostream& operator << (std::ostream &o, const Student stud){
    return o << stud.getFirstName() + ", " + stud.getLastName() + ", " + std::to_string(stud.getGrade()) + ", " +std::to_string(stud.getGpa());
}