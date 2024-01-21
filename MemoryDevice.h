#include "mbed.h"
#include <cstdint>

#define WR_BUFF_SIZE    257

class MemoryDevice
{
public:
    MemoryDevice( I2C& i2c_bus, char i2c_address );
    ~MemoryDevice();
    void write( int offset, const char *data, int length );
    void read( int offset, char *data, int length );

private:
	I2C&    i2c;
	char    i2c_addr;
    char    *wr_buff;
};

