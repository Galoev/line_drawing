#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

struct Point
{
  float x;
  float y;
};


void writeImage(const char *filename, char *img, int width, int height)
{
  FILE *file = fopen(filename, "wb");
  fprintf(file, "P5\n%i %i\n255\n", width, height);
  fwrite(img, sizeof(char), width * height, file);
}

int main()
{
  int width = 400;
  int height = 400;
  //cin >> width >> height;
  char *img = new char[width*height];
  memset(img, 255, width*height);
  
  Point p1, p2;
  p1.x = 50;
  p1.y = 90;
  p2.x = 130;
  p2.y = 60;
  float thickness = 5.0;
  /*cin >> start.x;
   cin >> start.y;
   cin >> end.x;
   cin >> end.y;*/
  int length_x = abs(p2.x - p1.x);
  int length_y = abs(p2.y - p1.y);
  
  int back = 255;
  int line = 0;
  
  float gback = pow(back / 255.0f, 2.2f);
  float gline = pow(line / 255.0f, 2.2f);
  
  if (length_x >= length_y) {
    Point start = p1.x < p2.x ? p1 : p2;
    Point end = p1.x < p2.x ? p2 : p1;
    int direction = 1;
    if (start.y > end.y){
      direction *= -1;
    }
    
    float h = (thickness / 2.0) / cos(atan2(length_y, length_x));
    
    for (int i = 1; i <= length_x - 1; i++)
    {
      float realIndex = i + 0.5; // пересчитать это для любых координат, а не только для целых
      float y_f = (float)((realIndex-1)*length_y) / length_x + start.y;
      float y_s = (float)(realIndex*length_y) / length_x + start.y;
      
      float maxLine = y_f > y_s ? y_f : y_s;
      float minLine = y_f < y_s ? y_f : y_s;
      
      float maxUp = maxLine + h;
      float minUp = minLine + h;
      float maxDown = maxLine - h;
      float minDown = minLine - h;
      
      int iMaxUp = maxUp;
      int iMinUp = minUp;
      int iMaxDown = ceil(maxDown);
      int iMinDown = ceil(minDown);
      
      int x = ceil(start.x);
      
      int realX = i + 0.5;
      //не совсем правльно закращивает
      for (int y_filling = iMaxDown; y_filling <= iMinUp; y_filling++) {
        img[y_filling * width + realX + x] = 0;
      }
      
      if (iMaxUp == iMinUp) {
        float a = minUp - iMinUp;
        float b = maxUp - iMaxUp;
        float S = (a + b)/2.0;
        img[iMinUp * width + i + x] = pow(((1 - S)*gback + S*gline), 1 / 2.2f) * 255 + 0.5;
      }
      else {
        float a = (maxUp - minUp);
        float b = minUp;
        float intersectionX = (iMaxUp - b)/a;
        float S = (intersectionX * (iMaxUp-minUp))/2.0;
        img[iMinUp * width + i + x] = pow((S*gback + (1 - S)*gline), 1 / 2.2f) * 255 + 0.5;
        S = ((1 - intersectionX) * (maxUp - iMaxUp))/2.0;
        img[iMaxUp * width + i + x] = pow(((1 - S)*gback + S*gline), 1 / 2.2f) * 255 + 0.5;
      }
      
      if (iMaxDown == iMinDown) {
        float a = iMinDown - minDown;
        float b = iMaxDown - maxDown;
        float S = (a + b)/2.0;
        img[iMinDown * width + i + x] = pow(((1 - S)*gback + S*gline), 1 / 2.2f) * 255 + 0.5;

      }
      else {
        float a = (maxDown - minDown);
        float b = minDown;
        float intersectionX = (iMinDown - b)/a;
        float S = (intersectionX * (iMinDown-minDown))/2.0;
        img[iMinDown * width + i + x] = pow(((1 - S)*gback + S*gline), 1 / 2.2f) * 255 + 0.5;
        S = ((1 - intersectionX) * (maxDown - iMinDown))/2.0;
        img[iMaxDown * width + i + x] = pow((S*gback + (1 - S)*gline), 1 / 2.2f) * 255 + 0.5;
      }
      
      if (iMaxDown == iMinUp) {
        
      }
      
      
      
      
      
    }
    
    /*
    for (int i = 0; i <= length_x; i++)
    {
      float y_f = (float)(i*length_y) / length_x;
      int y = y_f;
      float f = (y_f - y);
      y *= direction;
      if (f == 0)
        img[(y+start.y)*width + i + start.x] = line;
      else {
        img[(y + start.y)*width + i + start.x] = pow(((1 - f)*gline + f*gback), 1/2.2f)*255 + 0.5;
        img[(y + 1 * direction + start.y)*width + i + start.x] = pow(((1 - f)*gback + f*gline), 1 / 2.2f) * 255 + 0.5;
      }
    }
  }
  else {
    Point start = p1.y < p2.y ? p1 : p2;
    Point end = p1.y < p2.y ? p2 : p1;
    int direction = 1;
    if (end.x < start.x){
      direction *= -1;
    }
    for (int i = 0; i <= length_y; i++)
    {
      float x_f = (float)(i*length_x) / length_y;
      int x = x_f;
      float f = (x_f - x);
      x *= direction;
      if (f == 0)
      {
        img[(i+start.y)*width + x + start.x] = line;
      }
     else {
        img[(i+start.y)*width + x + start.x] = pow(((1 - f)*gline + f*gback), 1/2.2f)*255 + 0.5;
        img[(i+start.y)*width + x + 1*direction + start.x] = pow(((1 - f)*gback + f*gline), 1/2.2f)*255 + 0.5;
      }
    }*/
  }
  
  writeImage("/Users/ilkin_galoev/Documents/8 semester/Видеоинформационные технологии/myImage.pgm", img, width, height);
  
}
