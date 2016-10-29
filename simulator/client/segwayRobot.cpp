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
        std::cout << "Please indicate following argument(s): 'portNumber'!"
                  << std::endl << std::endl;
        extApi_sleepMs(5000);
        return 0;
    }

    int clientID = simxStart((simxChar*)"127.0.0.1", portNb, true, true, 2000, 5);
    if (clientID != -1) {
        std::cout << "Connected to server." << std::endl;

        Segway segway(clientID);

        // enable the synchronous mode
        if(simxSynchronous(clientID, true) == simx_return_ok) {
            std::cout << "Entered synchronous mode." << std::endl;
        } else {
            std::cout << "Failed to enter synchronous mode." << std::endl << std::endl;
        }

        // std::cout << simxStartSimulation(clientID, simx_opmode_oneshot) << std::endl << std::endl;
        if (simxStartSimulation(clientID, simx_opmode_oneshot) == simx_return_novalue_flag) {
            std::cout << "Started simulation." << std::endl;
        } else {
            std::cout << "Failed to start simulation." << std::endl << std::endl;
        }

        // for accepting ping time in milliseconds
        simxInt ping_time;

        while (simxGetConnectionId(clientID) != -1) {
            // retrieve system state
            segway.acquireState(&state);

            // state feedback control
            sp.vel = 1;
            segway.stateControler(&mixer, &state, &sp);

            // motor control
            segway.powerDistribution(&mixer);

            // trigger next simulation step
            if (simxSynchronousTrigger(clientID) == simx_return_ok) {
                std::cout << "Triggered next simulation step." << std::endl;
            } else {
                std::cout << "Failed to trigger next simulation step."
                          << std::endl << std::endl;
            }

            // after this call, the first simulation step is finished
            if (simxGetPingTime(clientID, &ping_time) == simx_return_ok) {
                std::cout << "Retrieved ping time." << std::endl;
            } else {
                std::cout << "Failed to retrieve ping time." << std::endl << std::endl;
            }
        }

        std::cout << "Exit control loop." << std::endl;
        simxFinish(clientID);
    } else {
        std::cout << "Failed to connect to server." << std::endl << std::endl;
    }

    return 0;
}
