// simulation.cpp

#include "simulation.h"
#include <fstream>
#include <queue>
#include <algorithm> // For std::max

RouterSimulation::RouterSimulation() {}

std::vector<Packet> RouterSimulation::parseInputFile(const std::string& fileName) {
    std::vector<Packet> packets;
    std::ifstream traceFile(fileName);
    double timestamp;
    int packetSize;

    while (traceFile >> timestamp >> packetSize) {
        packets.push_back({timestamp, 0.0, packetSize});
    }

    traceFile.close();
    return packets;
}

SimulationResults RouterSimulation::runSimulation(const std::string& fileName, int bufferSize, double wlanCapacity) {
    auto packets = parseInputFile(fileName);
    double currentTime = 0.0;
    double totalQueuingDelay = 0.0;
    size_t packetsDropped = 0;
    std::queue<Packet> routerQ;

    for (const auto& packet : packets) {
        while (!routerQ.empty() && routerQ.front().departureTime <= packet.arrivalTime) {
            totalQueuingDelay += routerQ.front().departureTime - routerQ.front().arrivalTime;
            routerQ.pop();
        }

        if (routerQ.size() < static_cast<size_t>(bufferSize)) {
            double transmissionTime = static_cast<double>(packet.packetSize) * 8.0 / (wlanCapacity * 1e6);
            double departureTime = std::max(currentTime, packet.arrivalTime) + transmissionTime;
            currentTime = departureTime;
            routerQ.push({packet.arrivalTime, departureTime, packet.packetSize});
        } else {
            packetsDropped++;
        }
    }

    SimulationResults results;
    results.incomingPackets = packets.size();
    results.deliveredPackets = packets.size() - packetsDropped;
    results.droppedPackets = packetsDropped;
    results.packetLossPercentage = (static_cast<double>(packetsDropped) / packets.size()) * 100.0;
    results.averageQueuingDelay = totalQueuingDelay / (packets.size() - packetsDropped);
    results.totalQueuingDelay = totalQueuingDelay;

    return results;
}
