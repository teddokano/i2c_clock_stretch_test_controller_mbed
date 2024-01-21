#include "mbed.h"
#include <cstdint>
#include <stdexcept>

#include "MemoryDevice.h"

#define DATALENGTH          16
#define MEMORY_DEVICE_ADDR  (0x5A << 1)
#define DEVICE_SETTING_ADDR (0x5B << 1)

enum {
    STRETCH_FOR_ADDRESS, 
    STRETCH_FOR_WRITE, 
    STRETCH_FOR_READ
};

void show_data( char *data, int length );
void set_clock_stretch_us( char type, unsigned int wait );

I2C             i2c( I2C_SDA, I2C_SCL );
MemoryDevice    mem( i2c, MEMORY_DEVICE_ADDR );

int main()
{
    char write_data[ DATALENGTH ];
    char overwrite_data[ DATALENGTH / 2 ];
    char read_data[ DATALENGTH ];

    i2c.frequency( 1000000 );

    for ( int i = 0; i < DATALENGTH; i++ )
        *(write_data + i)  = i;

    for ( int i = 0; i < DATALENGTH / 2; i++ )
        *(overwrite_data + i)  = ~i;

    while ( true )
    {
        //  transfer without clock stretch
        //
        set_clock_stretch_us( STRETCH_FOR_ADDRESS,      0 );    //  micro seconds
        set_clock_stretch_us( STRETCH_FOR_WRITE,        0 );    //  micro seconds
        set_clock_stretch_us( STRETCH_FOR_READ,         0 );    //  micro seconds

        mem.write(  0,  write_data,      sizeof( write_data )      );
        mem.write(  8,  overwrite_data,  sizeof( overwrite_data )  );
        mem.read(   0,  read_data,       sizeof( read_data )       );

        //  show_data( read_data, sizeof( read_data ) );


        //  transfer with clock stretch
        //
        set_clock_stretch_us( STRETCH_FOR_ADDRESS,    100 );    //  micro seconds
        set_clock_stretch_us( STRETCH_FOR_WRITE,       50 );    //  micro seconds
        set_clock_stretch_us( STRETCH_FOR_READ,      1000 );    //  micro seconds

        mem.write(  0,  write_data,      sizeof( write_data )      );
        mem.write(  8,  overwrite_data,  sizeof( overwrite_data )  );
        mem.read(   0,  read_data,       sizeof( read_data )       );

        //  show_data( read_data, sizeof( read_data ) );
    }
}

void set_clock_stretch_us( char type, unsigned int wait )
{
    char    data[ 5 ];
    int     length;

    data[ 0 ]   = type;
    data[ 1 ]   = (wait >>  0) & 0xFF;
    data[ 2 ]   = (wait >>  8) & 0xFF;
    data[ 3 ]   = (wait >> 16) & 0xFF;
    data[ 4 ]   = (wait >> 24) & 0xFF;

#if 1   //  To remove unnecessary data transfer
    for ( length = 4; 1 < length; length-- )
        if ( data[ length ] )
            break;

    i2c.write( DEVICE_SETTING_ADDR, data, length + 1 );
#else
    i2c.write( DEVICE_SETTING_ADDR, data, sizeof( data ) );
#endif
}

void show_data( char *data, int length )
{
    for ( int i = 0; i < length; i++ )
        printf( " %02X", *data++ );
    
    printf( "\r\n" );
}
