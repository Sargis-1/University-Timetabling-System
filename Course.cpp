#include "Course.h"

void Course::swap(Course& c) {
    std::swap(name, c.name);
    std::swap(preferredTimeSlots, c.preferredTimeSlots);
}

Course::Course(std::string thename) : name(std::move(thename))
{}

Course::Course(std::string thename, std::vector<TimeSlot> slots) : name(std::move(thename)), preferredTimeSlots(std::move(slots))
{}

Course::Course(Course&& src) noexcept {
    swap(src);
}

Course& Course::operator=(Course&& src) noexcept {
    Course moved{ std::move(src) };
    swap(moved);
    return *this;
}

Course::~Course() {}

void Course::addPreferredTimeSlot(TimeSlot ts) {
    preferredTimeSlots.push_back(std::move(ts));
}

void Course::displayInfo() const {
    std::cout << std::setw(12) << std::left << name << "   ";
}

    