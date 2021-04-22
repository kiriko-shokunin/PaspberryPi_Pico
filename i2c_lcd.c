#include "hardware/i2c.h"

/*------------------------------------------------------------------*/
/* int i2clcd_write_string(char ,char* ); 　　　　　　　　　　　　　　　*/
/* I2C通信で0x40＋文字列を送信する関数　入力した文字列がLCDに表示される　　*/
/*   引数:　char addr　   | I2Cスレーブアドレス  　　　　　　　　　　　　*/
/*         char* in_data |  送信文字列ポインタ　先頭アドレスを入れる     */ 
/*   戻り値   int         |  0:正常　/ -1:文字列長すぎ                  */ 
/*------------------------------------------------------------------*/
int i2clcd_write_string(char addr,char* in_data){
    int i;
    char t_data[32];
    t_data[0] = 0x40;
    for(i=0; (*(in_data+i) != 0x00);i++){
        t_data[i+1]= *(in_data+i);
        if(i>32)return -1;
    }
    i++;
    i2c_write_blocking(i2c_default, addr, t_data, i, false);
    sleep_ms(1);
    return 0;
}

/*------------------------------------------------------------------*/
/* void i2clcd_writeCommand(char ,char );　　　　　　　　　　　　　　　*/
/* I2C通信で0x00＋コマンドを送信する関数一度に1バイト送信する           　*/
/*   引数:　char addr　   | I2Cスレーブアドレス  　　　　　　　　　　　　*/
/*         char t_command |  送信コマンドデータ（１BYTE）             */ 
/*   戻り値   void                                                   */ 
/*------------------------------------------------------------------*/
void i2clcd_writeCommand(char addr,char t_command){
    char t_data[2];
    t_data[0]=0x00;
    t_data[1]=t_command;
    i2c_write_blocking(i2c_default, addr, t_data, 2, false);
    sleep_ms(1);
}

/*------------------------------------------------------------------*/
/*  void i2clcd_contrast_set(char , int );　　　　　　　　　　　　　　　*/
/* I2C通信でLCDのコントラス設定をする関数                               */
/*   引数:　char addr　   | I2Cスレーブアドレス  　　　　　　　　　　　　*/
/*         char contrast |  コントラストデータ（１BYTE）               */ 
/*   戻り値   void                                                   */ 
/*------------------------------------------------------------------*/
void i2clcd_contrast_set(char addr, char contrast) {
    i2clcd_writeCommand(addr, 0x2a);//RE=1
    i2clcd_writeCommand(addr, 0x79);//SD=1
    i2clcd_writeCommand(addr, 0x81);//コントラストセット
    i2clcd_writeCommand(addr, contrast);//輝度設定
    i2clcd_writeCommand(addr, 0x78);//SD を０にもどす
    i2clcd_writeCommand(addr, 0x28); //2C=高文字　28=ノーマル
    sleep_ms(1);
}

/*------------------------------------------------------------------*/
/*  void i2clcd_init_display(char addr)　　　　　　　　　　　　　　    */
/* I2C通信でLCDの初期化をする関数   ＬＣＤの電源投入後この関数で初期化する*/
/*   引数:　char addr　   | I2Cスレーブアドレス  　　　　　　　　　　　　*/
/*         char contrast |  コントラストデータ（１BYTE）               */ 
/*   戻り値   void                                                   */ 
/*------------------------------------------------------------------*/
void i2clcd_init_display(char addr)
{
    sleep_ms(100);
    i2clcd_writeCommand(addr,0x28);//RE=0 IS=0
    i2clcd_writeCommand(addr,0x08); // DisplayをOFF
    i2clcd_writeCommand(addr,0x01); // Displayのメモリ全消去
    sleep_ms(100);
    i2clcd_writeCommand(addr,0x02); // カーソルをアドレス0x00に移動
    sleep_ms(1);
    i2clcd_writeCommand(addr,0x0c); // DisplayをON
    sleep_ms(1);
    i2clcd_writeCommand(addr,0x01); // Displayのメモリ全消去
    sleep_ms(100);
}