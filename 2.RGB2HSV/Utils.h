#pragma once
#include <algorithm>
#include <cmath>
using namespace std;
struct RGB {
	int r;
	int g;
	int b;
};

struct HSV
{
	int h;
	double s;
	double v;
};

class Utils
{
public :
	static HSV RGB2HSV(RGB rgb);
	static RGB HSV2RGB(HSV hsv);
	static HSV changeHSV(HSV hsv, HSV deltaHsv);
	static bool IsEquals(double a, double b);
};

