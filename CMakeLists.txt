cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
  
project(my_project C CXX ASM)
set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

#pico-sdkの有効化
pico_sdk_init()

#プロジェクトに含めるファイルの設定
add_executable(my_project
        main.c 
        i2c_lcd
        ) 

 #使用するライブラリを設定
target_link_libraries(my_project 
        pico_stdlib 
        hardware_gpio
        hardware_adc
        hardware_pwm
        hardware_uart
        hardware_i2c
        ) 

pico_add_extra_outputs(my_project)
 
#stdioのprintfで出力する先の設定 USBをEnableにしてUARTをDisableにする。
pico_enable_stdio_usb(my_project 1)
pico_enable_stdio_uart(my_project 0)