#include "University.h"

void University::breakIntoHours(int start, int end, const std::string& day, std::vector<TimeSlot>& vec)
{
    for (int i = start; i < end; ++i) {
        vec.push_back(TimeSlot(day, toStringHours[i], toStringHours[i + 1]));
    }

}

void University::readCoursesFromJson(const nlohmann::json& jsonData, const std::string& day)
{
    for (const auto& courseJson : jsonData["courses"]) {
        Course course;
        course.name = courseJson["name"];
        for (const auto& timeSlotJson : courseJson["preferredTimeSlots"]) {
            TimeSlot slot;
            slot.day = day;
            if (stoiHours[timeSlotJson["startTime"]] + 1 < stoiHours[timeSlotJson["endTime"]]) {
                std::vector<TimeSlot> brokenTimeSlots;
                breakIntoHours(stoiHours[timeSlotJson["startTime"]], stoiHours[timeSlotJson["endTime"]], day, brokenTimeSlots);
                for (const TimeSlot& ts : brokenTimeSlots) {
                    course.preferredTimeSlots.push_back(slot);
                }
            }
            else {
                slot.startTime = timeSlotJson["startTime"];
                slot.endTime = timeSlotJson["endTime"];
                course.preferredTimeSlots.push_back(slot);
            }
            
        }
        courses[day][course.name] = course;
    }
}

void University::readInstructorsFromJson(const nlohmann::json& jsonData, const std::string& day)
{
    for (const auto& inst : jsonData["instructors"]) {
        Instructor lecturer;
        lecturer.name = inst["name"];
        for (const auto& ts : inst["availability"]) {
            TimeSlot obj;
            obj.day = day;
            if (stoiHours[ts["startTime"]] + 1 < stoiHours[ts["endTime"]]) {
                std::vector<TimeSlot> brokenTimeSlots;
                breakIntoHours(stoiHours[ts["startTime"]], stoiHours[ts["endTime"]], day, brokenTimeSlots);
                for (const TimeSlot& ts : brokenTimeSlots) {
                    lecturer.addAvailableTimeSlot(ts);
                }
            }
            else {
                obj.startTime = ts["startTime"];
                obj.endTime = ts["endTime"];
                lecturer.addAvailableTimeSlot(obj);
            }
            
        }

        for (const auto& coursename : inst["preferredCourses"]) {
            std::string name = coursename;
            lecturer.addPrefferedCourse(courses[day][name]);
        }
        instructors[day].push_back(lecturer);
    }
}

void University::readTimeSlotsFromJson(const nlohmann::json& jsonData, const std::string& day)
{
    for (const auto& timeSlotJson : jsonData["timeSlots"]) {
        TimeSlot slot;
        slot.day = day;
        if (stoiHours[timeSlotJson["startTime"]] + 1 < stoiHours[timeSlotJson["endTime"]]) {
            std::vector<TimeSlot> brokenTimeSlots;
            breakIntoHours(stoiHours[timeSlotJson["startTime"]], stoiHours[timeSlotJson["endTime"]], day, brokenTimeSlots);
            for (const TimeSlot& ts : brokenTimeSlots) {
                timeSlots[day].push_back(ts);
            }
        }
        else {
            slot.startTime = timeSlotJson["startTime"];
            slot.endTime = timeSlotJson["endTime"];
            timeSlots[day].push_back(slot);
        }
    }

}

void University::addCourse(Course c) {
    courses[c.preferredTimeSlots[0].day].insert({c.name, std::move(c)});
}

void University::addInstructor(Instructor lecturer) {
    Instructor copy;
    copy.name = std::move(lecturer.name);
    copy.preferredCourses = std::move(lecturer.preferredCourses);
    for (int i = 0; i < lecturer.availability.size(); ++i) {
        if (stoiHours[lecturer.availability[i].startTime] + 1 < stoiHours[lecturer.availability[i].endTime]) {
            std::vector<TimeSlot> brokenTimeSlots;
            breakIntoHours(stoiHours[lecturer.availability[i].startTime], stoiHours[lecturer.availability[i].endTime],
                lecturer.availability[0].day, brokenTimeSlots);
            for (TimeSlot& ts : brokenTimeSlots) {
                copy.availability.push_back(std::move(ts));
            }
        }
        else {
            copy.availability.push_back(std::move(lecturer.availability[i]));
        }
    }

    instructors[copy.availability[0].day].push_back(std::move(copy));
}

void University::addTimeSlot(TimeSlot ts) {
    if (stoiHours[ts.startTime] + 1 < stoiHours[ts.endTime]) {
        std::vector<TimeSlot> brokenTimeSlots;
        breakIntoHours(stoiHours[ts.startTime], stoiHours[ts.endTime], ts.day, brokenTimeSlots);
        for (TimeSlot& ts : brokenTimeSlots) {
            timeSlots[ts.day].push_back(std::move(ts));
        }
    }
    else {
        timeSlots[ts.day].push_back(std::move(ts));
    }
}

void University::saveState() const {
    using json = nlohmann::json;

    std::string DestFileName;
    std::string preferredPath;

    std::cout << "Enter path to file where state will be saved or press Ctrl^z for default (default: Resource Files\\state.json): ";
    std::getline(std::cin, preferredPath);
    DestFileName = preferredPath.empty() ? "Resource Files\\state.json" : preferredPath;

    std::ofstream f(DestFileName);
    if (!f.is_open()) {
        std::cerr << "Could not open file : " << DestFileName << std::endl;
        return; 
    }

    json file;

    for (const auto& day : days) {
        json dayData;

        if (timeSlots.find(day) != timeSlots.end()) {
            dayData["timeSlots"] = timeSlots.at(day);
        }
        else {
            dayData["timeSlots"] = json::array(); 
        }

        if (instructors.find(day) != instructors.end()) {
            dayData["instructors"] = instructors.at(day);
        }
        else {
            dayData["instructors"] = json::array();
        }

        if (courses.find(day) != courses.end()) {
            json coursesArray;
            for (const auto& course : courses.at(day)) {
                coursesArray.push_back(course);
            }
            dayData["courses"] = coursesArray;
        }
        else {
            dayData["courses"] = json::array(); 
        }

        file[day] = dayData;
    }

    f << std::setw(4) << file << std::endl;
    f.close();

    std::cout << "State saved successfully to: \n";
}

bool University::loadState(const std::string& filename)
{
    std::ifstream jsonFile(filename);
    if (!jsonFile.is_open()) {
        std::cerr << "Error opening json file: " << filename << std::endl;
        return false;
    }

    nlohmann::json jsonData;
    try {
        jsonFile >> jsonData;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    }

    for (const auto& day : days) {
        if (jsonData.contains(day)) {
            readTimeSlotsFromJson(jsonData[day], day);
            readCoursesFromJson(jsonData[day], day);
            readInstructorsFromJson(jsonData[day], day);
        }
    }
    
    return true;
}


std::unordered_map < std::string, std::vector<std::tuple<TimeSlot, Course, Instructor>>> University::schedule() {
    for (const auto& day : days) {
        scheduleHelper(courses[day], instructors[day], day);
    }

    return m_schedule;
}

void University::printWeekSchedule(){
    for (const auto& day : days) {
        printSchedule(m_schedule[day], day);
        std::cout << std::endl;
    }
}

std::vector<std::tuple<TimeSlot, Course, Instructor>> University::scheduleHelper(const std::unordered_map<std::string, Course> coursesOfTheDay,
    const std::vector<Instructor>& instructorsOfTheDay, const std::string& theDay) {
    int idx = 0;
    std::unordered_map<int, Course> coursemap;//integers to each course so graph can be formed easily
    for (const auto& elem : coursesOfTheDay) {
        coursemap.insert(std::make_pair(idx++, elem.second));
    }

    int rightindex = coursemap.size();
    std::unordered_map<int, std::pair<TimeSlot, Instructor>> rightSide;//mapping integers to each possible Timeslot - Intructor pair, 
                                                                       //where timeslots are Instructors preferred timelsots
    for (const auto& teacher : instructorsOfTheDay) {
        for (const TimeSlot& ts : teacher.availability) {
            rightSide[rightindex++] = std::make_pair(ts, teacher);
        }
    }

    BipGraph graph;
    graph.setFirstRight(coursemap.size(), rightindex);
    graph.addVertexesUntil(rightindex);

    for (const auto& [index, c] : coursemap) {
        for (const TimeSlot& ts : c.preferredTimeSlots) {
            for (const auto& [key, value] : rightSide) {//C++ 17 needed for structured binding
                bool isinside = value.first.isInsideOrMatches(ts);
                if (isinside && value.second.prefers(c.name)) {
                    graph.addEdge(index, key);
                }
            }
        }
    }

    myPairset answer;
    graph.HopcroftKarp(answer);//fills answer with maxiumum matching pairs


    //If not all courses are matched until this stage //Here are 2 options
        //1.course doesnt have preferred timeslots
        //   1.1 some instructors prefer this course  - new graph edge, so at least one soft constraint will be satisfied(2nd one)
        //   1.2 no instructor prefers this course    - random match(after algorithms passes second time)
        //2.course has preferred timeslots
        //  2.1 but no Instructor is available at those timeslos
        //     2.1.1 Some instructors want this course when they are availabe - new graph edge, so at least one soft constraint will be satisfied(2nd one)
        //  2.2 there are some Instructors available in those timeslots
        //     2.2.1 some of them prefer this course  - already found this kind of matchings
        //     2.2.2 no one prefers this course       -  new graph edge, so at least one soft constraint will be satisfied(1st one)
    // new graph will be constructed from unmatched vertexes and still available timeslot - instructor pairs,
    //edges will be constructed from points mentioned above 1.1 , 2.1.1 , 2.2.2
    //if after this step some courses stay unmatched, we have 1.2 case 

    if (answer.size() < courses[theDay].size()) {
        myPairset OneConditionmatches;
        std::vector<int> unmatchedLtoR = graph.getUnmatchedLeftVertexes();
        std::vector<int> unmatchedRtoL = graph.getUnmatchedRightVertexes();

        std::unordered_set<int> unmatchedRights;
        for (const auto& elem : unmatchedRtoL) {
            unmatchedRights.insert(elem);
        }

        BipGraph g;
        g.setFirstRight(unmatchedRtoL.front(), unmatchedLtoR.size() + unmatchedRtoL.size());

        for (int i = 0; i < unmatchedLtoR.size(); ++i) {
            g.addVertex(unmatchedLtoR[i]);
        }

        for (int i = 0; i < unmatchedRtoL.size(); ++i) {
            g.addVertex(unmatchedRtoL[i]);
        }

        for (const auto& left : unmatchedLtoR) {
            const Course& c = coursemap[left];
            if (c.preferredTimeSlots.empty()) {
                for (const auto& [key, value] : rightSide) {
                    if (unmatchedRights.find(key) != unmatchedRights.end() && value.second.prefers(c.name)) {
                        g.addEdge(left, key);
                    }
                }
            }
            else {
                bool instructorsAvailable = false;
                for (const TimeSlot& ts : c.preferredTimeSlots) {
                    for (const auto& [key, value] : rightSide) {
                        bool isinside = value.first.isInsideOrMatches(ts);
                        if (unmatchedRights.find(key) != unmatchedRights.end() && isinside) {
                            instructorsAvailable = true;
                            g.addEdge(left, key);
                        }
                    }
                }

                if (!instructorsAvailable) {
                    for (const auto& [key, value] : rightSide) {
                        if (unmatchedRights.find(key) != unmatchedRights.end() && value.second.prefers(c.name)) {
                            g.addEdge(left, key);
                        }
                    }
                }
            }
               
            
        }

        g.HopcroftKarp(OneConditionmatches);//using the algorithm to find maxiumum matches with one soft constraints satisfied
        for (const auto& pair : OneConditionmatches) {
            answer.insert(pair);
        }

        //case when even with one soft constraint the course wasn't matched
        myPairset lastMatches;
        std::vector<int> unmatchedLtoRlast = g.getUnmatchedLeftVertexes();
        std::vector<int> unmatchedRtoLlast = g.getUnmatchedRightVertexes();

        std::vector<int> randommatchingsLtoR; 
        std::set_intersection(unmatchedLtoRlast.begin(), unmatchedLtoRlast.end(),//finding vertexes which werent matched after algorithm passes them 2 times
            unmatchedLtoR.begin(), unmatchedLtoR.end(),
            std::back_inserter(randommatchingsLtoR));

        std::vector<int> randommatchingsRtoL;
        std::set_intersection(unmatchedRtoLlast.begin(), unmatchedRtoLlast.end(),
            unmatchedRtoL.begin(), unmatchedRtoL.end(),
            std::back_inserter(randommatchingsRtoL));

        for (int i = 0; i < randommatchingsLtoR.size() && i < randommatchingsRtoL.size(); ++i) {
                lastMatches.insert({ randommatchingsRtoL[i] ,randommatchingsLtoR[i] });
        }

        for (const auto& pair : lastMatches) {
            answer.insert(pair);
        }
    }

    std::vector<std::tuple<TimeSlot, Course, Instructor>> scheduledDay;
    for (const auto& [tsInst, course] : answer) {
        scheduledDay.push_back(std::make_tuple(std::move(rightSide[tsInst].first), std::move(coursemap[course]), std::move(rightSide[tsInst].second)));
    }

    std::sort(scheduledDay.begin(), scheduledDay.end(), [](const std::tuple<TimeSlot, Course, Instructor>& lhs, const std::tuple<TimeSlot, Course, Instructor>& rhs) {
        return std::get<0>(lhs).startTime < std::get<0>(rhs).startTime;
        });

    m_schedule[theDay] = scheduledDay;

    return scheduledDay;
}

void University::printSchedule(const std::vector<std::tuple<TimeSlot, Course, Instructor>>& scheduleOfTheDay, const std::string& theDay) const
{
    if (m_schedule.empty()) {
        std::cout << "There is no schedule for " << theDay << "yet. Call schedule method before printing the schedule\n";
        return;
    }

    std::cout << "                 " << theDay << "                 " << std::endl;
    for (const auto& tuple : scheduleOfTheDay) {
        if (std::get<1>(tuple).name.empty()) {//Some very edge case
            continue;
        }

        std::get<0>(tuple).displayInfo();

        std::get<1>(tuple).displayInfo();

        std::get<2>(tuple).displayInfo();

        std::cout << std::endl; 
    }
}

University::University()
{
    days.push_back("Monday");
    days.push_back("Tuesday");
    days.push_back("Wednesday");
    days.push_back("Thursday");
    days.push_back("Friday");

    for (int i = 0; i < 24; ++i) {
        std::string timeString = (i < 10 ? "0" : "") + std::to_string(i) + ":00";
        stoiHours[timeString] = i;
    }

    for (const auto& pair : stoiHours) {
        toStringHours[pair.second] = pair.first;
    }
}









