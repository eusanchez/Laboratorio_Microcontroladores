//Librerías
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/usart.h> 
#include <libopencm3/stm32/adc.h>
#include <libopencm3/cm3/nvic.h>
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "lcd-spi.h"
#include "gfx.h"

//Defines
#define GYR_RNW			(1 << 7) /* Write when zero  (ahorita en 1, read)*/  
#define GYR_MNS			(1 << 6) /* Multiple reads when 1 */
#define GYR_I_AM_AM_I		0x0F
#define GYR_OUT_TEMP		0x26
#define GYR_STATUS_REG		0x27
#define GYR_CTRL_REG1		0x20
#define GYR_CTRL_REG1_PD	(1 << 3)
#define GYR_CTRL_REG1_XEN	(1 << 1)
#define GYR_CTRL_REG1_YEN	(1 << 0)
#define GYR_CTRL_REG1_ZEN	(1 << 2)
#define GYR_CTRL_REG1_BW_SHIFT	4
#define GYR_CTRL_REG4		0x23
#define GYR_CTRL_REG4_FS_SHIFT	4
#define GYR_OUT_X_L		0x28
#define GYR_OUT_X_H		0x29
#define GYR_OUT_Y_L		0x2A
#define GYR_OUT_Y_H		0x2B
#define GYR_OUT_Z_L		0x2C
#define GYR_OUT_Z_H		0x2D
#define L3GD20_SENSITIVITY_250DPS  (0.00875F)      
#define L3GD20_SENSITIVITY_500DPS  (0.0175F)       
#define L3GD20_SENSITIVITY_2000DPS (0.070F)        
#define L3GD20_DPS_TO_RADS         (0.017453293F)  

// Variables globales
int comms_enable, alarm_enable = 0;

//Declaracion de funciones
void global_setup(void);
void spi_setup(void); 
void button_setup(void);
void blinkingLED_setup(void);
void adc_setup(void);
uint16_t read_adc_naiive(uint8_t channel);


void spi_setup(void)
{   
    //Perifericos reloj
    rcc_periph_clock_enable(RCC_SPI5);
    rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOF);
	

    //GPIO
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
    gpio_set(GPIOC, GPIO1);
    gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO7 | GPIO8 | GPIO9);   
	gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);

    //Set de SPI inicializacion
    spi_set_master_mode(SPI5);
	spi_set_baudrate_prescaler(SPI5, SPI_CR1_BR_FPCLK_DIV_64);
	spi_set_clock_polarity_0(SPI5);
	spi_set_clock_phase_0(SPI5);
	spi_set_full_duplex_mode(SPI5);
	spi_set_unidirectional_mode(SPI5);
	spi_enable_software_slave_management(SPI5);
	spi_send_msb_first(SPI5);
	spi_set_nss_high(SPI5);
    SPI_I2SCFGR(SPI5) &= ~SPI_I2SCFGR_I2SMOD;
	spi_enable(SPI5);
}

void adc_setup(void){
	rcc_periph_clock_enable(RCC_ADC1);
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2);
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO6);

	adc_power_off(ADC1);
	adc_disable_scan_mode(ADC1);
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);

	adc_power_on(ADC1);
}

void button_setup(void){
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Ponemos el GPIO0 (pin PA0) del puerto A como input*/
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
}

void blinkingLED_setup(void){
	/* Enable GPIOG clock. */
	rcc_periph_clock_enable(RCC_GPIOG);

	/* Set GPIO13 (in GPIO port G) to 'output push-pull' (pin PG13, el cual
	corresponde a la una LED) */
	gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO13);

	/* LED de emergencia de bateria*/
	gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO14);
}

void global_setup(){
	clock_setup();
	console_setup(115200); //valor a ingresar en el archivo de Python (baudrate).
	
    spi_setup();
	button_setup();
	blinkingLED_setup();
	adc_setup();

    gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, GYR_CTRL_REG1); 
	spi_read(SPI5);
	spi_send(SPI5, GYR_CTRL_REG1_PD | GYR_CTRL_REG1_XEN |
			GYR_CTRL_REG1_YEN | GYR_CTRL_REG1_ZEN |
			(3 << GYR_CTRL_REG1_BW_SHIFT));
	spi_read(SPI5);
	gpio_set(GPIOC, GPIO1); 

    gpio_clear(GPIOC, GPIO1);
	spi_send(SPI5, GYR_CTRL_REG4);
	spi_read(SPI5);
	spi_send(SPI5, (1 << GYR_CTRL_REG4_FS_SHIFT));
	spi_read(SPI5);
	gpio_set(GPIOC, GPIO1);


}

uint16_t read_adc_naiive(uint8_t channel){
	uint8_t channel_array[16];
	channel_array[0] = channel;
	adc_set_regular_sequence(ADC1, 1, channel_array);
	adc_start_conversion_regular(ADC1);
	while (!adc_eoc(ADC1));
	uint16_t reg16 = adc_read_regular(ADC1);
	return reg16;
}

int main(void)
{
	global_setup();
	sdram_init();
	lcd_spi_init();
	msleep(2000);
	gfx_init(lcd_draw_pixel, 240, 320);
	gfx_fillScreen(LCD_WHITE);
	gfx_setTextSize(2);
	gfx_setCursor(15, 25);
	gfx_puts("ACELERACIONES");
	gfx_setTextSize(1);
	gfx_setCursor(15, 49);
	gfx_puts("Se van a desplegar los");
	gfx_setCursor(15, 60);
	gfx_puts("valores de x, y y z.");
	lcd_show_frame();
	msleep(2000);
	gfx_setTextColor(LCD_WHITE, LCD_BLACK);
	gfx_setTextSize(3);

	 

	while (1) {
		if (gpio_get(GPIOA, GPIO0)) {
			for (int i = 0; i < 3000000; i++) {	
				__asm__("nop");
			}
			comms_enable = ~comms_enable;
		}

		for (int i = 0; i < 3000000; i++) {	
				__asm__("nop");
			}

		uint8_t t, I_AM;
        int16_t X, Y, Z, V;
		char X_str[100], Y_str[100], Z_str[100], V_str[100];

		gpio_clear(GPIOC, GPIO1);             
		spi_send(SPI5, GYR_I_AM_AM_I | GYR_RNW);
		spi_read(SPI5); 
		spi_send(SPI5, 0);    
		I_AM=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_STATUS_REG | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		t=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_TEMP | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		t=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);  

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_X_L | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		X=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_X_H | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		X|=spi_read(SPI5) << 8;
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Y_L | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		Y=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Y_H | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		Y|=spi_read(SPI5) << 8;
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Z_L | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		Z=spi_read(SPI5);
		gpio_set(GPIOC, GPIO1);

		gpio_clear(GPIOC, GPIO1);
		spi_send(SPI5, GYR_OUT_Z_H | GYR_RNW);
		spi_read(SPI5);
		spi_send(SPI5, 0);
		Z|=spi_read(SPI5) << 8;
		gpio_set(GPIOC, GPIO1);

		// Se escalan las lecturas para hacer datos mas manejables.
        X = X*L3GD20_SENSITIVITY_500DPS;
        Y = Y*L3GD20_SENSITIVITY_500DPS;
        Z = Z*L3GD20_SENSITIVITY_500DPS;

		V = read_adc_naiive(2)*0.7407407;
		uint16_t input_adc1 = read_adc_naiive(6);

		/*alarma de bateria baja, se activa si la bateria llega a 78% */ 
		if (V < 78) {
			alarm_enable = ~alarm_enable; 
			gpio_toggle(GPIOG, GPIO14);
		}
		
		sprintf(X_str, "%d", X);
		sprintf(Y_str, "%d", Y);
		sprintf(Z_str, "%d", Z);
		sprintf(V_str, "%d", V);

		if (comms_enable) {
			gpio_toggle(GPIOG, GPIO13);
			
			console_puts(X_str);
			console_puts("\t");
			console_puts(Y_str);
			console_puts("\t");
			console_puts(Z_str);
			console_puts("\t");
			console_puts(V_str);
			console_puts("\n");
		}
		else gpio_clear(GPIOG, GPIO13);

		gfx_fillScreen(LCD_BLACK);
		gfx_setCursor(50, 36); // (COLUMNA, FILA)
		gfx_setTextSize(3);
		gfx_puts("DATOS");

		gfx_setCursor(15, 100);
		gfx_setTextSize(2);
		gfx_puts("X-Axis: ");
		gfx_setCursor(150, 100); 
		gfx_puts(X_str);

		gfx_setCursor(15, 125);
		gfx_puts("Y-Axis: ");
		gfx_setCursor(150, 125);
		gfx_puts(Y_str);

		gfx_setCursor(15, 150);
		gfx_puts("Z-Axis: ");
		gfx_setCursor(150, 150);
		gfx_puts(Z_str);

		gfx_setCursor(15, 175);
		gfx_setTextSize(1);
		if (comms_enable) gfx_puts("SERIAL COMMS: ON");
		else gfx_puts("SERIAL COMMS: OFF");

		gfx_setCursor(15, 200);
		gfx_setTextSize(1);
		gfx_puts("Battery Level: ");
		gfx_setCursor(150, 200);
		gfx_puts(V_str);
		
		lcd_show_frame();
	}

	return 0;
}