#ifndef COURSE_HPP
#define COURSE_HPP
#include"TimeSlot.h"
#include <vector>

struct Course {//Add restrictions, optimizations
private:
    void swap(Course& c);
public:
    Course() = default;
    explicit Course(std::string thename);

    explicit Course(std::string thename, std::vector<TimeSlot> slots);

    Course(const Course& rhs) = default;

    Course& operator=(const Course& rhs) = default;

    Course(Course&& src) noexcept;

    Course& operator=(Course&& src) noexcept;

    ~Course();

    void addPreferredTimeSlot(TimeSlot ts);

    void displayInfo() const;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Course, name, preferredTimeSlots)

    std::string name;
    std::vector<TimeSlot> preferredTimeSlots;
};
#endif // COURSE_HPP
