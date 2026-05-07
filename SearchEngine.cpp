#include <iostream>
#include "Event.h"

using namespace std;

const int MAX_EVENTS = 100;

// Search by Venue
void searchByVenue(Event events[], int totalEvents, string venue) {

    cout << "\n===== EVENTS AT " << venue << " =====\n";

    bool found = false;

    for(int i = 0; i < totalEvents; i++) {

        if(events[i].venue == venue) {

            cout << "Event ID: " << events[i].id << endl;
            cout << "Name: " << events[i].name << endl;
            cout << "Date: " << events[i].date << endl;
            cout << "Venue: " << events[i].venue << endl;
            cout << "--------------------------\n";

            found = true;
        }
    }

    if(!found) {
        cout << "No events found at this venue.\n";
    }
}

// Search by Date
void searchByDate(Event events[], int totalEvents, string date) {

    cout << "\n===== EVENTS ON " << date << " =====\n";

    bool found = false;

    for(int i = 0; i < totalEvents; i++) {

        if(events[i].date == date) {

            cout << "Event ID: " << events[i].id << endl;
            cout << "Name: " << events[i].name << endl;
            cout << "Venue: " << events[i].venue << endl;
            cout << "--------------------------\n";

            found = true;
        }
    }

    if(!found) {
        cout << "No events found on this date.\n";
    }
}

// Search by Event Name
void searchByName(Event events[], int totalEvents, string eventName) {

    bool found = false;

    cout << "\n===== SEARCH RESULTS =====\n";

    for(int i = 0; i < totalEvents; i++) {

        if(events[i].name == eventName) {

            cout << "Event Found!\n";

            cout << "ID: " << events[i].id << endl;
            cout << "Name: " << events[i].name << endl;
            cout << "Date: " << events[i].date << endl;
            cout << "Venue: " << events[i].venue << endl;

            found = true;
        }
    }

    if(!found) {
        cout << "Event not found.\n";
    }
}

// Display Upcoming Events
void showUpcomingEvents(Event events[], int totalEvents) {

    cout << "\n===== UPCOMING EVENTS =====\n";

    if(totalEvents == 0) {

        cout << "No events available.\n";
        return;
    }

    for(int i = 0; i < totalEvents; i++) {

        cout << "Event ID: " << events[i].id << endl;
        cout << "Name: " << events[i].name << endl;
        cout << "Date: " << events[i].date << endl;
        cout << "Venue: " << events[i].venue << endl;
        cout << "--------------------------\n";
    }
}