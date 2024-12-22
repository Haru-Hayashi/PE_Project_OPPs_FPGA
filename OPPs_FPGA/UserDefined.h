#ifndef USERDEFINED_H_
#define USERDEFINED_H_

// *** header file include *** //
// #define MATLAB_SIM
#ifndef MATLAB_SIM
    #include <mwio4.h>
#else
    #include "mwio4.h"
#endif
#include <math.h>

// *** macro definition *** //
// * 制御設定 * //
// #define HILS_ENVIRONMENT
#define DEADTIME_COMPENSATE

#ifndef HILS_ENVIRONMENT
    // #define CONTROL_FREQ 2000
    // #define SAMPLING_FREQ 40000
    #define CTRL_BDN 0      //BoarD Number for control default→0
    #define SENS_BDN 0      //BoarD Number for sensor  default→1
    #define FPGA_BDN 4
#else
    #define FS 40000
    #define CONTROL_FREQ 40000
    #define SAMPLING_FREQ CONTROL_FREQ
    #define CTRL_BDN 0
    #define SENS_BDN 0
#endif

#define REF_UPDATE_FREQ CONTROL_FREQ    // 電圧指令値更新周波数は合わせる
#define DEADTIME 200
#define ENC_MAX 8191    //2048*4=2^13
#ifndef M_PI
    #define M_PI 3.14159
#endif
// * センサ設定 * //
#define CURRENT_SENSOR_RANGE 50.0/3.0//12.5
#define DCLINK_VOLTAGE_RANGE 1016
#define TORQUE_SENSOR_RANGE 20.0
#define DCLINK_CURRENT_RANGE 31.25
#define SWITCHING_FREQ_CALC_CYCLE 10000 //[Hz]
// * 帯域設定 * //
#define BW_OMEGARE 800  //速度応答ローパス

// *** type definition *** //

// *** prototype definition *** //
int rnd(float val);

// *** prototype definition (static)*** //

#endif
