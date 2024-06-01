#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <stack>
#include <queue>  // For BFS

using namespace std;

class UserType {
public:
    string name;
    string address;
    string emailID;
    string password;
    string DOB;
    stack<string> myLifeEvents;

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

    void addLifeEvent(const string& event) {
        myLifeEvents.push(event);
    }

    void deleteLifeEvent() {
        if (!myLifeEvents.empty()) {
            myLifeEvents.pop();
        } else {
            cout << "No life events to delete." << endl;
        }
    }

    string getRecentLifeEvent() const {
        if (!myLifeEvents.empty()) {
            return myLifeEvents.top();
        } else {
            return "No life events available.";
        }
    }

    void showAllLifeEvents() const {
        if (myLifeEvents.empty()) {
            cout << "No life events available." << endl;
            return;
        }

        stack<string> tempStack = myLifeEvents;
        vector<string> events;

        while (!tempStack.empty()) {
            events.push_back(tempStack.top());
            tempStack.pop();
        }

        reverse(events.begin(), events.end());
        cout << "Life Events: " << endl;
        for (const string& event : events) {
            cout << event << endl;
        }
    }

    void saveLifeEvents(const string& fileName) const {
        ofstream outFile(fileName);
        stack<string> lifeEvents = myLifeEvents;
        vector<string> reversedEvents;

        while (!lifeEvents.empty()) {
            reversedEvents.push_back(lifeEvents.top());
            lifeEvents.pop();
        }
        reverse(reversedEvents.begin(), reversedEvents.end());
        for (const string& event : reversedEvents) {
            outFile << event << '\n';
        }
        outFile.close();
    }

    void loadLifeEvents(const string& fileName) {
        ifstream inFile(fileName);
        if (!inFile) {
            return;
        }

        string event;
        while (getline(inFile, event)) {
            myLifeEvents.push(event);
        }
        inFile.close();
    }
};

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

    void bfs(T start) {
        if (adjList.find(start) == adjList.end()) {
            cout << "User not found in the graph." << endl;
            return;
        }

        set<T> visited;
        queue<T> q;
        q.push(start);
        visited.insert(start);

        cout << "BFS Traversal starting from " << start << ": ";
        while (!q.empty()) {
            T node = q.front();
            q.pop();
            cout << node << " ";

            for (T neighbor : adjList[node]) {
                if (visited.find(neighbor) == visited.end()) {
                    q.push(neighbor);
                    visited.insert(neighbor);
                }
            }
        }
        cout << endl;
    }

    void dfsUtil(T node, set<T>& visited) {
        visited.insert(node);
        cout << node << " ";

        for (T neighbor : adjList[node]) {
            if (visited.find(neighbor) == visited.end()) {
                dfsUtil(neighbor, visited);
            }
        }
    }

    void dfs(T start) {
        if (adjList.find(start) == adjList.end()) {
            cout << "User not found in the graph." << endl;
            return;
        }

        set<T> visited;
        cout << "DFS Traversal starting from " << start << ": ";
        dfsUtil(start, visited);
        cout << endl;
    }

    void saveAdjList(const string& fileName) {
        ofstream outFile(fileName);
        for (const auto& pair : adjList) {
            outFile << pair.first.emailID << ":";
            for (const auto& friendUser : pair.second) {
                outFile << friendUser.emailID << ",";
            }
            outFile << endl;
        }
        outFile.close();
    }

    void loadAdjList(const string& fileName, const map<string, UserType>& users) {
        ifstream inFile(fileName);
        if (!inFile) {
            cout << "No existing friend data found. Starting fresh." << endl;
            return;
        }

        string line;
        while (getline(inFile, line)) {
            size_t pos = line.find(':');
            if (pos == string::npos) continue;

            string userEmail = line.substr(0, pos);
            string friendsStr = line.substr(pos + 1);

            if (users.find(userEmail) != users.end()) {
                UserType user = users.at(userEmail);
                size_t start = 0;
                size_t end = friendsStr.find(',');

                while (end != string::npos) {
                    string friendEmail = friendsStr.substr(start, end - start);
                    if (users.find(friendEmail) != users.end()) {
                        UserType friendUser = users.at(friendEmail);
                        adjList[user].push_back(friendUser);
                    }
                    start = end + 1;
                    end = friendsStr.find(',', start);
                }
            }
        }
        inFile.close();
    }
};

class SocialMediaNetwork {
    map<string, UserType> users;
    Graph<UserType> friendships;
    UserType* loggedInUser = nullptr;
    const string userFileName = "users.txt";
    const string friendsFileName = "friends.txt";
    const string lifeEventsFileName = "life_events_";

public:
    SocialMediaNetwork() {
        loadUsers();
        friendships.loadAdjList(friendsFileName, users);
    }

    ~SocialMediaNetwork() {
        saveUsers();
        friendships.saveAdjList(friendsFileName);
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

    bool loginUser(bool& loggedIn) {
        string email, password;
        cin.ignore(); // Clear any previous input
        cout << "Enter email: ";
        getline(cin, email);
        cout << "Enter password: ";
        getline(cin, password);

        if (users.find(email) != users.end() && users[email].password == password) {
            loggedInUser = &users[email];
            cout << "Login successful! Welcome, " << loggedInUser->name << endl;
            loggedIn = true;
            loggedInUser->loadLifeEvents(lifeEventsFileName + loggedInUser->emailID + ".txt");
        } else {
            cout << "Invalid email or password!" << endl;
            loggedIn = false;
        }

        return loggedIn;
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

    void addLifeEvent() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        string event;
        cin.ignore(); // Clear any previous input
        cout << "Enter life event: ";
        getline(cin, event);
        loggedInUser->addLifeEvent(event);
        loggedInUser->saveLifeEvents(lifeEventsFileName + loggedInUser->emailID + ".txt");
        cout << "Life event added successfully!" << endl;
    }

    void deleteLifeEvent() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        loggedInUser->deleteLifeEvent();
        loggedInUser->saveLifeEvents(lifeEventsFileName + loggedInUser->emailID + ".txt");
    }

    void showMyLifeEvents() const {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        loggedInUser->showAllLifeEvents();
    }

    void traverseFriendsBFS() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }
        friendships.bfs(*loggedInUser);
    }

    void traverseFriendsDFS() {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }
        friendships.dfs(*loggedInUser);
    }

    void saveUsers() const {
        ofstream outFile(userFileName);
        for (const auto& pair : users) {
            const UserType& user = pair.second;
            outFile << user.name << '\n' << user.address << '\n' << user.emailID << '\n' << user.password << '\n' << user.DOB << '\n';
        }
        outFile.close();
    }

    void loadUsers() {
        ifstream inFile(userFileName);
        if (!inFile) {
            return;
        }

        string name, address, email, password, dob;
        while (getline(inFile, name)) {
            getline(inFile, address);
            getline(inFile, email);
            getline(inFile, password);
            getline(inFile, dob);
            users[email] = UserType(name, address, email, password, dob);
        }
        inFile.close();
    }

    void getRecentLifeEvent() const {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        cout << loggedInUser->getRecentLifeEvent() << endl;
    }

    void showAllLifeEvents() const {
        if (!loggedInUser) {
            cout << "You need to log in first!" << endl;
            return;
        }

        loggedInUser->showAllLifeEvents();
    }

    void showUserLifeEvents() {
    if (!loggedInUser) {
        cout << "You need to log in first!" << endl;
        return;
    }

    string email;
    cout << "Enter the email of the user: ";
    cin >> email;

    if (users.find(email) != users.end()) {
        UserType& user = users.at(email);

        //loading the user's life events from file by file name
        string lifeEventsFileName = "life_events_" + email + ".txt";
        user.loadLifeEvents(lifeEventsFileName);

        user.showAllLifeEvents();
    } else {
        cout << "User not found!" << endl;
    }
}
};





int main() {
    SocialMediaNetwork smn;
    bool isLoggedIn = false;

    while (true) {
        int choice;
        if (!isLoggedIn) {
            cout << "\t****************************************************" << endl;
            cout << "\t\t\t1. Register\n\t\t\t2. Login\n\t\t\t3. Exit" << endl;
            cout << "\t****************************************************" << endl;
            cout << "\nChoose an option: ";
        } else {
            cout << "\t***************************************************************" << endl;
            cout << "\t\t\t1. Add Friend\n\t\t\t2. Show Friends\n\t\t\t3. Show Friends of Friends\n\t\t\t4. Add Life Event\n\t\t\t5. Delete Life Event\n\t\t\t6. Show Life Events\n\t\t\t7. BFS Friends Traversal\n\t\t\t8. DFS Friends Traversal\n\t\t\t9. Logout" << endl ;
            cout << "\t***************************************************************" << endl;
            cout << "\nChoose an option: ";
        }
        cin >> choice;

        if (!isLoggedIn) {
            switch (choice) {
            case 1:
                smn.registerUser();
                break;
            case 2:
                smn.loginUser(isLoggedIn);
                break;
            case 3:
                return 0;
            default:
                cout << "Invalid option! Please try again." << endl;
            }
        } else {
            switch (choice) {
            case 1:
                smn.addFriend();
                break;
            case 2:
                smn.showMyFriends();
                break;
            case 3:
                smn.showMyFriendsOfFriends();
                break;
            case 4:
                smn.addLifeEvent();
                break;
            case 5:
                smn.deleteLifeEvent();
                break;
            //case 6:
            //    smn.showMyLifeEvents();
           //     break;

            case 6:
                int leChoice;
                do {
                    cout << "\t**************************************************" << endl;
                    cout << "\t\tWould you like to: \n\t\t1. See your most recent life event \n\t\t2. See all of your life events \n\t\t3. Show another user's life events \n\t\t4. Return to Main Menu\n";
                    cout << "\t**************************************************" << endl;
                    cout << "Enter Your Choice: "<< endl;
                    cin >> leChoice;
                    switch (leChoice) {
                    case 1:
                        cout << "Your Most Recent Life Event: " << endl;
                        smn.getRecentLifeEvent();
                        break;
                    case 2:
                        cout << "Your ";
                        smn.showAllLifeEvents();
                        break;
                    case 3:
                        smn.showUserLifeEvents();
                        break;
                    case 4:
                        cout << "Going Back to Main Menu: " << endl;
                        break;
                    default:
                        cout << "Invalid option! Please try again." << endl;
                    }
                } while (leChoice != 4);
                break;

            case 7:
                smn.traverseFriendsBFS();
                break;
            case 8:
                smn.traverseFriendsDFS();
                break;
            case 9:
                isLoggedIn = false;
                break;

            default:
                cout << "Invalid option! Please try again." << endl;
            }
        }
    }

    return 0;
}
