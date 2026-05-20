/*
 * Campus Event Management System
 * C++ Console Application
 * 
 * Features:
 * - Multi-Role Access (Admin/Student/Faculty)
 * - Event Creation and Management
 * - Venue Booking and Tracking
 * - RSVP and Waitlist Management
 * - Search and Filter
 * - Budget Management
 * - Attendance Verification
 * - File Persistence
 * 
 * Default Login: admin / admin123
 */

#include "ConsoleUI.h"

int main() {
    try {
        EventManager manager;
        ConsoleUI ui(manager);
        ui.run();
    }
    catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}