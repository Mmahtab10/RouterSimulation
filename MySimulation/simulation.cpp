// Include necessary headers and declare use of standard namespace elements
#include "simulation.h"
#include <fstream>      // For file input operations
#include <queue>        // For using the queue data structure
#include <algorithm>    // For std::max function

// Default constructor definition for RouterSimulation class
RouterSimulation::RouterSimulation() {}

// Parses a trace file to extract packet information and returns a vector of Packet structs
std::vector<Packet> RouterSimulation::parseInputFile(const std::string& fileName) {
    std::vector<Packet> packets; // Vector to store parsed packets
    std::ifstream traceFile(fileName); // Open the trace file for reading
    double timestamp; // Variable to store packet arrival time
    int packetSize; // Variable to store packet size

    // Read packet information from file until end of file is reached
    while (traceFile >> timestamp >> packetSize) {
        // Add new packet to the vector with current timestamp and packetSize, setting departureTime to 0.0 initially
        packets.push_back({timestamp, 0.0, packetSize});
    }

    traceFile.close(); // Close the file stream
    return packets; // Return the populated vector of packets
}

// Runs the simulation with given parameters and returns the results as a SimulationResults struct
SimulationResults RouterSimulation::runSimulation(const std::string& fileName, int bufferSize, double wlanCapacity) {
    auto packets = parseInputFile(fileName); // Parse the input file to get packet data
    double currentTime = 0.0; // Tracks the current time in the simulation
    double totalQueuingDelay = 0.0; // Accumulates total queuing delay of all packets
    size_t packetsDropped = 0; // Counts the number of packets dropped
    std::queue<Packet> routerQ; // Queue to simulate the router's buffer

    // Process each packet in the order they are read
    for (const auto& packet : packets) {
        // Remove packets from the queue that have departed before the current packet's arrival
        while (!routerQ.empty() && routerQ.front().departureTime <= packet.arrivalTime) {
            totalQueuingDelay += routerQ.front().departureTime - routerQ.front().arrivalTime;
            routerQ.pop();
        }

        // Check if there's room in the buffer, else drop the packet
        if (routerQ.size() < static_cast<size_t>(bufferSize)) {
            // Calculate packet's transmission time and departure time
            double transmissionTime = static_cast<double>(packet.packetSize) * 8.0 / (wlanCapacity * 1e6);
            double departureTime = std::max(currentTime, packet.arrivalTime) + transmissionTime;
            currentTime = departureTime; // Update currentTime to reflect the latest packet's departure
            routerQ.push({packet.arrivalTime, departureTime, packet.packetSize}); // Add packet to the queue
        } else {
            packetsDropped++; // Increment drop count if buffer is full
        }
    }

    // Compile and return simulation results
    SimulationResults results;
    results.incomingPackets = packets.size(); // Total incoming packets
    results.deliveredPackets = packets.size() - packetsDropped; // Total delivered packets
    results.droppedPackets = packetsDropped; // Total dropped packets
    results.packetLossPercentage = (static_cast<double>(packetsDropped) / packets.size()) * 100.0; // Percentage of packets dropped
    results.averageQueuingDelay = totalQueuingDelay / (packets.size() - packetsDropped); // Average queuing delay
    results.totalQueuingDelay = totalQueuingDelay; // Total queuing delay

    return results; // Return the compiled results
}