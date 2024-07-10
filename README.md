# University-Timetabling-System

University Timetabling System is a program which effectively matches every course to an available instructor, so that maximum number of instructors get their preferred courses and maximum count of courses get their preferred time slots.


 Use C++ 17  !!!!!!!!!!!!!!


 IMPORTANT NOTES : add zero before inserting a new timeslot hour, 9:00 - wrong,   09:00 - right
 Algorithm works with 1 hour long timeslots, but you can input university timeslots and Instrcutor objects with longer availability time, for example 
 09:00 - 16 : 00, it will be broken into 1 hours long timeslots. No Course or Instructor object can have timeslots with different days.
 No course object can have preffered hours that are longer than one hour

 My json input file is divided by days, so its easier to make schedule by just running the algorithm for each day, but every timeslot object
 has it's day data member too
 Every day data is unique, same day data is overlapping, some courses dont have preferres timeslots, some instrcutors dont have preferres courses 


 Main algoritm of matching courses and instructors in avaialable timeslots, while satisfying as much constraints as possible, is
 Hopcroft - Karp bipartite graph mathing algorithm
 After each iteration Hopcroft - Karp algorithm increases size of matching pairs, using found M augmenting paths, at the end of tha algorithm
 it is guaranteed that maximum possible matchings are found for given bipartite graph
 
 Hopcroft - Karp algorithm runs in O(sqrt(V) * E) time complexity, where V is count of vertexes, E is count of edges
 
 Left vertexes of the graph represent courses
 Right vertexes of the graph represent all possible TimeSlot - Instructor pairs for each instructor, where timeslots are instructor's 
 available timeslots.
 
 At the first pass of the algorithm, the edges represent connection from left vertexes to right vertexes, where each pair [left, right]
 satisifes BOTH soft constraints and all hard constraints,
 At the second pass edges represent ONLY ONE soft constraint and all hard constraints, because there were NOT found matches satisying 
 both soft constraints
 At the end if any course isn't matched, we randomy pick a right vertex from unmatched ones, because there was no option to satisfy 
 any soft constraint

