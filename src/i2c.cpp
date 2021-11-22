
#include <avr/io.h>
#include "Arduino.h"
#include "i2c.h"

#define wait_for_completion while(!(TWCR & (1 << TWINT)));



void initI2C() {
  
  PRR0 &= ~(1<<PRTWI);  // wake up I2C module on AT2560 power management register
  
  TWSR |= (1 << TWPS0);  // prescaler = 4 (4 ^ 1)
  TWSR &= ~(1 << TWPS1); // 
  //𝑇𝑊𝐵𝑅=((𝐶𝑃𝑈 𝐶𝑙𝑜𝑐𝑘 𝑓𝑟𝑒𝑞𝑢𝑒𝑛𝑐𝑦)/(𝑆𝐶𝐿 𝑓𝑟𝑒𝑞𝑢𝑒𝑛𝑐𝑦)−16)/(2∗〖(4)〗^𝑇𝑊𝑃𝑆 )
  TWBR = 0xC6; // bit rate generator = 10k  (TWBR = 198)

  TWCR |= (1 << TWINT )| (1 << TWEN); // enable two wire interface
  // TWINT -> interrupt flag; flagged when reads or writes are completed
  // TWEN -> turn I2C module on

}


 
void StartI2C_Trans(unsigned char SLA) {
  // this function initiates a start condition and calls slave device with SLA
  
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // clear TWINT, intiate a start condition and enable
  // TWINT -> interrupt flag; flagged when reads or writes are completed
  // TWSTA -> start condition bit
  // TWEN -> turn I2C module on

  wait_for_completion;

  TWDR = (SLA << 1); // slave address + write bit '0'

  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action:clear flag and enable TWI
  wait_for_completion;
}



void StopI2C_Trans() {
  // this function sends a stop condition to stop I2C transmission

  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); //trigger action:  send stop condition

}



void write(unsigned char data){
  // this function loads the data passed into the I2C data register and transmits

  TWDR = data; //load data into TWDR register
  // This is the register address.

  TWCR = (1<<TWINT)|(1<<TWEN);  // trigger action:  clear flag and enable TWI
  
  wait_for_completion;
}



void Read_from(unsigned char SLA, unsigned char MEMADDRESS){
  // this function sets up reading from SLA at the SLA MEMADDRESS 

  StartI2C_Trans(SLA); // device address
 
  write(MEMADDRESS); // register address
  
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // restart to switch to read mode
  // TWINT -> interrupt flag; flagged when reads or writes are completed
  // TWSTA -> start condition bit
  // TWEN -> turn I2C module on

  wait_for_completion;



  TWDR = (SLA << 1) | 0x01; // 7 bit address for slave plus read bit
  TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// trigger with master sending ack
  wait_for_completion;
          Serial.println("Completed");

  TWCR = (1<< TWINT) | (1 << TWEN);  // master can send a nack now
  wait_for_completion;
          Serial.println("CompletedCompleted");

  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Stop condition
  // after this function is executed the TWDR register has the data from SLA that Master wants to read
}
  


unsigned char Read_data() // Returns the last byte from the data register
{
  return TWDR;
}