#ifndef DATATYPES_H
#define DATATYPES_H

struct vector3d_s {
    float x;
    float y;
    float z;
};

typedef struct vector3d_s acc_t;
typedef struct vector3d_s rat_t; // angular rate
typedef struct vector3d_s vel_t; // linear velocity
typedef struct vector3d_s pos_t;
typedef struct vector3d_s att_t;

typedef struct state_s {
    pos_t pos;
    att_t att;
    vel_t vel;
    rat_t rat;
} state_t;

typedef struct motor_s {
    float left;
    float right;
} motor_t;

typedef struct mixer_s {
    float pit;
    float yaw;
    float vel;
} mixer_t;

typedef struct setpoint_s {
    float vel;
    float yaw;
} setpoint_t;

typedef struct control_s {
    float error;
    // float prev_error;
    float reference;
    // float dt;
    float kp;
} control_t;

#endif
