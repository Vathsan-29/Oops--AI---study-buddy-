# AI Study Buddy (C++)

A console learning assistant written in C++ for **IT23304 Object Oriented Programming** (Department of Information Technology, Madras Institute of Technology, Anna University).

**Team 17:** Sriram R (2025506050), Pooja S (2025506066), Shrivathsan A (2025506074), Mohammad Athil B (2025506124)

## Features
- Login with a 3-attempt limit
- Manage subjects (Core, Practical and Elective): add, update, delete, search
- Subject quiz with feedback and automatic difficulty change
- Study session log, progress report and a suggestion for what to study next
- Data saved to text files and loaded again on the next run

## Build and run
```bash
g++ -std=c++11 -Wall -o study_buddy main.cpp
./study_buddy
```
On Windows (MinGW) run `study_buddy.exe`. Demo login: `student` / `study123`.

The program creates `subjects.txt`, `quizzes.txt` and `sessions.txt` in the folder you run it from. Delete them to start fresh.

## OOP concepts used
| Exercise | Concept | Where in `main.cpp` |
|---|---|---|
| 1 | Basic commands, safe input, login | `readString`, `readInt`, `Student::login` |
| 2 | Classes and objects, constructors, destructor | `Subject`, `StudyBuddy::~StudyBuddy` |
| 3 | Function overloading | `showSubject` (by ID, by name, by category) |
| 4 | Operator overloading | `Subject::operator==` rejects duplicate IDs |
| 5 | Friend function and friend class | `progressReport`, `Tutor` |
| 6 | Inheritance | `CoreSubject`, `PracticalSubject`, `ElectiveSubject` |
| 7 | Templates and inline functions | `printCell`, `RecordBox<T>`, `isLowProgress` |
| 8 | Runtime polymorphism, exceptions | virtual `status()`, `try / throw / catch` |
| 9 | File handling | `loadRecords`, `saveRecords`, `StudyBuddy::load/save` |
| 10 | STL containers and iterators | `map` (subjects), `vector` (quizzes), `list` (sessions) |
