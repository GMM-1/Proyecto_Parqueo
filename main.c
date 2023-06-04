////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Librerias/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/adc.h"
#include "utils/ustdlib.h"

/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////definiciones/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#define XTAL 16000000 //Reloj externo de 16MHz

uint32_t i;
//variables de los espacios de parqueo
int8_t parq_1;
int8_t parq_2;
int8_t parq_3;
int8_t parq_4;
char sum_parq; // sera la suma de todos los parqueos para mandar por UART


//declaraciones de funciones
void config(void);
void sensor_value(void);
void sensor_UART(void);

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////codigo principal////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    config();
////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////Bucle infinito////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
    while(1)
    {
        sensor_value();
        sensor_UART();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////funciones////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void config(void)
{
        ///////////////////////////////setup oscilador////////////////////////////////////////////
        //Configuración del reloj a 40MHz
        SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

        ///////////////////////////////setup puertos//////////////////////////////////////////////
        //PORTA
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
        //PORTB
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
        //PORTD
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
        //PORTE
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));
        //PORTF
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

        //OUTPUTS
        GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7); // D4, D5, D6, D7
        GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6); // E4, E5, E6
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2); // F2
        //INPUTS
        GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); //A0, A1, A2, A3
        ///////////////////////////setup UARTs//////////////////////////////////////////////////////
        //UART0
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
        GPIOPinConfigure(GPIO_PA0_U0RX); //RX
        GPIOPinConfigure(GPIO_PA1_U0TX); //TX
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
        UARTEnable(UART0_BASE);
        //UART1
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
        GPIOPinConfigure(GPIO_PB0_U1RX); //RX
        GPIOPinConfigure(GPIO_PB1_U1TX); //TX
        GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
        UARTEnable(UART1_BASE);
}

void sensor_value(void)
{
    ///////////////////////////////PRIMER SENSOR////////////////////////////////////////////
    if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3) == 0)
    {
        parq_1 = 1; //parqueo ocupado
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4, 0);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_5, GPIO_PIN_5); //led ocupado
    }
    else
    {
        parq_1 = 0; //parqueo vacio
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4, GPIO_PIN_4); //led vacio
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_5, 0);
    }

    /////////////////////////SEGUNDO SENSOR////////////////////////////////////////////////
    if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2) == 0)
    {
        parq_2 = 1; //parqueo ocupado
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_PIN_7); //led ocupado
    }
    else
    {
        parq_2 = 0; //parqueo vacio
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6); //led vacio
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, 0);
    }

    ///////////////////////////TERCER SENSOR///////////////////////////////////////////////
    if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_1) == 0)
    {
        parq_3 = 1; //parqueo ocupado
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0);
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_PIN_5); //led ocupado
    }
    else
    {
        parq_3 = 0; //parqueo vacio
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_PIN_4); //led vacio
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0);
    }

    /////////////////////////CUARTO SENSOR//////////////////////////////////////////////////
    if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_0) == 0)
    {
        parq_4 = 1; //Variable en 1
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_6, 0);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2); // led ocupado
    }
    else
    {
        parq_4 = 0; //parqueo vacio
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_6, GPIO_PIN_6); //led vacio
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }
}


void sensor_UART() {
    uint8_t sum_parq = 0;

    // Ponderar los valores de cada parqueo
    sum_parq += parq_1 * 1;
    sum_parq += parq_2 * 2;
    sum_parq += parq_3 * 4;
    sum_parq += parq_4 * 8;

    UARTCharPutNonBlocking(UART1_BASE, sum_parq); // Mandar valores de parqueos a UART
}
