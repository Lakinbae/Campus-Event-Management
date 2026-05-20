#ifndef EVENT_H
#define EVENT_H

#include "Types.h"
#include "Templates.h"

class Event {
private:
    string eventId, title, description, venueId, organizerId;
    EventType type;
    EventStatus status;
    Date startDate, endDate, createdAt;
    int maxCapacity, currentRegs;
    Budget budget;
    vector<RSVP> rsvpList;
    Queue<string> waitlist;
    vector<string> tags;
    bool requiresReg;

public:
    Event() : type(OTHER), status(DRAFT), maxCapacity(0), currentRegs(0), requiresReg(true), createdAt(Date::now()) {}
    Event(string id, string t, string desc, EventType et, Date start, Date end)
        : eventId(id), title(t), description(desc), type(et), status(DRAFT),
          startDate(start), endDate(end), maxCapacity(100), currentRegs(0), requiresReg(true), createdAt(Date::now()) {}

    // Getters
    string getEventId() const { return eventId; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    EventType getType() const { return type; }
    EventStatus getStatus() const { return status; }
    Date getStartDate() const { return startDate; }
    Date getEndDate() const { return endDate; }
    string getVenueId() const { return venueId; }
    string getOrganizerId() const { return organizerId; }
    int getMaxCapacity() const { return maxCapacity; }
    int getCurrentRegs() const { return currentRegs; }
    Budget getBudget() const { return budget; }
    vector<RSVP> getRsvpList() const { return rsvpList; }

    // Setters
    void setTitle(string t) { title = t; }
    void setDescription(string d) { description = d; }
    void setType(EventType t) { type = t; }
    void setStatus(EventStatus s) { status = s; }
    void setVenueId(string v) { venueId = v; }
    void setOrganizerId(string o) { organizerId = o; }
    void setMaxCapacity(int c) { maxCapacity = c; }
    void setBudget(Budget b) { budget = b; }

    void addTag(string tag) {
        if (find(tags.begin(), tags.end(), tag) == tags.end()) tags.push_back(tag);
    }

    bool registerAttendee(string userId, string userName) {
        if (isUserRegistered(userId)) return false;
        RSVP r(userId, eventId, userName);
        if (currentRegs < maxCapacity) {
            r.status = CONFIRMED;
            currentRegs++;
        } else {
            r.status = WAITLISTED;
            waitlist.enqueue(userId);
        }
        rsvpList.push_back(r);
        return true;
    }

    bool cancelRegistration(string userId) {
        for (auto& r : rsvpList) {
            if (r.odorId == userId) {
                bool wasConfirmed = (r.status == CONFIRMED);
                r.status = RSVP_CANCELLED;
                if (wasConfirmed) {
                    currentRegs--;
                    string waitUser;
                    if (waitlist.dequeue(waitUser)) {
                        for (auto& wr : rsvpList) {
                            if (wr.odorId == waitUser && wr.status == WAITLISTED) {
                                wr.status = CONFIRMED;
                                currentRegs++;
                                break;
                            }
                        }
                    }
                }
                return true;
            }
        }
        return false;
    }

    bool isUserRegistered(string userId) const {
        for (auto& r : rsvpList)
            if (r.odorId == userId && (r.status == CONFIRMED || r.status == WAITLISTED)) return true;
        return false;
    }

    bool markAttendance(string userId, bool attended) {
        for (auto& r : rsvpList) {
            if (r.odorId == userId) {
                r.attended = attended;
                if (attended) r.status = ATTENDED;
                return true;
            }
        }
        return false;
    }

    int getAttendeeCount() const {
        int c = 0;
        for (auto& r : rsvpList) if (r.status == CONFIRMED || r.status == ATTENDED) c++;
        return c;
    }

    int getWaitlistCount() const {
        int c = 0;
        for (auto& r : rsvpList) if (r.status == WAITLISTED) c++;
        return c;
    }

    bool publish() { if (status == DRAFT) { status = PUBLISHED; return true; } return false; }
    bool cancel() { if (status != COMPLETED) { status = CANCELLED; return true; } return false; }
    bool complete() { status = COMPLETED; return true; }
    bool isUpcoming() const { return startDate > Date::now() && status == PUBLISHED; }
    bool isPast() const { return endDate < Date::now() || status == COMPLETED; }

    void displayInfo() const {
        cout << "\n============================================\n";
        cout << "           EVENT DETAILS\n";
        cout << "============================================\n";
        cout << "Event ID:     " << eventId << "\n";
        cout << "Title:        " << title << "\n";
        cout << "Description:  " << description << "\n";
        cout << "Type:         " << eventTypeToStr(type) << "\n";
        cout << "Status:       " << statusToStr(status) << "\n";
        cout << "Start:        " << startDate.toString() << "\n";
        cout << "End:          " << endDate.toString() << "\n";
        cout << "Venue ID:     " << venueId << "\n";
        cout << "Capacity:     " << currentRegs << "/" << maxCapacity << "\n";
        cout << "Waitlist:     " << getWaitlistCount() << "\n";
        cout << "============================================\n";
    }

    void displayAttendeeList() const {
        cout << "\n===== Attendee List: " << title << " =====\n";
        cout << left << setw(15) << "User ID" << setw(25) << "Name"
             << setw(15) << "Status" << setw(10) << "Attended" << "\n";
        cout << string(65, '-') << "\n";
        for (auto& r : rsvpList) {
            cout << left << setw(15) << r.odorId << setw(25) << r.odorName
                 << setw(15) << rsvpToStr(r.status) << setw(10) << (r.attended ? "Yes" : "No") << "\n";
        }
        cout << "\nConfirmed: " << getAttendeeCount() << " | Waitlisted: " << getWaitlistCount() << "\n";
    }

    void displayBudgetReport() const {
        cout << "\n===== Budget: " << title << " =====\n";
        budget.display();
    }

    string serialize() const {
        ostringstream oss;
        oss << eventId << "|" << title << "|" << description << "|"
            << eventTypeToStr(type) << "|" << statusToStr(status) << "|"
            << startDate.day << "|" << startDate.month << "|" << startDate.year << "|"
            << startDate.hour << "|" << startDate.minute << "|"
            << endDate.day << "|" << endDate.month << "|" << endDate.year << "|"
            << endDate.hour << "|" << endDate.minute << "|"
            << venueId << "|" << organizerId << "|" << maxCapacity << "|"
            << currentRegs << "|" << (requiresReg ? "1" : "0") << "|"
            << budget.venueRental << "|" << budget.equipment << "|"
            << budget.catering << "|" << budget.marketing << "|"
            << budget.misc << "|" << budget.ticketPrice << "|" << budget.expectedAttendees << "|";
        for (int i = 0; i < tags.size(); i++) { oss << tags[i]; if (i < tags.size() - 1) oss << ","; }
        oss << "|" << rsvpList.size();
        for (auto& r : rsvpList) oss << "|" << r.serialize();
        return oss.str();
    }

    static Event deserialize(string data) {
        Event e;
        vector<string> t;
        stringstream ss(data);
        string token;
        while (getline(ss, token, '|')) t.push_back(token);

        if (t.size() >= 27) {
            e.eventId = t[0]; e.title = t[1]; e.description = t[2];
            e.type = strToEventType(t[3]); e.status = strToStatus(t[4]);
            e.startDate = Date(stoi(t[5]), stoi(t[6]), stoi(t[7]), stoi(t[8]), stoi(t[9]));
            e.endDate = Date(stoi(t[10]), stoi(t[11]), stoi(t[12]), stoi(t[13]), stoi(t[14]));
            e.venueId = t[15]; e.organizerId = t[16];
            e.maxCapacity = stoi(t[17]); e.currentRegs = stoi(t[18]);
            e.requiresReg = t[19] == "1";
            e.budget.venueRental = stod(t[20]); e.budget.equipment = stod(t[21]);
            e.budget.catering = stod(t[22]); e.budget.marketing = stod(t[23]);
            e.budget.misc = stod(t[24]); e.budget.ticketPrice = stod(t[25]);
            e.budget.expectedAttendees = stoi(t[26]);

            if (t.size() > 27 && !t[27].empty()) {
                stringstream ts(t[27]);
                string tag;
                while (getline(ts, tag, ',')) if (!tag.empty()) e.tags.push_back(tag);
            }
            if (t.size() > 28) {
                int rsvpCount = stoi(t[28]);
                for (int i = 0; i < rsvpCount && (29 + i) < t.size(); i++)
                    e.rsvpList.push_back(RSVP::deserialize(t[29 + i]));
            }
        }
        return e;
    }
};

#endif
