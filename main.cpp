#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

class UserType {
public:
    string name;
    string address;
    string emailID;
    string password;
    string DOB;

    UserType() {
        name = "Unknown";
        address = "Unknown";
        emailID = "Unknown";
        password = "Unknown";
        DOB = "Unknown";
    }

    UserType(string n, string a, string e, string p, string d) {
        name = n;
        address = a;
        emailID = e;
        password = p;
        DOB = d;
    }

    bool operator<(const UserType& other) const {
        return emailID < other.emailID;
    }

    bool operator==(const UserType& other) const {
        return emailID == other.emailID;
    }

    bool operator!=(const UserType& other) const {
        return !(*this == other);
    }
};

// Overload << operator for UserType to enable printing
ostream& operator<<(ostream& os, const UserType& user) {
    os << user.name;
    return os;
}

template<typename T>
class Graph {
    map<T, list<T>> adjList;

public:
    void addEdge(T u, T v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    void showFriends(T user) {
        if (adjList.find(user) == adjList.end()) {
            cout << "User not found in the graph." << endl;
            return;
        }

        cout << "Friends of " << user << " are: ";
        for (T friendUser : adjList[user]) {
            cout << friendUser << ", ";
        }
        cout << endl;
    }

    void showFriendsOfFriends(T user) {
        if (adjList.find(user) == adjList.end()) {
            cout << "User not found in the graph." << endl;
            return;
        }

        cout << "Friends of friends of " << user << " are: ";
        set<T> friendsOfFriends;
        for (T friendUser : adjList[user]) {
            for (T friendOfFriend : adjList[friendUser]) {
                if (friendOfFriend != user && adjList[user].end() == find(adjList[user].begin(), adjList[user].end(), friendOfFriend)) {
                    friendsOfFriends.insert(friendOfFriend);
                }
            }
        }

        for (T fof : friendsOfFriends) {
            cout << fof << ", ";
        }
        cout << endl;
    }
};

class SocialMediaNetwork {
    map<string, UserType> users;
    Graph<UserType> friendships;
    UserType* loggedInUser = nullptr;
    const string userFileName = "users.txt";

public:
    SocialMediaNetwork() {
        loadUsers();
    }

    ~SocialMediaNetwork() {
        saveUsers();
    }

    void registerUser() {
        string name, address, email, password, dob;
        cin.ignore(); // Clear any previous input
        cout << "Enter name: ";
        getline(cin, name);
        cout << "Enter address: ";
        getline(cin, address);
        cout << "Enter email: ";
        getline(cin, email);
        cout << "Enter password: ";
        getline(cin, password);
        cout << "Enter DOB (YYYY-MM-DD): ";
        getline(cin, dob);

        if (users.find(email) != users.end()) {
            cout << "Email already registered!" << endl;
            return;
        }

        UserType newUser(name, address, email, password, dob);
        users[email] = newUser;
        cout << "Registration successful!" << endl;
        saveUsers();
    }

    void loginUser() {
        string email, password;
        cin.ignore(); // Clear any previous input
        cout << "Enter email: ";
        getline(cin, email);
        cout << "Enter password: ";
        getline(cin, password);

        if (users.find(email) != users.end() && users[email].password == password) {
            loggedInUser = &users[email];
            cout << "Login successful! Welcome, " << loggedInUser->name << endl;
        } else {
            cout << "Invalid email or password!" << endl;
        }
    }

    void addFriend() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        string friendEmail;
        cin.ignore(); // Clear any previous input
        cout << "Enter friend's email: ";
        getline(cin, friendEmail);

        if (users.find(friendEmail) != users.end()) {
            UserType& friendUser = users[friendEmail];
            friendships.addEdge(*loggedInUser, friendUser);
            cout << "Friend added successfully!" << endl;
        } else {
            cout << "User not found!" << endl;
        }
    }

    void showMyFriends() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        friendships.showFriends(*loggedInUser);
    }

    void showMyFriendsOfFriends() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        friendships.showFriendsOfFriends(*loggedInUser);
    }

private:
    void saveUsers() {
        ofstream outFile(userFileName);
        for (const auto& [email, user] : users) {
            outFile << user.name << '\n'
                    << user.address << '\n'
                    << user.emailID << '\n'
                    << user.password << '\n'
                    << user.DOB << '\n';
        }
        outFile.close();
    }

    void loadUsers() {
        ifstream inFile(userFileName);
        if (!inFile) {
            cout << "No existing user data found. Starting fresh." << endl;
            return;
        }

        string name, address, email, password, dob;
        while (getline(inFile, name) && getline(inFile, address) &&
               getline(inFile, email) && getline(inFile, password) &&
               getline(inFile, dob)) {
            UserType user(name, address, email, password, dob);
            users[email] = user;
        }
        inFile.close();
    }
};

int main() {
    SocialMediaNetwork network;
    int choice;

    while (true) {
        cout << "1. Register\n2. Login\n3. Add Friend\n4. Show My Friends\n5. Show Friends of My Friends\n6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                network.registerUser();
                break;
            case 2:
                network.loginUser();
                break;
            case 3:
                network.addFriend();
                break;
            case 4:
                network.showMyFriends();
                break;
            case 5:
                network.showMyFriendsOfFriends();
                break;
            case 6:
                return 0;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    }

    return 0;
}
