#include <stdio.h>
#include "pico/stdlib.h"  
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "i2c_lcd.h"

#define  ON_BORD_LED    (25)
#define  LED_1          (15)
#define  ADC0_PIN       (26)

char moji[] ={ "HELL WORLD"};
char addr = 0x3d;

int main(){
    int timer0;
    struct repeating_timer timer;
    unsigned short adc0_data;
    unsigned short pwm0_duty;

    char i2c_send_data[32];

    /*GPIO設定設定*/
    gpio_init(ON_BORD_LED);
    gpio_set_dir(ON_BORD_LED, GPIO_OUT);
    /*ADC端子設定*/
    adc_init(); 
    adc_gpio_init(ADC0_PIN);                //ADC0(GPIO26)の端子設定
    adc_select_input(0);                    //ADC0を設定

    /*I2C設定*/
    i2c_init(i2c_default, 100 * 1000);                          //I2C初期化（100kHz）
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C); //GPIO4をSDAに設定
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C); //GPIO5をSCLに設定
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);                     //GPIOプルアップ設定
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);                     //GPIOプルアップ設定
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    /*stdio初期化*/
    stdio_init_all();

    /*キャラクタディスプレイの初期化*/
    i2clcd_init_display(addr);  
    i2clcd_contrast_set(addr,0xe0); //輝度を設定

    i2clcd_write_string(addr,"Hell word");    
    sleep_ms(1000);
    i2clcd_writeCommand(addr,0x01); //画面クリア
    while(1){
        if( timer0++ >= 999 ){
            timer0=0;
        }
 
        adc0_data = adc_read();                                 //ADC完了まで処理が停止するので注意
        printf("Timer:%3d ADC0:%5d \n",timer0,adc0_data);       //USB経由でシリアル出力（TeraTarm等で見る用）    

        /*キャラクタディスプレイに表示*/    
        i2clcd_writeCommand(addr,0x80); //1行目の左端に移動
        sprintf(i2c_send_data,"Timer:%3d ",timer0);
        i2clcd_write_string(addr,i2c_send_data);
        i2clcd_writeCommand(addr,0x20 + 0x80); //2 行目の左端に移動
        sprintf(i2c_send_data,"ADC0:%4d ",adc0_data);
        i2clcd_write_string(addr,i2c_send_data);

        sleep_ms(100);
    } 
    return 0;
}
