#ifndef OPPS_FPGA
#define OPPS_FPGA




#include <mwio4.h>
#include <math.h>


// センサフィードバッグポート
typedef enum{
    PORT_CURRENT_U = 0,
    PORT_CURRENT_V,
    PORT_CURRENT_W,
    PORT_VUV,
    PORT_VVW,
    PORT_VDC,
    PORT_TORQUE,
    INPUT_DATA_ORDER_NUM
} INPUT_DATA_ORDER;

extern float matlab_var[20];
extern float matlab_ref[10];


#endif