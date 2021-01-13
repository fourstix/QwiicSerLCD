#ifndef QWIIC_SER_LCD_H
#define QWIIC_SER_LCD_H

#include <Arduino.h>
#include <Wire.h>
#include <Stream.h>
#include <SPI.h>

#define DISPLAY_ADDRESS1 0x72 //This is the default address of the OpenLCD
#define MAX_ROWS 4
#define MAX_COLUMNS 20

/*
 *OpenLCD command characters
 */
#define SPECIAL_COMMAND 254  //Magic number for sending a special command
#define SETTING_COMMAND 0x7C //124, |, the pipe character: The command to change settings: baud, lines, width, backlight, splash, etc

/*
 * OpenLCD commands
 */
#define CLEAR_COMMAND 0x2D					//45, -, the dash character: command to clear and home the display
#define CONTRAST_COMMAND 0x18				//Command to change the contrast setting
#define ADDRESS_COMMAND 0x19				//Command to change the i2c address
#define SET_RGB_COMMAND 0x2B				//43, +, the plus character: command to set backlight RGB value
#define ENABLE_SYSTEM_MESSAGE_DISPLAY 0x2E  //46, ., command to enable system messages being displayed
#define DISABLE_SYSTEM_MESSAGE_DISPLAY 0x2F //47, /, command to disable system messages being displayed
#define ENABLE_SPLASH_DISPLAY 0x30			//48, 0, command to enable splash screen at power on
#define DISABLE_SPLASH_DISPLAY 0x31			//49, 1, command to disable splash screen at power on
#define SAVE_CURRENT_DISPLAY_AS_SPLASH 0x0A //10, Ctrl+j, command to save current text on display as splash

/*
 * Hitachi HD44780 Display DDRAM Commands
 */
#define SET_DDRAM_HOME    0x02
#define SET_DDRAM_ADDRESS 0x80

/*
 * Hitachi HD44780 Display Mode Bits
 */
#define DISPLAY_MODE_BIT   0x04
#define LEFT_TO_RIGHT_BIT  0x02
#define SCROLL_BIT         0x01

/*
 * Hitachi HD44780 Display Control Bits
 */
#define DISPLAY_CTRL_BIT 0x08
#define DISPLAY_BIT      0x04
#define CURSOR_BIT       0x02
#define BLINK_BIT        0x01

/*
 * Hitachi HD44780 Display Shift Bits
 */
#define SHIFT_CURSOR_BIT  0x10
#define SHIFT_DISPLAY_BIT 0x08
#define SHIFT_RIGHT_BIT   0x04

class SerLCD : public Print
{

  public:
	SerLCD();
	~SerLCD();
	void begin(TwoWire &wirePort);
	void begin(TwoWire &wirePort, byte i2c_addr);
	void begin(Stream &serial);
	void begin(SPIClass &spiPort, byte csPin);
//Only available for Arduino 1.6 and greater
#ifdef SPI_HAS_TRANSACTION
	//pass SPISettings by value to allow settings object creation in fucntion call like examples
	void begin(SPIClass &spiPort, byte csPin, SPISettings spiSettings);
#endif
  //functions defined by LiquidCrystal API
	void clear();
	void home();
	void setCursor(byte col, byte row);
  void cursor();
  void noCursor();
  void blink();
  void noBlink();
  void display();
  void noDisplay();
  void scrollDisplayLeft();
	void scrollDisplayRight();
  void autoscroll();
	void noAutoscroll();
  void leftToRight();
	void rightToLeft();
	void createChar(byte location, byte charmap[]);

  //additional functions supported by SerLCD
	void writeChar(byte location);
  void scrollDisplayLeft(byte count);
	void scrollDisplayRight(byte count);
	void moveCursorLeft();
	void moveCursorRight();
	void moveCursorLeft(byte count);
	void moveCursorRight(byte count);
	void setBacklight(unsigned long rgb);
	void setBacklight(byte r, byte g, byte b);
	void setFastBacklight(unsigned long rgb);
	void setFastBacklight(byte r, byte g, byte b);
	void setContrast(byte new_val);
	void setAddress(byte new_addr);
	void command(byte command);
	void specialCommand(byte command);
	void specialCommand(byte command, byte count);
	void enableSystemMessages();
	void disableSystemMessages();
	void enableSplash();
	void disableSplash();
	void saveSplash();

  //virtual functions for write and print methods
  virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *buffer, size_t size);
	virtual size_t write(const char *str);

  private:
	TwoWire *_i2cPort = NULL;   //The generic connection to user's chosen I2C hardware
	Stream *_serialPort = NULL; //The generic connection to user's chosen serial hardware
	SPIClass *_spiPort = NULL;  //The generic connection to user's chosen spi hardware

//SPI transactions only available for Arduino 1.6 and later
#ifdef SPI_HAS_TRANSACTION
	SPISettings _spiSettings = SPISettings(100000, MSBFIRST, SPI_MODE0);
	bool _spiTransaction = false; //since we pass by value, we need a flag
#endif
	byte _csPin = 10;
	byte _i2cAddr = DISPLAY_ADDRESS1;
	byte _displayControl = DISPLAY_BIT;  //display on, blink off, cursor off
	byte _displayMode = LEFT_TO_RIGHT_BIT;  //left to right, no scrolling
	void init();
	void beginTransmission();
	void transmit(byte data);
	void endTransmission();
};

#endif
