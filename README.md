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
### Raw data
Sample data is available [here](https://github.com/teddokano/i2c_clock_stretch_test_controller_mbed/blob/master/resource/sample_transfer.sal).  
The data is in Saleae logic analyzer format. It can be opened by [Logic 2](https://www.saleae.com/downloads/) application. 

### Snapshots from the logic analyzer data.  

#### Writing memory
The controller access to target address: **0x5A**. The target performs a memory device. 
One byte write after the target address is offset for writing start. Following data are written into memory.  
In 1st tansfer, 16 byte data (values: 0x00 .. 0x0F) are written from offset 0.  
2nd transfer set the offset as 8 and writes 8 bytes (values: 0xFF .. 0xF8).  
![write0.png](https://github.com/teddokano/i2c_clock_stretch_test_controller_mbed/blob/master/resource/write0.png)

#### Writing and reading memory
Next picture shows the 2nd write transfer and read transfer.  
After the 2nd transfer, the only one byte transfer done to set offset for reading start. After the offset setting, read transfer performed.  
The read transfer shows the data are written by 1st and 2nd write transfers as 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, **0xFF**, **0xFE** ..
![write_and_read.png](https://github.com/teddokano/i2c_clock_stretch_test_controller_mbed/blob/master/resource/write_and_read.png)

#### Setting clock stretch
Clock stretch setting can be done with target sub-address: **0x5B**.  
A byte rgiht after the target address specifies types of stretch. 0 = Stretch at ACK for target address, 1 = Stretch at ACK for written data, 2 = Stretch before next byte for reading.  
The 1st stretch setting tranfer is for "ACK for target address" with value of 0x64 (100 in decimal). This means the stretch will be made for 100 micro-seconds.  
The 2nd stretch setting tranfer is for "ACK for written data" with value of 0x32 (50 in decimal).  
The 3rd is for "Stretch before next byte for reading" with value of 0x03E8 (1000 in decimal). Since this setting transfer is done in little-endian order, the transfer in order of 0xE8 and 0x03 is interpreted as 0x03E8. The value can be 4 baytes in maximum.  
The transfer for the setting value is not require to be 4 bytes. The higher bytes which were not transferred is treated as 0x00.  

After those stretch setting, next memory access are having the clock stretch for specified durations.  
The 0x5A address ACK has 100 micro-sedonds stretch, wirte data ACK comes after 50 micro-sedonds stretch. 
![seting_stretch.png](https://github.com/teddokano/i2c_clock_stretch_test_controller_mbed/blob/master/resource/seting_stretch.png)

#### Data reading stretch with 1000 micro-seconds
1000 micro-seconds stretch is set for data read. The stretch keeps the SCL LOW for 1 mili-second at every next byte transfer start.  
![read_w_stretch.png](https://github.com/teddokano/i2c_clock_stretch_test_controller_mbed/blob/master/resource/read_w_stretch.png)
