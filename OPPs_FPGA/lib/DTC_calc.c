#include "DTC_calc.h"
#include "math.h"

#define PI 3.14159265358979



void DTC_calc(float phi_s_nrm, float phi_ref, float delta_phi_lim, float theta_phi_s, float tau_m, float tau_ref, float delta_tau_lim, float **Vector){
// void DTC_calc(DTC_Calc DTC, int *Vector){

    float delta_phi;
    float delta_tau;
    int phi_sel;
    int T_sel;
    int sector;


    //LUTの磁束増減信号
	delta_phi = phi_s_nrm - phi_ref;
    if(delta_phi > delta_phi_lim/2)
    {
        phi_sel = 0;
    }
    else if(delta_phi < -delta_phi_lim/2)
    {
        phi_sel = 1;
    }
	//LUTのトルク増減信号
    delta_tau = tau_m - tau_ref;
    if(delta_tau > delta_tau_lim/2)
    {
        T_sel = 0;
    }
    else if(delta_tau < -delta_tau_lim/2)
    {
        T_sel = 1;
    }
	//磁束位置からセクターを決定
	if(theta_phi_s >= -PI/6 & theta_phi_s < PI/6)
    {
        sector = 1;    
    }
    else if(theta_phi_s >= PI/6 & theta_phi_s < PI/2)
    {
        sector = 2;
    }
    else if(theta_phi_s >= PI/2 & theta_phi_s < PI*5/6)
    {
        sector = 3;
    }
    else if(PI >= theta_phi_s & theta_phi_s >= PI*5/6 || -PI <= theta_phi_s & theta_phi_s < -PI*5/6)
    {
        sector = 4;
    }
    else if(theta_phi_s >= -PI*5/6 & theta_phi_s < -PI/2)
    {
        sector = 5;
    }
    else if(theta_phi_s >= -PI/2 & theta_phi_s < -PI/6)
    {
        sector = 6;
    }

    //DTCのLUTでベクトルを決定
    if(phi_sel == 0 & T_sel == 0)
    {

            if(sector == 1)
            {
                *Vector = 0; //5;   
            }
            else if(sector == 2)
            {
                *Vector = 7; //6;
            }
            else if(sector == 3)
            {
                *Vector = 0; //1;
            }
            else if(sector == 4)
            {
                *Vector = 7; //2;
            }
            else if(sector == 5)
            {
                *Vector = 0; //3;
            }
            else if (sector == 6)
            {
                *Vector = 7; //4;
            }
            // if(sector == 1)
            // {
            //     *Vector = 5;   
            // }
            // else if(sector == 2)
            // {
            //     *Vector = 6;
            // }
            // else if(sector == 3)
            // {
            //     *Vector = 1;
            // }
            // else if(sector == 4)
            // {
            //     *Vector = 2;
            // }
            // else if(sector == 5)
            // {
            //     *Vector = 3;
            // }
            // else if (sector == 6)
            // {
            //     *Vector = 4;
            // }
     }

     if(phi_sel == 0 & T_sel == 1)
     {
            if(sector == 1)
            {
                *Vector = 3;   
            }
            else if(sector == 2)
            {
                *Vector = 4;
            }
            else if(sector == 3)
            {
                *Vector = 5;
            }
            else if(sector == 4)
            {
                *Vector = 6;
            }
            else if(sector == 5)
            {
                *Vector = 1;
            }
            else if (sector == 6)
            {
                *Vector = 2;
            }

    }
     else if(phi_sel == 1 & T_sel == 0)
    {

            if(sector == 1)
            {
                *Vector = 7; //6;   
            }
            else if(sector == 2)
            {
                *Vector = 0; //1;
            }
            else if(sector == 3)
            {
                *Vector = 7; //2;
            }
            else if(sector == 4)
            {
                *Vector = 0 ;//3;
            }
            else if(sector == 5)
            {
                *Vector = 7; //4;
            }
            else if (sector == 6)
            {
                *Vector = 0; //5;
            }
            // if(sector == 1)
            // {
            //     *Vector = 6;   
            // }
            // else if(sector == 2)
            // {
            //     *Vector = 1;
            // }
            // else if(sector == 3)
            // {
            //     *Vector = 2;
            // }
            // else if(sector == 4)
            // {
            //     *Vector = 3;
            // }
            // else if(sector == 5)
            // {
            //     *Vector = 4;
            // }
            // else if (sector == 6)
            // {
            //     *Vector = 5;
            // }
        }

     else if(phi_sel == 1 & T_sel == 1)
    {
            if(sector == 1)
            {
                *Vector = 2;   
            }
            else if(sector == 2)
            {
                *Vector = 3;
            }
            else if(sector == 3)
            {
                *Vector = 4;
            }
            else if(sector == 4)
            {
                *Vector = 5;
            }
            else if(sector == 5)
            {
                *Vector = 6;
            }
            else if (sector == 6)
            {
                *Vector = 1;
            }

    }
    return;
}