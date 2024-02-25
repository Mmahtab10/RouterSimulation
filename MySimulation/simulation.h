// Guard to prevent multiple inclusions of this header file
#ifndef SIMULATION_H
#define SIMULATION_H

// Include directives for required standard library headers
#include <vector> // Needed for std::vector
#include <string> // Needed for std::string

// Definition of Packet struct for storing packet information
struct Packet {
    double arrivalTime; // Time when the packet arrives
    double departureTime; // Time when the packet departs
    int packetSize; // Size of the packet in bytes
};

// Definition of SimulationResults struct for storing the results of the simulation
struct SimulationResults {
    size_t incomingPackets; // Total number of packets that have arrived
    size_t deliveredPackets; // Number of packets that were successfully delivered
    size_t droppedPackets; // Number of packets that were dropped due to buffer overflow
    double packetLossPercentage; // Percentage of incoming packets that were dropped
    double averageQueuingDelay; // Average time packets spent in the queue
    double totalQueuingDelay; // Total time all packets spent in the queue
};

// Declaration of the RouterSimulation class
class RouterSimulation {
public:
    RouterSimulation(); // Constructor declaration
    // Function to run the simulation with provided parameters and return the results
    SimulationResults runSimulation(const std::string& fileName, int bufferSize, double wlanCapacity);

private:
    // Helper function to parse packet data from a file and return a vector of Packet structs
    std::vector<Packet> parseInputFile(const std::string& fileName);
};

#endif // SIMULATION_H