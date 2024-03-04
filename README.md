# RouterSimulation

![GUI](https://github.com/Mmahtab10/RouterSimulation/blob/main/Screen%20Recording%202024-02-24%20at%206.gif)

### Overview

#### Router Queue Simulation

This program, written in C++, simulates the packet processing within a router queue. It takes into account various parameters such as buffer size and WLAN capacity. The simulation has been rigorously tested using `zoom.txt` and `stream.txt` trace files. Compatibility checks were conducted on both a personal Mac computer and Linux CPSC servers.

#### Adjustable Parameters

Within the GUI window, you can modify `bufferSize` and `wlanCapacity` to simulate different network scenarios.

### Sample Output

```
Incoming Packets: 712841
Delivered Packets: 712841
Dropped Packets: 0
Packet Loss Percentage: 0%
Average Queuing Delay: 0.00387264 seconds
```

### File Structure

- MySimulation
    MySimulation.pro
    MySimulation.pro.user
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    simulation.cpp
    simulation.h
  
- trace_files_testing
    stream.txt
    zoom.txt
  
- data_analysis.pdf
  
### Dependencies

- C++11 or higher
- Qt Creator installed
- Libraries:
  - `<iostream>`
  - `<fstream>`
  - `<queue>`
  - `<vector>`
  - Qt-related libraries: can be found in main.cpp 

### Assumptions

- The `bufferSize` and `wlanCapacity` should be adjusted according to specific network configurations and requirements.
- Input files must follow the prescribed format, containing timestamp and packet size data.
- Extremely large input files should be avoided to prevent simulation performance issues.


