#ifndef INSTRUCTOR_HPP
#define INSTRUCTOR_HPP
#include "Course.h"

struct Instructor {//Add restrictions, optimizations
private:
    void swap(Instructor& src);
public:
    Instructor() = default;
    explicit Instructor(std::string thename);

    explicit Instructor(std::string thename, std::vector<TimeSlot> avialableslots, std::vector<Course> courses);

    Instructor(const Instructor& rhs) = default;

    Instructor& operator=(const Instructor& rhs) = default;

    void addAvailableTimeSlot(TimeSlot ts);

    void addPrefferedCourse(Course c);

    bool prefers(std::string coursename) const;

    void displayInfo() const;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Instructor, name, availability, preferredCourses)

    //private:
    std::string name;
    std::vector<TimeSlot> availability;
    std::vector<Course> preferredCourses;
};

#endif // !INSTRUCTOR_HPP

