#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Types.h"
#include "User.h"
#include "Venue.h"
#include "Event.h"
#include "FileHandler.h"
#include "Templates.h"

class EventManager {
private:
    Repository<User*> users;
    Repository<Venue> venues;
    Repository<Event> events;
    FileHandler fileHandler;
    User* currentUser;
    int nextEventId, nextUserId, nextVenueId;

    string genEventId() { return "EVT" + to_string(nextEventId++); }
    string genUserId() { return "USR" + to_string(nextUserId++); }
    string genVenueId() { return "VEN" + to_string(nextVenueId++); }

public:
    EventManager() : fileHandler("data"), currentUser(nullptr), nextEventId(1), nextUserId(1), nextVenueId(1) {}

    ~EventManager() {
        saveAll();
        for (auto& p : users) delete p.second;
    }

    void saveAll() {
        fileHandler.saveUsers(users);
        fileHandler.saveVenues(venues);
        fileHandler.saveEvents(events);
    }

    void loadAll() {
        fileHandler.loadUsers(users);
        fileHandler.loadVenues(venues);
        fileHandler.loadEvents(events);

        // Update ID counters
        for (auto& p : users) {
            string id = p.second->getUserId();
            if (id.substr(0, 3) == "USR") {
                int n = stoi(id.substr(3));
                if (n >= nextUserId) nextUserId = n + 1;
            }
        }
        for (auto& p : venues) {
            string id = p.second.getVenueId();
            if (id.substr(0, 3) == "VEN") {
                int n = stoi(id.substr(3));
                if (n >= nextVenueId) nextVenueId = n + 1;
            }
        }
        for (auto& p : events) {
            string id = p.second.getEventId();
            if (id.substr(0, 3) == "EVT") {
                int n = stoi(id.substr(3));
                if (n >= nextEventId) nextEventId = n + 1;
            }
        }
    }

    void initDefaults() {
        // Default admin
        Admin* admin = new Admin(genUserId(), "admin", "admin123", "admin@university.edu", "System Administrator");
        admin->setDepartment("IT Services");
        users.add("admin", admin);

        // Sample venues
        Venue v1(genVenueId(), "Harvard Memorial Hall", "Cambridge, MA", AUDITORIUM, 1000, 500.0);
        v1.addFacility("Stage"); v1.addFacility("Sound System"); v1.addEquipment("Microphones");
        venues.add(v1.getVenueId(), v1);

        Venue v2(genVenueId(), "Science Center Lecture Hall", "Oxford Road", CLASSROOM, 250, 150.0);
        v2.addFacility("Whiteboard"); v2.addFacility("Projector");
        venues.add(v2.getVenueId(), v2);

        Venue v3(genVenueId(), "University Stadium", "Athletics Complex", SPORTS_FIELD, 5000, 1000.0);
        v3.addFacility("Floodlights"); v3.addEquipment("Scoreboard");
        venues.add(v3.getVenueId(), v3);

        Venue v4(genVenueId(), "Conference Room A", "Admin Building", CONFERENCE_ROOM, 30, 75.0);
        v4.addFacility("Video Conferencing");
        venues.add(v4.getVenueId(), v4);

        saveAll();
    }

    void initialize() {
        loadAll();
        if (users.empty()) initDefaults();
    }

    // Authentication
    bool login(string username, string password) {
        User** u = users.find(username);
        if (u && *u && (*u)->authenticate(password)) {
            currentUser = *u;
            fileHandler.logAction(currentUser->getUserId(), "Logged in");
            return true;
        }
        return false;
    }

    void logout() {
        if (currentUser) {
            fileHandler.logAction(currentUser->getUserId(), "Logged out");
            currentUser = nullptr;
        }
    }

    User* getCurrentUser() const { return currentUser; }
    bool isLoggedIn() const { return currentUser != nullptr; }

    // User Management
    bool registerUser(string username, string password, string email, string fullName, UserRole role) {
        if (users.exists(username)) return false;
        User* u = nullptr;
        string id = genUserId();
        switch (role) {
            case ADMIN: u = new Admin(id, username, password, email, fullName); break;
            case STUDENT: u = new Student(id, username, password, email, fullName); break;
            case FACULTY: u = new Faculty(id, username, password, email, fullName); break;
            default: u = new User(id, username, password, email, fullName, role);
        }
        users.add(username, u);
        saveAll();
        return true;
    }

    User* findUser(string username) {
        User** u = users.find(username);
        return u ? *u : nullptr;
    }

    vector<User*> getAllUsers() { return users.getAll(); }

    // Event Management
    bool createEvent(string title, string desc, EventType type, Date start, Date end, string venueId, int maxCap) {
        if (!currentUser || !currentUser->canCreateEvent()) return false;
        string id = genEventId();
        Event e(id, title, desc, type, start, end);
        e.setVenueId(venueId);
        e.setOrganizerId(currentUser->getUserId());
        e.setMaxCapacity(maxCap);

        Venue* v = venues.find(venueId);
        if (v) v->bookVenue(id, start);

        events.add(id, e);
        saveAll();
        fileHandler.logAction(currentUser->getUserId(), "Created event: " + title);
        return true;
    }

    bool editEvent(string eventId, string title, string desc, EventType type) {
        Event* e = events.find(eventId);
        if (!e || !currentUser) return false;
        if (currentUser->getRole() != ADMIN && e->getOrganizerId() != currentUser->getUserId()) return false;
        e->setTitle(title);
        e->setDescription(desc);
        e->setType(type);
        saveAll();
        return true;
    }

    bool cancelEvent(string eventId) {
        Event* e = events.find(eventId);
        if (!e || !currentUser) return false;
        if (currentUser->getRole() != ADMIN && e->getOrganizerId() != currentUser->getUserId()) return false;
        e->cancel();
        Venue* v = venues.find(e->getVenueId());
        if (v) v->cancelBooking(eventId, e->getStartDate());
        saveAll();
        return true;
    }

    bool publishEvent(string eventId) {
        Event* e = events.find(eventId);
        if (!e || !currentUser) return false;
        if (currentUser->getRole() != ADMIN && e->getOrganizerId() != currentUser->getUserId()) return false;
        bool r = e->publish();
        if (r) saveAll();
        return r;
    }

    bool completeEvent(string eventId) {
        Event* e = events.find(eventId);
        if (!e || !currentUser) return false;
        e->complete();
        saveAll();
        return true;
    }

    Event* findEvent(string eventId) { return events.find(eventId); }
    vector<Event> getAllEvents() { return events.getAll(); }

    vector<Event> getUpcomingEvents() {
        return events.filter([](const Event& e) { return e.isUpcoming(); });
    }

    vector<Event> getPastEvents() {
        return events.filter([](const Event& e) { return e.isPast(); });
    }

    vector<Event> searchEvents(SearchFilter& f) {
        return events.filter([&f](const Event& e) {
            if (!f.keyword.empty()) {
                string title = e.getTitle(), desc = e.getDescription(), kw = f.keyword;
                transform(title.begin(), title.end(), title.begin(), ::tolower);
                transform(desc.begin(), desc.end(), desc.begin(), ::tolower);
                transform(kw.begin(), kw.end(), kw.begin(), ::tolower);
                if (title.find(kw) == string::npos && desc.find(kw) == string::npos) return false;
            }
            if (f.filterByType && e.getType() != f.eventType) return false;
            if (f.filterByDate && (e.getStartDate() < f.startDate || e.getStartDate() > f.endDate)) return false;
            return true;
        });
    }

    // RSVP Management
    bool registerForEvent(string eventId) {
        if (!currentUser) return false;
        Event* e = events.find(eventId);
        if (!e) return false;
        bool r = e->registerAttendee(currentUser->getUserId(), currentUser->getFullName());
        if (r) {
            if (currentUser->getRole() == STUDENT)
                static_cast<Student*>(currentUser)->registerForEvent(eventId);
            saveAll();
        }
        return r;
    }

    bool cancelEventRegistration(string eventId) {
        if (!currentUser) return false;
        Event* e = events.find(eventId);
        if (!e) return false;
        bool r = e->cancelRegistration(currentUser->getUserId());
        if (r) {
            if (currentUser->getRole() == STUDENT)
                static_cast<Student*>(currentUser)->unregisterFromEvent(eventId);
            saveAll();
        }
        return r;
    }

    bool markEventAttendance(string eventId, string userId, bool attended) {
        if (!currentUser) return false;
        Event* e = events.find(eventId);
        if (!e) return false;
        if (currentUser->getRole() != ADMIN && e->getOrganizerId() != currentUser->getUserId()) return false;
        bool r = e->markAttendance(userId, attended);
        if (r) saveAll();
        return r;
    }

    // Venue Management
    bool addVenue(string name, string location, VenueType type, int capacity, double cost) {
        if (!currentUser || !currentUser->canManageVenue()) return false;
        string id = genVenueId();
        Venue v(id, name, location, type, capacity, cost);
        venues.add(id, v);
        saveAll();
        return true;
    }

    bool editVenue(string venueId, string name, string location, int capacity, double cost) {
        if (!currentUser || !currentUser->canManageVenue()) return false;
        Venue* v = venues.find(venueId);
        if (!v) return false;
        v->setName(name);
        v->setLocation(location);
        v->setCapacity(capacity);
        v->setCostPerHour(cost);
        saveAll();
        return true;
    }

    bool swapEventVenue(string eventId, string newVenueId) {
        if (!currentUser) return false;
        Event* e = events.find(eventId);
        Venue* newV = venues.find(newVenueId);
        if (!e || !newV) return false;
        if (currentUser->getRole() != ADMIN && e->getOrganizerId() != currentUser->getUserId()) return false;
        if (!newV->isAvailableOn(e->getStartDate())) return false;

        Venue* oldV = venues.find(e->getVenueId());
        if (oldV) oldV->cancelBooking(eventId, e->getStartDate());
        newV->bookVenue(eventId, e->getStartDate());
        e->setVenueId(newVenueId);
        saveAll();
        return true;
    }

    Venue* findVenue(string venueId) { return venues.find(venueId); }
    vector<Venue> getAllVenues() { return venues.getAll(); }

    vector<Venue> getAvailableVenues(Date date, int minCap = 0) {
        return venues.filter([&date, minCap](const Venue& v) {
            return v.isAvailableOn(date) && v.getCapacity() >= minCap;
        });
    }

    // Budget
    bool updateEventBudget(string eventId, Budget budget) {
        Event* e = events.find(eventId);
        if (!e || !currentUser) return false;
        if (currentUser->getRole() != ADMIN && e->getOrganizerId() != currentUser->getUserId()) return false;
        e->setBudget(budget);
        saveAll();
        return true;
    }

    // Statistics
    void displayEventStats() {
        auto all = events.getAll();
        int total = all.size(), upcoming = 0, past = 0, cancelled = 0, totalRegs = 0;
        map<EventType, int> typeCount;

        for (auto& e : all) {
            if (e.isUpcoming()) upcoming++;
            if (e.isPast()) past++;
            if (e.getStatus() == CANCELLED) cancelled++;
            totalRegs += e.getCurrentRegs();
            typeCount[e.getType()]++;
        }

        cout << "\n========== EVENT STATISTICS ==========\n";
        cout << "Total Events:        " << total << "\n";
        cout << "Upcoming Events:     " << upcoming << "\n";
        cout << "Past Events:         " << past << "\n";
        cout << "Cancelled Events:    " << cancelled << "\n";
        cout << "Total Registrations: " << totalRegs << "\n";
        cout << "\nEvents by Type:\n";
        for (auto& p : typeCount)
            cout << "  " << left << setw(15) << eventTypeToStr(p.first) << ": " << p.second << "\n";
        cout << "======================================\n";
    }

    void displayVenueStats() {
        auto all = venues.getAll();
        cout << "\n========== VENUE UTILIZATION ==========\n";
        cout << left << setw(25) << "Venue" << setw(15) << "Type" << setw(10) << "Capacity" << setw(10) << "Status" << "\n";
        cout << string(60, '-') << "\n";
        for (auto& v : all)
            cout << left << setw(25) << v.getName() << setw(15) << venueTypeToStr(v.getType())
                 << setw(10) << v.getCapacity() << setw(10) << (v.getIsAvailable() ? "Available" : "Booked") << "\n";
        cout << "========================================\n";
    }

    void displayUserStats() {
        auto all = users.getAll();
        int admins = 0, students = 0, faculty = 0;
        for (auto& u : all) {
            if (u->getRole() == ADMIN) admins++;
            else if (u->getRole() == STUDENT) students++;
            else if (u->getRole() == FACULTY) faculty++;
        }
        cout << "\n========== USER STATISTICS ==========\n";
        cout << "Total Users:   " << all.size() << "\n";
        cout << "Admins:        " << admins << "\n";
        cout << "Students:      " << students << "\n";
        cout << "Faculty:       " << faculty << "\n";
        cout << "=====================================\n";
    }

    FileHandler& getFileHandler() { return fileHandler; }
};

#endif