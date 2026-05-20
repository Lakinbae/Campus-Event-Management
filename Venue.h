#ifndef VENUE_H
#define VENUE_H

#include "Types.h"

class Venue {
private:
    string venueId, name, location;
    VenueType type;
    int capacity;
    vector<string> facilities, equipment;
    double costPerHour;
    bool isAvailable;
    map<string, vector<string>> bookings; // date -> eventIds

public:
    Venue() : type(CLASSROOM), capacity(0), costPerHour(0), isAvailable(true) {}
    Venue(string id, string n, string loc, VenueType t, int cap, double cost)
        : venueId(id), name(n), location(loc), type(t), capacity(cap), costPerHour(cost), isAvailable(true) {}

    // Getters
    string getVenueId() const { return venueId; }
    string getName() const { return name; }
    string getLocation() const { return location; }
    VenueType getType() const { return type; }
    int getCapacity() const { return capacity; }
    double getCostPerHour() const { return costPerHour; }
    bool getIsAvailable() const { return isAvailable; }

    // Setters
    void setName(string n) { name = n; }
    void setLocation(string l) { location = l; }
    void setCapacity(int c) { capacity = c; }
    void setCostPerHour(double c) { costPerHour = c; }
    void setIsAvailable(bool a) { isAvailable = a; }

    void addFacility(string f) {
        if (find(facilities.begin(), facilities.end(), f) == facilities.end())
            facilities.push_back(f);
    }

    void addEquipment(string e) {
        if (find(equipment.begin(), equipment.end(), e) == equipment.end())
            equipment.push_back(e);
    }

    bool isAvailableOn(Date date) const {
        if (!isAvailable) return false;
        string key = date.toDateOnly();
        auto it = bookings.find(key);
        return it == bookings.end() || it->second.empty();
    }

    bool bookVenue(string eventId, Date date) {
        if (!isAvailable) return false;
        bookings[date.toDateOnly()].push_back(eventId);
        return true;
    }

    void cancelBooking(string eventId, Date date) {
        string key = date.toDateOnly();
        auto it = bookings.find(key);
        if (it != bookings.end()) {
            auto& v = it->second;
            v.erase(remove(v.begin(), v.end(), eventId), v.end());
        }
    }

    void displayInfo() const {
        cout << "\n===== Venue Info =====\n";
        cout << "ID:       " << venueId << "\n";
        cout << "Name:     " << name << "\n";
        cout << "Location: " << location << "\n";
        cout << "Type:     " << venueTypeToStr(type) << "\n";
        cout << "Capacity: " << capacity << "\n";
        cout << "Cost/Hr:  $" << costPerHour << "\n";
        cout << "Status:   " << (isAvailable ? "Available" : "Unavailable") << "\n";
    }

    string serialize() const {
        ostringstream oss;
        oss << venueId << "|" << name << "|" << location << "|"
            << venueTypeToStr(type) << "|" << capacity << "|"
            << costPerHour << "|" << (isAvailable ? "1" : "0") << "|";
        for (int i = 0; i < facilities.size(); i++) {
            oss << facilities[i];
            if (i < facilities.size() - 1) oss << ",";
        }
        oss << "|";
        for (int i = 0; i < equipment.size(); i++) {
            oss << equipment[i];
            if (i < equipment.size() - 1) oss << ",";
        }
        return oss.str();
    }

    static Venue deserialize(string data) {
        Venue v;
        vector<string> t;
        stringstream ss(data);
        string token;
        while (getline(ss, token, '|')) t.push_back(token);

        if (t.size() >= 7) {
            v.venueId = t[0]; v.name = t[1]; v.location = t[2];
            v.type = strToVenueType(t[3]);
            v.capacity = stoi(t[4]);
            v.costPerHour = stod(t[5]);
            v.isAvailable = t[6] == "1";

            if (t.size() > 7 && !t[7].empty()) {
                stringstream fs(t[7]);
                string f;
                while (getline(fs, f, ',')) if (!f.empty()) v.facilities.push_back(f);
            }
            if (t.size() > 8 && !t[8].empty()) {
                stringstream es(t[8]);
                string e;
                while (getline(es, e, ',')) if (!e.empty()) v.equipment.push_back(e);
            }
        }
        return v;
    }
};

#endif