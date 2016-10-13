#include <math.h>
#include <iostream>
#include "control.h"

#define PID_PITCH_KP 15
#define PID_PITCH_RATE_KP 0.065
#define PID_YAW_KP 0
#define PID_YAW_RATE_KP 0
#define PID_VELOCITY_KP 0

Segway::Segway(simxInt id) : client_id(id)
{
    // get simulation object handles
    if (simxGetObjectHandle(client_id, "segway", &segway_handle,
                simx_opmode_oneshot_wait) == simx_return_ok) {
        std::cout << "Retrieved segway handle." << std::endl;
    } else {
        std::cout << "Failed to retrieve segway handle." << std::endl;
    }
    if (simxGetObjectHandle(client_id, "motor_left_model", &left_motor_handle,
                simx_opmode_oneshot_wait) == simx_return_ok) {
        std::cout << "Retrieved left motor handle." << std::endl;
    } else {
        std::cout << "Failed to retrieve left motor handle." << std::endl;
    }
    if (simxGetObjectHandle(client_id, "motor_right_model", &right_motor_handle,
                simx_opmode_oneshot_wait) == simx_return_ok) {
        std::cout << "Retrieved right motor handle." << std::endl;
    } else {
        std::cout << "Failed to retrieve right motor handle." << std::endl;
    }

    // initial state space, streaming mode
    simxGetObjectPosition(client_id, segway_handle, -1, pos, simx_opmode_streaming);
    simxGetObjectOrientation(client_id, segway_handle, -1, att, simx_opmode_streaming);
    simxGetObjectVelocity(client_id, segway_handle, vel, rat, simx_opmode_streaming);

    // initialize controller
    controllerInit();

    std::cout << "Initialized client." << std::endl;
}

Segway::~Segway()
{
}

void Segway::acquireState(state_t *state)
{
    // position acquisition
    if (simxGetObjectPosition(client_id, segway_handle, -1, pos,
                simx_opmode_buffer) == simx_return_ok) {
        state->pos.x = pos[0];
        state->pos.y = pos[1];
        state->pos.z = pos[2];
        // std::cout << "state->pos: " << state->pos.x << " , "
        //                             << state->pos.y << " , "
        //                             << state->pos.z << std::endl;
    } else {
        std::cout << "Failed to read position." << std::endl;
    }

    // attitude acquisition
    if (simxGetObjectOrientation(client_id, segway_handle, -1, att,
                simx_opmode_buffer) == simx_return_ok) {
        state->att.x = att[0] * 180 / M_PI;
        state->att.y = att[1] * 180 / M_PI;
        state->att.z = att[2] * 180 / M_PI;
        std::cout << "state->att: " << state->att.x << " , "
                                    << state->att.y << " , "
                                    << state->att.z << std::endl;

    } else {
        std::cout << "Failed to read attitude." << std::endl;
    }
 
    // linear/angular velocity acquisition
    if (simxGetObjectVelocity(client_id, segway_handle, vel, rat,
                simx_opmode_buffer) == simx_return_ok) {
        state->vel.x = vel[0];
        state->vel.y = vel[1];
        state->vel.z = vel[2];
        // std::cout << "state->vel: " << state->vel.x << " , "
        //                             << state->vel.y << " , "
        //                             << state->vel.z << std::endl;
        state->rat.x = rat[0] * 180 / M_PI;
        state->rat.y = rat[1] * 180 / M_PI;
        state->rat.z = rat[2] * 180 / M_PI;
        std::cout << "state->rat: " << state->rat.x << " , "
                                    << state->rat.y << " , "
                                    << state->rat.z << std::endl;
    } else {
        std::cout << "Failed to read velocity." << std::endl;
    }
}

void Segway::pidInit(control_t *pid, const float ref, const float kp)
{
    pid->error = 0;
    pid->reference = ref;
    pid->kp = kp;
}

void Segway::controllerInit()
{
    pidInit(&pid_pitch, 0, PID_PITCH_KP);
    pidInit(&pid_pitch_rate, 0, PID_PITCH_RATE_KP);
    pidInit(&pid_yaw, 0, PID_YAW_KP);
    pidInit(&pid_yaw_rate, 0, PID_YAW_RATE_KP);
    pidInit(&pid_velocity, 0, PID_VELOCITY_KP);
}

float Segway::pidUpdate(control_t *pid, const float measurement)
{
    float output;

    pid->error = pid->reference - measurement;
    output = pid->kp * pid->error;

    return output;
}

void Segway::stateControler(mixer_t *mixer, const state_t *state, const setpoint_t *sp)
{
    // pitch control
    pid_pitch_rate.reference = pidUpdate(&pid_pitch, state->att.x);
    mixer->pit = pidUpdate(&pid_pitch_rate, state->rat.x);
    mixer->pit *= -1; // to match motor's rotational direction

    // mixer->pit = pid_pitch.kp * (0 - state->att.x) + pid_pitch_rate.kp * (0 - state->rat.x);
    // mixer->pit *= -1;
}

void Segway::powerDistribution(mixer_t *mixer)
{
    // left motor speed
    if (simxSetJointTargetVelocity(client_id, left_motor_handle, mixer->pit,
                simx_opmode_oneshot) == simx_return_ok) {
        std::cout << "left motor speed: " << mixer->pit << std::endl;
    } else {
        std::cout << "Failed to set left motor speed." << std::endl;
    }

    // right motor speed
    if (simxSetJointTargetVelocity(client_id, right_motor_handle, mixer->pit,
                simx_opmode_oneshot) == simx_return_ok) {
        std::cout << "right motor speed: " << mixer->pit << std::endl;
    } else {
        std::cout << "Failed to set right motor speed." << std::endl;
    }
}
