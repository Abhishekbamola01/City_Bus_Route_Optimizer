#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>

using namespace std;

// Stores the details of one driver.
class Driver {
private:
    string id, name, phone, licence;

public:
    Driver() {
        id = "";
        name = "";
        phone = "";
        licence = "";
    }

    Driver(string driverId, string driverName, string contact, string license) {
        id = driverId;
        name = driverName;
        phone = contact;
        licence = license;
    }

    string getId() const {
        return id;
    }

    void display() const {
        cout << "Driver ID   : " << id << '\n';
        cout << "Driver Name : " << name << '\n';
        cout << "Contact     : " << phone << '\n';
        cout << "Licence     : " << licence << '\n';
    }
};

// Stores one bus and its route.
class Bus {
private:
    string id, registration, colour;
    int capacity, departure, minutesPerStop;
    vector<string> stops;
    Driver driver;

public:
    Bus(string busId, string reg, string busColour, int seats,
        int depart, int segmentTime, vector<string> route, Driver busDriver) {
        id = busId;
        registration = reg;
        colour = busColour;
        capacity = seats;
        departure = depart;
        minutesPerStop = segmentTime;
        stops = route;
        driver = busDriver;
    }

    string getId() const {
        return id;
    }

    string getColour() const {
        return colour;
    }

    string getRegistration() const {
        return registration;
    }

    int getCapacity() const {
        return capacity;
    }

    int getDeparture() const {
        return departure;
    }

    int getMinutesPerStop() const {
        return minutesPerStop;
    }

    string getDriverId() const {
        return driver.getId();
    }

    vector<string> getStops() const {
        return stops;
    }

    void displayBusDetails() const {
        cout << "Bus number       : " << id << '\n';
        cout << "Registration     : " << registration << '\n';
        cout << "Colour           : " << colour << '\n';
        cout << "Seating capacity : " << capacity << '\n';
        cout << "Scheduled time   : " << departure / 60 << ":";
        cout << setfill('0') << setw(2) << departure % 60 << setfill(' ') << '\n';
        cout << "Driver ID        : " << driver.getId() << '\n';
        cout << "Complete route   :\n";
        for (int i = 0; i < (int)stops.size(); i++) {
            cout << "  " << i + 1 << ". " << stops[i] << '\n';
        }
    }

    void displayDriverDetails() const {
        driver.display();
    }

    int findStop(string name) const {
        for (int i = 0; i < (int)stops.size(); i++) {
            if (sameText(stops[i], name)) return i;
        }
        return -1;
    }

    static bool sameText(string a, string b) {
        if (a.size() != b.size()) return false;
        for (int i = 0; i < (int)a.size(); i++) {
            if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i]))
                return false;
        }
        return true;
    }

    void displayDetails() const {
        cout << "\nBus ID       : " << id << '\n';
        cout << "Registration : " << registration << '\n';
        cout << "Colour       : " << colour << '\n';
        cout << "Capacity     : " << capacity << '\n';
        driver.display();

        cout << "Complete route:\n";
        for (int i = 0; i < (int)stops.size(); i++)
            cout << "  " << i + 1 << ". " << stops[i] << '\n';
    }
};

// Each graph edge means a bus can travel from one stop to the next.
struct Edge {
    int from;
    int to;
    int busIndex;
    int minutes;
};

// Stores the route found by BFS or Dijkstra.
struct RouteResult {
    string method;
    vector<int> edgePath;
    int startTime = 0;
    int arrivalTime = 0;
    int duration = 0;
    int changes = 0;
    bool found = false;
};

class RouteOptimizer {
private:
    vector<Bus> buses;
    vector<string> stops;
    vector<Edge> edges;
    vector<vector<int>> graph;
    int transferWait = 5;

    int findStopIndex(string name) const {
        for (int i = 0; i < (int)stops.size(); i++) {
            if (Bus::sameText(stops[i], name)) return i;
        }
        return -1;
    }

    // Adds the original sample bus records and creates the graph.
    void addBusData() {
        stops = {
            "Graphic Era Hill University", "GEU Gate", "Prem Nagar",
            "Clement Town", "Ballupur Chowk", "Clock Tower",
            "Rajpur Road", "Parade Ground", "ISBT Dehradun",
            "Dilaram Chowk"
        };

        buses.clear();
        buses.push_back(Bus("DUN-07", "UK07 AK 8798", "Blue", 40, 480, 8,
            {"Graphic Era Hill University", "GEU Gate", "Clement Town",
             "Ballupur Chowk", "Clock Tower", "Parade Ground", "ISBT Dehradun"},
            Driver("D001", "Aarav Sharma", "Sample contact", "Sample licence")));

        buses.push_back(Bus("DUN-14", "UK07 AK 4621", "White", 35, 490, 5,
            {"Graphic Era Hill University", "GEU Gate", "Clement Town", "Clock Tower"},
            Driver("D002", "Priya Rawat", "Sample contact", "Sample licence")));

        buses.push_back(Bus("DUN-21", "UK07 AK 7354", "Green", 40, 510, 4,
            {"Clock Tower", "Rajpur Road", "Parade Ground", "ISBT Dehradun"},
            Driver("D003", "Rohit Negi", "Sample contact", "Sample licence")));

        buses.push_back(Bus("DUN-09", "UK07 AK 2186", "Red", 32, 500, 7,
            {"Graphic Era Hill University", "Prem Nagar", "Ballupur Chowk",
             "Clock Tower", "Dilaram Chowk", "Rajpur Road", "ISBT Dehradun"},
            Driver("D004", "Neha Bisht", "Sample contact", "Sample licence")));

        buses.push_back(Bus("DUN-12", "UK07 AK 5812", "Orange", 35, 510, 6,
            {"Graphic Era Hill University", "GEU Gate", "Clement Town",
             "Clock Tower", "Parade Ground", "ISBT Dehradun"},
            Driver("D005", "Ananya Rawat", "Sample contact", "Sample licence")));

        buses.push_back(Bus("DUN-18", "UK07 AK 9043", "Yellow", 30, 500, 6,
            {"Prem Nagar", "Ballupur Chowk", "Clock Tower", "Parade Ground",
             "ISBT Dehradun"},
            Driver("D006", "Vikram Singh", "Sample contact", "Sample licence")));

        edges.clear();
        graph.clear();
        graph.resize(stops.size());

        for (int b = 0; b < (int)buses.size(); b++) {
            vector<string> route = buses[b].getStops();
            for (int i = 0; i + 1 < (int)route.size(); i++) {
                int from = findStopIndex(route[i]);
                int to = findStopIndex(route[i + 1]);
                if (from != -1 && to != -1) {
                    Edge e = {from, to, b, buses[b].getMinutesPerStop()};
                    edges.push_back(e);
                    graph[from].push_back((int)edges.size() - 1);
                }
            }
        }
    }

    // Simulated traffic: estimated travel time rises during rush hours.
    int trafficTime(int minutes, int time) const {
        int hour = (time / 60) % 24;
        if ((hour >= 8 && hour < 10) || (hour >= 17 && hour < 20))
            return minutes + minutes / 2;
        return minutes;
    }

    int countChanges(const vector<int>& path) const {
        int changes = 0;
        int previousBus = -1;
        for (int i = 0; i < (int)path.size(); i++) {
            int currentBus = edges[path[i]].busIndex;
            if (previousBus != -1 && currentBus != previousBus) changes++;
            previousBus = currentBus;
        }
        return changes;
    }

    RouteResult buildResult(string method, vector<int> path, int requestedTime) const {
        RouteResult result;
        result.method = method;
        result.edgePath = path;
        result.startTime = requestedTime;
        result.found = !path.empty();

        if (!result.found) return result;

        int currentTime = requestedTime;
        int previousBus = -1;
        for (int i = 0; i < (int)path.size(); i++) {
            Edge e = edges[path[i]];

            if (previousBus == -1) {
                currentTime = max(currentTime, buses[e.busIndex].getDeparture());
            } else if (e.busIndex != previousBus) {
                currentTime = max(currentTime + transferWait,
                                  buses[e.busIndex].getDeparture());
            }

            currentTime += trafficTime(e.minutes, currentTime);
            previousBus = e.busIndex;
        }

        result.startTime = requestedTime;
        result.arrivalTime = currentTime;
        result.duration = currentTime - requestedTime;
        result.changes = countChanges(path);
        return result;
    }

public:
    RouteOptimizer() { addBusData(); }

    int stopCount() const { return (int)stops.size(); }
    string stopName(int index) const { return stops[index]; }

    void displayStops() const {
        cout << "\nAVAILABLE BUS STOPS\n";
        for (int i = 0; i < (int)stops.size(); i++)
            cout << i + 1 << ". " << stops[i] << '\n';
    }

    const Bus& getBus(int index) const { return buses[index]; }
    const Edge& getEdge(int index) const { return edges[index]; }

    int busCount() const {
        return (int)buses.size();
    }

    // Shows buses whose  departure time is at or after the requested time.
    void displayAvailableBuses(string source, int requestedTime) const {
        cout << "\n============================================\n";
        cout << "       BUSES AVAILABLE AT YOUR TIME\n";
        cout << "============================================\n";

        int count = 0;
        for (int i = 0; i < (int)buses.size(); i++) {
            int sourcePosition = buses[i].findStop(source);
            if (sourcePosition >= 0 && buses[i].getDeparture() >= requestedTime) {
                count++;
                cout << "\nBUS OPTION " << count << "\n";
                cout << "--------------------------------------------\n";
                buses[i].displayBusDetails();
            }
        }

        if (count == 0) {
            cout << "No sample bus serving your starting stop is scheduled at or after your requested time.\n";
        }
        cout << "\nTotal available sample buses: " << count << '\n';
    }

    void displayDriverDetailsForAvailableBuses(string source, int requestedTime) const {
        int count = 0;
        for (int i = 0; i < (int)buses.size(); i++) {
            if (buses[i].findStop(source) >= 0 && buses[i].getDeparture() >= requestedTime) {
                count++;
                cout << "\nDRIVER DETAILS - BUS " << buses[i].getId() << "\n";
                cout << "--------------------------------------------\n";
                buses[i].displayDriverDetails();
            }
        }
        if (count == 0) {
            cout << "No available bus driver details to display.\n";
        }
    }

    int estimatedTravelTime(int edgeIndex, int currentTime) const {
        return trafficTime(edges[edgeIndex].minutes, currentTime);
    }

    int scheduledDeparture(int busIndex) const {
        return buses[busIndex].getDeparture();
    }

    // BFS finds a route using the fewest stop-to-stop connections.
    RouteResult findRouteBFS(string source, string destination, int time) const {
        int start = findStopIndex(source);
        int finish = findStopIndex(destination);
        if (start == -1 || finish == -1) return RouteResult();

        vector<int> previousStop(stops.size(), -1);
        vector<int> previousEdge(stops.size(), -1);
        vector<int> visited(stops.size(), 0);
        queue<int> q;

        q.push(start);
        visited[start] = 1;

        while (!q.empty()) {
            int current = q.front();
            q.pop();
            if (current == finish) break;

            for (int i = 0; i < (int)graph[current].size(); i++) {
                int edgeIndex = graph[current][i];
                int next = edges[edgeIndex].to;

                if (!visited[next]) {
                    visited[next] = 1;
                    previousStop[next] = current;
                    previousEdge[next] = edgeIndex;
                    q.push(next);
                }
            }
        }

        if (!visited[finish]) return RouteResult();

        vector<int> path;
        int current = finish;
        while (current != start) {
            path.push_back(previousEdge[current]);
            current = previousStop[current];
        }
        reverse(path.begin(), path.end());
        return buildResult("BFS", path, time);
    }

    // Dijkstra finds the route with the lowest estimated travel-time cost.
    RouteResult findRouteDijkstra(string source, string destination, int time) const {
        int start = findStopIndex(source);
        int finish = findStopIndex(destination);
        if (start == -1 || finish == -1) return RouteResult();

        const int INF = 1000000000;
        vector<int> distance(stops.size(), INF);
        vector<int> previousStop(stops.size(), -1);
        vector<int> previousEdge(stops.size(), -1);
        vector<int> used(stops.size(), 0);

        distance[start] = 0;

        for (int step = 0; step < (int)stops.size(); step++) {
            int current = -1;
            for (int i = 0; i < (int)stops.size(); i++) {
                if (!used[i] && (current == -1 || distance[i] < distance[current]))
                    current = i;
            }

            if (current == -1 || distance[current] == INF) break;
            if (current == finish) break;
            used[current] = 1;

            for (int i = 0; i < (int)graph[current].size(); i++) {
                int edgeIndex = graph[current][i];
                Edge e = edges[edgeIndex];
                int cost = trafficTime(e.minutes, time + distance[current]);
                int newDistance = distance[current] + cost;

                if (newDistance < distance[e.to]) {
                    distance[e.to] = newDistance;
                    previousStop[e.to] = current;
                    previousEdge[e.to] = edgeIndex;
                }
            }
        }

        if (distance[finish] == INF) return RouteResult();

        vector<int> path;
        int current = finish;
        while (current != start) {
            if (previousEdge[current] == -1) return RouteResult();
            path.push_back(previousEdge[current]);
            current = previousStop[current];
        }
        reverse(path.begin(), path.end());
        return buildResult("Dijkstra", path, time);
    }
};

class TerminalUI {
private:
    RouteOptimizer optimizer;
    string source, destination;
    int preferredTime = 480;

    string timeText(int minutes) const {
        minutes %= 1440;
        if (minutes < 0) minutes += 1440;
        int hour = minutes / 60;
        int minute = minutes % 60;
        string suffix = (hour >= 12) ? "PM" : "AM";
        hour %= 12;
        if (hour == 0) hour = 12;

        ostringstream out;
        out << setfill('0') << setw(2) << hour << ":"
            << setw(2) << minute << " " << suffix;
        return out.str();
    }

    string encodeURL(string text) const {
        ostringstream out;
        out << uppercase << hex;
        for (int i = 0; i < (int)text.size(); i++) {
            unsigned char c = text[i];
            if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
                out << c;
            else
                out << '%' << setw(2) << setfill('0') << (int)c;
        }
        return out.str();
    }

    int readNumber(string prompt, int low, int high) const {
        int number;
        while (true) {
            cout << prompt;
            if (cin >> number && number >= low && number <= high) {
                cin.ignore(10000, '\n');
                return number;
            }
            cout << "Enter a number from " << low << " to " << high << ".\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    void inputScreen() {
        cout << "\n============================================\n";
        cout << "       CITY BUS ROUTE OPTIMIZER\n";
        cout << "============================================\n";
        optimizer.displayStops();

        int start, end;
        while (true) {
            start = readNumber("\nStarting stop number: ", 1, optimizer.stopCount());
            end = readNumber("Destination stop number: ", 1, optimizer.stopCount());
            if (start != end) break;
            cout << "Starting stop and destination must be different.\n";
        }

        source = optimizer.stopName(start - 1);
        destination = optimizer.stopName(end - 1);

        int hour = readNumber("Departure hour (0-23): ", 0, 23);
        int minute = readNumber("Departure minute (0-59): ", 0, 59);
        preferredTime = hour * 60 + minute;
    }

    void showComparison(const RouteResult& bfs, const RouteResult& dijkstra) const {
        cout << "\nROUTE COMPARISON\n";
        cout << left << setw(15) << "Algorithm"
             << setw(16) << "Arrival"
             << setw(16) << "Duration"
             << setw(12) << "Changes" << '\n';
        cout << string(59, '-') << '\n';

        if (bfs.found)
            cout << left << setw(15) << "BFS"
                 << setw(16) << timeText(bfs.arrivalTime)
                 << setw(16) << (to_string(bfs.duration) + " min")
                 << setw(12) << bfs.changes << '\n';
        else
            cout << left << setw(15) << "BFS" << "No route found\n";

        if (dijkstra.found)
            cout << left << setw(15) << "Dijkstra"
                 << setw(16) << timeText(dijkstra.arrivalTime)
                 << setw(16) << (to_string(dijkstra.duration) + " min")
                 << setw(12) << dijkstra.changes << '\n';
        else
            cout << left << setw(15) << "Dijkstra" << "No route found\n";
    }

    void showRoute(const RouteResult& result) const {
        cout << "\nSELECTED ROUTE: " << result.method << '\n';
        cout << "From       : " << source << '\n';
        cout << "To         : " << destination << '\n';
        cout << "Departure  : " << timeText(result.startTime) << '\n';
        cout << "Arrival    : " << timeText(result.arrivalTime) << '\n';
        cout << "Duration   : " << result.duration << " minutes\n";
        cout << "Bus changes: " << result.changes << '\n';

        cout << "\nSTOP-BY-STOP ROUTE\n";
        int currentTime = result.startTime;
        int previousBus = -1;
        cout << timeText(currentTime) << "  " << source << " [START]\n";

        for (int i = 0; i < (int)result.edgePath.size(); i++) {
            Edge e = optimizer.getEdge(result.edgePath[i]);
            const Bus& bus = optimizer.getBus(e.busIndex);

            if (previousBus == -1) {
                int busTime = optimizer.scheduledDeparture(e.busIndex);
                if (currentTime < busTime) {
                    cout << "           Wait for bus until "
                         << timeText(busTime) << '\n';
                    currentTime = busTime;
                }
            } else if (e.busIndex != previousBus) {
                int nextBusTime = optimizer.scheduledDeparture(e.busIndex);
                currentTime = max(currentTime + 5, nextBusTime);
                cout << "           Change bus at "
                     << optimizer.stopName(e.from) << " (wait for next bus)\n";
            }

            currentTime += optimizer.estimatedTravelTime(result.edgePath[i], currentTime);
            cout << timeText(currentTime) << "  "
                 << optimizer.stopName(e.to) << "  (Bus "
                 << bus.getId() << ", " << bus.getColour() << ")\n";
            previousBus = e.busIndex;
        }

    }
  // API CALLING FUNCTION TO SHOW GOOGLE MAPS LINK FOR THE SELECTED ROUTE
    void showGoogleMaps(const RouteResult& result) const {
        // Add every intermediate stop from our selected route as a waypoint.
        string waypoints = "";
        string lastStop = source;

        for (int i = 0; i < (int)result.edgePath.size(); i++) {
            Edge edge = optimizer.getEdge(result.edgePath[i]);
            string stop = optimizer.stopName(edge.to);

            // Do not add the final destination as a waypoint.
            if (!Bus::sameText(stop, destination) &&
                !Bus::sameText(stop, lastStop)) {
                if (waypoints != "") {
                    waypoints += "%7C";  // separates stops in the URL
                }
                waypoints += encodeURL(stop);
                lastStop = stop;
            }
        }

        string url = "https://www.google.com/maps/dir/?api=1&origin=" +
                     encodeURL(source) + "&destination=" +
                     encodeURL(destination);

        if (waypoints != "") {
            url += "&waypoints=" + waypoints;
        }

        url += "&travelmode=transit";

        cout << "\nGOOGLE MAPS ROUTE LINK\n";
        cout << "Stops included in our route: " << source;
        for (int i = 0; i < (int)result.edgePath.size(); i++) {
            Edge edge = optimizer.getEdge(result.edgePath[i]);
            cout << " -> " << optimizer.stopName(edge.to);
        }
        cout << "\n\nOpen this link to view the route in Google Maps:\n";
        cout << url << '\n';
        cout << "\nNote: Google Maps may adjust or ignore intermediate stops for transit directions.\n";
        cout << "The link shows the stops from our selected route, but it does not upload our sample bus timetable.\n";
    }

public:
    void run() {
        while (true) {
            inputScreen();

            RouteResult bfs = optimizer.findRouteBFS(source, destination, preferredTime);
            RouteResult dijkstra = optimizer.findRouteDijkstra(source, destination, preferredTime);

            showComparison(bfs, dijkstra);

            RouteResult selectedRoute;

            if (bfs.found && dijkstra.found) {
                cout << "\nChoose route to display:\n1. BFS (fewest stop connections)\n2. Dijkstra (lowest estimated travel time)\n";
                int choice = readNumber("Your choice: ", 1, 2);
                if (choice == 1) selectedRoute = bfs;
                else selectedRoute = dijkstra;
                showRoute(selectedRoute);
            } else if (bfs.found) {
                selectedRoute = bfs;
                showRoute(selectedRoute);
            } else if (dijkstra.found) {
                selectedRoute = dijkstra;
                showRoute(selectedRoute);
            } else {
                cout << "\nNo route found between these stops in the sample network.\n";
            }

            // Show every bus scheduled at or after the time entered by the user.
            optimizer.displayAvailableBuses(source, preferredTime);

            cout << "\nWould you like to see driver details for the available buses?\n";
            cout << "1. Yes\n";
            cout << "2. No\n";
            int driverChoice = readNumber("Choose: ", 1, 2);
            if (driverChoice == 1) {
                optimizer.displayDriverDetailsForAvailableBuses(source, preferredTime);
            }

            if (selectedRoute.found) {
                showGoogleMaps(selectedRoute);
            }
            cout << "\nNOTE: Bus, driver and timetable records are sample data, not verified live schedules.\n";

            int again = readNumber("\n1. Plan another route\n2. Exit\nChoose: ", 1, 2);
            if (again == 2) break;
        }

        cout << "\nThank you for using City Bus Route Optimizer!\n";
    }
};

int main() {
    TerminalUI app;
    app.run();
    return 0;
}
