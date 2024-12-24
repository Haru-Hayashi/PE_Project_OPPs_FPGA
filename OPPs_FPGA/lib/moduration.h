#ifndef MODURATION_H
#define MODURATION_H

#include "UserDefined.h"

typedef struct{
    int VV_ref[8];
    float VV_ref_time[8];
    int sector;
    float Vtheta;
    float tu;
    float tv;
    float tw;
} SVM_Calc;


typedef struct{
    float alpha[Angle_Num];
    float t_u[Angle_Num+1];
    float t_v[Angle_Num+1];
    float t_w[Angle_Num+1];
    float alpha_u[Angle_Num+1];
    float alpha_v[Angle_Num+1];
    float alpha_w[Angle_Num+1];
    int count_u;
    int count_v;
    int count_w;
    int VV_Num[Angle_Num+1];
    float VV_time[Angle_Num+1];
    int num_patterns;
} Pulse_Pattern;

void SVM_Calculation(SVM_Calc *svm, float V_a, float V_b, float Vdc, float Ts);
void SVM_Calculation_Init(SVM_Calc *svm);
void VValt(int *VV_alt, int sector);
void SwDuration(Pulse_Pattern* Pulse, float Ts);
void VV_Pattern(Pulse_Pattern* Pulse);


#endif