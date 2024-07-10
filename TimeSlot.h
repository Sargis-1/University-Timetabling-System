#ifndef TIMESLOT_HPP
#define TIMESLOT_HPP
#include <string>
#include <iostream>
#include "Resource files/json.hpp"

struct TimeSlot {//Add restrictions, optimizations
private:
    void swap(TimeSlot& ts);
public:
    TimeSlot() = default;

    explicit TimeSlot(std::string theDay, std::string start, std::string end);

    TimeSlot(const TimeSlot& rhs);

    TimeSlot& operator=(const TimeSlot& src);

    TimeSlot(TimeSlot&& src) noexcept;

    TimeSlot& operator=(TimeSlot&& src) noexcept;

    ~TimeSlot();

    bool isInsideOrMatches(const TimeSlot& other) const;

    void displayInfo() const;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(TimeSlot, day, startTime, endTime)

    std::string day;
    std::string startTime;
    std::string endTime;
};

#endif // TIMESLOT_HPP

