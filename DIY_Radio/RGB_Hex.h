#ifndef _RGB_Hex_
#define _RGB_Hex_

#define DATA_SIZE 5

enum RotationTypes
{
    none = 0,
    CW = 1,
    CCW = 2,
};
enum configSingle
{
    single = 0,
    solid,
    topTwo,
    bottomTwo
};
enum configDouble
{
    half_n_half = 0,
    equidistant,
    alternating,
    toptwo
};

class RGBHex
{
private:
    bool isRotating;
    char rotation;
    RotationTypes RotationDirection;
    char colorsArray[4][3];

    char n_colors;
    char ledconfig;
public:
    RGBHex();
    void writeData();
    void updateRotation();
    void setColors();
    void setPrimaryColor(char r, char g, char b);
    void setSecondaryColor(char r, char g, char b);
    void setRotating() {isRotating = true;};
    void setStopped() {isRotating = false;};
};

#endif