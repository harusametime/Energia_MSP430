/*****************************************************
 * This reads illumination from light sensor VCNL4010 and shows it on
 * LCD Display AQM1602A
 * written by Masaki Samejima on Jun. 3, 2017
 * VCNL4010: http://www.vishay.com/docs/83462/vcnl4010.pdf
 * AQM1602A: http://akizukidenshi.com/catalog/g/gK-08896/
 *****************************************************/

#include<Wire.h>
#define LCD_ADRS  0x3E
#define SENSOR_ADRS    0x13

void setup() {
  Wire.begin();
  init_LCD();
  //Enable Sensors
  I2CwriteByte(SENSOR_ADRS,0x80,0xFF);
}

// the loop routine runs over and over again forever:
void loop() {

  uint16_t light = read_light();
  char str[6] = {0};
  itoa(light, str, 10);
  print_LCD(str, 0);
}


/*************************************
 * Fuctions for reading and writing via I2C
 *************************************/

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();

  Wire.requestFrom(Address, Nbytes);
  uint8_t index=0;
  while (Wire.available())
    Data[index++]=Wire.read();
}


// Write a byte (Data) in device (Address) at register (Register)
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

/*************************************
 * Read data from light sensor
 * Return illumincation (lux)
 *************************************/

uint16_t read_light(){
  byte buf[2] = {0};
  I2Cread(sensor_ADDRESS,0x85 ,2,buf);
  return (buf[0] << 8) | buf[1];
}


/**************************************
 * Functions for LCD Display
 **************************************/

void print_LCD(String str, int row){
  setCursor(0, row);
  int n = str.length();
  for(int i=0;i<n ;i++){
    writeData(str.charAt(i));
  }
}

void writeData(byte t_data)
{
  Wire.beginTransmission(LCD_ADRS);
  Wire.write(0x40);
  Wire.write(t_data);
  Wire.endTransmission();
  delay(1);
}

void setCursor(byte col, byte row)
{
    //byte row_offsets[] = { 0x00, 0x40 };

    if ( row >= 1 ) {
        writeCommand( 0x80 | (col + 0x40) );
    }else{
        writeCommand( 0x80 | (col + 0x00) );
    }

}

void writeCommand(byte t_command)
{
  Wire.beginTransmission(LCD_ADRS);
  Wire.write(0x00);
  Wire.write(t_command);
  Wire.endTransmission();
  delay(10);
}

void clear_LCD(void)
{
    writeCommand(0x01);
    delay(2);
}

void init_LCD()
{
  delay(100);
  writeCommand(0x38);
  delay(20);
  writeCommand(0x39);
  delay(20);
  writeCommand(0x14);
  delay(20);
  writeCommand(0x73);
  delay(20);
  writeCommand(0x56);
  delay(20);
  writeCommand(0x6C);
  delay(20);
  writeCommand(0x38);
  delay(20);
  writeCommand(0x01);
  delay(20);
  writeCommand(0x0C);
  delay(20);
}

void turnoff_LCD(){
  writeCommand(0x08);
  delay(20);
}
