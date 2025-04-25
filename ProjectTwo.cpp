// ProjectTwo.cpp
// CS 300 Project Two – Advising Assistance Program
// Implements menu, file loading, course listing, and course lookup
// Author: Christopher Phillips
// Date: April 20, 2025

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

// --- Data structure for a course ----------------------------------------
struct Course {
    std::string number;               // e.g. "CSCI200"
    std::string title;                // e.g. "Data Structures"
    std::vector<std::string> prereqs; // list of courseNumbers
};

// Global storage for all loaded courses
static std::vector<Course> courses;
static bool dataLoaded = false;

// Utility: convert string to uppercase for case‑insensitive compare
std::string toUpper(const std::string &s) {
    std::string out = s;
    for (char &c : out) c = std::toupper(static_cast<unsigned char>(c));
    return out;
}

// Step 1 & 3: Load courses from CSV file into 'courses' vector -----------
void loadData() {
    std::cout << "Enter course data file name: ";
    std::string filename;
    std::getline(std::cin, filename);
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Error: cannot open file \"" << filename << "\"\n";
        return;
    }

    courses.clear();
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        Course c;
        // parse courseNumber
        std::getline(ss, c.number, ',');
        // parse title
        std::getline(ss, c.title, ',');
        // parse zero or more prereqs
        std::string pre;
        while (std::getline(ss, pre, ',')) {
            if (!pre.empty()) {
                // normalize to uppercase
                c.prereqs.push_back(toUpper(pre));
            }
        }
        // normalize course number uppercase
        c.number = toUpper(c.number);
        courses.push_back(c);
    }
    fin.close();

    dataLoaded = true;
    std::cout << "Data loaded. " << courses.size() << " courses available.\n";
}

// Step 4: Print sorted alphanumeric course list --------------------------
void printCourseList() {
    if (!dataLoaded) {
        std::cout << "Error: Data not loaded. Please choose option 1 first.\n";
        return;
    }
    // sort by course.number
    std::sort(courses.begin(), courses.end(),
        [](auto &a, auto &b){
            return a.number < b.number;
        });
    std::cout << "Here is a sample schedule:\n";
    for (auto &c : courses) {
        std::cout << c.number << ", " << c.title << "\n";
    }
}

// Step 5: Print details for a single course ------------------------------
void printCourseInfo() {
    if (!dataLoaded) {
        std::cout << "Error: Data not loaded. Please choose option 1 first.\n";
        return;
    }
    std::cout << "What course do you want to know about? ";
    std::string query;
    std::getline(std::cin, query);
    query = toUpper(query);

    // find course by number
    auto it = std::find_if(courses.begin(), courses.end(),
        [&](auto &c){ return c.number == query; });
    if (it == courses.end()) {
        std::cout << "Course " << query << " not found.\n";
        return;
    }
    // print course title
    std::cout << it->number << ", " << it->title << "\n";
    // print prerequisites
    if (it->prereqs.empty()) {
        std::cout << "Prerequisites: None\n";
    } else {
        std::cout << "Prerequisites: ";
        for (size_t i = 0; i < it->prereqs.size(); ++i) {
            std::cout << it->prereqs[i];
            if (i + 1 < it->prereqs.size())
                std::cout << ", ";
        }
        std::cout << "\n";
    }
}

// Display menu options
void displayMenu() {
    std::cout << "\nWelcome to the course planner.\n"
              << "1. Load Data Structure.\n"
              << "2. Print Course List.\n"
              << "3. Print Course.\n"
              << "9. Exit.\n"
              << "What would you like to do? ";
}

int main() {
    while (true) {
        displayMenu();
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            loadData();
        }
        else if (choice == "2") {
            printCourseList();
        }
        else if (choice == "3") {
            printCourseInfo();
        }
        else if (choice == "9") {
            std::cout << "Thank you for using the course planner!\n";
            break;
        }
        else {
            std::cout << choice
                      << " is not a valid option.\n";
        }
    }
    return 0;
}
