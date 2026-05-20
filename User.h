#ifndef USER_H
#define USER_H

#include "Types.h"

// ========== BASE USER CLASS ==========

class User {
protected:
    string userId, username, password, email, fullName, phone;
    UserRole role;
    Date createdAt;
    bool isActive;

public:
    User() : role(GUEST), createdAt(Date::now()), isActive(true) {}
    User(string id, string uname, string pwd, string mail, string name, UserRole r)
        : userId(id), username(uname), password(pwd), email(mail), fullName(name), role(r), createdAt(Date::now()), isActive(true) {}

    virtual ~User() = default;

    // Getters
    string getUserId() const { return userId; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getEmail() const { return email; }
    string getFullName() const { return fullName; }
    string getPhone() const { return phone; }
    UserRole getRole() const { return role; }
    bool getIsActive() const { return isActive; }

    // Setters
    void setEmail(string e) { email = e; }
    void setFullName(string n) { fullName = n; }
    void setPhone(string p) { phone = p; }
    void setPassword(string p) { password = p; }
    void setIsActive(bool a) { isActive = a; }

    bool authenticate(string pwd) const { return password == pwd && isActive; }

    virtual void displayInfo() const {
        cout << "\n===== User Info =====\n";
        cout << "ID:       " << userId << "\n";
        cout << "Username: " << username << "\n";
        cout << "Name:     " << fullName << "\n";
        cout << "Email:    " << email << "\n";
        cout << "Role:     " << roleToStr(role) << "\n";
        cout << "Active:   " << (isActive ? "Yes" : "No") << "\n";
    }

    virtual string serialize() const {
        ostringstream oss;
        oss << roleToStr(role) << "|" << userId << "|" << username << "|" << password << "|"
            << email << "|" << fullName << "|" << phone << "|"
            << createdAt.day << "|" << createdAt.month << "|" << createdAt.year << "|" << (isActive ? "1" : "0");
        return oss.str();
    }

    virtual bool canCreateEvent() const { return false; }
    virtual bool canManageVenue() const { return false; }
    virtual bool canViewReports() const { return false; }

    static User* deserialize(string data);
};

// ========== ADMIN CLASS ==========

class Admin : public User {
private:
    string department;
    int accessLevel;

public:
    Admin() : User(), accessLevel(10) { role = ADMIN; }
    Admin(string id, string uname, string pwd, string mail, string name)
        : User(id, uname, pwd, mail, name, ADMIN), department("Administration"), accessLevel(10) {}

    string getDepartment() const { return department; }
    void setDepartment(string d) { department = d; }
    void setAccessLevel(int l) { accessLevel = l; }

    void displayInfo() const override {
        User::displayInfo();
        cout << "Dept:     " << department << "\n";
        cout << "Access:   " << accessLevel << "\n";
    }

    string serialize() const override {
        return User::serialize() + "|" + department + "|" + to_string(accessLevel);
    }

    bool canCreateEvent() const override { return true; }
    bool canManageVenue() const override { return true; }
    bool canViewReports() const override { return true; }
};

// ========== STUDENT CLASS ==========

class Student : public User {
private:
    string studentId, major;
    int year;
    vector<string> registeredEvents;

public:
    Student() : User(), year(1) { role = STUDENT; }
    Student(string id, string uname, string pwd, string mail, string name)
        : User(id, uname, pwd, mail, name, STUDENT), year(1) {}

    string getStudentId() const { return studentId; }
    string getMajor() const { return major; }
    void setStudentId(string s) { studentId = s; }
    void setMajor(string m) { major = m; }
    void setYear(int y) { year = y; }

    void registerForEvent(string eventId) {
        if (find(registeredEvents.begin(), registeredEvents.end(), eventId) == registeredEvents.end())
            registeredEvents.push_back(eventId);
    }

    void unregisterFromEvent(string eventId) {
        auto it = find(registeredEvents.begin(), registeredEvents.end(), eventId);
        if (it != registeredEvents.end()) registeredEvents.erase(it);
    }

    bool isRegisteredFor(string eventId) const {
        return find(registeredEvents.begin(), registeredEvents.end(), eventId) != registeredEvents.end();
    }

    void displayInfo() const override {
        User::displayInfo();
        cout << "Student ID: " << studentId << "\n";
        cout << "Major:      " << major << "\n";
        cout << "Year:       " << year << "\n";
    }

    string serialize() const override {
        return User::serialize() + "|" + studentId + "|" + major + "|" + to_string(year);
    }
};

// ========== FACULTY CLASS ==========

class Faculty : public User {
private:
    string facultyId, department, designation;
    vector<string> organizedEvents;

public:
    Faculty() : User() { role = FACULTY; }
    Faculty(string id, string uname, string pwd, string mail, string name)
        : User(id, uname, pwd, mail, name, FACULTY) {}

    void setFacultyId(string f) { facultyId = f; }
    void setDepartment(string d) { department = d; }
    void setDesignation(string d) { designation = d; }
    void addOrganizedEvent(string eventId) { organizedEvents.push_back(eventId); }

    void displayInfo() const override {
        User::displayInfo();
        cout << "Faculty ID: " << facultyId << "\n";
        cout << "Dept:       " << department << "\n";
        cout << "Designation:" << designation << "\n";
    }

    string serialize() const override {
        return User::serialize() + "|" + facultyId + "|" + department + "|" + designation;
    }

    bool canCreateEvent() const override { return true; }
    bool canViewReports() const override { return true; }
};

// ========== DESERIALIZE FUNCTION ==========

User* User::deserialize(string data) {
    vector<string> t;
    stringstream ss(data);
    string token;
    while (getline(ss, token, '|')) t.push_back(token);

    if (t.size() < 11) return nullptr;

    UserRole role = strToRole(t[0]);
    User* user = nullptr;

    switch (role) {
        case ADMIN: {
            Admin* a = new Admin(t[1], t[2], t[3], t[4], t[5]);
            if (t.size() > 11) a->setDepartment(t[11]);
            if (t.size() > 12) a->setAccessLevel(stoi(t[12]));
            user = a;
            break;
        }
        case STUDENT: {
            Student* s = new Student(t[1], t[2], t[3], t[4], t[5]);
            if (t.size() > 11) s->setStudentId(t[11]);
            if (t.size() > 12) s->setMajor(t[12]);
            if (t.size() > 13) s->setYear(stoi(t[13]));
            user = s;
            break;
        }
        case FACULTY: {
            Faculty* f = new Faculty(t[1], t[2], t[3], t[4], t[5]);
            if (t.size() > 11) f->setFacultyId(t[11]);
            if (t.size() > 12) f->setDepartment(t[12]);
            if (t.size() > 13) f->setDesignation(t[13]);
            user = f;
            break;
        }
        default:
            user = new User(t[1], t[2], t[3], t[4], t[5], role);
    }

    user->phone = t[6];
    user->createdAt.day = stoi(t[7]);
    user->createdAt.month = stoi(t[8]);
    user->createdAt.year = stoi(t[9]);
    user->isActive = t[10] == "1";

    return user;
}

#endif