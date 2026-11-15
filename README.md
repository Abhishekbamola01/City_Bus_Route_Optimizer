
# City Bus Route Optimizer

A terminal-based C++ project for demonstrating bus-stop navigation and route optimization.

## Features

- Display all available bus stops.
- Find a route with the fewest connections using Breadth-First Search (BFS).
- Find the route with the lowest estimated travel time using Dijkstra's algorithm.
- Load bus and driver display records from a CSV file.
- Load route connections and sample travel times from a text file.
- Display bus registration-style numbers, colours, seating capacities, routes, and statuses.
- Print a Google Maps directions link for the selected route.
- Validate menu choices and stop inputs.

## Project Structure

```text
New folder/
├── src/
│   └── main.cpp
├── data/
│   ├── bus_data.csv
│   └── routes.txt
└── README.md
```

## Requirements

- A C++ compiler supporting C++17, such as g++
- Visual Studio Code
- The C/C++ extension for VS Code is recommended

## Compile

Open the terminal in the project root folder and run:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o CityBusRouteOptimizer.exe
```

## Run on Windows

```powershell
.\CityBusRouteOptimizer.exe
```

Keep the terminal's current directory set to the project root so the program can find the `data` folder.

## Run on Linux or macOS

Compile:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o CityBusRouteOptimizer
```

Run:

```bash
./CityBusRouteOptimizer
```

## Data Files

### bus_data.csv

Stores sample bus records. The program reads the header first and then loads the remaining rows.

### routes.txt

Stores connections in this format:

```text
Stop A|Stop B|Travel time in minutes
```

The program treats each connection as bidirectional.

## Algorithms

### Breadth-First Search (BFS)

BFS finds a path with the fewest connections in an unweighted graph. The program then adds the sample travel times along that path for display.

### Dijkstra's Algorithm

Dijkstra's algorithm finds a route with the lowest sum of the travel-time weights stored in `routes.txt`.

## Limitations

- Bus records, driver names, registrations, connections, and times are sample data, not verified real-world records.
- Travel times are estimates, not live traffic predictions.
- Bus records are displayed from CSV; the current program does not match buses to individual graph edges or calculate actual bus schedules.
- The Google Maps link opens directions; it does not provide live route optimization inside the C++ program.

## Testing Checklist

1. Compile the program without errors.
2. Choose option 1 and verify that bus stops are listed.
3. Choose option 2 and try two different stops.
4. Choose option 3 and compare the estimated travel time.
5. Choose options 4 and 5 to verify that CSV records load.
6. Try an invalid menu option.
7. Try entering an invalid stop name.
8. Choose option 6 to exit.
