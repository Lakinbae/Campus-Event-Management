#ifndef TYPES_H
#define TYPES_H

#include <bits/stdc++.h>
using namespace std;

// ========== ENUMS ==========

enum UserRole { ADMIN, STUDENT, FACULTY, GUEST };
enum EventType { ACADEMIC, CULTURAL, SPORTS, WORKSHOP, SEMINAR, CONFERENCE, SOCIAL, CAREER, CLUB, OTHER };
enum EventStatus { DRAFT, PUBLISHED, ONGOING, COMPLETED, CANCELLED };
enum VenueType { AUDITORIUM, CLASSROOM, LABORATORY, SPORTS_FIELD, CONFERENCE_ROOM, OUTDOOR, CAFETERIA, LIBRARY };
enum RSVPStatus { CONFIRMED, PENDING, WAITLISTED, RSVP_CANCELLED, ATTENDED };

// ========== CONVERSION FUNCTIONS ==========

string roleToStr(UserRole r) {
    string s[] = {"Admin", "Student", "Faculty", "Guest"};
    return s[r];
}

UserRole strToRole(string s) {
    if (s == "Admin") return ADMIN;
    if (s == "Student") return STUDENT;
    if (s == "Faculty") return FACULTY;
    return GUEST;
}

string eventTypeToStr(EventType t) {
    string s[] = {"Academic", "Cultural", "Sports", "Workshop", "Seminar", "Conference", "Social", "Career", "Club", "Other"};
    return s[t];
}

EventType strToEventType(string s) {
    if (s == "Academic") return ACADEMIC;
    if (s == "Cultural") return CULTURAL;
    if (s == "Sports") return SPORTS;
    if (s == "Workshop") return WORKSHOP;
    if (s == "Seminar") return SEMINAR;
    if (s == "Conference") return CONFERENCE;
    if (s == "Social") return SOCIAL;
    if (s == "Career") return CAREER;
    if (s == "Club") return CLUB;
    return OTHER;
}

string statusToStr(EventStatus s) {
    string arr[] = {"Draft", "Published", "Ongoing", "Completed", "Cancelled"};
    return arr[s];
}

EventStatus strToStatus(string s) {
    if (s == "Draft") return DRAFT;
    if (s == "Published") return PUBLISHED;
    if (s == "Ongoing") return ONGOING;
    if (s == "Completed") return COMPLETED;
    return CANCELLED;
}

string venueTypeToStr(VenueType t) {
    string s[] = {"Auditorium", "Classroom", "Laboratory", "Sports Field", "Conference Room", "Outdoor", "Cafeteria", "Library"};
    return s[t];
}

VenueType strToVenueType(string s) {
    if (s == "Auditorium") return AUDITORIUM;
    if (s == "Classroom") return CLASSROOM;
    if (s == "Laboratory") return LABORATORY;
    if (s == "Sports Field") return SPORTS_FIELD;
    if (s == "Conference Room") return CONFERENCE_ROOM;
    if (s == "Outdoor") return OUTDOOR;
    if (s == "Cafeteria") return CAFETERIA;
    return LIBRARY;
}

string rsvpToStr(RSVPStatus s) {
    string arr[] = {"Confirmed", "Pending", "Waitlisted", "Cancelled", "Attended"};
    return arr[s];
}

RSVPStatus strToRSVP(string s) {
    if (s == "Confirmed") return CONFIRMED;
    if (s == "Pending") return PENDING;
    if (s == "Waitlisted") return WAITLISTED;
    if (s == "Cancelled") return RSVP_CANCELLED;
    return ATTENDED;
}

// ========== DATE STRUCT ==========

struct Date {
    int day, month, year, hour, minute;

    Date() : day(1), month(1), year(2024), hour(0), minute(0) {}
    Date(int d, int m, int y, int h = 0, int min = 0) : day(d), month(m), year(y), hour(h), minute(min) {}

    static Date now() {
        time_t t = time(nullptr);
        tm* n = localtime(&t);
        return Date(n->tm_mday, n->tm_mon + 1, n->tm_year + 1900, n->tm_hour, n->tm_min);
    }

    string toString() const {
        ostringstream oss;
        oss << setfill('0') << setw(2) << day << "/" << setw(2) << month << "/" << year
            << " " << setw(2) << hour << ":" << setw(2) << minute;
        return oss.str();
    }

    string toDateOnly() const {
        ostringstream oss;
        oss << setfill('0') << setw(2) << day << "/" << setw(2) << month << "/" << year;
        return oss.str();
    }

    bool operator<(const Date& o) const {
        if (year != o.year) return year < o.year;
        if (month != o.month) return month < o.month;
        if (day != o.day) return day < o.day;
        if (hour != o.hour) return hour < o.hour;
        return minute < o.minute;
    }
    bool operator>(const Date& o) const { return o < *this; }
    bool operator<=(const Date& o) const { return !(o < *this); }
    bool operator>=(const Date& o) const { return !(*this < o); }
    bool operator==(const Date& o) const { return day == o.day && month == o.month && year == o.year; }
};

// ========== BUDGET STRUCT ==========

struct Budget {
    double venueRental, equipment, catering, marketing, misc, ticketPrice;
    int expectedAttendees;

    Budget() : venueRental(0), equipment(0), catering(0), marketing(0), misc(0), ticketPrice(0), expectedAttendees(0) {}

    double totalExpenses() const { return venueRental + equipment + catering + marketing + misc; }
    double expectedRevenue() const { return ticketPrice * expectedAttendees; }
    double netBudget() const { return expectedRevenue() - totalExpenses(); }

    void display() const {
        cout << "\n===== Budget Breakdown =====\n";
        cout << fixed << setprecision(2);
        cout << "Venue Rental:    $" << venueRental << "\n";
        cout << "Equipment:       $" << equipment << "\n";
        cout << "Catering:        $" << catering << "\n";
        cout << "Marketing:       $" << marketing << "\n";
        cout << "Miscellaneous:   $" << misc << "\n";
        cout << "-----------------------------\n";
        cout << "Total Expenses:  $" << totalExpenses() << "\n";
        cout << "Ticket Price:    $" << ticketPrice << "\n";
        cout << "Expected Attend: " << expectedAttendees << "\n";
        cout << "Expected Revenue:$" << expectedRevenue() << "\n";
        cout << "Net Budget:      $" << netBudget() << "\n";
    }
};

// ========== RSVP STRUCT ==========

struct RSVP {
    string odorId, eventId, odorName;
    RSVPStatus status;
    Date regDate;
    bool attended;

    RSVP() : status(PENDING), attended(false) {}
    RSVP(string odor, string event, string name) : odorId(odor), eventId(event), odorName(name), status(PENDING), regDate(Date::now()), attended(false) {}

    string serialize() const {
        ostringstream oss;
        oss << odorId << "|" << eventId << "|" << odorName << "|" << rsvpToStr(status) << "|"
            << regDate.day << "|" << regDate.month << "|" << regDate.year << "|" << (attended ? "1" : "0");
        return oss.str();
    }

    static RSVP deserialize(string data) {
        RSVP r;
        vector<string> t;
        stringstream ss(data);
        string token;
        while (getline(ss, token, '|')) t.push_back(token);
        if (t.size() >= 8) {
            r.odorId = t[0]; r.eventId = t[1]; r.odorName = t[2];
            r.status = strToRSVP(t[3]);
            r.regDate.day = stoi(t[4]); r.regDate.month = stoi(t[5]); r.regDate.year = stoi(t[6]);
            r.attended = t[7] == "1";
        }
        return r;
    }
};

// ========== SEARCH FILTER ==========

struct SearchFilter {
    string keyword;
    EventType eventType;
    VenueType venueType;
    Date startDate, endDate;
    bool filterByType, filterByVenue, filterByDate;

    SearchFilter() : filterByType(false), filterByVenue(false), filterByDate(false) {}
};

#endif