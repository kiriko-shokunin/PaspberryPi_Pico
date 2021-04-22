
#ifndef _I2C_LCD
#define _I2C_LCD

int i2clcd_write_string(char ,char* );
void i2clcd_writeCommand(char ,char );
void i2clcd_contrast_set(char , char );
void i2clcd_init_display(char );

#endif