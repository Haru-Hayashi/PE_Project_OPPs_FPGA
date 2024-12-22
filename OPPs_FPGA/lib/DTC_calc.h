#ifndef DTC_CALC
#define DTC_CALC

typedef struct{
    float phi_s_nrm;
    float phi_ref;
    float delta_phi_lim;
    float tau_m;
    float tau_ref;
    float delta_tau_lim;
    float theta_phi_s;
} DTC_Calc;


void DTC_calc(float phi_s_nrm, float phi_ref, float delta_phi_lim, float theta_phi_s, float tau_m, float tau_ref, float delta_tau_lim, float *Vector);
// void DTC_calc(DTC_Calc DTC, int *Vector);



#endif