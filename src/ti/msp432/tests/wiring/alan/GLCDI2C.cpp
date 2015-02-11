////////////////////////////////////////////////////////
//
// GLCDI2C v0.1 
//Uses I2C Wires interface
//Uses Analog pin 4 - SDA
//Uses Analog pin 5 - SCL
////////////////////////////////////////////////////////
  #include "GLCDI2C.h"

  #include <Wire.h>
  #include <stdio.h>
  #include <string.h>
  #include <inttypes.h>
  #include <Energia.h>
  

//--------------------------------------------------------

// (don't change here - specify on calling constructor)

unsigned int g_maxX = 128;
unsigned int g_maxY = 128;
unsigned char g_i2caddress = 0x90;   // Defalt address of the display
unsigned int g_oneByteAddr = 0;
unsigned char g_byte;


//constructor.  

GLCDI2C::GLCDI2C (unsigned int num_lines,unsigned int num_col,unsigned char i2c_address)
{
	
	g_maxX = num_lines;
	g_maxY = num_col;
	g_i2caddress = i2c_address;
	if((g_maxX > 255) || (g_maxY > 255))
    {
        g_oneByteAddr = 0;
    } else
    {
        g_oneByteAddr = 1;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// init LCD command
// reset and init LCD, fill a screen with BLACK color and set a current color as WHITE
// set cursor position x=0, y=0
/////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::init(void) {

  Wire.beginTransmission(g_i2caddress);
  Wire.write(GLCDI2C_COMMAND);
  Wire.write(GLCDI2C_INIT);
  Wire.endTransmission();
}


//////////////////////////////////////////////////////////////////////////////////////
// clear LCD command
// clear screen and fill screen with current color and set a cursor position x=0, y=0
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::clear(void){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_CLEAR);
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// cursor LCD command
// set a cursor position 
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::cursor(unsigned int x, unsigned int y){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_CURSOR);
      if(g_oneByteAddr)
      {
         g_byte = x;
         Wire.write(g_byte);  
         g_byte = y;
         Wire.write(g_byte); 
      } else
      {
         g_byte = x >> 8;
         Wire.write(g_byte);  
         g_byte = x;
         Wire.write(g_byte); 
         g_byte = y >> 8;
         Wire.write(g_byte);  
         g_byte = y;
         Wire.write(g_byte);
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// home LCD command
// set a cursor position x=0, y=0
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::home(void){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_HOME);
      Wire.endTransmission();
}


//////////////////////////////////////////////////////////////////////////////////////
// Set Color command
// set a currrent color 
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::setColor(unsigned int color){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_SET_COLOR);
      g_byte = color >> 8;
      Wire.write(g_byte);  
      g_byte = color;
      Wire.write(g_byte); 
      Wire.endTransmission();
}


/////////////////////////////////////////////////////////////////////////////////////
// Get Color command
// Get a currrent color 
//////////////////////////////////////////////////////////////////////////////////////
unsigned int GLCDI2C::getColor(void){

  unsigned int data = 0;
  unsigned char number = 1;

  Wire.beginTransmission(g_i2caddress);
  Wire.write(GLCDI2C_COMMAND);
  Wire.write(GLCDI2C_GET_COLOR);
  Wire.endTransmission();
  //delay(GLCDI2C_CMDDELAY);
  
  //  Connect to device and request byte
  Wire.beginTransmission(g_i2caddress);
  Wire.requestFrom(g_i2caddress, number);

  if (Wire.available()) {
    data = Wire.read();
  }
  return data;
}


//////////////////////////////////////////////////////////////////////////////////////
// font type command
// set a font type 
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::fontType(unsigned char font){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_FONT_TYPE);
      Wire.write(font);  
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// font orientation command
// set a font orientation (horiz or vert) 
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::fontOrientation(unsigned char font_orient){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_FONT_ORIENT);
      Wire.write(font_orient);  
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Line command
// This function draws a line with the current line type from the start point to the end point 
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_LINE);
      if(g_oneByteAddr)
      {
         g_byte = x1;
         Wire.write(g_byte);  
         g_byte = y1;
         Wire.write(g_byte); 
         g_byte = x2;
         Wire.write(g_byte);  
         g_byte = y2;
         Wire.write(g_byte); 
      } else
      {
         g_byte = x1 >> 8;
         Wire.write(g_byte);  
         g_byte = x1;
         Wire.write(g_byte); 
         g_byte = y1 >> 8;
         Wire.write(g_byte);  
         g_byte = y1;
         Wire.write(g_byte);
         g_byte = x2 >> 8;
         Wire.write(g_byte);  
         g_byte = x2;
         Wire.write(g_byte); 
         g_byte = y2 >> 8;
         Wire.write(g_byte);  
         g_byte = y2;
         Wire.write(g_byte);
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Line To command
// This function draws a line with the current line type from the current graphic cursor position
// to the given x, y position
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::lineTo(unsigned int x, unsigned int y){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_LINE_TO);
      if(g_oneByteAddr)
      {
         g_byte = x;
         Wire.write(g_byte);  
         g_byte = y;
         Wire.write(g_byte); 
      } else
      {
         g_byte = x >> 8;
         Wire.write(g_byte);  
         g_byte = x;
         Wire.write(g_byte); 
         g_byte = y >> 8;
         Wire.write(g_byte);  
         g_byte = y;
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Line type command
// set a line type: 0 - SOLID; 1 - DOTTED; 4 - DASHED 
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::lineType(unsigned char line_type){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_LINE_TYPE);
      Wire.write(line_type);  
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Line Size command
// set a line size: 0 - NORMAL (thickness 1 pixel); 1 - THICK (thickness 3 pixels)
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::lineSize(unsigned char line_size){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_LINE_SIZE);
      Wire.write(line_size);  
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Bar command
// This function draws a bar given the left, top and right, bottom corners with the current color 
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::bar(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_BAR);
      if(g_oneByteAddr)
      {
         g_byte = left;
         Wire.write(g_byte);  
         g_byte = top;
         Wire.write(g_byte); 
         g_byte = right;
         Wire.write(g_byte);  
         g_byte = bottom;
         Wire.write(g_byte); 
      } else
      {
         g_byte = left >> 8;
         Wire.write(g_byte);  
         g_byte = left;
         Wire.write(g_byte); 
         g_byte = top >> 8;
         Wire.write(g_byte);  
         g_byte = top;
         Wire.write(g_byte);
         g_byte = right >> 8;
         Wire.write(g_byte);  
         g_byte = right;
         Wire.write(g_byte); 
         g_byte = bottom >> 8;
         Wire.write(g_byte);  
         g_byte = bottom;
         Wire.write(g_byte);
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Rectangle command
// This function draws a rectangle given the left, top and right, bottom corners with the current color 
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::rectangle(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_RECT);
      if(g_oneByteAddr)
      {
         g_byte = left;
         Wire.write(g_byte);  
         g_byte = top;
         Wire.write(g_byte); 
         g_byte = right;
         Wire.write(g_byte);  
         g_byte = bottom;
         Wire.write(g_byte); 
      } else
      {
         g_byte = left >> 8;
         Wire.write(g_byte);  
         g_byte = left;
         Wire.write(g_byte); 
         g_byte = top >> 8;
         Wire.write(g_byte);  
         g_byte = top;
         Wire.write(g_byte);
         g_byte = right >> 8;
         Wire.write(g_byte);  
         g_byte = right;
         Wire.write(g_byte); 
         g_byte = bottom >> 8;
         Wire.write(g_byte);  
         g_byte = bottom;
         Wire.write(g_byte);
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Circle command
// This function draws a circle with the given center and radius
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::circle(unsigned int x, unsigned int y, unsigned int radius){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_CIRCLE);
      if(g_oneByteAddr)
      {
         g_byte = x;
         Wire.write(g_byte);  
         g_byte = y;
         Wire.write(g_byte); 
         g_byte = radius;
         Wire.write(g_byte);  
      } else
      {
         g_byte = x >> 8;
         Wire.write(g_byte);  
         g_byte = x;
         Wire.write(g_byte); 
         g_byte = y >> 8;
         Wire.write(g_byte);  
         g_byte = y;
         Wire.write(g_byte);
         g_byte = radius >> 8;
         Wire.write(g_byte);  
         g_byte = radius;
     }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// FillCircle command
// This function draws a filled circle with the given center and radius
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::fillCircle(unsigned int x, unsigned int y, unsigned int radius)
{
      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_FILL_CIRCLE);
      if(g_oneByteAddr)
      {
         g_byte = x;
         Wire.write(g_byte);  
         g_byte = y;
         Wire.write(g_byte); 
         g_byte = radius;
         Wire.write(g_byte);  
      } else
      {
         g_byte = x >> 8;
         Wire.write(g_byte);  
         g_byte = x;
         Wire.write(g_byte); 
         g_byte = y >> 8;
         Wire.write(g_byte);  
         g_byte = y;
         Wire.write(g_byte);
         g_byte = radius >> 8;
         Wire.write(g_byte);  
         g_byte = radius;
     }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Arc command
// Draws the octant arc of a beveled figure with given centers, radii and octant mask. 
// When r1 is zero and r2 has some value, a filled circle is drawn; 
// when the radii have values, an arc of thickness (r2-r1) is drawn; 
// when octant = 0xFF, a full ring is drawn. 
// When r1 and r2 are zero, a rectangular object is drawn, where xL, yT specifies the left top corner; xR, yB specifies the right bottom corner.
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::arc(unsigned int xL, unsigned int yT, unsigned int xR, unsigned int yB, unsigned int r1, unsigned int r2, unsigned char octant)
{
      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_ARC);
      if(g_oneByteAddr)
      {
         g_byte = xL;
         Wire.write(g_byte);  
         g_byte = yT;
         Wire.write(g_byte); 
         g_byte = xR;
         Wire.write(g_byte);  
         g_byte = yB;
         Wire.write(g_byte); 
         g_byte = r1;
         Wire.write(g_byte); 
         g_byte = r2;
         Wire.write(g_byte);
         g_byte = octant;
         Wire.write(g_byte);    
      } else
      {
         g_byte = xL >> 8;
         Wire.write(g_byte);  
         g_byte = xL;
         Wire.write(g_byte); 
         g_byte = yT >> 8;
         Wire.write(g_byte);  
         g_byte = yT;
         Wire.write(g_byte);
         g_byte = xR >> 8;
         Wire.write(g_byte);  
         g_byte = xR;
         Wire.write(g_byte); 
         g_byte = yB >> 8;
         Wire.write(g_byte);  
         g_byte = yB;
         Wire.write(g_byte);
         g_byte = r1 >> 8;
         Wire.write(g_byte);  
         g_byte = r1;
         Wire.write(g_byte);  
         g_byte = r2 >> 8;
         Wire.write(g_byte);  
         g_byte = r2;
         Wire.write(g_byte); 
         g_byte = octant;
         Wire.write(g_byte); 
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Bevel command
// Draws a beveled figure on the screen. For a pure circular object x1 = x2 and y1 = y2. 
// For a rectangular object radius = 0.
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::bevel(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int radius)
{
      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_BEVEL);
      if(g_oneByteAddr)
      {
         g_byte = x1;
         Wire.write(g_byte);  
         g_byte = y1;
         Wire.write(g_byte); 
         g_byte = x2;
         Wire.write(g_byte);  
         g_byte = y2;
         Wire.write(g_byte); 
         g_byte = radius;
         Wire.write(g_byte); 
      } else
      {
         g_byte = x1 >> 8;
         Wire.write(g_byte);  
         g_byte = x1;
         Wire.write(g_byte); 
         g_byte = y1 >> 8;
         Wire.write(g_byte);  
         g_byte = y1;
         Wire.write(g_byte);
         g_byte = x2 >> 8;
         Wire.write(g_byte);  
         g_byte = x2;
         Wire.write(g_byte); 
         g_byte = y2 >> 8;
         Wire.write(g_byte);  
         g_byte = y2;
         Wire.write(g_byte);
         g_byte = radius >> 8;
         Wire.write(g_byte);  
         g_byte = radius;
         Wire.write(g_byte); 
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Fill Bevel command
// Draws a filled beveled figure on the screen. For a filled circular object x1 = x2 and y1 = y2. 
// For a filled rectangular object radius = 0.
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::fillBevel(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int radius){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_FILL_BEVEL);
      if(g_oneByteAddr)
      {
         g_byte = x1;
         Wire.write(g_byte);  
         g_byte = y1;
         Wire.write(g_byte); 
         g_byte = x2;
         Wire.write(g_byte);  
         g_byte = y2;
         Wire.write(g_byte); 
         g_byte = radius;
         Wire.write(g_byte); 
      } else
      {
         g_byte = x1 >> 8;
         Wire.write(g_byte);  
         g_byte = x1;
         Wire.write(g_byte); 
         g_byte = y1 >> 8;
         Wire.write(g_byte);  
         g_byte = y1;
         Wire.write(g_byte);
         g_byte = x2 >> 8;
         Wire.write(g_byte);  
         g_byte = x2;
         Wire.write(g_byte); 
         g_byte = y2 >> 8;
         Wire.write(g_byte);  
         g_byte = y2;
         Wire.write(g_byte);
         g_byte = radius >> 8;
         Wire.write(g_byte);  
         g_byte = radius;
         Wire.write(g_byte); 
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Bevel type command
// This function sets the fill bevel type to be drawn.
// type: 0 - DRAWFULLBEVEL to draw the full shape 
//       1 - DRAWTOPBEVEL to draw the upper half portion 
//       2 - DRAWBOTTOMBEVEL to draw the lower half portion 
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::bevelType(unsigned char type){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_BEVEL_TYPE);
      Wire.write(type);  
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Bar Gradient command
// This created to do the bar gradient rendering
//       left - x position of the left top corner.
//       top - y position of the left top corner.
//       right - x position of the right bottom corner.
//       bottom - y position of the right bottom corner.
//       color1 - start color for the gradient
//      color2 - end color for the gradient
//      length - From 0-100%. How much of a gradient is wanted
//      direction - Gradient Direction
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::barGradient(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom, unsigned int color1, unsigned int color2, unsigned int length, unsigned char direction)
{

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_BAR_GRADIENT);
      if(g_oneByteAddr)
      {
         g_byte = left;
         Wire.write(g_byte);  
         g_byte = top;
         Wire.write(g_byte); 
         g_byte = right;
         Wire.write(g_byte);  
         g_byte = bottom;
         Wire.write(g_byte); 
         g_byte = color1 >> 8;
         Wire.write(g_byte);  
         g_byte = color1;
         Wire.write(g_byte);
         g_byte = color2 >> 8;
         Wire.write(g_byte);  
         g_byte = color2; 
         Wire.write(g_byte); 
         g_byte = length;
         Wire.write(g_byte);  
         g_byte = direction;
         Wire.write(g_byte); 
      } else
      {
         g_byte = left >> 8;
         Wire.write(g_byte);  
         g_byte = left;
         Wire.write(g_byte); 
         g_byte = top >> 8;
         Wire.write(g_byte);  
         g_byte = top;
         Wire.write(g_byte);
         g_byte = right >> 8;
         Wire.write(g_byte);  
         g_byte = right;
         Wire.write(g_byte); 
         g_byte = bottom >> 8;
         Wire.write(g_byte);  
         g_byte = bottom;
         Wire.write(g_byte);
         g_byte = color1 >> 8;
         Wire.write(g_byte);  
         g_byte = color1;
         Wire.write(g_byte); 
         g_byte = color2 >> 8;
         Wire.write(g_byte);  
         g_byte = color2;
         Wire.write(g_byte); 
         g_byte = length >> 8;
         Wire.write(g_byte);  
         g_byte = length;
         Wire.write(g_byte);  
         g_byte = direction;
         Wire.write(g_byte);
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Bevel Gradient command
// This created to do the bevel gradient rendering
//       x1 - x coordinate position of the upper left center of the circle that
//            draws the rounded corners.
//       y1 - y coordinate position of the upper left center of the circle that
//            draws the rounded corners.
//       x2 - x coordinate position of the lower right center of the circle that
//            draws the rounded corners.
//       y2 - y coordinate position of the lower right center of the circle that
//            draws the rounded corners.
//      rad - defines the redius of the circle, that draws the rounded corners.
//      color1 - start color for the gradient
//      color2 - end color for the gradient
//      length - From 0-100%. How much of a gradient is wanted
//      direction - Gradient Direction
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::bevelGradient(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int rad, unsigned int color1, unsigned int color2, unsigned int length, unsigned char direction)
{

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_BEVEL_GRADIENT);
      if(g_oneByteAddr)
      {
         g_byte = x1;
         Wire.write(g_byte);  
         g_byte = y1;
         Wire.write(g_byte); 
         g_byte = x2;
         Wire.write(g_byte);  
         g_byte = y2;
         Wire.write(g_byte); 
         g_byte = rad;
         Wire.write(g_byte); 
         g_byte = color1;
         Wire.write(g_byte);  
         g_byte = color2;
         Wire.write(g_byte); 
         g_byte = length;
         Wire.write(g_byte);  
         g_byte = direction;
         Wire.write(g_byte); 
      } else
      {
         g_byte = x1 >> 8;
         Wire.write(g_byte);  
         g_byte = x1;
         Wire.write(g_byte); 
         g_byte = y1 >> 8;
         Wire.write(g_byte);  
         g_byte = y1;
         Wire.write(g_byte);
         g_byte = x2 >> 8;
         Wire.write(g_byte);  
         g_byte = x2;
         Wire.write(g_byte); 
         g_byte = y2 >> 8;
         Wire.write(g_byte);  
         g_byte = y2;
         Wire.write(g_byte);
         g_byte = rad >> 8;
         Wire.write(g_byte);  
         g_byte = rad;
         Wire.write(g_byte); 
         g_byte = color1 >> 8;
         Wire.write(g_byte);  
         g_byte = color1;
         Wire.write(g_byte); 
         g_byte = color2 >> 8;
         Wire.write(g_byte);  
         g_byte = color2;
         Wire.write(g_byte); 
         g_byte = length >> 8;
         Wire.write(g_byte);  
         g_byte = length;
         Wire.write(g_byte);  
         g_byte = direction;
         Wire.write(g_byte);
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Put pixel command
// This function fills a pixel with the coordinates (x, y) with the current color.
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::putPixel(unsigned int x, unsigned int y){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_PUT_PIXEL);
      if(g_oneByteAddr)
      {
         g_byte = x;
         Wire.write(g_byte);  
         g_byte = y;
         Wire.write(g_byte); 
      } else
      {
         g_byte = x >> 8;
         Wire.write(g_byte);  
         g_byte = x;
         Wire.write(g_byte); 
         g_byte = y >> 8;
         Wire.write(g_byte);  
         g_byte = y;
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Draw pixel command
// This function fills a pixel with the coordinates (x, y) with the 16 bit color.
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::drawPixel(unsigned int x, unsigned int y, unsigned int color){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_DRAW_PIXEL);
      if(g_oneByteAddr)
      {
         g_byte = x;
         Wire.write(g_byte);  
         g_byte = y;
         Wire.write(g_byte); 
         g_byte = color >> 8;
         Wire.write(g_byte);  
         g_byte = color;
         Wire.write(g_byte); 
      } else
      {
         g_byte = x >> 8;
         Wire.write(g_byte);  
         g_byte = x;
         Wire.write(g_byte); 
         g_byte = y >> 8;
         Wire.write(g_byte);  
         g_byte = y;
         Wire.write(g_byte);
         g_byte = color >> 8;
         Wire.write(g_byte);  
         g_byte = color;
         Wire.write(g_byte); 
      }
      Wire.endTransmission();
}

//////////////////////////////////////////////////////////////////////////////////////
// Push Color command
// This function fills a pixel with the current graphic cursor position with the 16 bit color.
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::pushColor(unsigned int color){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_PUSH_COLOR);
      g_byte = color >> 8;
      Wire.write(g_byte);  
      g_byte = color;
      Wire.write(g_byte); 
      Wire.endTransmission();
}


//////////////////////////////////////////////////////////////////////////////////////
// write command
// Send a data byte to LCD 
//////////////////////////////////////////////////////////////////////////////////////
inline size_t GLCDI2C::write(unsigned char value) {
  Wire.beginTransmission(g_i2caddress);
  Wire.write(value);
  Wire.endTransmission();
  return 1; // assume sucess
}


/////////////////////////////////////////////////////////////////////////////////////
// Firmware version command
// Read the LCD firmware version 
//////////////////////////////////////////////////////////////////////////////////////
unsigned int GLCDI2C::firmware(void){

  unsigned int data = 0;
  unsigned char number = 1;

  Wire.beginTransmission(g_i2caddress);
  Wire.write(GLCDI2C_COMMAND);
  Wire.write(GLCDI2C_FIRM_VER);
  Wire.endTransmission();
  //delay(GLCDI2C_CMDDELAY);
  
  //  Connect to device and request byte
  Wire.beginTransmission(g_i2caddress);
  Wire.requestFrom(g_i2caddress, number);

  if (Wire.available()) {
    data = Wire.read();
  }
  return data;
}

//////////////////////////////////////////////////////////////////////////////////////
// Keypad Mode command
// This function sets the Keypad Port mode
// mode: 0x00 - 4x4 matrix keypad mode 
//       0x01 - 8 button mode
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::keypadMode(unsigned char mode){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_SET_KEYPAD_MODE);
      Wire.write(mode);  
      Wire.endTransmission();
}


//////////////////////////////////////////////////////////////////////////////////////
// Read keypad command
// Read a pressed keypad number from buffer
//////////////////////////////////////////////////////////////////////////////////////
unsigned int GLCDI2C::keypad (void){

  unsigned int data = 0;
  unsigned char number = 1;

  //  Send Keypad read command
  Wire.beginTransmission(g_i2caddress);
  Wire.write(GLCDI2C_COMMAND);
  Wire.write(GLCDI2C_READ_KEYPAD);
  Wire.endTransmission();
//  delay(GLCDI2C_CMDDELAY);
  
  //  Connect to device and request byte
  Wire.beginTransmission(g_i2caddress);
  Wire.requestFrom(g_i2caddress, number);

  if (Wire.available()) {
    data = Wire.read();
  }

return data;
}

//////////////////////////////////////////////////////////////////////////////////////
// Read buttons command
// Read a pressed button number from buffer
//////////////////////////////////////////////////////////////////////////////////////
unsigned int GLCDI2C::button (void){

  unsigned int data = 0;
  unsigned char number = 1;

  //  Send Keypad read command
  Wire.beginTransmission(g_i2caddress);
  Wire.write(GLCDI2C_COMMAND);
  Wire.write(GLCDI2C_READ_BUTTON);
  Wire.endTransmission();
//  delay(GLCDI2C_CMDDELAY);
  
  //  Connect to device and request byte
  Wire.beginTransmission(g_i2caddress);
  Wire.requestFrom(g_i2caddress, number);

  if (Wire.available()) {
    data = Wire.read();
  }

return data;
}



//////////////////////////////////////////////////////////////////////////////////////
// Backlight command
// This function sets the PWM value for the LCD backlight control
// light: 0x00 - backlight off 
//        0xFF - max backlight
//////////////////////////////////////////////////////////////////////////////////////
void GLCDI2C::backlight(unsigned char light){

      Wire.beginTransmission(g_i2caddress);
      Wire.write(GLCDI2C_COMMAND);
      Wire.write(GLCDI2C_BACKLIGHT);
      Wire.write(light);  
      Wire.endTransmission();
}





