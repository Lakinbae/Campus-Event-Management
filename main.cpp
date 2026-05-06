#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Event {
public:
    int id;
    char name[50];
    char date[15]; // YYYY-MM-DD
    char venue[30];
    char type[20];
    int capacity;
    int rsvpCount;
    double fee;
    bool isCompleted;
};

// Global variables so everyone can access them
Event events[100]; 
int eventCount = 0;

// Member 1 also writes the Login
int login() {
    int role;
    cout << "Login as: 1. Admin  2. Student  3. Faculty\nChoice: ";
    cin >> role;
    return role;
}

// --- MAIN INTERFACE (This is the Menu) ---
int main() {
    loadData(); // First, it loads existing events from events.txt
    
    int role; // Stores if you are Admin, Student, or Faculty
    cout << "Welcome to Campus Event Manager\n";
    cout << "1. Admin  2. Student  3. Faculty\nSelect Role: "; cin >> role;

    int choice = 0;
    while (choice != 7) { // The menu keeps running until you choose '7'
        cout << "\n--- MENU ---\n";
        cout << "1. View All Events\n2. Create Event (Admin Only)\n3. RSVP for Event (Student)\n";
        cout << "4. Search/Filter\n5. Event History\n6. Edit/Verify (Admin/Faculty)\n7. Exit\n";
        cout << "Choice: "; cin >> choice;

        switch (choice) {
            case 1:
                // Loops through the array and prints everything
                for (int i = 0; i < eventCount; i++)
                    cout << events[i].id << ". " << events[i].name << endl;
                break;
            case 2:
                if (role == 1) addEvent(); // Member 2's Task
                else cout << "Access Denied.\n";
                break;
            case 3:
                rsvpStudent(); // Member 3's Task
                break;
            case 4:
                searchEvents(); // Member 4's Task
                break;
            case 5:
                showHistory(); // Member 4's Task
                break;
            case 6:
                if (role == 1 || role == 3) {
                    swapVenue(); // Member 5's Task
                } else cout << "Access Denied.\n";
                break;
            case 7:
                saveData(); // Member 5's Task: Saves to file before closing
                cout << "Goodbye!\n";
                break;
        }
    }
    return 0;
}

