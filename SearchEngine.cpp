#include <iostream>
#include <vector>
#include "Event.h"

using namespace std;

void searchByVenue(vector<Event> events, string venue) {

    cout << "\nEvents at " << venue << ":\n";

    for(Event e : events) {
        if(e.venue == venue) {
            cout << e.name << endl;
        }
    }
}

void searchByDate(vector<Event> events, string date) {

    cout << "\nEvents on " << date << ":\n";

    for(Event e : events) {
        if(e.date == date) {
            cout << e.name << endl;
        }
    }
}

void showUpcomingEvents(vector<Event> events) {

    cout << "\n===== UPCOMING EVENTS =====\n";

    for(Event e : events) {
        cout << e.name << " - " << e.date << endl;
    }
}