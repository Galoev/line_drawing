#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

struct Point
{
  int x;
  int y;
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
  p1.x = 3;
  p1.y = 15;
  p2.x = 150;
  p2.y = 150;
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
  } else {
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
      } else {
        img[(i+start.y)*width + x + start.x] = pow(((1 - f)*gline + f*gback), 1/2.2f)*255 + 0.5;
        img[(i+start.y)*width + x + 1*direction + start.x] = pow(((1 - f)*gback + f*gline), 1/2.2f)*255 + 0.5;
      }
    }
  }
  
  writeImage("/Users/ilkin_galoev/Documents/8 semester/Видеоинформационные технологии/myImage.pgm", img, width, height);
  
}
