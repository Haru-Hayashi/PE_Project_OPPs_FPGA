/*********************************************************
 Title:		Template source file for c6657
 Copyright:	1999-2013  Myway Plus Co.,Ltd.


 Abstract:
	This C source code is the template program
	that is used when you generate a project newly.

 Model:	Expert4(c6657)
*********************************************************/

#include "OPPs_FPGA.h"
#include "stdbool.h"
#include "stdio.h"
#include "UserDefined.h"
#include "lib/DSPBoard.h"
#include "lib/InductionMotor.h"
#include "lib/Inverter.h"
#include "lib/moduration.h"

// ******* プロトタイプ宣言 ******* //
interrupt void IntrFunc1(void); // 制御ループ関数
interrupt void IntrFunc0(void); // ゲート出力関数

#define DEADT 0//300
#define PI 3.14159265358979
#define ENC_MAX 8191//2^13
#define BDN 0
#define DAC_BDN 3
#define VDC_CMD 100
#define MAX_LINE_LENGTH 1024

//モータパラメータ
const float p = 2, Ls = 90.5e-3, Lr = 90.5e-3, Rs = 0.411, Rr = 0.382, M = 86.7e-3, Jm = 0.0183;

// ******* 変数定義 ******* //

//オブジェクト
IM_tMotor m;
LPF_tLowPassFilter lpf;
far Switching_Instance SW_Inst;
Pulse_Pattern Pulse;

volatile float offset[INPUT_DATA_ORDER_NUM] = {0};

volatile float Tcon = 0;
volatile float Tact = 0;
volatile float Tact2 = 0;
// volatile bool LatchTrigger = 0;

// float matlab_var[20];
// float matlab_ref[10];

//キャリア周波数の設定
float SAMPLING_FREQ = 20000;
float TS_SAMPLE = (50e-6);
float TS;
float FS;

// FPGAボード設定
int FPGA_Ts;
int wInput_VV1 = 8;
int wInput_VV2 = 8;
int wInput_VV3 = 8;
int wInput_VV4 = 8;
int wInput_VV5 = 8;
int wInput_VV6 = 8;
int wInput_VV7 = 8;
float wInput_VV1_time = 0;
float wInput_VV2_time = 0;
float wInput_VV3_time = 0;
float wInput_VV4_time = 0;
float wInput_VV5_time = 0;
float wInput_VV6_time = 0;

//データ読み込み（応答値）
float data[8] = { 0 };
float iu_res = 0.0;
float iv_res = 0.0;
float iw_res = 0.0;
float Vu_res = 0.0;
float Vv_res = 0.0;
float Vw_res = 0.0;
float Vdc = 0.0;
float Vdc_sens = 0.0;
float t_res = 0.0;
float V_limit = 0.0;
float ENC_data[2] = { 0 };
int ENC_delta = 0;
// float tau_load_ref = 0.0; 
float tau_L = 0.0; 

//オフセット調節用
float data_offset_tmp[8];
float data_offset[8];
int offset_count = 0;
float offset_time = 0.0;

//ab座標での応答値
float ia_res = 0.0;
float ib_res = 0.0;
float va_res = 0.0;
float vb_res = 0.0;

//dq座標での応答値
float id_res = 0.0;
float iq_res = 0.0;

//速度関連
float w1 = 0.0;
float ws = 0.0;
float wre = 0.0;
float wrm = 0,wrm_filter=0;
const int g_filter = 800;
int flag_wrm = 1;
float omega_data[2];
//磁束と位相関連
float phi_2d=0;
float p2_theta = 0.0;

//制御につかう変数
int Vector_Times = 0;
int Vector_Number = 0;
int Vector_Number2 = 0;
float Ts = 0.0;
float Tc = 0.0;
int u = 0, v = 0, w = 0;
int du = 0, dv = 0, dw = 0;
int numLines = 0;
int out_swp = 0;
static int Flag_ASPS = 0;
float TS_conv[2];

//電圧指令値計算用
float Vref_nrm = 0.0;
float Vref_nrm_abs = 0.0;
float V_amp = 0.0;
float Vref_a = 0.0, Vref_b = 0.0;
float mod = 0.0;
float omega_ref = 0.0;
float f_ref = 0.0;
float wt = 0.0;
float dwt = 0.0;
//周波数指令値を徐々に上げる時に使用
float T_trans;
float delta_omega;
int Counter_up = 0;
int Counter_down = 0;
static float omega_tmp = 0.0;
static float omega_tmp_up = 0.0;
static float omega_tmp_down = 0.0;
//DPWM指令値生成に使用
float max = 0.0, min = 0.0, z = 0.0;

//ASPSをFPGAで出力するときに使用
int n = 2;
int siny[2][200];
// int **siny;
int Flag_TableRead;
int Table_num;
int Slot_Num;
int Table_num_prev;
int Check = 0;
int Vector_cont_time = 0;
int Vector_cont_count = 0;
int Flag_ini = 0;
int Flag_stop = 0;
int Flag_stop2 = 0;
float wInput_VV_time_tmp = 0;
float wInput_VV2_tmp = 0;
float wInput_VV2_time_tmp2 = 0;
float wInput_VV2_time_tmp = 0;
float wInput_VV3_tmp = 0;
float wInput_VV3_time_tmp = 0;
float wInput_VV4_tmp = 0;
float wInput_VV4_time_tmp = 0;
float wInput_VV2_time_veiw = 0;
float wInput_VV1_time_veiw = 0;

//OPPsの実装に使用
bool Init_write = 0;
int pulse_sector[2] = 0;
int phase_sector = 0;
float trans_time = 0.0;
int trans_count = 0;
float Mod = 0.0;
float T_fnd = 0.0;
int alt_num[2] = 0;
float lap_time[2];
int Flag_Init = 1;
int phase_sector_trans = 0;
int Flag_OPP = 0;
// 描画用
float VV_time[9];
float Conf1[9];
float Conf2[9];
float Conf3[9];

//線間電圧、相電圧、電圧積分値計算
float Vu_inv = 0.0, Vv_inv = 0.0, Vw_inv = 0.0;
float Vwu = 0.0, Vuv = 0.0, Vvw = 0.0;
float Vu, Vv, Vw;
float Vu_ref = 0.0, Vv_ref = 0.0, Vw_ref = 0.0;
float Vun_ref = 0.0, Vvn_ref = 0.0, Vwn_ref = 0.0;
float Vest_a = 0.0, Vest_b = 0.0;
float Pout_a = 0.0, Pout_b = 0.0;
float Pref_a, Pref_b = 0.0;
float Perr_a, Perr_b = 0.0;
float Perr_nrm = 0.0;
float Hysteresis_Band = 0.0;
float Vtheta = 0.0;

//スイッチング周波数計算
int u_sw = 0.0;
float sf_u_ave = 0.0;
int U_tr[2];
int v_sw = 0.0;
float sf_v_ave = 0.0;
int V_tr[2];
int w_sw = 0.0;
float sf_w_ave = 0.0;
int W_tr[2];
float sf_ave = 0.0;
float sf_ave2 = 0.0;
float uvw_sw = 0.0;
float uvw_sw_total = 0.0;
float average_time = 0.0;
int TotalCounter = 0;
float SF_average = 0.0;

//汎用変数
int i;
int j;

//確認用の諸々
float Vdc_command = 282.0;
int Flag_Vdc_command = 0;
int noptime = 0.0;
int Flag_omega_max;
int omega_max = 100;
//OPP
int trans_time_first = 0;


// ******* 関数定義 ******* //
void MW_main(void){

	// ***** PEVボード設定 ***** //
	int0_disable_int();
	int_disable();
    // 電圧ベクトル出力の初期値
	int Current_Limit = 8000;
	int Voltage_Limit = 8000;
	// FPGA_write(FPGA_BDN, 0x007, FPGA_Ts);	
	FPGA_write(FPGA_BDN, 0x001, 0);	// デッドタイム(>200ns/10ns=20)
	FPGA_write(FPGA_BDN, 0x003, Current_Limit);	// 電流リミット（機能していない）
	FPGA_write(FPGA_BDN, 0x004, Voltage_Limit);	// 電圧リミット（機能していない）
	// VV番号
	FPGA_write(FPGA_BDN, 0x005, 8);	// 1つめの電圧ベクトル番号 0~8, 8は全ゲート信号off
	FPGA_write(FPGA_BDN, 0x006, 8);	// 2つめの電圧ベクトル番号 0~8, 8は全ゲート信号off
	FPGA_write(FPGA_BDN, 0x008, 8);	// 3つめの電圧ベクトル番号 0~8, 8は全ゲート信号off
	FPGA_write(FPGA_BDN, 0x009, 8);	// 4つめの電圧ベクトル番号 0~8, 8は全ゲート信号off
	FPGA_write(FPGA_BDN, 0x010, 8);	// 5つめの電圧ベクトル番号 0~8, 8は全ゲート信号off
	FPGA_write(FPGA_BDN, 0x011, 8);	// 6つめの電圧ベクトル番号 0~8, 8は全ゲート信号off
	FPGA_write(FPGA_BDN, 0x012, 8);	// 7つめの電圧ベクトル番号 0~8, 8は全ゲート信号off
	// 持続時間の指定（切り換え時間なのでそれまでの合計値）
	FPGA_write(FPGA_BDN, 0x000, 0);	//1つめの電圧ベクトルの持続時間1
	FPGA_write(FPGA_BDN, 0x00A, 0);	//2つめの電圧ベクトルの持続時間2
	FPGA_write(FPGA_BDN, 0x00B, 0);	//3つめの電圧ベクトルの持続時間3
	FPGA_write(FPGA_BDN, 0x00C, 0);	//4つめの電圧ベクトルの持続時間4
	FPGA_write(FPGA_BDN, 0x00D, 0);	//5つめの電圧ベクトルの持続時間5
	FPGA_write(FPGA_BDN, 0x00E, 0);	//6つめの電圧ベクトルの持続時間6

	 // ***** PEVボード設定 ***** //
	PEV_init(CTRL_BDN);  // PEVボードの初期化。ソフトウェア実行前に内部パラメータを初期化
	// *** 割り込み設定（拒否・設定） *** //
    PEV_int_init(CTRL_BDN, 0, 1, 0, 0, 0, 0, 0, 0); // 割り込み設定（int1をAD完了割り込み）
	 // *** AD変換設定 *** //
    float range[] = { CURRENT_SENSOR_RANGE, CURRENT_SENSOR_RANGE, CURRENT_SENSOR_RANGE, 5, 5, DCLINK_VOLTAGE_RANGE, TORQUE_SENSOR_RANGE, DCLINK_CURRENT_RANGE};
    PEV_ad_set_range(CTRL_BDN, range);  // A/D変換入力電圧レンジ設定
    PEV_ad_set_mode(CTRL_BDN, 2);       // AD変換モード設定（2:シングルショット, PEV_ad_start()によりAD変換, 変換開始後500ns以上経過して読み込むこと．）
    PEV_ad_enable_int(CTRL_BDN);        //AD変換完了割り込み許可
	// *** エンコーダ設定 *** //
    PEV_abz_set_mode(CTRL_BDN, 5, 0); // 5=(1:zパルス割り込み発生enable, 0:reserve, 1:zパルスクリアenable)
    PEV_abz_init_maxcount(CTRL_BDN, ENC_MAX);   // エンコーダカウント値の最大値設定

    // ***** 関数の割り込み設定 ***** //
 	int0_init_vector(IntrFunc0, (CSL_IntcVectId)4, TRUE);  // 外部割込み設定（int0(FPGA割り込み信号に同期)でIntrFunc0を呼び出し）
    int1_init_vector(IntrFunc1, (CSL_IntcVectId)5, TRUE);  // 外部割込み設定（int1(AD変換完了に同期)でIntrFunc1を呼び出し）

	// ******** DACの設定 ******** //
	DAC_da_set_offset(DAC_BDN, 0, 0);
	DAC_da_set_range(DAC_BDN, 0, 10);
	DAC_da_set_offset(DAC_BDN, 1, 0);
	DAC_da_set_range(DAC_BDN, 1, 10);
	DAC_da_set_offset(DAC_BDN, 2, 0);
	DAC_da_set_range(DAC_BDN, 2, 10);
	DAC_da_set_offset(DAC_BDN, 3, 0);
	DAC_da_set_range(DAC_BDN, 3, 10);
	DAC_da_set_offset(DAC_BDN, 4, 0);
	DAC_da_set_range(DAC_BDN, 4, 10);
	DAC_da_set_offset(DAC_BDN, 5, 0);
	DAC_da_set_range(DAC_BDN, 5, 10);
	DAC_da_set_offset(DAC_BDN, 6, 0);
	DAC_da_set_range(DAC_BDN, 6, 10);
	DAC_da_set_offset(DAC_BDN, 7, 0);
	DAC_da_set_range(DAC_BDN, 7, 10);
	DAC_da_set_offset(DAC_BDN, 8, 0);
	DAC_da_set_range(DAC_BDN, 8, 10);

	// **** タイマの設定 **** //
	C6657_timer0_init(10e6);	
	C6657_timer1_init(10e6);	

    // *** 設定終了 *** //
    int0_enable_int();      // 外部からの割り込み入力の許可
    int1_enable_int();      // 外部からの割り込み入力の許可
    int_enable();           // 多重割り込みの許可

    while(1);

}

// キャリア同期割り込み(AD変換割り込み):制御用
interrupt void IntrFunc1(void){
    int1_ack(); // 外部割込みアクノリッジの通知（割り込み関数先頭で必ず呼び出し）
//     // ******* 多重割り込みの許可設定 ******* //
//     MWINT_IEINFO b;
//     UINT32 newIer;
//     newIer = 0x00000010;    // VectID4を設定した割り込み=int0を許可
//     int_begin_nestint(newIer, &b);
//     int_enable();

	// SAMPLING_FREQ = 50000;
	TS_SAMPLE = 1/SAMPLING_FREQ;
	TS = TS_SAMPLE;
	FS = SAMPLING_FREQ;
	FPGA_Ts = TS_SAMPLE*1e6*100;  		  // 制御周期の設定 0~4095の範囲(0~40.95us)
	FPGA_write(FPGA_BDN, 0x007, FPGA_Ts);	

	float data_dac[12] = { 0 };

   // ************** センサフィードバックの取得 ************** //
    m.ms.ires.uvw[0] = iu_res;
    m.ms.ires.uvw[1] = iv_res;
    m.ms.ires.uvw[2] = iw_res;
	m.ms.torque = t_res;

   /***************エンコーダパルス検出と速度計算***************/
	// エンコーダ情報読み取り
	DSP_InputEncoderData(FS, CTRL_BDN, ENC_data, omega_data, &lpf);
	wrm = omega_data[0];
	wrm_filter = omega_data[1];
	m.ms.theta_1 = ENC_data[1];
    m.ms.theta_2 = ENC_data[0];
    m.ms.omega_rm = wrm;

	// dq軸二次鎖交磁束位相の演算
    IM_CalcStatus(&m, TS);
	
	// ***** 状態量推定 ***** //
    uvw2ab(m.ms.ires.uvw[0], m.ms.ires.uvw[1], m.ms.ires.uvw[2], &(m.ms.ires.ab[0]), &(m.ms.ires.ab[1]));
    ab2dq(m.ms.ires.ab[0], m.ms.ires.ab[1], m.ms.phi2.phase + TS*m.ms.omega_1, &(m.ms.ires.dq[0]), &(m.ms.ires.dq[1]));
    // 代入
    id_res = m.ms.ires.dq[0];
    iq_res = m.ms.ires.dq[1];

	// ******************* 制御記述 ******************* //

	if(!Init_write){
		// // 最初だけ書き込み
		Init_write = 1;

		// u相のスイッチング角を定義(0～pi/2[rad])
		float alpha[Angle_Num] = {0.2098, 0.3335, 0.4713, 0.5687, 0.7221, 0.7905, 1.1176, 1.1805};
		for (i = 0; i < Angle_Num; i++) {
			Pulse.alpha[i] = alpha[i];
		}
		// 各相スイッチング角からスイッチング時間を計算
		SwDuration(&Pulse, TS);
		// 各相スイッチング時間からベクトル番号とその持続時間を計算
		VV_Pattern(&Pulse);

	}

	// 確認用
	for (i = 0; i <= Angle_Num; i++) {
		Conf1[i] = Pulse.t_u[i]*1e6;
		Conf2[i] = Pulse.t_v[i]*1e6;
		Conf3[i] = Pulse.t_w[i]*1e6;
		// Conf1[i] = Pulse.VV_Num[i];
		// Conf2[i] = Pulse.VV_time[i];
		// Conf3[0] = Pulse.count_w;
	}

	// TS_conv[0] = TS*1e6;
	// TS_conv[1] = TS*1e6+1000;

	// int *VV_alt = (int *)malloc(3 * sizeof(int));  

	// // float t_alpha[9] = {200.4688435, 156.526073, 104.2437986, 136.6395637, 194.0538405, 145.6205679, 324.2784012, 164.8655769, 252.7471906};
	// // float t_beta[9] = {252.7471906, 164.8655769, 324.2784012, 145.6205679, 194.0538405, 136.6395637, 104.2437986, 156.526073, 200.4688435};

	// float t_alpha[9];
	// float t_beta[9]; 

	// for (i = 0; i <= Angle_Num; i++) {
	// 	t_alpha[i] = Pulse.VV_time[i];
	// }
	// for (i = Angle_Num; i >=0; i--) {
	// 	t_beta[i] = Pulse.VV_time[i];
	// }

	// if(!Init_write){
	// 	// 最初だけ書き込み
	// 	Init_write = 1;

	// 	trans_time = floor(t_alpha[0] / TS_conv[0]);
	// 	trans_time_first = trans_time;	
	// 	phase_sector = 1;
	// 	pulse_sector[0] = 1;
	// 	Flag_Init = 0;
	// 	VValt(VV_alt, phase_sector);
	// }                                                                                                                                                                                  

	// if(trans_count >= trans_time){

	// 	trans_count = 0;

	// 	pulse_sector[1] = pulse_sector[0];
	// 	pulse_sector[0]++;
	// 	if(pulse_sector[0] > 9){
	// 		pulse_sector[0] = 1;
	// 		phase_sector++;
	// 		phase_sector_trans = 1;
	// 		if(phase_sector > 12){
	// 			phase_sector = 1;
	// 		}
	// 	}
		
	// 	if(pulse_sector[0] % 2 != 0){
	// 		alt_num[1] = alt_num[0];
	// 		alt_num[0] = 0;
	// 	}else{
	// 		alt_num[1] = alt_num[0];
	// 		alt_num[0] = 1;
	// 	}

	// 	if(phase_sector % 2 != 0){
	// 		if(phase_sector_trans == 1){
	// 			lap_time[0] = TS_conv[0]*(trans_time+1) - (t_beta[8]-lap_time[1]);
	// 			wInput_VV1 = VV_alt[0];
	// 			VValt(VV_alt, phase_sector);
	// 			wInput_VV2 = VV_alt[0];
	// 			phase_sector_trans = 0;
	// 		}else{
	// 			lap_time[0] = TS_conv[0]*(trans_time+1) - (t_alpha[pulse_sector[1]-1]-lap_time[1]);
	// 			wInput_VV1 = VV_alt[alt_num[1]];
	// 			wInput_VV2 = VV_alt[alt_num[0]];
	// 		}
	// 		lap_time[1] = TS_conv[0]-lap_time[0];
	// 		if(pulse_sector[0] == 4 || pulse_sector[0] == 8){
	// 			alt_num[1] = 0;
	// 			alt_num[0] = 2;
	// 			wInput_VV1 = VV_alt[alt_num[1]];
	// 			wInput_VV2 = VV_alt[alt_num[0]];
	// 		}
	// 		trans_time = floor((t_alpha[pulse_sector[0]-1]-lap_time[1]) / TS_conv[0]);
	// 	}else{
	// 		if(phase_sector_trans == 1){
	// 			lap_time[0] = TS_conv[0]*(trans_time+1) - (t_alpha[8]-lap_time[1]);
	// 			wInput_VV1 = VV_alt[0];
	// 			VValt(VV_alt, phase_sector);
	// 			wInput_VV2 = VV_alt[0];
	// 			phase_sector_trans = 0;
	// 		}else{
	// 			lap_time[0] = TS_conv[0]*(trans_time+1) - (t_beta[pulse_sector[1]-1]-lap_time[1]);
	// 			wInput_VV1 = VV_alt[alt_num[1]];
	// 			wInput_VV2 = VV_alt[alt_num[0]];
	// 		}
	// 		lap_time[1] = TS_conv[0]-lap_time[0];
	// 		if(pulse_sector[0] == 2 || pulse_sector[0] == 6){
	// 			alt_num[1] = 0;
	// 			alt_num[0] = 2;
	// 			wInput_VV1 = VV_alt[alt_num[1]];
	// 			wInput_VV2 = VV_alt[alt_num[0]];
	// 		}
	// 		trans_time = floor((t_beta[pulse_sector[0]-1]-lap_time[1]) / TS_conv[0]);
	// 	}

	// 	wInput_VV3 = wInput_VV2;
	// 	wInput_VV4 = wInput_VV2;
	// 	wInput_VV5 = wInput_VV2;
	// 	wInput_VV6 = wInput_VV2;
	// 	wInput_VV7 = wInput_VV2;
	// 	wInput_VV1_time = lap_time[0];
	// 	wInput_VV2_time = lap_time[1];
	// 	wInput_VV3_time = TS_conv[1];
	// 	wInput_VV4_time = TS_conv[1];
	// 	wInput_VV5_time = TS_conv[1];
	// 	wInput_VV6_time = TS_conv[1];

	// }else{

	// 	//ベクトルを持続する
	// 	wInput_VV1 = VV_alt[alt_num[0]];
	// 	wInput_VV2 = wInput_VV1;
	// 	wInput_VV3 = wInput_VV1;
	// 	wInput_VV4 = wInput_VV1;
	// 	wInput_VV5 = wInput_VV1;
	// 	wInput_VV6 = wInput_VV1;
	// 	wInput_VV7 = wInput_VV1;
	// 	wInput_VV1_time = TS_conv[0];
	// 	wInput_VV2_time = TS_conv[1];
	// 	wInput_VV3_time = TS_conv[1];
	// 	wInput_VV4_time = TS_conv[1];
	// 	wInput_VV5_time = TS_conv[1];
	// 	wInput_VV6_time = TS_conv[1];
	// }
	// out_swp = wInput_VV1;

	// trans_count++;

	// free(VV_alt);

	if(Flag_stop == 1){
		wInput_VV1 = 8;
		wInput_VV2 = 8;
		wInput_VV3 = 8;
		wInput_VV4 = 8;
		wInput_VV5 = 8;
		wInput_VV6 = 8;
		wInput_VV7 = 8;
		wInput_VV1_time = 0;
		wInput_VV2_time = 0;
		wInput_VV3_time = 0;
		wInput_VV4_time = 0;
		wInput_VV5_time = 0;
		wInput_VV6_time = 0;

		Vector_cont_time = 0;
		Vector_cont_count = 0;
		Vector_Number = 0;
		Flag_ini = 0;
	}

	// ***** スイッチング周波数計算 *****// 
	// スイッチング状態からuvw相信号を取得
	SW_Inst.U_Edge[0] = SwitchingState[out_swp][0];
	SW_Inst.V_Edge[0] = SwitchingState[out_swp][1];
	SW_Inst.W_Edge[0] = SwitchingState[out_swp][2];
	SF_calculation(&SW_Inst, TS);
	SF_average = SW_Inst.SF_average;

	// DACの出力設定
	data_dac[0] = tau_L/1.0;
	DAC_da_out(DAC_BDN, 0 , data_dac[0]);

}

// 同期割り込み(FPGA割り込み信号):ゲート信号出力
interrupt void IntrFunc0(void){
	int0_ack(); // 外部割込みアクノリッジの通知（割り込み関数先頭で必ず呼び出し）
    // ******* 多重割り込みの許可設定 ******* //
    MWINT_IEINFO b;
    UINT32 newIer;
    newIer = 0x00000020;    // VectID5を設定した割り込み=int1を許可
    int_begin_nestint(newIer, &b);
    int_enable();

	C6657_timer0_start();	// DSP内蔵タイマーのカウント動作スタート

	// 電圧ベクトル信号と継続時間を設定
	int input_VV1 = wInput_VV1;
	int input_VV2 = wInput_VV2;
	int input_VV3 = wInput_VV3;
	int input_VV4 = wInput_VV4;
	int input_VV5 = wInput_VV5;
	int input_VV6 = wInput_VV6;
	int input_VV7 = wInput_VV7;
	float input_VV1_time = wInput_VV1_time;
	float input_VV2_time = wInput_VV2_time;
	float input_VV3_time = wInput_VV3_time;
	float input_VV4_time = wInput_VV4_time;
	float input_VV5_time = wInput_VV5_time;
	float input_VV6_time = wInput_VV6_time;

	FPGA_write(FPGA_BDN, 0x005, input_VV1); // FPGAのポード5に1つめの電圧ベクトル番号を入力
	FPGA_write(FPGA_BDN, 0x006, input_VV2); // FPGAのポード6に2つめの電圧ベクトル番号を入力
	FPGA_write(FPGA_BDN, 0x008, input_VV3);
	FPGA_write(FPGA_BDN, 0x009, input_VV4);
	FPGA_write(FPGA_BDN, 0x010, input_VV5);
	FPGA_write(FPGA_BDN, 0x011, input_VV6);
	FPGA_write(FPGA_BDN, 0x012, input_VV7);

	// FPGAのポード0に1つめの電圧ベクトルの持続時間を入力（持続時間/10nsを入力、25us->2500等）
	FPGA_write(FPGA_BDN, 0x000, input_VV1_time*100); 
	FPGA_write(FPGA_BDN, 0x00A, input_VV2_time*100);
	FPGA_write(FPGA_BDN, 0x00B, input_VV3_time*100);
	FPGA_write(FPGA_BDN, 0x00C, input_VV4_time*100);
	FPGA_write(FPGA_BDN, 0x00D, input_VV5_time*100);
	FPGA_write(FPGA_BDN, 0x00E, input_VV6_time*100);

	// AD変換完了時に割り込みフラグを生成
	// float data[8] = { 0 };
	PEV_ad_start(CTRL_BDN);
	while (PEV_ad_in_st(CTRL_BDN)) {}
	PEV_ad_in_grp(CTRL_BDN, data);

	/****************オフセット調整****************/
	if(offset_time < 5.0){
		for(i = 0; i < 8; ++i){
			data_offset_tmp[i] += data[i];
		}
		offset_count++;
		offset_time = offset_count*TS;

		for(i = 0; i < 8; ++i){
			data_offset[i] = data_offset_tmp[i]/offset_count;
		}
	}
	
	/************AD変換の値とオフセットを代入************/
	if(offset_time >= 5.0){
		for(i = 0; i < 8; ++i){
			data_offset_tmp[i] = 0.0;
		}

		iu_res = data[0]-data_offset[0];
		iv_res = data[1]-data_offset[1];
		iw_res = data[2]-data_offset[2];

	 	Vdc_sens = data[5]-data_offset[5];
		t_res = data[6]-data_offset[6];

		Vdc = Vdc_sens;

		if(Vdc > 275){
			Vdc = 282;
		}
	}
	//デバッグするときにVdc=282[V]にする
	if(Flag_Vdc_command == 1){
		Vdc = Vdc_command;
	}

	Tact = (float)C6657_timer0_read() * 4.8 * 1e-3; // (4.8 [ns/count])

	C6657_timer0_stop();
    C6657_timer0_clear();

	// ***** 多重割り込みからの復帰 ***** //
    int_disable();
    int_end_nestint(&b);
}

