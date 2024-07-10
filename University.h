#ifndef UNVIERSIY_HPP
#define UNVIERSIY_HPP
#include <unordered_map>
#include <fstream>
#include "Instructor.h" 
#include "BipGraph.h" 
#include "Resource files/json.hpp"


class University {
private:
    void breakIntoHours(int start, int end, const std::string& day, std::vector<TimeSlot>& vec);

    void readCoursesFromJson(const nlohmann::json& jsonData, const std::string& day);

    void readInstructorsFromJson(const nlohmann::json& jsonData, const std::string& day);

    void readTimeSlotsFromJson(const nlohmann::json& jsonData, const std::string& day);

    std::vector<std::tuple<TimeSlot, Course, Instructor>> scheduleHelper(const std::unordered_map<std::string, Course> coursesOfTheDay,
        const std::vector<Instructor>& instructorsOfTheDay, const std::string& theDay);

    void printSchedule(const std::vector<std::tuple<TimeSlot, Course, Instructor>>& scheduleOfTheDay, const std::string& theDay) const;
public:
    University();

    void addCourse(Course c);

    void addInstructor(Instructor lecturer);

    void addTimeSlot(TimeSlot ts);

    void saveState() const;
    
    bool loadState(const std::string& fileName);

    std::unordered_map < std::string, std::vector<std::tuple<TimeSlot, Course, Instructor>>> schedule();

    void printWeekSchedule();
private:
    std::unordered_map<std::string, std::unordered_map<std::string, Course>> courses;//Day to - (name to Course map) map, While 
    //constructing Instructor objects we need preffered courses, altough we have only preferred course names in json file,
    //so using map we can find correct course to add to Instructor object very fast
    std::unordered_map<std::string, std::vector<Instructor>> instructors;//day to Instructors map
    std::unordered_map<std::string, std::vector<TimeSlot>> timeSlots;//day to timeslots map
    std::unordered_map < std::string, std::vector<std::tuple<TimeSlot, Course, Instructor>>> m_schedule;//day to day's schedule map
    std::vector<std::string> days;//Monday to Friday days
    std::unordered_map<std::string, int> stoiHours;//string day -> integer map
    std::unordered_map<int, std::string> toStringHours;//integer to string day map
};

#endif // UNVIERSIY_HPP

