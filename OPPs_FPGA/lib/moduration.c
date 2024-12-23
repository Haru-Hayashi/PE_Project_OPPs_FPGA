#include "moduration.h"
#include "math.h"
#include "UserDefined.h"
#include "Common.h"

#define PI 3.14159265358979
#define PI2_3 2.094395102
#define SQRT2 1.414213562


void SVM_Calculation_Init(SVM_Calc *svm){
    int i;
    for(i = 0; i < 8; ++i){
        svm->VV_ref[i] = 0;
        svm->VV_ref_time[i] = 0;
    }

    svm->Vtheta = 0.0;
    svm->sector = 0;
    svm->tu = 0.0;
    svm->tv = 0.0;
    svm->tw = 0.0;
}

void SVM_Calculation(SVM_Calc *svm, float V_a, float V_b,float Vdc, float Ts){
    int sector = 0;
    int Fu = 0, Fv = 0, Fw = 0;
    float Vtheta = 0;
    //float Vdc = 282.0;
    float tu = 0.0, tv = 0.0, tw = 0.0;
    float Vt[8] = {0};
    int Su = 0, Sv = 0, Sw = 0;
    float t_out[8] = {0};
    // int VV_ref[8] = {0};
    // float VV_ref_time[8] = {0};

     Vtheta = atan2(V_b, V_a);
    // svm->Vtheta = atan2(V_b, V_a);
    svm->Vtheta = Vtheta;

    if((Vtheta >= 0) && (Vtheta < PI2_3)){
        sector = 1;
        Fu = 0;
        Fv = 0; 
        Fw = 1; 
    }else 
    if((Vtheta >= PI2_3) && (Vtheta <= PI)){
        sector = 2;
        Fu = 1;
        Fv = 0; 
        Fw = 0; 
    }else
    if((Vtheta < -PI2_3) && (Vtheta >= -PI)){
        sector = 2;
        Fu = 1;
        Fv = 0; 
        Fw = 0; 
    }else
    if((Vtheta < 0) && (Vtheta >= -PI2_3)){
        sector = 3;
        Fu = 0;
        Fv = 1; 
        Fw = 0; 
    }

    tu = Ts/sqrt(2)/Vdc*((Fv+Fw)*sqrt(3)*V_a+(-Fv+Fw)*V_b);
    tv = Ts/sqrt(2)/Vdc*((-Fu)*sqrt(3)*V_a+(2*Fw+Fu)*V_b);
    tw = Ts/sqrt(2)/Vdc*((-Fu)*sqrt(3)*V_a+(-Fu-2*Fv)*V_b);

    if(sector == 1){

        if(tu > tv){
            svm->VV_ref[0] = 2;
            svm->VV_ref[1] = 1;
            svm->VV_ref[2] = 0;
            svm->VV_ref[3] = 1;
            svm->VV_ref[4] = 2;
            svm->VV_ref[5] = 2;
            svm->VV_ref[6] = 2;

            Vt[0] = tv*0.5;
            Vt[1] = (tu - tv)*0.5;
            Vt[2] = Ts-tv-(tu-tv);
            Vt[3] = (tu - tv)*0.5;
            Vt[4] = tv*0.5;
        
            svm->VV_ref_time[0] = Vt[0];
            svm->VV_ref_time[1] = svm->VV_ref_time[0]+Vt[1];
            svm->VV_ref_time[2] = svm->VV_ref_time[1]+Vt[2];
            svm->VV_ref_time[3] = svm->VV_ref_time[2]+Vt[3];
            svm->VV_ref_time[4] = svm->VV_ref_time[3]+Vt[4];
            svm->VV_ref_time[5] = Ts;
            svm->VV_ref_time[6] = Ts;
        }
        if(tu < tv){
            svm->VV_ref[0] = 2;
            svm->VV_ref[1] = 3;
            svm->VV_ref[2] = 0;
            svm->VV_ref[3] = 3;
            svm->VV_ref[4] = 2;
            svm->VV_ref[5] = 2;
            svm->VV_ref[6] = 2;

            Vt[0] = tu*0.5;
            Vt[1] = (tv - tu)*0.5;
            Vt[2] = Ts-tu-(tv-tu);
            Vt[3] = (tv - tu)*0.5;
            Vt[4] = tu*0.5;
        
            svm->VV_ref_time[0] = Vt[0];
            svm->VV_ref_time[1] = svm->VV_ref_time[0]+Vt[1];
            svm->VV_ref_time[2] = svm->VV_ref_time[1]+Vt[2];
            svm->VV_ref_time[3] = svm->VV_ref_time[2]+Vt[3];
            svm->VV_ref_time[4] = svm->VV_ref_time[3]+Vt[4];
            svm->VV_ref_time[5] = Ts;
            svm->VV_ref_time[6] = Ts;
        }
    
    }
    if(sector == 2){

        if(tv > tw){
            svm->VV_ref[0] = 4;
            svm->VV_ref[1] = 3;
            svm->VV_ref[2] = 0;
            svm->VV_ref[3] = 3;
            svm->VV_ref[4] = 4;
            svm->VV_ref[5] = 4;
            svm->VV_ref[6] = 4;

            Vt[0] = tw*0.5;
            Vt[1] = (tv - tw)*0.5;
            Vt[2] = Ts-tw-(tv-tw);
            Vt[3] = (tv - tw)*0.5;
            Vt[4] = tw*0.5;
        
            svm->VV_ref_time[0] = Vt[0];
            svm->VV_ref_time[1] = svm->VV_ref_time[0]+Vt[1];
            svm->VV_ref_time[2] = svm->VV_ref_time[1]+Vt[2];
            svm->VV_ref_time[3] = svm->VV_ref_time[2]+Vt[3];
            svm->VV_ref_time[4] = svm->VV_ref_time[3]+Vt[4];
            svm->VV_ref_time[5] = Ts;
            svm->VV_ref_time[6] = Ts;
        }
        if(tv < tw){
            svm->VV_ref[0] = 4;
            svm->VV_ref[1] = 5;
            svm->VV_ref[2] = 0;
            svm->VV_ref[3] = 5;
            svm->VV_ref[4] = 4;
            svm->VV_ref[5] = 4;
            svm->VV_ref[6] = 4;

            Vt[0] = tv*0.5;
            Vt[1] = (tw - tv)*0.5;
            Vt[2] = Ts-tv-(tw-tv);
            Vt[3] = (tw - tv)*0.5;
            Vt[4] = tv*0.5;
        
            svm->VV_ref_time[0] = Vt[0];
            svm->VV_ref_time[1] = svm->VV_ref_time[0]+Vt[1];
            svm->VV_ref_time[2] = svm->VV_ref_time[1]+Vt[2];
            svm->VV_ref_time[3] = svm->VV_ref_time[2]+Vt[3];
            svm->VV_ref_time[4] = svm->VV_ref_time[3]+Vt[4];
            svm->VV_ref_time[5] = Ts;
            svm->VV_ref_time[6] = Ts;
        }
    
    }
    if(sector == 3){

        if(tw > tu){
            svm->VV_ref[0] = 6;
            svm->VV_ref[1] = 5;
            svm->VV_ref[2] = 0;
            svm->VV_ref[3] = 5;
            svm->VV_ref[4] = 6;
            svm->VV_ref[5] = 6;
            svm->VV_ref[6] = 6;

            Vt[0] = tu*0.5;
            Vt[1] = (tw - tu)*0.5;
            Vt[2] = Ts-tu-(tw-tu);
            Vt[3] = (tw - tu)*0.5;
            Vt[4] = tu*0.5;
        
            svm->VV_ref_time[0] = Vt[0];
            svm->VV_ref_time[1] = svm->VV_ref_time[0]+Vt[1];
            svm->VV_ref_time[2] = svm->VV_ref_time[1]+Vt[2];
            svm->VV_ref_time[3] = svm->VV_ref_time[2]+Vt[3];
            svm->VV_ref_time[4] = svm->VV_ref_time[3]+Vt[4];
            svm->VV_ref_time[5] = Ts;
            svm->VV_ref_time[6] = Ts;
        }
        if(tw < tu){
            svm->VV_ref[0] = 6;
            svm->VV_ref[1] = 1;
            svm->VV_ref[2] = 0;
            svm->VV_ref[3] = 1;
            svm->VV_ref[4] = 6;
            svm->VV_ref[5] = 6;
            svm->VV_ref[6] = 6;

            Vt[0] = tw*0.5;
            Vt[1] = (tu - tw)*0.5;
            Vt[2] = Ts-tw-(tu-tw);
            Vt[3] = (tu - tw)*0.5;
            Vt[4] = tw*0.5;
        
            svm->VV_ref_time[0] = Vt[0];
            svm->VV_ref_time[1] = svm->VV_ref_time[0]+Vt[1];
            svm->VV_ref_time[2] = svm->VV_ref_time[1]+Vt[2];
            svm->VV_ref_time[3] = svm->VV_ref_time[2]+Vt[3];
            svm->VV_ref_time[4] = svm->VV_ref_time[3]+Vt[4];
            svm->VV_ref_time[5] = Ts;
            svm->VV_ref_time[6] = Ts;
        }
    
    }

    svm->sector = sector;

    svm->tu = tu;
    svm->tv = tv;
    svm->tw = tw;

    return;
}

void SwDuration(Pulse_Pattern* Pulse, float Ts) {
    int i;
    int j=1;
    // 0～pi/3 rad, pi/3～pi/6 rad, pi/6~～pi/2 radの3区間に分類
	int count_u = 0, count_v = 0, count_w = 0; 

    // 初期化
    for (i = 0; i <= Angle_Num; i++) {
		Pulse->alpha_u[i] = 0;
        Pulse->alpha_v[i] = 0;
        Pulse->alpha_w[i] = 0;
        Pulse->t_u[i] = 0;
        Pulse->t_v[i] = 0;
        Pulse->t_w[i] = 0;
	}

    // 分類処理
    for (i = 0; i < 8; i++) {
        if (Pulse->alpha[i] < 0.523598) {
            Pulse->alpha_u[count_u++] = Pulse->alpha[i]; // 範囲1に格納(0～pi/3)
        } else if (Pulse->alpha[i] < PI13) {
            Pulse->alpha_w[count_w++] = Pulse->alpha[i]; // 範囲2に格納(pi/3～pi/6)
        } else if (Pulse->alpha[i] < PI12) {
            Pulse->alpha_v[count_v++] = Pulse->alpha[i]; // 範囲3に格納(pi/6~～pi/2)
        }
    }
    // ↑このときcountは0から始めり最大8にインクリメント。つまり角度の数になる。
    // t_u,v,wは[0]～[count]、alpha_u,v,wは[0]～[count-1]の範囲で値を持つ

    // u相のオンオフ出力時間を計算
    Pulse->t_u[0] = Pulse->alpha_u[0]/PI02*Ts;
    for(i=1; i<count_u; ++i){
        Pulse->t_u[i] = (Pulse->alpha_u[i]-Pulse->alpha_u[i-1])/PI02*Ts;
    }
    Pulse->t_u[count_u] = (PI16-Pulse->alpha_u[count_u-1])/PI02*Ts;

    // v相のオンオフ出力時間を計算
    Pulse->t_v[0] = (Pulse->alpha_v[0]-PI13)/PI02*Ts;
    for(i=1; i<count_v; ++i){
        Pulse->t_v[i] = (Pulse->alpha_v[i]-Pulse->alpha_v[i-1])/PI02*Ts;
    }
    Pulse->t_v[count_v] = (PI12-Pulse->alpha_v[count_v-1])/PI02*Ts;

    // w相のオンオフ出力時間を計算
    Pulse->t_w[0] = (Pulse->alpha_w[0]-PI16)/PI02*Ts;
    for(i=1; i<count_w; ++i){
        Pulse->t_w[i] = (Pulse->alpha_w[i]-Pulse->alpha_w[i-1])/PI02*Ts;
    }
    Pulse->t_w[count_w] = (PI13-Pulse->alpha_w[count_w-1])/PI02*Ts;
    // 逆順にする処理
    // (配列の最初の要素と最後の要素を交換し、次に2番目の要素と倒数2番目の要素を交換を繰り返す)
    for (i = 0; i <= count_w / 2; ++i) {
        // i番目と (count_w - 1 - i)番目の要素を交換
        float temp = Pulse->t_w[i];
        Pulse->t_w[i] = Pulse->t_w[count_w - i];
        Pulse->t_w[count_w - i] = temp;
    }
}

void VV_Pattern(Pulse_Pattern* Pulse) {

    int len_u = Angle_Num+1;
    int len_v = Angle_Num+1;
    int len_w = Angle_Num+1;
    int idx_u = 0, idx_v = 0, idx_w = 0;
    float current_time = 0.0f;
    int num_patterns = 0;

    // すべての時間を統合して処理
    while (idx_u < len_u || idx_v < len_v || idx_w < len_w) {
        // 次のイベント時間を決定
        float next_time = 1e9; // 非常に大きな値
        if (idx_u < len_u) next_time = (next_time > Pulse->t_u[idx_u]) ? Pulse->t_u[idx_u] : next_time;
        if (idx_v < len_v) next_time = (next_time > Pulse->t_v[idx_v]) ? Pulse->t_v[idx_v] : next_time;
        if (idx_w < len_w) next_time = (next_time > Pulse->t_w[idx_w]) ? Pulse->t_w[idx_w] : next_time;

        // 現在の状態を計算
        int current_state = 0;
        if (idx_u % 2 == 0) current_state |= 0x01; // uがオンなら
        if (idx_v % 2 == 0) current_state |= 0x02; // vがオンなら
        if (idx_w % 2 == 0) current_state |= 0x04; // wがオンなら

        // スイッチングパターンと持続時間を記録
        Pulse->VV_Num[num_patterns] = current_state;
        Pulse->VV_time[num_patterns] = next_time - current_time;
        (num_patterns)++;

        // 時刻を更新
        current_time = next_time;

        // イベントインデックスを更新
        if (idx_u < len_u && Pulse->t_u[idx_u] == next_time) idx_u++;
        if (idx_v < len_v && Pulse->t_v[idx_v] == next_time) idx_v++;
        if (idx_w < len_w && Pulse->t_w[idx_w] == next_time) idx_w++;
    }

}


void VValt(int *VV_alt, int sector) {
    if (sector == 1) {
        VV_alt[0] = 6;
        VV_alt[1] = 5;
        VV_alt[2] = 7;
    }
    if (sector == 2) {
        VV_alt[0] = 6;
        VV_alt[1] = 1;
        VV_alt[2] = 7;
    }
    if (sector == 3) {
        VV_alt[0] = 1;
        VV_alt[1] = 6;
        VV_alt[2] = 0;
    }
    if (sector == 4) {
        VV_alt[0] = 1;
        VV_alt[1] = 2;
        VV_alt[2] = 0;
    }
    if (sector == 5) {
        VV_alt[0] = 2;
        VV_alt[1] = 1;
        VV_alt[2] = 7;
    }
    if (sector == 6) {
        VV_alt[0] = 2;
        VV_alt[1] = 3;
        VV_alt[2] = 7;
    }
    if (sector == 7) {
        VV_alt[0] = 3;
        VV_alt[1] = 2;
        VV_alt[2] = 0;
    }
    if (sector == 8) {
        VV_alt[0] = 3;
        VV_alt[1] = 4;
        VV_alt[2] = 0;
    }
    if (sector == 9) {
        VV_alt[0] = 4;
        VV_alt[1] = 3;
        VV_alt[2] = 7;
    }
    if (sector == 10) {
        VV_alt[0] = 4;
        VV_alt[1] = 5;
        VV_alt[2] = 7;
    }
    if (sector == 11) {
        VV_alt[0] = 5;
        VV_alt[1] = 4;
        VV_alt[2] = 0;
    }
    if (sector == 12) {
        VV_alt[0] = 5;
        VV_alt[1] = 6;
        VV_alt[2] = 0;
    }
}