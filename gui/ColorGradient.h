#ifndef _GUI_CLRGR_H_
#define _GUI_CLRGR_H_

#include <cmath>
#include <vector>
#include "raylib.h"

class ColorGradient {
private:
  struct ColorPoint  // Internal class used to store colors at different points in the gradient.
  {
    float r,g,b;      // Red, green and blue values of our color.
    float val;        // Position of our color along the gradient (between 0 and 1).
    ColorPoint(float red, float green, float blue, float value)
      : r(red), g(green), b(blue), val(value) {}
  };
  std::vector<ColorPoint> color;      // An array of color points in ascending value.
  
public:
  //-- Default constructor:
  ColorGradient()  {  createDefaultHeatMapGradient();  }
  
  //-- Inserts a new color point into its correct position:
  void addColorPoint(float red, float green, float blue, float value)
  {
    for(int i=0; i<color.size(); i++)  {
      if(value < color[i].val) {
        color.insert(color.begin()+i, ColorPoint(red,green,blue, value));
        return;  }}
    color.push_back(ColorPoint(red,green,blue, value));
  }
  
  //-- Inserts a new color point into its correct position:
  void clearGradient() { color.clear(); }
 
  //-- Places a 5 color heapmap gradient into the "color" vector:
  void createDefaultHeatMapGradient()
  {
    color.clear();
    color.push_back(ColorPoint(0, 0, 255,   0.0f));      // Blue.
    color.push_back(ColorPoint(0, 255, 255,   0.25f));     // Cyan.
    color.push_back(ColorPoint(0, 255, 0,   0.5f));      // Green.
    color.push_back(ColorPoint(255, 255, 0,   0.75f));     // Yellow.
    color.push_back(ColorPoint(255, 0, 0,   1.0f));      // Red.
  }
  
  //-- Inputs a (value) between 0 and 1 and outputs the (red), (green) and (blue)
  //-- values representing that position in the gradient.
  Color getColorAtValue(const float value)
  {
    Color c{0, 0, 0, 255};
    
    if(color.size()==0)
      return c;
    
    for(int i=0; i<color.size(); i++)
    {
      ColorPoint &currC = color[i];
      if(value < currC.val) {
        int colorIndex = i - 1 < 0 ? 0 : i - 1;
        ColorPoint &prevC  = color[colorIndex];
        float valueDiff    = (prevC.val - currC.val);
        float fractBetween = (valueDiff==0) ? 0 : (value - currC.val) / valueDiff;
        c.r   = (prevC.r - currC.r)*fractBetween + currC.r;
        c.g = (prevC.g - currC.g)*fractBetween + currC.g;
        c.b  = (prevC.b - currC.b)*fractBetween + currC.b;
        return c;
      }
    }
    c.r   = color.back().r;
    c.g = color.back().g;
    c.b  = color.back().b;
    return c;
  }
};
#endif