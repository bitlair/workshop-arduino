#include "OneWire.h"

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// http://milesburton.com/Dallas_Temperature_Control_Library

// platte kant naar boven, poten naar je toe
// van links naar rechts 1 (GND), 2 (Data), 3 (Vcc)
// pullup: 4.7kOhm tussen 2 en 3


int pinData = 4;
OneWire ds(pinData);
byte addr[8];
byte type_s;


void setup(void) 
{
  Serial.begin(115200);
  
  if (!ds.search(addr)) 
  {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
  Serial.print("ROM =");
  for (int i = 0; i < 8; i++) 
  {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) 
  {
    Serial.println("CRC is not valid!");
    return;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) 
  {
    case 0x10:
      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 
}

void loop(void) 
{
  byte present = 0;
  byte data[12];
  float celsius, fahrenheit;

  ds.reset();
  ds.select(addr);
  // start conversion, with parasite power on at the end
  ds.write(0x44,1);
  
  // maybe 750ms is enough, maybe not
  delay(1000);
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  // Read Scratchpad
  ds.write(0xBE);

  Serial.print("  Data = ");
  Serial.print(present,HEX);
  Serial.print(" ");
  // we need 9 bytes
  for (int i = 0; i < 9; i++) 
  {
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // convert the data to actual temperature

  unsigned int raw = (data[1] << 8) | data[0];
  if (type_s) 
  {
    // 9 bit resolution default
    raw = raw << 3;
    if (data[7] == 0x10) 
    {
      // count remain gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } 
  else 
  {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00)
    {
      // 9 bit resolution, 93.75 ms
      raw = raw << 3;
    }
    else if (cfg == 0x20) 
    {
      // 10 bit res, 187.5 ms
      raw = raw << 2;
    }
    else if (cfg == 0x40) 
    {
      // 11 bit res, 375 ms
      raw = raw << 1;
    }
    // default is 12 bit resolution, 750 ms conversion time
  }
  
  celsius = (float) raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
}


