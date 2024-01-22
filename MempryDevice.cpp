#include "MemoryDevice.h"

MemoryDevice::MemoryDevice( I2C& i2c_bus, char i2c_address ) : i2c( i2c_bus ), i2c_addr( i2c_address )
{
}

MemoryDevice::~MemoryDevice()
{
}

void MemoryDevice::write( int offset, const char *data, int length )
 {
    length  = (length < (WR_BUFF_SIZE - 1)) ? length : (WR_BUFF_SIZE - 1);

    memcpy( wr_buff + 1, data, length );
    wr_buff[ 0 ]    = offset;

    i2c.write( i2c_addr, wr_buff, length + 1 );
}

void MemoryDevice::read( int offset, char *data, int length )
{
    i2c.write( i2c_addr, (char *)&offset, 1, false );
    i2c.read( i2c_addr, data, length );
}
