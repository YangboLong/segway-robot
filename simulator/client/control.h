#ifndef CONTROL_H
#define CONTROL_H

#include "datatypes.h"

extern "C" {
    #include "extApi.h"
}

class Segway {
private:
    simxInt client_id;
    simxInt segway_handle;
    simxInt left_motor_handle;
    simxInt right_motor_handle;

    simxFloat pos[3];
    simxFloat att[3];
    simxFloat vel[3];
    simxFloat rat[3];

    control_t pid_pitch;
    control_t pid_pitch_rate;
    control_t pid_yaw;
    control_t pid_yaw_rate;
    control_t pid_velocity;

    void pidInit(control_t *pid, const float ref, const float kp);
    void controllerInit();
    float pidUpdate(control_t *pid, const float measurement);

public:
    Segway(simxInt id);
    ~Segway();

    void acquireState(state_t *state);
    void stateControler(mixer_t *mixer, const state_t *state, const setpoint_t *sp);
    void powerDistribution(mixer_t *mixer);
};

#endif
