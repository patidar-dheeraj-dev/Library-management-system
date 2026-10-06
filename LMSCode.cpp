#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <limits>

using namespace std;

const string BOOK_FILE = "book.dat";
const string STUDENT_FILE = "student.dat";

struct Book {
    string bookNo;
    string title;
    string author;
};

struct Student {
    string admissionNo;
    string name;
    int token = 0;
    string bookNo = "";
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

vector<Book> loadBooks() {
    vector<Book> books;
    ifstream in(BOOK_FILE);
    if (!in) {
        return books;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Book b;
        getline(ss, b.bookNo, '|');
        getline(ss, b.title, '|');
        getline(ss, b.author, '|');
        if (!b.bookNo.empty()) {
            books.push_back(b);
        }
    }
    return books;
}

void saveBooks(const vector<Book>& books) {
    ofstream out(BOOK_FILE, ios::trunc);
    for (const auto& b : books) {
        out << b.bookNo << "|" << b.title << "|" << b.author << "\n";
    }
}

vector<Student> loadStudents() {
    vector<Student> students;
    ifstream in(STUDENT_FILE);
    if (!in) {
        return students;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Student s;
        getline(ss, s.admissionNo, '|');
        getline(ss, s.name, '|');
        string tokenStr;
        getline(ss, tokenStr, '|');
        getline(ss, s.bookNo, '|');
        s.token = tokenStr.empty() ? 0 : stoi(tokenStr);
        if (!s.admissionNo.empty()) {
            students.push_back(s);
        }
    }
    return students;
}

void saveStudents(const vector<Student>& students) {
    ofstream out(STUDENT_FILE, ios::trunc);
    for (const auto& s : students) {
        out << s.admissionNo << "|" << s.name << "|" << s.token << "|" << s.bookNo << "\n";
    }
}

Book* findBook(vector<Book>& books, const string& bookNo) {
    auto it = find_if(books.begin(), books.end(), [&](const Book& b) {
        return b.bookNo == bookNo;
    });
    return it == books.end() ? nullptr : &(*it);
}

Student* findStudent(vector<Student>& students, const string& admissionNo) {
    auto it = find_if(students.begin(), students.end(), [&](const Student& s) {
        return s.admissionNo == admissionNo;
    });
    return it == students.end() ? nullptr : &(*it);
}

void createBook(vector<Book>& books) {
    Book b;
    cout << "\nNEW BOOK ENTRY\n";
    cout << "Enter Book No: ";
    cin >> b.bookNo;
    cout << "Enter Book Name: ";
    cin.ignore();
    getline(cin, b.title);
    cout << "Enter Author Name: ";
    getline(cin, b.author);
    books.push_back(b);
    saveBooks(books);
    cout << "\nBook created successfully.";
    pauseScreen();
}

void createStudent(vector<Student>& students) {
    Student s;
    cout << "\nNEW STUDENT ENTRY\n";
    cout << "Enter Admission No.: ";
    cin >> s.admissionNo;
    cout << "Enter Student Name: ";
    cin.ignore();
    getline(cin, s.name);
    s.token = 0;
    s.bookNo = "";
    students.push_back(s);
    saveStudents(students);
    cout << "\nStudent record created.";
    pauseScreen();
}

void displayAllBooks(const vector<Book>& books) {
    clearScreen();
    cout << "\n\n\t\tBOOK LIST\n\n";
    cout << left << setw(12) << "Book No" << setw(30) << "Book Name" << "Author" << endl;
    cout << string(72, '=') << endl;
    if (books.empty()) {
        cout << "No books found." << endl;
    } else {
        for (const auto& b : books) {
            cout << left << setw(12) << b.bookNo << setw(30) << b.title << b.author << endl;
        }
    }
    pauseScreen();
}

void displayAllStudents(const vector<Student>& students) {
    clearScreen();
    cout << "\n\n\t\tSTUDENT LIST\n\n";
    cout << left << setw(12) << "Adm No" << setw(22) << "Student Name" << "Book Issued" << endl;
    cout << string(72, '=') << endl;
    if (students.empty()) {
        cout << "No students found." << endl;
    } else {
        for (const auto& s : students) {
            cout << left << setw(12) << s.admissionNo << setw(22) << s.name << s.bookNo << endl;
        }
    }
    pauseScreen();
}

void displaySpecificBook(vector<Book>& books) {
    string bookNo;
    cout << "\nEnter Book No.: ";
    cin >> bookNo;

    Book* b = findBook(books, bookNo);
    if (b == nullptr) {
        cout << "\nBook does not exist.";
    } else {
        cout << "\nBook Number: " << b->bookNo << endl;
        cout << "Book Name: " << b->title << endl;
        cout << "Book Author: " << b->author << endl;
    }
    pauseScreen();
}

void displaySpecificStudent(vector<Student>& students) {
    string admNo;
    cout << "\nEnter Admission No.: ";
    cin >> admNo;

    Student* s = findStudent(students, admNo);
    if (s == nullptr) {
        cout << "\nStudent does not exist.";
    } else {
        cout << "\nAdmission Number: " << s->admissionNo << endl;
        cout << "Student Name: " << s->name << endl;
        cout << "Books Issued: " << s->token << endl;
        if (s->token == 1) {
            cout << "Issued Book No.: " << s->bookNo << endl;
        }
    }
    pauseScreen();
}

void modifyBook(vector<Book>& books) {
    string bookNo;
    cout << "\nEnter Book No. to modify: ";
    cin >> bookNo;

    Book* b = findBook(books, bookNo);
    if (b == nullptr) {
        cout << "\nBook not found.";
    } else {
        cout << "\nEnter new Book Name: ";
        cin.ignore();
        getline(cin, b->title);
        cout << "Enter new Author Name: ";
        getline(cin, b->author);
        saveBooks(books);
        cout << "\nBook record updated.";
    }
    pauseScreen();
}

void modifyStudent(vector<Student>& students) {
    string admissionNo;
    cout << "\nEnter Admission No. to modify: ";
    cin >> admissionNo;

    Student* s = findStudent(students, admissionNo);
    if (s == nullptr) {
        cout << "\nStudent not found.";
    } else {
        cout << "\nEnter new student name: ";
        cin.ignore();
        getline(cin, s->name);
        saveStudents(students);
        cout << "\nStudent record updated.";
    }
    pauseScreen();
}

void deleteBook(vector<Book>& books) {
    string bookNo;
    cout << "\nEnter Book No. to delete: ";
    cin >> bookNo;

    auto it = remove_if(books.begin(), books.end(), [&](const Book& b) {
        return b.bookNo == bookNo;
    });

    if (it == books.end()) {
        cout << "\nBook not found.";
    } else {
        books.erase(it, books.end());
        saveBooks(books);
        cout << "\nBook deleted successfully.";
    }
    pauseScreen();
}

void deleteStudent(vector<Student>& students) {
    string admissionNo;
    cout << "\nEnter Admission No. to delete: ";
    cin >> admissionNo;

    auto it = remove_if(students.begin(), students.end(), [&](const Student& s) {
        return s.admissionNo == admissionNo;
    });

    if (it == students.end()) {
        cout << "\nStudent not found.";
    } else {
        students.erase(it, students.end());
        saveStudents(students);
        cout << "\nStudent deleted successfully.";
    }
    pauseScreen();
}

void issueBook(vector<Book>& books, vector<Student>& students) {
    string admissionNo, bookNo;
    cout << "\nEnter Admission No.: ";
    cin >> admissionNo;
    Student* s = findStudent(students, admissionNo);
    if (s == nullptr) {
        cout << "\nStudent record does not exist.";
        pauseScreen();
        return;
    }

    if (s->token == 1) {
        cout << "\nThis student has already issued a book.";
        pauseScreen();
        return;
    }

    cout << "Enter Book No.: ";
    cin >> bookNo;
    Book* b = findBook(books, bookNo);
    if (b == nullptr) {
        cout << "\nBook does not exist.";
        pauseScreen();
        return;
    }

    s->token = 1;
    s->bookNo = b->bookNo;
    saveStudents(students);
    cout << "\nBook issued successfully.";
    pauseScreen();
}

void depositBook(vector<Student>& students) {
    string admissionNo;
    cout << "\nEnter Admission No.: ";
    cin >> admissionNo;

    Student* s = findStudent(students, admissionNo);
    if (s == nullptr) {
        cout << "\nStudent record does not exist.";
        pauseScreen();
        return;
    }

    if (s->token == 0) {
        cout << "\nNo book issued to this student.";
        pauseScreen();
        return;
    }

    int days;
    cout << "Enter number of days used: ";
    cin >> days;
    int fine = 0;
    if (days > 15) {
        fine = (days - 15) * 1;
        cout << "\nFine = " << fine << " Rs.";
    }

    s->token = 0;
    s->bookNo = "";
    saveStudents(students);
    cout << "\nBook deposited successfully.";
    pauseScreen();
}

void startScreen() {
    clearScreen();
    cout << "\n=====================================" << endl;
    cout << "          LIBRARY MANAGEMENT" << endl;
    cout << "              SYSTEM" << endl;
    cout << "=====================================" << endl;
    cout << "\nBy: Dheeraj Patidar" << endl;
    pauseScreen();
}


void adminMenu(vector<Book>& books, vector<Student>& students) {
    int option;
    do {
        clearScreen();
        cout << "\n\n\tADMINISTRATOR MENU" << endl;
        cout << "\n\t1. CREATE STUDENT RECORD";
        cout << "\n\t2. DISPLAY ALL STUDENT RECORDS";
        cout << "\n\t3. DISPLAY SPECIFIC STUDENT RECORD";
        cout << "\n\t4. MODIFY STUDENT RECORD";
        cout << "\n\t5. DELETE STUDENT RECORD";
        cout << "\n\t6. CREATE BOOK";
        cout << "\n\t7. DISPLAY ALL BOOKS";
        cout << "\n\t8. DISPLAY SPECIFIC BOOK";
        cout << "\n\t9. MODIFY BOOK RECORD";
        cout << "\n\t10. DELETE BOOK RECORD";
        cout << "\n\t11. BACK TO MAIN MENU";
        cout << "\n\n\tEnter your choice (1-11): ";
        cin >> option;

        switch (option) {
            case 1:
                createStudent(students);
                break;
            case 2:
                displayAllStudents(students);
                break;
            case 3:
                displaySpecificStudent(students);
                break;
            case 4:
                modifyStudent(students);
                break;
            case 5:
                deleteStudent(students);
                break;
            case 6:
                createBook(books);
                break;
            case 7:
                displayAllBooks(books);
                break;
            case 8:
                displaySpecificBook(books);
                break;
            case 9:
                modifyBook(books);
                break;
            case 10:
                deleteBook(books);
                break;
            case 11:
                return;
            default:
                cout << "\nInvalid choice.";
                pauseScreen();
        }
    } while (option != 11);
}

int main() {
    vector<Book> books = loadBooks();
    vector<Student> students = loadStudents();

    startScreen();

    char choice;
    do {
        clearScreen();
        cout << "\n\n\tMAIN MENU" << endl;
        cout << "\n\t1. BOOK ISSUE";
        cout << "\n\t2. BOOK DEPOSIT";
        cout << "\n\t3. ADMINISTRATOR MENU";
        cout << "\n\t4. EXIT";
        cout << "\n\n\tPlease select your option (1-4): ";
        cin >> choice;

        switch (choice) {
            case '1':
                issueBook(books, students);
                break;
            case '2':
                depositBook(students);
                break;
            case '3':
                adminMenu(books, students);
                break;
            case '4':
                cout << "\nExiting the system." << endl;
                break;
            default:
                cout << "\nInvalid choice.";
                pauseScreen();
        }
    } while (choice != '4');

    saveBooks(books);
    saveStudents(students);
    return 0;
}
