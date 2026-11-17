# Student Record Management System

**Subject:** Object-Oriented Programming Using C++ (CIC-211)  
**Group:** 1  
**Students:** Akriti Shukla and Harnit Gautam

## Description
A console-based C++ mini project for adding, displaying, searching and updating student records. It calculates total marks, percentage and grade for five subjects and stores records in `students.txt`.

## Features
- Add a student with roll number, name, course and five subject marks
- Display all student records
- Search by roll number
- Update name, course or marks
- Prevent duplicate roll numbers
- Calculate total, percentage and grade
- Save and load records using text-file handling
- Validate menu choices and input values

## OOP concepts
- Classes and objects (`Student`, `StudentManager`)
- Encapsulation through private data members
- Constructors
- Member functions
- Vector of objects
- File handling

## Build
Requires a C++11-compatible compiler such as g++.

```bash
g++ -std=c++11 -Wall -Wextra main.cpp -o student_records
```

Run on Linux/macOS:
```bash
./student_records
```

Run on Windows:
```powershell
student_records.exe
```

## Data file
The program creates/uses `students.txt` in its current working directory. Keep this file with the executable if you want to retain the records.

## Grading rules used in this project
- A+: 90% and above
- A: 80% to below 90%
- B: 70% to below 80%
- C: 60% to below 70%
- D: 50% to below 60%
- F: below 50%

These are project-defined grading bands and should be changed if the faculty specifies a different scheme.

## Authors
- Akriti Shukla
- Harnit Gautam
