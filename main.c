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
    uint8_t availableSpaces = 4; // Inicializa variable para contar los espacios disponibles
    uint32_t prevParkingStatus[4] = {0, 0, 0, 0}; // Almacena el estado anterior de cada espacio de estacionamiento

    // Configurar el reloj del sistema a 40 MHz usando el PLL
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Habilitar los periféricos GPIOB, GPIOF y GPIOD
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    // Configurar los pines PB0 - PB3 como entradas y PF1 - PF4 y PD0 - PD3 como salidas
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

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
            if (i < 2) { // para los primeros dos espacios de estacionamiento (PF1 - PF4)
                if (parkingStatus) {
                    GPIOPinWrite(GPIO_PORTF_BASE, (1 << (2*i + 1)), 0x00); // LED verde
                    GPIOPinWrite(GPIO_PORTF_BASE, (1 << (2*i + 2)), (1 << (2*i + 2))); // LED rojo
                } else {
                    GPIOPinWrite(GPIO_PORTF_BASE, (1 << (2*i + 1)), (1 << (2*i + 1))); // LED verde
                    GPIOPinWrite(GPIO_PORTF_BASE, (1 << (2*i + 2)), 0x00); // LED rojo
                }
            } else { // para los últimos dos espacios de estacionamiento (PD0 - PD3)
                if (parkingStatus) {
                    GPIOPinWrite(GPIO_PORTD_BASE, (1 << (2*(i - 2))), 0x00); // LED verde
                    GPIOPinWrite(GPIO_PORTD_BASE, (1 << (2*(i - 2) + 1)), (1 << (2*(i - 2) + 1))); // LED rojo
                } else {
                    GPIOPinWrite(GPIO_PORTD_BASE, (1 << (2*(i - 2))), (1 << (2*(i - 2)))); // LED verde
                    GPIOPinWrite(GPIO_PORTD_BASE, (1 << (2*(i - 2) + 1)), 0x00); // LED rojo
                }
            }
        }
    }
}
