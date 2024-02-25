// simulation.h

#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <string>

struct Packet {
    double arrivalTime;
    double departureTime;
    int packetSize;
};

struct SimulationResults {
    size_t incomingPackets;
    size_t deliveredPackets;
    size_t droppedPackets;
    double packetLossPercentage;
    double averageQueuingDelay;
    double totalQueuingDelay;
};

class RouterSimulation {
public:
    RouterSimulation();
    SimulationResults runSimulation(const std::string& fileName, int bufferSize, double wlanCapacity);

private:
    std::vector<Packet> parseInputFile(const std::string& fileName);
};

#endif // SIMULATION_H
