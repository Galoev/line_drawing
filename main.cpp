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

float m_gamma = 2.2f;

void writePixel(char* img, float S, float gline)
{
  float gback = pow(*(unsigned char*)img / 255.0f, m_gamma);
  *img = pow(((1 - S)*gback + S*gline), 1 / m_gamma) * 255 + 0.5;
}

void drawline(int width, char* img, Point p1, Point p2, float thickness, int backColor, int lineColor)
{
  /*cin >> start.x;
   cin >> start.y;
   cin >> end.x;
   cin >> end.y;*/
  float length_x = abs(p2.x - p1.x);
  float length_y = abs(p2.y - p1.y);
  
  int back = backColor;
  int line = lineColor;
  
  
  
  float gback = pow(back / 255.0f, m_gamma);
  float gline = pow(line / 255.0f, m_gamma);
  
  if (length_x >= length_y) {
    Point start = p1.x < p2.x ? p1 : p2;
    Point end = p1.x < p2.x ? p2 : p1;
    int direction = 1;
    if (start.y > end.y) {
      direction *= -1;
    }
    
    float h = (thickness / 2.0) / cos(atan2(length_y, length_x));
    
    for (int i = 1; i <= length_x - 1; i++)
    {
      //float realIndex = i + 0.5; // пересчитать это для любых координат, а не только для целых
      float realIndex = i + round(start.x*2)/2 - start.x;
      float y_f = (float)((realIndex - 1)*length_y) / length_x * direction + start.y + 0.5;
      float y_s = (float)(realIndex*length_y) / length_x * direction + start.y + 0.5;
      
      float maxLine = y_f > y_s ? y_f : y_s;
      float minLine = y_f < y_s ? y_f : y_s;
      
      float maxUp = maxLine + h;
      float minUp = minLine + h;
      float maxDown = maxLine - h;
      float minDown = minLine - h;
      
      int iMaxUp = maxUp;
      int iMinUp = minUp;
      int iMaxDown = maxDown;
      int iMinDown = minDown;
      //+ условие если дробная часть равна нулю
      
      int x = ceil(start.x); //БРЕД
      //int realX = i + 0.5; //БРЕД
      //не совсем правльно закрашивает
      
      for (int y_filling = iMaxDown + 1; y_filling < iMinUp; y_filling++) {
        img[y_filling * width + i + x] = 0;
      }
      
      
      float SU, SD;
      
      if (iMaxUp == iMinUp) {
        float a = minUp - iMinUp;
        float b = maxUp - iMaxUp;
        SU = (a + b) / 2.0;
      }
      else {
        float a = (maxUp - minUp);
        float b = minUp;
        float intersectionX = (iMaxUp - b) / a;
        SU = 1 - (intersectionX * (iMaxUp - minUp)) / 2.0;
        float S = ((1 - intersectionX) * (maxUp - iMaxUp)) / 2.0;
        
        writePixel(&img[iMaxUp * width + i + x], S, gline);// = pow(((1 - S)*gback + S*gline), 1 / m_gamma) * 255 + 0.5;
      }
      
      if (iMaxDown == iMinDown) {
        float a = minDown - iMinDown;
        float b = maxDown - iMaxDown;
        SD = 1 - (a + b) / 2.0;
        
      }
      else {
        float a = (maxDown - minDown);
        float b = minDown;
        float intersectionX = (iMaxDown - b) / a;
        float S = (intersectionX * (iMaxDown - minDown)) / 2.0;
        writePixel(&img[iMinDown * width + i + x], S, gline); // = pow(((1 - S)*gback + S*gline), 1 / m_gamma) * 255 + 0.5;
        SD = 1 - ((1 - intersectionX) * (maxDown - iMaxDown)) / 2.0;
      }
      
      if (iMaxDown == iMinUp) {
        float S = SU + SD - 1;
        writePixel(&img[iMinUp * width + i + x], S, gline); // = pow(((1 - S)*gback + S*gline), 1 / m_gamma) * 255 + 0.5;
      }
      else
      {
        writePixel(&img[iMinUp * width + i + x], SU, gline); // = pow(((1 - SU)*gback + SU*gline), 1 / m_gamma) * 255 + 0.5;
        writePixel(&img[iMaxDown * width + i + x], SD, gline); // = pow(((1 - SD)*gback + SD*gline), 1 / m_gamma) * 255 + 0.5;
      }
      
    }
  } else {
    Point start = p1.y < p2.y ? p1 : p2;
    Point end = p1.y < p2.y ? p2 : p1;
    int direction = 1;
    if (start.x > end.x) {
      direction *= -1;
    }
    
    float h = (thickness / 2.0) / cos(atan2(length_x, length_y));
    
    for (int i = 1; i <= length_y - 1; i++)
    {
      //float realIndex = i + 0.5; // пересчитать это для любых координат, а не только для целых
      float realIndex = i + round(start.y*2)/2 - start.y;
      float x_f = (float)((realIndex - 1)*length_x) / length_y * direction + start.x + 0.5;
      float x_s = (float)(realIndex*length_x) / length_y * direction + start.x + 0.5;
      
      float maxLine = x_f > x_s ? x_f : x_s;
      float minLine = x_f < x_s ? x_f : x_s;
      
      float maxUp = maxLine + h;
      float minUp = minLine + h;
      float maxDown = maxLine - h;
      float minDown = minLine - h;
      
      int iMaxUp = maxUp;
      int iMinUp = minUp;
      int iMaxDown = maxDown;
      int iMinDown = minDown;
      //+ условие если дробная часть равна нулю
      int y = ceil(start.y); //БРЕД
      
      //int realY = i + 0.5; //БРЕД
      //не совсем правльно закрашивает
      
      for (int x_filling = iMaxDown + 1; x_filling < iMinUp; x_filling++) {
        img[(y + i) * width + x_filling] = 0;
      }
      
      
      float SU, SD;
      
      if (iMaxUp == iMinUp) {
        float a = minUp - iMinUp;
        float b = maxUp - iMaxUp;
        SU = (a + b) / 2.0;
      }
      else {
        float a = (maxUp - minUp);
        float b = minUp;
        float intersectionX = (iMaxUp - b) / a;
        SU = 1 - (intersectionX * (iMaxUp - minUp)) / 2.0;
        float S = ((1 - intersectionX) * (maxUp - iMaxUp)) / 2.0;
        
        writePixel(&img[(i + y) * width + iMaxUp], S, gline);// = pow(((1 - S)*gback + S*gline), 1 / m_gamma) * 255 + 0.5;
      }
      
      if (iMaxDown == iMinDown) {
        float a = minDown - iMinDown;
        float b = maxDown - iMaxDown;
        SD = 1 - (a + b) / 2.0;
        
      }
      else {
        float a = (maxDown - minDown);
        float b = minDown;
        float intersectionX = (iMaxDown - b) / a;
        float S = (intersectionX * (iMaxDown - minDown)) / 2.0;
        writePixel(&img[ (i + y) * width + iMinDown], S, gline); // = pow(((1 - S)*gback + S*gline), 1 / m_gamma) * 255 + 0.5;
        SD = 1 - ((1 - intersectionX) * (maxDown - iMaxDown)) / 2.0;
      }
      
      if (iMaxDown == iMinUp) {
        float S = SU + SD - 1;
        writePixel(&img[(i + y) * width + iMinUp], S, gline); // = pow(((1 - S)*gback + S*gline), 1 / m_gamma) * 255 + 0.5;
      }
      else
      {
        writePixel(&img[(i + y) * width + iMinUp], SU, gline); // = pow(((1 - SU)*gback + SU*gline), 1 / m_gamma) * 255 + 0.5;
        writePixel(&img[(i + y) * width + iMaxDown], SD, gline); // = pow(((1 - SD)*gback + SD*gline), 1 / m_gamma) * 255 + 0.5;
      }
      
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
     img[(y + start.y)*width + i + start.x] = pow(((1 - f)*gline + f*gback), 1/m_gamma)*255 + 0.5;
     img[(y + 1 * direction + start.y)*width + i + start.x] = pow(((1 - f)*gback + f*gline), 1 / m_gamma) * 255 + 0.5;
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
     img[(i+start.y)*width + x + start.x] = pow(((1 - f)*gline + f*gback), 1/m_gamma)*255 + 0.5;
     img[(i+start.y)*width + x + 1*direction + start.x] = pow(((1 - f)*gback + f*gline), 1/m_gamma)*255 + 0.5;
     }
     }*/
  
}

int main(int argc, char* argv[])
{
  int width = 400;
  int height = 400;
  int back = 255;
  int line = 0;
  char* file_name = "";
  Point p1, p2;
  //cin >> width >> height;
  char *img = new char[width*height];  
  
  p1.x = 250;
  p1.y = 60;
  p2.x = 10;
  p2.y = 60;
  float thickness = 1;
  
  //memset(img, back, width*height);
  //drawline(width, img, p1, p2, thickness, back, line);
  
  //writeImage("/Users/ilkin_galoev/Documents/8 semester/Видеоинформационные технологии/myImage_norm.pgm", img, width, height);
  
  if (!(argc == 10 || argc == 11))
  {
    cout << "Insufficient number of arguments" << argc <<endl;
    for (int i = 0; i < argc; i++)
    {
      cout<<i<<" "<<argv[i]<<endl;
    }
  }
  
  if (argc == 11)
  {
    thickness = atoi(argv[10]);
  }
  
  file_name = argv[1];
  width = atoi(argv[2]);
  height = atoi(argv[3]);
  back = atoi(argv[4]);
  char *endptr;
  p1.x = strtof(argv[5], &endptr);
  if (*endptr)
  {
    cout<<"ERROR!!!"<<endl;
    return 0;
  }
  p1.y = strtof(argv[6], &endptr);
  if (*endptr)
  {
    cout<<"ERROR!!!"<<endl;
    return 0;
  }
  p2.x = strtof(argv[7], &endptr);
  if (*endptr)
  {
    cout<<"ERROR!!!"<<endl;
    return 0;
  }
  p2.y = strtof(argv[8], &endptr);
  if (*endptr)
  {
    cout<<"ERROR!!!"<<endl;
    return 0;
  }
  line = atoi(argv[9]);
  
  memset(img, back, width*height);
  drawline(width, img, p1, p2, thickness, back, line);
  writeImage(file_name, img, width, height);
    
  
  
}
