#include <QApplication> // For the application itself
#include <QWidget>       // Base class for all UI objects
#include <QPushButton>   // For the buttons
#include <QVBoxLayout>   // For vertical box layout
#include <QHBoxLayout>   // For horizontal box layout
#include <QLabel>        // For the label displaying results
#include <QLineEdit>     // For input fields
#include <QFileDialog>   // For the file dialog to select a trace file
#include <QString>       // For strings, used in signal-slot connections
#include <QMessageBox>   // For showing warning messages
#include <QGroupBox>     // For grouping simulation parameters visually
#include <QFormLayout>   // For aligning labels and fields in a form-like manner
#include <QFontMetrics>

#include "simulation.h"

class RouterSimulatorApp : public QWidget {
    Q_OBJECT

public:
    RouterSimulatorApp(QWidget *parent = nullptr) : QWidget(parent) {
        auto *layout = new QVBoxLayout(this);

        // Group Box for Input Parameters
        QGroupBox *inputGroupBox = new QGroupBox("Simulation Parameters");
        QFormLayout *inputLayout = new QFormLayout; // Form layout for better alignment

        // Buffer Size Input
        bufferSizeInput = new QLineEdit(this);
        bufferSizeInput->setPlaceholderText("Buffer Size");
        bufferSizeInput->setToolTip("Enter the buffer size (number of packets)");
        inputLayout->addRow("Buffer Size:", bufferSizeInput);
        layout->addWidget(bufferSizeInput);
        bufferSizeInput->setMinimumWidth(200); // width in pixels
        bufferSizeInput->setStyleSheet("background-color: white;");

        // WLAN Capacity Input
        wlanCapacityInput = new QLineEdit(this);
        wlanCapacityInput->setPlaceholderText("WLAN Capacity (Mbps)");
        wlanCapacityInput->setToolTip("Enter the WLAN capacity in Mbps");
        inputLayout->addRow("WLAN Capacity:", wlanCapacityInput);
        wlanCapacityInput->setMinimumWidth(200); // width in pixels
        wlanCapacityInput->setStyleSheet("background-color: white;");

        // Select Trace File Button
        auto *selectFileButton = new QPushButton("Select Trace File", this);
        selectFileButton->setToolTip("Select the trace file containing packet data");
        selectFileButton->setStyleSheet("QPushButton { background-color: white; color: black; }"); // Styling
        connect(selectFileButton, &QPushButton::clicked, this, &RouterSimulatorApp::selectTraceFile);

        // Start Simulation Button
        auto *startButton = new QPushButton("Start Simulation", this);
        startButton->setStyleSheet("QPushButton { background-color: #5cb85c; color: white; }"); // Styling
        connect(startButton, &QPushButton::clicked, this, &RouterSimulatorApp::startSimulation);

        // Add buttons outside the form layout but inside the group box
        auto *buttonsLayout = new QHBoxLayout;
        buttonsLayout->addWidget(selectFileButton);
        buttonsLayout->addWidget(startButton);

        // Setup Group Box Layout
        auto *groupBoxLayout = new QVBoxLayout;
        groupBoxLayout->addLayout(inputLayout);
        groupBoxLayout->addLayout(buttonsLayout);
        inputGroupBox->setLayout(groupBoxLayout);

        layout->addWidget(inputGroupBox);

        // Create Labels for result output
        incomingPacketsLabel = new QLabel(this);
        deliveredPacketsLabel = new QLabel(this);
        droppedPacketsLabel = new QLabel(this);
        packetLossPercentageLabel = new QLabel(this);
        averageQueuingDelayLabel = new QLabel(this);

        layout->addWidget(incomingPacketsLabel);
        layout->addWidget(deliveredPacketsLabel);
        layout->addWidget(droppedPacketsLabel);
        layout->addWidget(packetLossPercentageLabel);
        layout->addWidget(averageQueuingDelayLabel);

        this->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");

        setLayout(layout); // Set the main layout

        // Window properties
        setWindowTitle("Router Simulation");
        setMinimumSize(400, 300); // Set a minimum size for the window
    }

private slots:
    void selectTraceFile() {
        traceFilePath = QFileDialog::getOpenFileName(this, "Select Trace File", "", "Trace Files (*.txt)");
    }

    void startSimulation() {
        if (traceFilePath.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please select a trace file.");
            return;
        }

        bool ok;
        int bufferSize = bufferSizeInput->text().toInt(&ok);
        if (!ok || bufferSize <= 0) {
            QMessageBox::warning(this, "Warning", "Invalid Buffer Size.");
            return;
        }

        double wlanCapacity = wlanCapacityInput->text().toDouble(&ok);
        if (!ok || wlanCapacity <= 0) {
            QMessageBox::warning(this, "Warning", "Invalid WLAN Capacity.");
            return;
        }

        // Call simulation function from simulation.cpp to return results
        SimulationResults results = mySimulation.runSimulation(traceFilePath.toStdString(), bufferSize, wlanCapacity);

        // Update resultsLabel with the simulation results
        incomingPacketsLabel->setText(QString("Incoming Packets: %1").arg(results.incomingPackets));
        deliveredPacketsLabel->setText(QString("Delivered Packets: %1").arg(results.deliveredPackets));
        droppedPacketsLabel->setText(QString("Dropped Packets: %1").arg(results.droppedPackets));
        packetLossPercentageLabel->setText(QString("Packet Loss Percentage: %1%").arg(results.packetLossPercentage, 0, 'f', 2));
        averageQueuingDelayLabel->setText(QString("Average Queuing Delay: %1 seconds").arg(results.averageQueuingDelay, 0, 'f', 3));
    }

private:
    RouterSimulation mySimulation;
    QLineEdit *bufferSizeInput;
    QLineEdit *wlanCapacityInput;
    QString traceFilePath;
    QLabel *incomingPacketsLabel;
    QLabel *deliveredPacketsLabel;
    QLabel *droppedPacketsLabel;
    QLabel *packetLossPercentageLabel;
    QLabel *averageQueuingDelayLabel;
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyleSheet("QWidget { background-color: #009999; }");

    RouterSimulatorApp window;
    window.show();

    return app.exec();
}
