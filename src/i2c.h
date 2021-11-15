#ifndef I2C_H
#define I2C_H

void Read_from(unsigned char SLA, unsigned char MEMADDRESS);
unsigned char Read_data();
void write(unsigned char data);
void StopI2C_Trans();
void StartI2C_Trans(unsigned char SLA);
void initI2C();

#endif