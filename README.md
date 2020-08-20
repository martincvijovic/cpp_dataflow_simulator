# cpp_oop_project
Created a Data Flow machine in C++ capable of calculating polynomial expressions in simulated time and manipulating simulated memory, using the Discrete Event Simulation Engine via the provided API

The code I wrote is located in 'oop_pr' folder.

NOTE : The version of the fully, bug-free, working software is, unfortunately, unavailable at the moment. This was my faculty project and a lot of modifications were done in the laboratories at the faculty which, eventually, led to the fully working and clean-written code. Unfortunately, those modifications are unavailable now.

There is A LOT to improve ( some improvements were already made in the laboratories, but were also, eventually,lost ): 
1. Cleaner code (a huge problem, I can't read my code which was written a year ago), less confusing var/function/method names, more commments and OOP-style declarations.
2. .txt parsing is not working with float values, only integers.
3. Machine.cpp's code needs to be fully rewritten and the Event creation should only rely upon the virtual Scheduler::notify() method
4. MemorySinleton.cpp -> Memory.cpp (also instead of 2 vectors, use std::unordered_map<std::string, double> for average of O(1) search time instead of O(n). The space complexity would, of course, remain linear. 
