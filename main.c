#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"

int main(void) {
    uint8_t i; // Variable para iterar a través de los espacios de estacionamiento
    uint32_t parkingStatus; // Variable para almacenar el estado actual de un espacio de estacionamiento
    uint8_t availableSpaces = 4, prevAvailableSpaces = 4; // Inicializa variables para contar los espacios disponibles y almacenar el valor previo
    uint32_t prevParkingStatus[4] = {0, 0, 0, 0}; // Almacena el estado anterior de cada espacio de estacionamiento

    // Configurar el reloj del sistema a 40 MHz usando el PLL
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Habilitar los periféricos GPIOB, GPIOF, UART0 y GPIOA
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configurar los pines PB0 - PB3 como entradas y PF1 - PF4 como salidas
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);

    // Configurar UART0 con los pines PA0 y PA1
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configurar UART0 con una velocidad de 115200 baudios, 8 bits de datos, 1 bit de parada y sin paridad
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
    while (1) {
        // Iterar a través de los 4 espacios de estacionamiento
        for (i = 0; i < 4; i++) {
            // Leer el estado del receptor de infrarrojos correspondiente al espacio de estacionamiento actual
            parkingStatus = GPIOPinRead(GPIO_PORTB_BASE, 1 << i);

            // Verifica si el estado del espacio de estacionamiento ha cambiado
            if (parkingStatus != prevParkingStatus[i]) {
                // Si el espacio está ocupado, decrementa la cuenta de espacios disponibles
                if (parkingStatus) {
                    availableSpaces--;
                } else {
                    // Si el espacio está desocupado, incrementa la cuenta de espacios disponibles
                    availableSpaces++;
                }

                // Actualiza el estado anterior del espacio de estacionamiento
                prevParkingStatus[i] = parkingStatus;
            }

            // Enciende el LED rojo si el espacio está ocupado y el LED verde si está desocupado
            if (parkingStatus) {
                GPIOPinWrite(GPIO_PORTF_BASE, (1 << (i + 1)), 0x00);
            } else {
                GPIOPinWrite(GPIO_PORTF_BASE, (1 << (i + 1)), (1 << (i + 1)));
            }
        }

        // Si el número de espacios disponibles ha cambiado, envía el nuevo valor a través de UART
        if (availableSpaces != prevAvailableSpaces) {
            UARTCharPut(UART0_BASE, availableSpaces); // Envía el número de espacios disponibles a través de UART
            prevAvailableSpaces = availableSpaces; // Actualiza el número previo de espacios disponibles
        }
    }
}
