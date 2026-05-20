#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Types.h"
#include "User.h"
#include "Venue.h"
#include "Event.h"
#include "Templates.h"
#include <sys/stat.h>

class FileHandler {
private:
    string dataDir;

public:
    FileHandler(string dir = "data") : dataDir(dir) {}

    bool ensureDir() const {
        struct stat st;
        if (stat(dataDir.c_str(), &st) != 0) {
#ifdef _WIN32
            return _mkdir(dataDir.c_str()) == 0;
#else
            return mkdir(dataDir.c_str(), 0755) == 0;
#endif
        }
        return true;
    }

    // Save/Load Users
    bool saveUsers(Repository<User*>& users) {
        ensureDir();
        ofstream f(dataDir + "/users.dat");
        if (!f.is_open()) return false;
        for (auto& p : users) if (p.second) f << p.second->serialize() << "\n";
        f.close();
        return true;
    }

    bool loadUsers(Repository<User*>& users) {
        ifstream f(dataDir + "/users.dat");
        if (!f.is_open()) return false;
        string line;
        while (getline(f, line)) {
            if (!line.empty()) {
                User* u = User::deserialize(line);
                if (u) users.add(u->getUsername(), u);
            }
        }
        f.close();
        return true;
    }

    // Save/Load Venues
    bool saveVenues(Repository<Venue>& venues) {
        ensureDir();
        ofstream f(dataDir + "/venues.dat");
        if (!f.is_open()) return false;
        for (auto& p : venues) f << p.second.serialize() << "\n";
        f.close();
        return true;
    }

    bool loadVenues(Repository<Venue>& venues) {
        ifstream f(dataDir + "/venues.dat");
        if (!f.is_open()) return false;
        string line;
        while (getline(f, line)) {
            if (!line.empty()) {
                Venue v = Venue::deserialize(line);
                venues.add(v.getVenueId(), v);
            }
        }
        f.close();
        return true;
    }

    // Save/Load Events
    bool saveEvents(Repository<Event>& events) {
        ensureDir();
        ofstream f(dataDir + "/events.dat");
        if (!f.is_open()) return false;
        for (auto& p : events) f << p.second.serialize() << "\n";
        f.close();
        return true;
    }

    bool loadEvents(Repository<Event>& events) {
        ifstream f(dataDir + "/events.dat");
        if (!f.is_open()) return false;
        string line;
        while (getline(f, line)) {
            if (!line.empty()) {
                Event e = Event::deserialize(line);
                events.add(e.getEventId(), e);
            }
        }
        f.close();
        return true;
    }

    // Logging
    void logAction(string userId, string action) {
        ensureDir();
        ofstream f(dataDir + "/logs.txt", ios::app);
        if (f.is_open()) {
            time_t now = time(nullptr);
            char ts[64];
            strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", localtime(&now));
            f << "[" << ts << "] User: " << userId << " - " << action << "\n";
            f.close();
        }
    }

    // Export to CSV
    bool exportEventsCSV(vector<Event>& events, string filename) {
        ofstream f(filename);
        if (!f.is_open()) return false;
        f << "Event ID,Title,Type,Status,Start Date,End Date,Venue,Capacity,Registrations\n";
        for (auto& e : events) {
            f << e.getEventId() << ",\"" << e.getTitle() << "\"," << eventTypeToStr(e.getType()) << ","
              << statusToStr(e.getStatus()) << "," << e.getStartDate().toString() << ","
              << e.getEndDate().toString() << "," << e.getVenueId() << ","
              << e.getMaxCapacity() << "," << e.getCurrentRegs() << "\n";
        }
        f.close();
        return true;
    }

    bool exportAttendeesCSV(Event& event, string filename) {
        ofstream f(filename);
        if (!f.is_open()) return false;
        f << "User ID,Name,Status,Attended,Registration Date\n";
        for (auto& r : event.getRsvpList()) {
            f << r.odorId << ",\"" << r.odorName << "\"," << rsvpToStr(r.status) << ","
              << (r.attended ? "Yes" : "No") << "," << r.regDate.toDateOnly() << "\n";
        }
        f.close();
        return true;
    }
};

#endif