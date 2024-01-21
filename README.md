# Test code for I²C target device emulator with variable clock stretching setting

## What is this?
This is controller side test code for [I²C target device emulator with variable clock stretching setting](https://github.com/teddokano/clock_stretching_on_lpcxpresso55s36_i2c_polling_b2b_slave).  
The code runs on [FRDM-K64F](https://os.mbed.com/platforms/FRDM-K64F/).  
I²C signals are on [I2C_SDA (= PTE25) and I2C_SCL (= PTE24) pins](https://os.mbed.com/platforms/FRDM-K64F/#board-pinout). 

## Environment for build
[Keil Studio Cloud](https://studio.keil.arm.com/)

## Demo setup
A FRDM-K64F board works as an I²C controller and an LPC55S36-EVK works as an I²C target device.  
Those boards need to be connected by wires for SDA, SCL and GND. The SDA and the SCL needed to be pulled-up.  
The pull-up may be connected to 3.3V of one of those board.  

Signal	|FRDM-K64F	|LPC55S36-EVK
---|---|---
SDA		|J2-18 (PTE25, I2C_SDA)	|J122-6
SCL		|J2-20 (PTE24, I2C_SCL)	|J122-12
GND		|J2-14, J3-12, J3-14, etc.	|J92-14
V3.3V for pull-up	|J3-8		|---

A picture blow shows connection of those boards. 
A bread board is used to pull-up SDA and SCL signal and connect boards and a logic-analyzer.  
![wiring.JPG](https://github.com/teddokano/i2c_clock_stretch_test_controller_mbed/blob/master/resource/wiring.JPG)
_Wire colors on each boards: Yellow=SDA, Blue=SCL and White=GND_

## Sample data
Sample data is available [here](https://github.com/teddokano/i2c_clock_stretch_test_controller_mbed/blob/master/resource/sample_transfer.sal).  
The data is in Saleae logic analyzer format. It can be opened by [Logic 2](https://www.saleae.com/downloads/) application. 
