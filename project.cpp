#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

// Template class to handle various data types for customer information
template <typename T>
class CustomerInfo {
public:
    T info;
    CustomerInfo(T input) : info(input) {}
    void display() const {
        cout << info;
    }
};

// Base class for Room
class Room {
public:
    int roomNumber;
    double baseRate;
    bool isAvailable;

    Room(int number, double rate) : roomNumber(number), baseRate(rate), isAvailable(true) {}

    // Polymorphic method to calculate bill
    virtual double calculateBill(int nights, int roomsNeeded) {
        return baseRate * nights * roomsNeeded;
    }

    virtual void displayRoomInfo() const {
        cout << "Room Number: " << roomNumber << ", Rate: ₹" << baseRate << ", Available: "
             << (isAvailable ? "Yes" : "No") << endl;
    }

    virtual ~Room() {}
};

// Derived class for DeluxeRoom with a 20% surcharge
class DeluxeRoom : public Room {
public:
    DeluxeRoom(int number) : Room(number, 15000.0) {}

    double calculateBill(int nights, int roomsNeeded) override {
        return baseRate * nights * roomsNeeded * 1.2; // 20% surcharge
    }

    void displayRoomInfo() const override {
        cout << "Deluxe Room Number: " << roomNumber << ", Rate: ₹" << baseRate << ", Available: "
             << (isAvailable ? "Yes" : "No") << endl;
    }
};

// Customer class
class Customer {
public:
    static int idCounter;
    int id;
    string name;
    CustomerInfo<string> phone;
    int numGuests;
    CustomerInfo<string> aadhar;
    CustomerInfo<string> address;
    vector<string> guestNames;
    int loyaltyPoints = 0;
    string bookingDate;

    Customer(string customerName, string phoneNumber, int guests, string aadharNumber, string customerAddress)
        : name(customerName), phone(phoneNumber), numGuests(guests), aadhar(aadharNumber), address(customerAddress) {
        id = ++idCounter;
    }

    void addPoints(int points) {
        loyaltyPoints += points;
    }

    void setBookingDate() {
        bookingDate = "2024-11-07"; // Hardcoded date for simplicity
    }

    void displayCustomerInfo() const {
        cout << "Customer ID: " << id << ", Name: " << name << ", Phone: ";
        phone.display();
        cout << ", Aadhar: ";
        aadhar.display();
        cout << ", Address: ";
        address.display();
        cout << ", Loyalty Points: " << loyaltyPoints << endl;
        cout << "Booked on: " << bookingDate << endl;
        cout << "Guests: ";
        for (const auto& guest : guestNames) {
            cout << guest << " ";
        }
        cout << endl;
    }

    void checkOffers() {
        cout << "Offers for " << name << ": \n";
        if (loyaltyPoints >= 100) {
            cout << "You are eligible for a 10% discount!\n";
        } else {
            cout << "No offers available yet. Keep collecting points!\n";
        }
    }
};

int Customer::idCounter = 0;

// Hotel class
class Hotel {
public:
    vector<Room*> rooms;
    vector<Customer> customers;

    Hotel() {
        // Initialize rooms (5 Standard and 5 Deluxe)
        for (int i = 0; i < 5; ++i) {
            rooms.push_back(new Room(i, 10000.0));
        }
        for (int i = 5; i < 10; ++i) {
            rooms.push_back(new DeluxeRoom(i));
        }
    }

    void displayAllRooms() {
        cout << "Room details:\n";
        for (const auto& room : rooms) {
            room->displayRoomInfo();
        }
    }

    void bookRoom(const string& customerName, int roomType, int nights, string phone, int numGuests,
                  string aadhar, string addr, int roomsNeeded, vector<string> guestNames) {
        try {
            if (roomType != 0 && roomType != 1) {
                throw out_of_range("Invalid room type. Please select either 0 for Standard or 1 for Deluxe.");
            }

            double totalBill = 0;
            int roomsAvailable = 0;

            // Check room availability
            for (int i = 0; i < rooms.size(); ++i) {
                if ((roomType == 0 && i < 5 && rooms[i]->isAvailable) ||
                    (roomType == 1 && i >= 5 && rooms[i]->isAvailable)) {
                    roomsAvailable++;
                }
            }

            if (roomsAvailable < roomsNeeded) {
                cout << "Not enough rooms available! Only " << roomsAvailable << " rooms are available.\n";
                return;
            }

            // Book the rooms and calculate the total bill
            int roomsBooked = 0;
            for (auto& room : rooms) {
                if (room->isAvailable && roomsBooked < roomsNeeded) {
                    totalBill += room->calculateBill(nights, roomsNeeded); // Calculate total bill for rooms and nights
                    room->isAvailable = false;
                    roomsBooked++;
                }
            }

            bool existingCustomer = false;
            for (auto& customer : customers) {
                if (customer.name == customerName) {
                    customer.addPoints(nights * 10);
                    customer.setBookingDate();
                    existingCustomer = true;
                    break;
                }
            }

            if (!existingCustomer) {
                Customer newCustomer(customerName, phone, numGuests, aadhar, addr);
                newCustomer.addPoints(nights * 10);
                newCustomer.setBookingDate();
                newCustomer.guestNames = guestNames;
                customers.push_back(newCustomer);

                cout << "Customer ID assigned: " << newCustomer.id << endl;
            }

            cout << "Room(s) booked successfully for " << customerName << ". Total bill: ₹" << totalBill << endl;
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void displayAllCustomers() const {
        cout << "Customer details:\n";
        for (const auto& customer : customers) {
            customer.displayCustomerInfo();
        }
    }

    void checkCustomerOffersById(int customerId) {
        for (auto& customer : customers) {
            if (customer.id == customerId) {
                customer.checkOffers();
                return;
            }
        }
        cout << "Customer not found!\n";
    }
};

int main() {
    Hotel hotel;
    char choice;

    do {
        cout << "\n1. Display all rooms\n";
        cout << "2. Book a room\n";
        cout << "3. Display all customers\n";
        cout << "4. Check offers for a customer by ID\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        string customerName, phone, aadhar, address;
        int roomType, nights, numGuests, roomsNeeded;
        vector<string> guestNames;

        switch (choice) {
            case '1':
                hotel.displayAllRooms();
                break;
            case '2':
                cout << "Enter customer name: ";
                cin.ignore();
                getline(cin, customerName);

                cout << "Enter room type (0 for Standard, 1 for Deluxe): ";
                cin >> roomType;

                cout << "Enter number of guests: ";
                cin >> numGuests;

                if (numGuests > 2) {
                    cout << "Enter number of rooms you want to book: ";
                    cin >> roomsNeeded;
                } else {
                    roomsNeeded = 1;
                }

                cout << "Enter number of nights: ";
                cin >> nights;

                cout << "Enter phone number: ";
                cin >> phone;

                cout << "Enter Aadhar card number: ";
                cin >> aadhar;

                cout << "Enter address: ";
                cin.ignore();
                getline(cin, address);

                // Get guest names
                for (int i = 0; i < numGuests; ++i) {
                    string guestName;
                    cout << "Enter name of guest " << (i + 1) << ": ";
                    getline(cin, guestName);
                    guestNames.push_back(guestName);
                }

                hotel.bookRoom(customerName, roomType, nights, phone, numGuests, aadhar, address, roomsNeeded, guestNames);
                break;
            case '3':
                hotel.displayAllCustomers();
                break;
            case '4':
                int customerId;
                cout << "Enter customer ID to check offers: ";
                cin >> customerId;
                hotel.checkCustomerOffersById(customerId);
                break;
            case '5':
                cout << "Exiting the system.\n";
                break;
            default:
                cout << "Invalid choice. Please select a valid option.\n";
                break;
        }
    } while (choice != '5');

    return 0;
}