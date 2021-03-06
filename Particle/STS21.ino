// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// STS21
// This code is designed to work with the STS21_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Temperature?sku=STS21_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// STS21 I2C address is 0x4A(74)
#define addr 0x4A

double cTemp = 0.0;

void setup() 
  {         
    // Set variable
    Particle.variable("i2cdevice", "STS21");
    Particle.variable("cTemp", cTemp);
       
    // Initialise I2C communication as MASTER
    Wire.begin();  
    // Start serial communication, set baud rate = 9600
    Serial.begin(9600);
    delay(300);
}
  
void loop() 
{
    unsigned int data[2];
    // Start I2C Transmission 
    Wire.beginTransmission(addr);   
    // Select no hold master
    Wire.write(0xF3);
    // End I2C Transmission
    Wire.endTransmission();
    delay(500);
    
    // Request 2 bytes of data
    Wire.requestFrom(addr, 2);       
   
    // Read 2 bytes of data
    if (Wire.available() == 2) 
    {                     
        data[0] = Wire.read();
        data[1] = Wire.read();
    }
    
    // Convert the data
    int rawtmp = data[0] * 256 + data[1];
    int value = rawtmp & 0xFFFC;
    cTemp = -46.85 + (175.72 * (value / 65536.0));
    float fTemp = cTemp * 1.8 + 32;
    
    // Output data to dashboard      
    Particle.publish("Temperature in Celsius : ", String(cTemp));
    Particle.publish("Temperature in Fahrenheit: ", String(fTemp));
    delay(1000); 

}
