// Graphics library  GLCDI2C
#ifndef GLCDI2C_h
#define GLCDI2C_h

#include <inttypes.h>
#include <Print.h>

//////////////////// Command List ///////////////////////////////////////
#define GLCDI2C_RESET                   0x01
#define GLCDI2C_INIT                    0x02
#define GLCDI2C_CLEAR                   0x03
#define GLCDI2C_CURSOR                  0x04
#define GLCDI2C_HOME                    0x05
#define GLCDI2C_ GET_X                  0x06
#define GLCDI2C_GET_Y                   0x07
#define GLCDI2C_SET_COLOR               0x08
#define GLCDI2C_GET_COLOR               0x09
#define GLCDI2C_FONT_TYPE               0x0A
#define GLCDI2C_FONT_ORIENT             0x0B
#define GLCDI2C_LINE                    0x0C
#define GLCDI2C_LINE_TO                 0x0D
#define GLCDI2C_LINE_TYPE               0x0E
#define GLCDI2C_LINE_SIZE               0x0F
#define GLCDI2C_BAR                     0x10
#define GLCDI2C_RECT                    0x11
#define GLCDI2C_CIRCLE                  0x12
#define GLCDI2C_FILL_CIRCLE             0x13
#define GLCDI2C_ARC                     0x14
#define GLCDI2C_BEVEL                   0x15
#define GLCDI2C_FILL_BEVEL              0x16
#define GLCDI2C_BEVEL_TYPE              0x17
#define GLCDI2C_BAR_GRADIENT            0x18
#define GLCDI2C_BEVEL_GRADIENT          0x19
#define GLCDI2C_PUT_PIXEL               0x1A
#define GLCDI2C_DRAW_PIXEL              0x1B
#define GLCDI2C_PUSH_COLOR              0x1C


#define GLCDI2C_FIRM_VER                0x30
#define GLCDI2C_SET_KEYPAD_MODE         0x31
#define GLCDI2C_READ_KEYPAD             0x32
#define GLCDI2C_READ_BUTTON             0x33
#define GLCDI2C_BACKLIGHT               0x34

#define GLCDI2C_COMMAND                 0xFE

// Font types
#define FONT_SMALL       0x01 
#define FONT_MEDIUM      0x02
#define FONT_LARGE       0x03
#define FONT_BIG         0x04
#define FONT_CUSTOM      0x05

#define FONT_HORIZ       0x00 
#define FONT_VERT        0x01

// GRADIENT Direction
#define GRAD_DOWN            1      // gradient changes in the vertical direction
#define GRAD_RIGHT           2      // gradient change in the horizontal direction
#define GRAD_UP              3      // gradient changes in the vertical direction
#define GRAD_LEFT            4      // gradient change in the horizontal direction
#define GRAD_DOUBLE_VER      5      // two gradient transitions in the vertical direction
#define GRAD_DOUBLE_HOR      6      // two gradient transitions in the horizontal direction

#define KEYPAD_MODE       0x00 
#define BUTTON_MODE       0x01


class GLCDI2C : public Print {
 public:
 
  GLCDI2C (unsigned int num_lines,unsigned int num_col,unsigned char i2c_address);

  void init(void);
  void clear(void);
  void cursor(unsigned int x, unsigned int y);
  void home(void); 
  void setColor(unsigned int color);
  unsigned int getColor(void);
  void fontType(unsigned char font);
  void fontOrientation(unsigned char font_orient);
  void line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
  void lineTo(unsigned int x, unsigned int y);
  void lineType(unsigned char line_type); 
  void lineSize(unsigned char line_size);
  void bar(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom);
  void rectangle(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom);
  void circle(unsigned int x, unsigned int y, unsigned int radius);
  void fillCircle(unsigned int x, unsigned int y, unsigned int radius);
  void arc(unsigned int xL, unsigned int yT, unsigned int xR, unsigned int yB, unsigned int r1, unsigned int r2, unsigned char octant);
  void bevel(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int radius);
  void fillBevel(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int radius);
  void bevelType(unsigned char type);
  void barGradient(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom, unsigned int color1, unsigned int color2, unsigned int length, unsigned char direction);
  void bevelGradient(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int rad, unsigned int color1, unsigned int color2, unsigned int length, unsigned char direction);
  void putPixel(unsigned int x, unsigned int y);
  void drawPixel(unsigned int x, unsigned int y, unsigned int color);
  void pushColor(unsigned int color);

  virtual size_t write(unsigned char);
  using Print::write;  

  unsigned int firmware(void);
  void keypadMode(unsigned char mode);
  unsigned int keypad (void);
  unsigned int button (void);
  void backlight(unsigned char light);
   
 private:
  
};

#endif


