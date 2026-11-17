// Student Record Management System
// Subject: Object-Oriented Programming Using C++ (CIC-211)

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;

const int NUM_SUBJECTS = 5;
const int MAX_MARKS = 100;
const string DATA_FILE = "students.txt";

// ---------------------------------------------------------------
// Input helper functions
// ---------------------------------------------------------------

// Reads a whole line. If input ends (Ctrl+D / Ctrl+Z), the program exits.
string readLine(const string& prompt)
{
    string line;
    cout << prompt;
    if (!getline(cin, line)) {
        cout << "\nInput ended. Exiting program.\n";
        exit(0);
    }
    return line;
}

// Converts text to an integer. Returns false if the text is not a whole number.
bool toInt(const string& text, int& value)
{
    istringstream iss(text);
    char extra;
    if (!(iss >> value)) return false;
    if (iss >> extra) return false;   // something left after the number
    return true;
}

// Keeps asking until the user enters an integer between low and high.
int readInt(const string& prompt, int low, int high)
{
    int value;
    while (true) {
        string line = readLine(prompt);
        if (toInt(line, value) && value >= low && value <= high)
            return value;
        cout << "Invalid input. Enter a number between " << low
             << " and " << high << ".\n";
    }
}

// Keeps asking until the user enters non-empty text without the '|' character
// ('|' is used as the separator in the data file).
string readText(const string& prompt)
{
    while (true) {
        string line = readLine(prompt);
        if (line.empty()) {
            cout << "This field cannot be empty.\n";
        } else if (line.find('|') != string::npos) {
            cout << "The character '|' is not allowed.\n";
        } else {
            return line;
        }
    }
}

// ---------------------------------------------------------------
// Student class: stores the data of one student
// ---------------------------------------------------------------
class Student {
private:
    int rollNo;
    string name;
    string course;
    int marks[NUM_SUBJECTS];

public:
    // Default constructor
    Student() : rollNo(0), name(""), course("")
    {
        for (int i = 0; i < NUM_SUBJECTS; i++)
            marks[i] = 0;
    }

    // Parameterized constructor
    Student(int roll, const string& n, const string& c, const int m[])
        : rollNo(roll), name(n), course(c)
    {
        for (int i = 0; i < NUM_SUBJECTS; i++)
            marks[i] = m[i];
    }

    // Getters
    int getRollNo() const { return rollNo; }
    string getName() const { return name; }
    string getCourse() const { return course; }

    // Setters (roll number is not changed after creation)
    void setName(const string& n) { name = n; }
    void setCourse(const string& c) { course = c; }
    void setMarks(const int m[])
    {
        for (int i = 0; i < NUM_SUBJECTS; i++)
            marks[i] = m[i];
    }

    int getTotal() const
    {
        int total = 0;
        for (int i = 0; i < NUM_SUBJECTS; i++)
            total += marks[i];
        return total;
    }

    double getPercentage() const
    {
        return (getTotal() * 100.0) / (NUM_SUBJECTS * MAX_MARKS);
    }

    string getGrade() const
    {
        double p = getPercentage();
        if (p >= 90) return "A+";
        if (p >= 80) return "A";
        if (p >= 70) return "B";
        if (p >= 60) return "C";
        if (p >= 50) return "D";
        return "F";
    }

    // One row of the table shown by "Display all"
    void displayRow() const
    {
        cout << left << setw(8) << rollNo
             << setw(20) << name
             << setw(12) << course
             << right << setw(8) << getTotal()
             << setw(10) << fixed << setprecision(2) << getPercentage()
             << setw(7) << getGrade() << "\n";
        cout << left;
    }

    // Full details, used by search and update
    void displayFull() const
    {
        cout << "Roll No    : " << rollNo << "\n";
        cout << "Name       : " << name << "\n";
        cout << "Course     : " << course << "\n";
        for (int i = 0; i < NUM_SUBJECTS; i++)
            cout << "Subject " << (i + 1) << "  : " << marks[i] << "\n";
        cout << "Total      : " << getTotal() << " / "
             << NUM_SUBJECTS * MAX_MARKS << "\n";
        cout << "Percentage : " << fixed << setprecision(2)
             << getPercentage() << "%\n";
        cout << "Grade      : " << getGrade() << "\n";
    }

    // Converts the student to one line of the data file
    string toFileLine() const
    {
        ostringstream oss;
        oss << rollNo << "|" << name << "|" << course;
        for (int i = 0; i < NUM_SUBJECTS; i++)
            oss << "|" << marks[i];
        return oss.str();
    }

    // Reads one line of the data file. Returns false if the line is not valid.
    bool fromFileLine(const string& line)
    {
        vector<string> parts;
        string part;
        istringstream iss(line);
        while (getline(iss, part, '|'))
            parts.push_back(part);

        if (parts.size() != 3 + NUM_SUBJECTS) return false;

        int roll;
        if (!toInt(parts[0], roll) || roll <= 0) return false;
        if (parts[1].empty() || parts[2].empty()) return false;

        int m[NUM_SUBJECTS];
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            if (!toInt(parts[3 + i], m[i])) return false;
            if (m[i] < 0 || m[i] > MAX_MARKS) return false;
        }

        rollNo = roll;
        name = parts[1];
        course = parts[2];
        setMarks(m);
        return true;
    }
};

// ---------------------------------------------------------------
// StudentManager class: keeps all students and handles the file
// ---------------------------------------------------------------
class StudentManager {
private:
    vector<Student> students;

    // Returns the index of the student with this roll number, or -1
    int findIndex(int roll) const
    {
        for (size_t i = 0; i < students.size(); i++) {
            if (students[i].getRollNo() == roll)
                return (int)i;
        }
        return -1;
    }

    void saveToFile() const
    {
        ofstream out(DATA_FILE.c_str());
        if (!out) {
            cout << "Error: could not save records to " << DATA_FILE << ".\n";
            return;
        }
        for (size_t i = 0; i < students.size(); i++)
            out << students[i].toFileLine() << "\n";
    }

    void loadFromFile()
    {
        ifstream in(DATA_FILE.c_str());
        if (!in) return;   // file does not exist yet: start with no records

        string line;
        int skipped = 0;
        while (getline(in, line)) {
            if (line.empty()) continue;
            Student s;
            if (s.fromFileLine(line) && findIndex(s.getRollNo()) == -1)
                students.push_back(s);
            else
                skipped++;
        }
        if (skipped > 0)
            cout << "Warning: " << skipped
                 << " invalid or duplicate line(s) in the file were skipped.\n";
    }

    void readMarks(int m[]) const
    {
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            string prompt = "Marks in subject " + to_string(i + 1)
                          + " (0-" + to_string(MAX_MARKS) + "): ";
            m[i] = readInt(prompt, 0, MAX_MARKS);
        }
    }

public:
    // Constructor loads the saved records
    StudentManager()
    {
        loadFromFile();
    }

    void addStudent()
    {
        int roll = readInt("Roll number: ", 1, 999999);
        if (findIndex(roll) != -1) {
            cout << "A student with roll number " << roll << " already exists.\n";
            return;
        }
        string name = readText("Name: ");
        string course = readText("Course: ");
        int m[NUM_SUBJECTS];
        readMarks(m);

        students.push_back(Student(roll, name, course, m));
        saveToFile();
        cout << "Student added successfully.\n";
    }

    void displayAll() const
    {
        if (students.empty()) {
            cout << "No records available.\n";
            return;
        }
        cout << left << setw(8) << "Roll" << setw(20) << "Name"
             << setw(12) << "Course" << right << setw(8) << "Total"
             << setw(10) << "Percent" << setw(7) << "Grade" << "\n";
        cout << left << string(65, '-') << "\n";
        for (size_t i = 0; i < students.size(); i++)
            students[i].displayRow();
    }

    void searchStudent() const
    {
        int roll = readInt("Enter roll number to search: ", 1, 999999);
        int index = findIndex(roll);
        if (index == -1) {
            cout << "No student found with roll number " << roll << ".\n";
            return;
        }
        students[index].displayFull();
    }

    void updateStudent()
    {
        int roll = readInt("Enter roll number to update: ", 1, 999999);
        int index = findIndex(roll);
        if (index == -1) {
            cout << "No student found with roll number " << roll << ".\n";
            return;
        }

        cout << "\nCurrent details:\n";
        students[index].displayFull();
        cout << "\nWhat do you want to update?\n";
        cout << "1. Name\n2. Course\n3. Marks\n0. Cancel\n";
        int choice = readInt("Enter choice: ", 0, 3);

        if (choice == 0) {
            cout << "Update cancelled.\n";
            return;
        }
        if (choice == 1) {
            students[index].setName(readText("New name: "));
        } else if (choice == 2) {
            students[index].setCourse(readText("New course: "));
        } else {
            int m[NUM_SUBJECTS];
            readMarks(m);
            students[index].setMarks(m);
        }
        saveToFile();
        cout << "Record updated successfully.\n";
    }
};

// ---------------------------------------------------------------
// Menu
// ---------------------------------------------------------------
void showMenu()
{
    cout << "\n===== Student Record Management System =====\n";
    cout << "1. Add student\n";
    cout << "2. Display all students\n";
    cout << "3. Search student by roll number\n";
    cout << "4. Update student\n";
    cout << "0. Exit\n";
}

int main()
{
    StudentManager manager;
    int choice;

    do {
        showMenu();
        choice = readInt("Enter your choice: ", 0, 4);
        cout << "\n";

        switch (choice) {
            case 1: manager.addStudent(); break;
            case 2: manager.displayAll(); break;
            case 3: manager.searchStudent(); break;
            case 4: manager.updateStudent(); break;
            case 0: cout << "Records are saved. Goodbye!\n"; break;
        }
    } while (choice != 0);

    return 0;
}
