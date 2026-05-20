#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "EventManager.h"

class ConsoleUI {
private:
    EventManager& mgr;
    bool running;

    // Utility
    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void waitEnter() {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    void printHeader(string title) {
        cout << "\n";
        cout << "============================================================\n";
        cout << "  " << title << "\n";
        cout << "============================================================\n";
    }

    void printLine() { cout << "------------------------------------------------------------\n"; }

    int getChoice(int min, int max) {
        int c;
        while (true) {
            cout << "\nChoice (" << min << "-" << max << "): ";
            if (cin >> c && c >= min && c <= max) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return c;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
        }
    }

    string getStr(string prompt) {
        string s;
        cout << prompt;
        getline(cin, s);
        return s;
    }

    int getInt(string prompt) {
        int v;
        while (true) {
            cout << prompt;
            if (cin >> v) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return v; }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid number!\n";
        }
    }

    double getDouble(string prompt) {
        double v;
        while (true) {
            cout << prompt;
            if (cin >> v) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return v; }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid number!\n";
        }
    }

    Date getDate(string prompt) {
        cout << prompt << "\n";
        int d = getInt("  Day (1-31): ");
        int m = getInt("  Month (1-12): ");
        int y = getInt("  Year: ");
        int h = getInt("  Hour (0-23): ");
        int min = getInt("  Minute (0-59): ");
        return Date(d, m, y, h, min);
    }

    bool getYesNo(string prompt) {
        string s;
        while (true) {
            cout << prompt << " (y/n): ";
            getline(cin, s);
            if (s == "y" || s == "Y") return true;
            if (s == "n" || s == "N") return false;
            cout << "Enter y or n!\n";
        }
    }

public:
    ConsoleUI(EventManager& m) : mgr(m), running(true) {}

    void run() {
        mgr.initialize();

        while (running) {
            clearScreen();

            if (!mgr.isLoggedIn()) {
                // Login Menu
                printHeader("CAMPUS EVENT MANAGEMENT SYSTEM");
                cout << "\n  1. Login\n  2. Register\n  3. Exit\n";
                int c = getChoice(1, 3);

                if (c == 1) {
                    // Login
                    printHeader("LOGIN");
                    string u = getStr("Username: ");
                    string p = getStr("Password: ");
                    if (mgr.login(u, p)) cout << "\nLogin successful!\n";
                    else cout << "\nInvalid credentials!\n";
                    waitEnter();
                }
                else if (c == 2) {
                    // Register
                    printHeader("REGISTER");
                    string u = getStr("Username: ");
                    string p = getStr("Password: ");
                    string e = getStr("Email: ");
                    string n = getStr("Full Name: ");
                    cout << "\nRole: 1. Student  2. Faculty\n";
                    int r = getChoice(1, 2);
                    UserRole role = (r == 1) ? STUDENT : FACULTY;
                    if (mgr.registerUser(u, p, e, n, role)) cout << "\nRegistration successful!\n";
                    else cout << "\nUsername already exists!\n";
                    waitEnter();
                }
                else {
                    running = false;
                }
            }
            else {
                User* user = mgr.getCurrentUser();
                UserRole role = user->getRole();

                // Show menu based on role
                if (role == ADMIN) {
                    printHeader("ADMIN DASHBOARD");
                    cout << "\nWelcome, " << user->getFullName() << "!\n";
                    printLine();
                    cout << "  [EVENTS]\n";
                    cout << "  1.  Create Event\n";
                    cout << "  2.  Edit Event\n";
                    cout << "  3.  View All Events\n";
                    cout << "  4.  Search Events\n";
                    cout << "  5.  Publish Event\n";
                    cout << "  6.  Cancel Event\n";
                    cout << "  7.  Complete Event\n";
                    printLine();
                    cout << "  [VENUES]\n";
                    cout << "  8.  Add Venue\n";
                    cout << "  9.  Edit Venue\n";
                    cout << "  10. View Venues\n";
                    cout << "  11. Swap Event Venue\n";
                    printLine();
                    cout << "  [ATTENDEES]\n";
                    cout << "  12. View Attendee List\n";
                    cout << "  13. Mark Attendance\n";
                    printLine();
                    cout << "  [REPORTS]\n";
                    cout << "  14. View Statistics\n";
                    cout << "  15. Update Budget\n";
                    cout << "  16. View Budget Report\n";
                    printLine();
                    cout << "  0.  Logout\n";

                    int c = getChoice(0, 16);
                    handleAdminChoice(c);
                }
                else if (role == STUDENT) {
                    printHeader("STUDENT PORTAL");
                    cout << "\nWelcome, " << user->getFullName() << "!\n";
                    printLine();
                    cout << "  1. View Upcoming Events\n";
                    cout << "  2. Search Events\n";
                    cout << "  3. View Event Details\n";
                    cout << "  4. Register for Event\n";
                    cout << "  5. Cancel Registration\n";
                    cout << "  6. View My Registrations\n";
                    cout << "  7. View Past Events\n";
                    printLine();
                    cout << "  0. Logout\n";

                    int c = getChoice(0, 7);
                    handleStudentChoice(c);
                }
                else if (role == FACULTY) {
                    printHeader("FACULTY PORTAL");
                    cout << "\nWelcome, " << user->getFullName() << "!\n";
                    printLine();
                    cout << "  [EVENTS]\n";
                    cout << "  1.  Create Event\n";
                    cout << "  2.  Edit Event\n";
                    cout << "  3.  View All Events\n";
                    cout << "  4.  Search Events\n";
                    cout << "  5.  Publish Event\n";
                    cout << "  6.  Cancel Event\n";
                    printLine();
                    cout << "  [REGISTRATION]\n";
                    cout << "  7.  Register for Event\n";
                    cout << "  8.  View Attendee List\n";
                    cout << "  9.  Mark Attendance\n";
                    printLine();
                    cout << "  [REPORTS]\n";
                    cout << "  10. View Statistics\n";
                    cout << "  11. Update Budget\n";
                    printLine();
                    cout << "  0.  Logout\n";

                    int c = getChoice(0, 11);
                    handleFacultyChoice(c);
                }
            }
        }
        cout << "\nThank you for using Campus Event Management System!\n";
    }

private:
    // ========== ADMIN HANDLERS ==========
    void handleAdminChoice(int c) {
        switch (c) {
            case 0: mgr.logout(); cout << "\nLogged out.\n"; waitEnter(); break;
            case 1: createEvent(); break;
            case 2: editEvent(); break;
            case 3: viewAllEvents(); waitEnter(); break;
            case 4: searchEvents(); break;
            case 5: publishEvent(); break;
            case 6: cancelEvent(); break;
            case 7: completeEvent(); break;
            case 8: addVenue(); break;
            case 9: editVenue(); break;
            case 10: viewVenues(); waitEnter(); break;
            case 11: swapVenue(); break;
            case 12: viewAttendees(); break;
            case 13: markAttendance(); break;
            case 14: viewStats(); break;
            case 15: updateBudget(); break;
            case 16: viewBudgetReport(); break;
        }
    }

    // ========== STUDENT HANDLERS ==========
    void handleStudentChoice(int c) {
        switch (c) {
            case 0: mgr.logout(); cout << "\nLogged out.\n"; waitEnter(); break;
            case 1: viewUpcoming(); waitEnter(); break;
            case 2: searchEvents(); break;
            case 3: viewEventDetails(); break;
            case 4: registerForEvent(); break;
            case 5: cancelRegistration(); break;
            case 6: viewMyRegistrations(); break;
            case 7: viewPast(); waitEnter(); break;
        }
    }

    // ========== FACULTY HANDLERS ==========
    void handleFacultyChoice(int c) {
        switch (c) {
            case 0: mgr.logout(); cout << "\nLogged out.\n"; waitEnter(); break;
            case 1: createEvent(); break;
            case 2: editEvent(); break;
            case 3: viewAllEvents(); waitEnter(); break;
            case 4: searchEvents(); break;
            case 5: publishEvent(); break;
            case 6: cancelEvent(); break;
            case 7: registerForEvent(); break;
            case 8: viewAttendees(); break;
            case 9: markAttendance(); break;
            case 10: viewStats(); break;
            case 11: updateBudget(); break;
        }
    }

    // ========== SHARED FUNCTIONS ==========
    void viewAllEvents() {
        printHeader("ALL EVENTS");
        auto events = mgr.getAllEvents();
        if (events.empty()) { cout << "\nNo events.\n"; return; }
        cout << "\n" << left << setw(10) << "ID" << setw(30) << "Title" << setw(12) << "Type" << setw(12) << "Status" << setw(12) << "Date" << "\n";
        printLine();
        for (auto& e : events)
            cout << left << setw(10) << e.getEventId() << setw(30) << e.getTitle().substr(0, 28)
                 << setw(12) << eventTypeToStr(e.getType()) << setw(12) << statusToStr(e.getStatus())
                 << setw(12) << e.getStartDate().toDateOnly() << "\n";
    }

    void viewUpcoming() {
        printHeader("UPCOMING EVENTS");
        auto events = mgr.getUpcomingEvents();
        if (events.empty()) { cout << "\nNo upcoming events.\n"; return; }
        for (auto& e : events)
            cout << e.getEventId() << " - " << e.getTitle() << " (" << e.getStartDate().toDateOnly() << ")\n";
    }

    void viewPast() {
        printHeader("PAST EVENTS");
        auto events = mgr.getPastEvents();
        if (events.empty()) { cout << "\nNo past events.\n"; return; }
        for (auto& e : events)
            cout << e.getEventId() << " - " << e.getTitle() << " (" << e.getStartDate().toDateOnly() << ")\n";
    }

    void viewVenues() {
        printHeader("ALL VENUES");
        auto venues = mgr.getAllVenues();
        if (venues.empty()) { cout << "\nNo venues.\n"; return; }
        cout << "\n" << left << setw(8) << "ID" << setw(28) << "Name" << setw(15) << "Type" << setw(10) << "Capacity" << setw(10) << "$/Hour" << "\n";
        printLine();
        for (auto& v : venues)
            cout << left << setw(8) << v.getVenueId() << setw(28) << v.getName().substr(0, 26)
                 << setw(15) << venueTypeToStr(v.getType()) << setw(10) << v.getCapacity()
                 << setw(10) << v.getCostPerHour() << "\n";
    }

    void createEvent() {
        printHeader("CREATE EVENT");
        string title = getStr("Title: ");
        string desc = getStr("Description: ");

        cout << "\nEvent Type:\n";
        cout << "  1. Academic    2. Cultural    3. Sports\n";
        cout << "  4. Workshop    5. Seminar     6. Conference\n";
        cout << "  7. Social      8. Career      9. Club\n";
        int t = getChoice(1, 9);
        EventType types[] = {ACADEMIC, CULTURAL, SPORTS, WORKSHOP, SEMINAR, CONFERENCE, SOCIAL, CAREER, CLUB};
        EventType type = types[t - 1];

        Date start = getDate("Start Date/Time:");
        Date end = getDate("End Date/Time:");

        cout << "\nAvailable Venues:\n";
        auto venues = mgr.getAllVenues();
        for (int i = 0; i < venues.size(); i++)
            cout << "  " << (i + 1) << ". " << venues[i].getName() << " (Cap: " << venues[i].getCapacity() << ")\n";
        int vi = getChoice(1, venues.size());
        string venueId = venues[vi - 1].getVenueId();

        int cap = getInt("Max Capacity: ");

        if (mgr.createEvent(title, desc, type, start, end, venueId, cap))
            cout << "\nEvent created!\n";
        else
            cout << "\nFailed to create event.\n";
        waitEnter();
    }

    void editEvent() {
        viewAllEvents();
        string id = getStr("\nEvent ID to edit: ");
        Event* e = mgr.findEvent(id);
        if (!e) { cout << "Event not found.\n"; waitEnter(); return; }

        cout << "\nCurrent Title: " << e->getTitle() << "\n";
        string title = getStr("New Title (Enter to keep): ");
        if (title.empty()) title = e->getTitle();

        cout << "Current Desc: " << e->getDescription() << "\n";
        string desc = getStr("New Desc (Enter to keep): ");
        if (desc.empty()) desc = e->getDescription();

        if (mgr.editEvent(id, title, desc, e->getType()))
            cout << "\nEvent updated!\n";
        else
            cout << "\nFailed to update.\n";
        waitEnter();
    }

    void publishEvent() {
        viewAllEvents();
        string id = getStr("\nEvent ID to publish: ");
        if (mgr.publishEvent(id)) cout << "\nEvent published!\n";
        else cout << "\nFailed to publish.\n";
        waitEnter();
    }

    void cancelEvent() {
        viewAllEvents();
        string id = getStr("\nEvent ID to cancel: ");
        if (getYesNo("Are you sure?")) {
            if (mgr.cancelEvent(id)) cout << "\nEvent cancelled.\n";
            else cout << "\nFailed to cancel.\n";
        }
        waitEnter();
    }

    void completeEvent() {
        viewAllEvents();
        string id = getStr("\nEvent ID to complete: ");
        if (mgr.completeEvent(id)) cout << "\nEvent completed.\n";
        else cout << "\nFailed.\n";
        waitEnter();
    }

    void searchEvents() {
        printHeader("SEARCH EVENTS");
        SearchFilter f;
        f.keyword = getStr("Keyword (Enter to skip): ");

        if (getYesNo("Filter by type?")) {
            cout << "  1. Academic  2. Cultural  3. Sports  4. Workshop  5. Seminar  6. Conference\n";
            int t = getChoice(1, 6);
            EventType types[] = {ACADEMIC, CULTURAL, SPORTS, WORKSHOP, SEMINAR, CONFERENCE};
            f.eventType = types[t - 1];
            f.filterByType = true;
        }

        if (getYesNo("Filter by date?")) {
            f.startDate = getDate("From:");
            f.endDate = getDate("To:");
            f.filterByDate = true;
        }

        auto results = mgr.searchEvents(f);
        cout << "\nFound " << results.size() << " event(s):\n";
        for (auto& e : results)
            cout << e.getEventId() << " - " << e.getTitle() << " (" << e.getStartDate().toDateOnly() << ")\n";
        waitEnter();
    }

    void viewEventDetails() {
        string id = getStr("Event ID: ");
        Event* e = mgr.findEvent(id);
        if (e) {
            e->displayInfo();
            Venue* v = mgr.findVenue(e->getVenueId());
            if (v) cout << "\nVenue: " << v->getName() << " (" << v->getLocation() << ")\n";
        }
        else cout << "Event not found.\n";
        waitEnter();
    }

    void registerForEvent() {
        printHeader("REGISTER FOR EVENT");
        auto upcoming = mgr.getUpcomingEvents();
        if (upcoming.empty()) { cout << "\nNo upcoming events.\n"; waitEnter(); return; }

        cout << "\nUpcoming Events:\n";
        for (auto& e : upcoming)
            cout << "  " << e.getEventId() << " - " << e.getTitle() << " (" << e.getCurrentRegs() << "/" << e.getMaxCapacity() << ")\n";

        string id = getStr("\nEvent ID: ");
        if (mgr.registerForEvent(id)) cout << "\nRegistered!\n";
        else cout << "\nRegistration failed.\n";
        waitEnter();
    }

    void cancelRegistration() {
        string id = getStr("Event ID: ");
        if (mgr.cancelEventRegistration(id)) cout << "\nCancelled.\n";
        else cout << "\nFailed.\n";
        waitEnter();
    }

    void viewMyRegistrations() {
        printHeader("MY REGISTRATIONS");
        User* user = mgr.getCurrentUser();
        auto events = mgr.getAllEvents();
        bool found = false;
        for (auto& e : events) {
            if (e.isUserRegistered(user->getUserId())) {
                if (!found) { cout << "\nYour Events:\n"; printLine(); found = true; }
                cout << e.getEventId() << " - " << e.getTitle() << " (" << e.getStartDate().toString() << ")\n";
            }
        }
        if (!found) cout << "\nNo registrations.\n";
        waitEnter();
    }

    void viewAttendees() {
        string id = getStr("Event ID: ");
        Event* e = mgr.findEvent(id);
        if (e) e->displayAttendeeList();
        else cout << "Event not found.\n";
        waitEnter();
    }

    void markAttendance() {
        viewAllEvents();
        string eid = getStr("\nEvent ID: ");
        Event* e = mgr.findEvent(eid);
        if (!e) { cout << "Event not found.\n"; waitEnter(); return; }
        e->displayAttendeeList();
        string uid = getStr("\nUser ID: ");
        bool a = getYesNo("Attended?");
        if (mgr.markEventAttendance(eid, uid, a)) cout << "\nUpdated.\n";
        else cout << "\nFailed.\n";
        waitEnter();
    }

    void addVenue() {
        printHeader("ADD VENUE");
        string name = getStr("Name: ");
        string loc = getStr("Location: ");

        cout << "\nType:\n";
        cout << "  1. Auditorium  2. Classroom  3. Laboratory  4. Sports Field\n";
        cout << "  5. Conference  6. Outdoor    7. Cafeteria   8. Library\n";
        int t = getChoice(1, 8);
        VenueType types[] = {AUDITORIUM, CLASSROOM, LABORATORY, SPORTS_FIELD, CONFERENCE_ROOM, OUTDOOR, CAFETERIA, LIBRARY};
        VenueType type = types[t - 1];

        int cap = getInt("Capacity: ");
        double cost = getDouble("Cost/Hour ($): ");

        if (mgr.addVenue(name, loc, type, cap, cost)) cout << "\nVenue added!\n";
        else cout << "\nFailed.\n";
        waitEnter();
    }

    void editVenue() {
        viewVenues();
        string id = getStr("\nVenue ID: ");
        Venue* v = mgr.findVenue(id);
        if (!v) { cout << "Not found.\n"; waitEnter(); return; }

        cout << "\nCurrent Name: " << v->getName() << "\n";
        string name = getStr("New Name (Enter to keep): ");
        if (name.empty()) name = v->getName();

        cout << "Current Location: " << v->getLocation() << "\n";
        string loc = getStr("New Location (Enter to keep): ");
        if (loc.empty()) loc = v->getLocation();

        cout << "Current Capacity: " << v->getCapacity() << "\n";
        int cap = getInt("New Capacity: ");

        cout << "Current Cost/Hour: $" << v->getCostPerHour() << "\n";
        double cost = getDouble("New Cost/Hour: ");

        if (mgr.editVenue(id, name, loc, cap, cost)) cout << "\nUpdated!\n";
        else cout << "\nFailed.\n";
        waitEnter();
    }

    void swapVenue() {
        viewAllEvents();
        string eid = getStr("\nEvent ID: ");
        Event* e = mgr.findEvent(eid);
        if (!e) { cout << "Not found.\n"; waitEnter(); return; }

        cout << "\nCurrent Venue: " << e->getVenueId() << "\n";
        cout << "\nAvailable Venues:\n";
        auto venues = mgr.getAvailableVenues(e->getStartDate());
        for (auto& v : venues)
            cout << "  " << v.getVenueId() << " - " << v.getName() << " (Cap: " << v.getCapacity() << ")\n";

        string vid = getStr("\nNew Venue ID: ");
        if (mgr.swapEventVenue(eid, vid)) cout << "\nSwapped!\n";
        else cout << "\nFailed.\n";
        waitEnter();
    }

    void updateBudget() {
        viewAllEvents();
        string id = getStr("\nEvent ID: ");
        Event* e = mgr.findEvent(id);
        if (!e) { cout << "Not found.\n"; waitEnter(); return; }

        Budget b;
        b.venueRental = getDouble("Venue Rental ($): ");
        b.equipment = getDouble("Equipment ($): ");
        b.catering = getDouble("Catering ($): ");
        b.marketing = getDouble("Marketing ($): ");
        b.misc = getDouble("Miscellaneous ($): ");
        b.ticketPrice = getDouble("Ticket Price ($): ");
        b.expectedAttendees = getInt("Expected Attendees: ");

        if (mgr.updateEventBudget(id, b)) {
            cout << "\nBudget updated!\n";
            b.display();
        }
        else cout << "\nFailed.\n";
        waitEnter();
    }

    void viewBudgetReport() {
        string id = getStr("Event ID: ");
        Event* e = mgr.findEvent(id);
        if (e) e->displayBudgetReport();
        else cout << "Not found.\n";
        waitEnter();
    }

    void viewStats() {
        printHeader("SYSTEM STATISTICS");
        mgr.displayEventStats();
        mgr.displayVenueStats();
        mgr.displayUserStats();
        waitEnter();
    }
};

#endif