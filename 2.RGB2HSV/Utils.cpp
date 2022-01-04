#include "Utils.h"
#include <algorithm>


HSV Utils::RGB2HSV(RGB rgb)
{
    HSV hsv;
    double b, g, r;
    double h, s, v;
    double Min, Max;
    double delta;

    b = rgb.b / 255.0;
    g = rgb.g / 255.0;
    r = rgb.r / 255.0;

    if (r > g)
    {
        Max = max(r, b);
        Min = min(g, b);
    }
    else
    {
        Max = max(g, b);
        Min = min(r, b);
    }

    v = Max;
    delta = Max - Min;

    if (IsEquals(Max, 0))
    {
        s = 0.0;
    }
    else
    {
        s = delta / Max;
    }

    if (Max == Min)
    {
        h = 0.0;
    }
    else
    {
        if (IsEquals(r, Max) && g >= b)
        {
            h = 60 * (g - b) / delta + 0;
        }
        else if (IsEquals(r, Max) && g < b)
        {
            h = 60 * (g - b) / delta + 360;
        }
        else if (IsEquals(g, Max))
        {
            h = 60 * (b - r) / delta + 120;
        }
        else if (IsEquals(b, Max))
        {
            h = 60 * (r - g) / delta + 240;
        }
    }

    hsv.h = (int)(h + 0.5);
    hsv.h = (hsv.h > 359) ? (hsv.h - 360) : hsv.h;
    hsv.h = (hsv.h < 0) ? (hsv.h + 360) : hsv.h;
    hsv.s = s;
    hsv.v = v;
    return hsv;
}

RGB Utils::HSV2RGB(HSV hsv)
{
    RGB rgb;
    int h = hsv.h;
    double s = hsv.s;
    double v = hsv.v;
    double b = 0.0;
    double g = 0.0;
    double r = 0.0;

    int flag = (int)abs(h / 60.0);
    double f = h / 60.0 - flag;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch (flag)
    {
    case 0:
        b = p;
        g = t;
        r = v;
        break;
    case 1:
        b = p;
        g = v;
        r = q;
        break;
    case 2:
        b = t;
        g = v;
        r = p;
        break;
    case 3:
        b = v;
        g = q;
        r = p;
        break;
    case 4:
        b = v;
        g = p;
        r = t;
        break;
    case 5:
        b = q;
        g = p;
        r = v;
        break;
    default:
        break;
    }

    int blue = int(b * 255);
    rgb.b = (blue > 255) ? 255 : blue;
    rgb.b = (blue < 0) ? 0 : rgb.b;

    int green = int(g * 255);
    rgb.g = (green > 255) ? 255 : green;
    rgb.g = (green < 0) ? 0 : rgb.g;

    int red = int(r * 255);
    rgb.r = (red > 255) ? 255 : red;
    rgb.r = (red < 0) ? 0 : rgb.r;
    return rgb;
}

HSV Utils::changeHSV(HSV hsv, HSV deltaHsv)
{
    HSV newHsv;
    newHsv.h = (hsv.h + deltaHsv.h + 360) % 360;
    newHsv.s = min(1.0, max(hsv.s + deltaHsv.s, 0.0));
    newHsv.v = min(1.0, max(hsv.v + deltaHsv.v, 0.0));
    return newHsv;
}

bool Utils::IsEquals(double a, double b)
{
    return fabs(a-b)<1e-3;
}
