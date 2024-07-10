#include "TimeSlot.h"

void TimeSlot::swap(TimeSlot& ts)
{
	std::swap(day, ts.day);
	std::swap(startTime, ts.startTime);
	std::swap(endTime, ts.endTime);
}

TimeSlot::TimeSlot(std::string theDay, std::string start, std::string end) : day(std::move(theDay)), startTime(std::move(start)), endTime(std::move(end))
{}

TimeSlot::TimeSlot(const TimeSlot& rhs) : day(rhs.day), startTime(rhs.startTime), endTime(rhs.endTime)
{}

TimeSlot& TimeSlot::operator=(const TimeSlot& src)
{
	TimeSlot tmp{ src };
	swap(tmp);
	return *this;
}

TimeSlot::TimeSlot(TimeSlot&& src) noexcept
{
	swap(src);
}

TimeSlot& TimeSlot::operator=(TimeSlot&& src) noexcept
{
	TimeSlot moved{ std::move(src) };
    swap(moved);
    return *this;
}

TimeSlot::~TimeSlot()
{}

bool TimeSlot::isInsideOrMatches(const TimeSlot& other) const
{
	return day == other.day && (startTime <= other.startTime && endTime >= other.endTime);
}

void TimeSlot::displayInfo() const
{
	std::cout << startTime << " - " << endTime << " ";
}
