#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "control.h"

extern "C" {
    #include "extApi.h"
}

static state_t state;
static mixer_t mixer;
static setpoint_t sp;

int main(int argc, char *argv[])
{
    int portNb = 0;

    if (argc >= 1) {
        portNb = atoi(argv[1]);
    } else {
        std::cout << "Please indicate following argument(s): 'portNumber'!" << std::endl;
        extApi_sleepMs(5000);
        return 0;
    }

    int clientID = simxStart((simxChar*)"127.0.0.1", portNb, true, true, 2000, 5);
    if (clientID != -1) {
        std::cout << "Connected to server." << std::endl;

        Segway segway(clientID);

        while (simxGetConnectionId(clientID) != -1) {
            // retrieve system state
            segway.acquireState(&state);

            // state feedback control
            segway.stateControler(&mixer, &state, &sp);

            // motor control
            segway.powerDistribution(&mixer);

            extApi_sleepMs(20);
        }

        std::cout << "Exit control loop." << std::endl;
        simxFinish(clientID);
    } else {
        std::cout << "Failed to connect to server." << std::endl;
    }

    return 0;
}
