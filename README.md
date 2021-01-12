# STM32G474_BOOTLOADER

Bootloader application:
Works on NUCLEO-G474RE board
STM32G4 Microcontroller.

Main application can be found in NG474-BOOT.

Two user applications for test:
 * GPIO_InfiniteLedToggling - toggles the board LED ON/OFF
 * UART_Hello - sends "Hello from user app!" using USART1 (virtual com port)

Data loading is made using FDCAN1, 80Mhz, 1Mb/s

When MCU is reset, bootloader app is first started.
It waits for "enter bootloader" command from host application: id 0x00.
After enter bootloader, flash is erased and data can be sent.
Data ID's must be consecutive from 0x100 to 0x11F. (32 data frames x 8 bytes each)
Booloader writes to flash using Flash_fast_program 32 x 64bit values.

After all data has been received, 0x90 command jumps to user application.

A Python script is available in Python folder to automatically generate Transmit 
List Files (*.xmt) for the PCAN Viewer application starting from a binary file. 
These files can be loaded into the application and an almost auto transmition
can be performed.

The bootloader application also sends a checksum (id: 0x300) and a 
Flash write succesfull (id: 0x400) after each 32 frames have been received. 

