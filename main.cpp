
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// --- DATA STRUCTURE (Member 1) ---
struct Event {
    int id;
    string name;
    string date;   // Format: YYYY-MM-DD
    string venue;
    string type;
    int capacity;
    int rsvpCount;
    double fee;
    bool isCompleted;
};

// Global variables for the whole team
Event events[100];
int eventCount = 0;
const string fileName = "events.txt";

// --- FILE MANAGEMENT (Member 5) ---
void saveData() {
    ofstream outFile(fileName);
    for (int i = 0; i < eventCount; i++) {
        outFile << events[i].id << " " << events[i].name << " " << events[i].date << " " 
                << events[i].venue << " " << events[i].type << " " << events[i].capacity << " " 
                << events[i].rsvpCount << " " << events[i].fee << " " << events[i].isCompleted << endl;
    }
    outFile.close();
}

void loadData() {
    ifstream inFile(fileName);
    if (!inFile) return; 
    eventCount = 0;
    while (inFile >> events[eventCount].id >> events[eventCount].name >> events[eventCount].date 
           >> events[eventCount].venue >> events[eventCount].type >> events[eventCount].capacity 
           >> events[eventCount].rsvpCount >> events[eventCount].fee >> events[eventCount].isCompleted) {
        eventCount++;
    }
    inFile.close();
}

// --- EVENT CREATION & BUDGET (Member 2) ---
void addEvent() {
    if (eventCount >= 100) {
        cout << "Database full!\n";
        return;
    }
    Event &e = events[eventCount];
    e.id = eventCount + 1;
    cout << "Enter Event Name (use_underscores): "; cin >> e.name;
    cout << "Enter Date (YYYY-MM-DD): "; cin >> e.date;
    cout << "Enter Venue: "; cin >> e.venue;
    cout << "Enter Type (Workshop/Social/Sports): "; cin >> e.type;
    cout << "Enter Capacity: "; cin >> e.capacity;
    cout << "Enter Entry Fee: "; cin >> e.fee;
    e.rsvpCount = 0;
    e.isCompleted = false;
    eventCount++;
    saveData();
    cout << "Event Created Successfully with ID: " << e.id << endl;
}

// --- REGISTRATION & CAPACITY (Member 3) ---
void rsvpStudent() {
    int id;
    cout << "Enter Event ID to RSVP: "; cin >> id;
    int idx = id - 1;

    if (idx < 0 || idx >= eventCount) {
        cout << "Invalid Event ID!\n";
        return;
    }
    if (events[idx].rsvpCount < events[idx].capacity) {
        events[idx].rsvpCount++;
        saveData();
        cout << "RSVP Successful for " << events[idx].name << "!\n";
        cout << "Total Revenue so far: $" << events[idx].rsvpCount * events[idx].fee << endl;
    } else {
        cout << "Event is at Full Capacity!\n";
    }
}

// --- SEARCH & FILTERS (Member 4) ---
void searchEvents() {
    int choice;
    cout << "Search by: 1. Venue  2. Type  3. Date\nChoice: "; cin >> choice;
    string query;
    cout << "Enter search term: "; cin >> query;

    cout << "\n--- Search Results ---\n";
    for (int i = 0; i < eventCount; i++) {
        bool match = false;
        if (choice == 1 && events[i].venue == query) match = true;
        if (choice == 2 && events[i].type == query) match = true;
        if (choice == 3 && events[i].date == query) match = true;

        if (match) {
            cout << "ID: " << events[i].id << " | " << events[i].name << " at " << events[i].venue << endl;
        }
    }
}

void showHistory() {
    cout << "\n--- Past/Completed Events ---\n";
    for (int i = 0; i < eventCount; i++) {
        if (events[i].isCompleted) cout << events[i].name << " (" << events[i].date << ")\n";
    }
    cout << "\n--- Upcoming Events ---\n";
    for (int i = 0; i < eventCount; i++) {
        if (!events[i].isCompleted) cout << events[i].name << " (" << events[i].date << ")\n";
    }
}

// --- EDITING & VERIFICATION (Member 5) ---
void swapVenue() {
    int id;
    cout << "Enter Event ID to change venue: "; cin >> id;
    cout << "Enter New Venue: "; cin >> events[id - 1].venue;
    saveData();
    cout << "Venue updated.\n";
}

void verifyAttendance() {
    int id;
    cout << "Enter Event ID to mark as Completed: "; cin >> id;
    events[id - 1].isCompleted = true;
    saveData();
    cout << "Event finalized.\n";
}

// --- MAIN INTERFACE (Member 1) ---
int main() {
    loadData();
    int role; // 1: Admin, 2: Student, 3: Faculty
    cout << "Welcome to Campus Event Manager\n";
    cout << "1. Admin  2. Student  3. Faculty\nSelect Role: "; cin >> role;

    int choice = 0;
    while (choice != 7) {
        cout << "\n--- MENU ---\n";
        cout << "1. View All Events\n2. Create Event (Admin Only)\n3. RSVP for Event (Student)\n";
        cout << "4. Search/Filter\n5. Event History\n6. Edit/Verify (Admin/Faculty)\n7. Exit\n";
        cout << "Choice: "; cin >> choice;

        switch (choice) {
            case 1:
                cout << left << setw(5) << "ID" << setw(15) << "Name" << setw(15) << "Venue" << setw(10) << "RSVPs" << endl;
                for (int i = 0; i < eventCount; i++)
                    cout << left << setw(5) << events[i].id << setw(15) << events[i].name << setw(15) << events[i].venue << events[i].rsvpCount << "/" << events[i].capacity << endl;
                break;
            case 2:
                if (role == 1) addEvent();
                else cout << "Access Denied.\n";
                break;
            case 3:
                rsvpStudent();
                break;
            case 4:
                searchEvents();
                break;
            case 5:
                showHistory();
                break;
            case 6:
                if (role == 1 || role == 3) {
                    int sub; cout << "1. Swap Venue  2. Verify Completion\nChoice: "; cin >> sub;
                    if (sub == 1) swapVenue(); else verifyAttendance();
                } else cout << "Access Denied.\n";
                break;
            case 7:
                saveData();
                cout << "All data saved to " << fileName << ". Goodbye!\n";
                break;
        }
    }
    return 0;
}
