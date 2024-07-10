#include "Instructor.h"


void Instructor::swap(Instructor& src) {
    std::swap(name, src.name);
    std::swap(availability, src.availability);
    std::swap(preferredCourses, src.preferredCourses);
}


Instructor::Instructor(std::string thename) : name(std::move(thename))
{}

Instructor::Instructor(std::string thename, std::vector<TimeSlot> avialableslots, std::vector<Course> courses) :
    name(std::move(thename)), availability(std::move(avialableslots)), preferredCourses(std::move(courses))
{}

void Instructor::addAvailableTimeSlot(TimeSlot ts) {
    availability.push_back(std::move(ts));
}

void Instructor::addPrefferedCourse(Course c) {
    preferredCourses.push_back(std::move(c));
}

bool Instructor::prefers(std::string coursename) const {
    for (const auto& c : preferredCourses) {
        if (c.name == coursename) {
            return true;
        }
    }
    return false;
}

void Instructor::displayInfo() const {
    std::cout << "Professor " << name;
}

    