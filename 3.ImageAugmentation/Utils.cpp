#include "Utils.h"

void Utils::LogTrans(int T[], double a, int b)
{
    for (int i = 0; i < 256; i++)
    {
        T[i] = (int)((log(i + 1.0)) / a + b + 0.5);
        T[i] = qBound(0, T[i], 255);
    }
}

void Utils::PowerTrans(int T[], double r, int b)
{
    for (int i = 0; i < 256; i++)
    {
        T[i] = (int)(255*pow(i/255.0,r) + b + 0.5);
        T[i] = qBound(0, T[i], 255);
    }
}

void Utils::ExpTrans(int T[], double c, int a)
{
    for (int i = 0; i < 256; i++)
    {
        T[i] = (int)(exp(c*(i-a)) - 1 + 0.5);
        T[i] = qBound(0, T[i], 255);
    }
}

void Utils::NegTrans(int T[])
{
    for (int i = 0; i < 256; i++)
    {
        T[i] = 255-i;
        T[i] = qBound(0, T[i], 255);
    }
}

void Utils::SegTrans(int T[], int x1, int y1, int x2, int y2)
{


    //第一段0<=x<=x1
    if (0 == x1) T[0] = y1;
    else {
        for (int i = 0;i <= x1;i++) {
            T[i] = (int)((y1 - 0.0) / (x1 - 0.0) * i + 0.5);
            T[i] = qBound(0, T[i], 255);
            
        }

    }

    //第二段
    if (x1 == x2) T[x1] = y2;
    else {
        for (int i = x1;i <= x2;i++) {
            T[i] = (int)((y2 - y1+0.0) / (x2 - x1+0.0) * (i - x1) + y1 + 0.5);
            T[i] = qBound(0, T[i], 255);
            qDebug() << T[i];
        }

    }
    //第三段
    if (x2 == 255)T[x2] = 255;
    else {
        for (int i = x2;i <= 255;i++) {
            T[i] = (int)((255.0 - y2) / (255.0 - x2) * (i - x2) + y2 + 0.5);
            T[i] = qBound(0, T[i], 255);
        }
    }
}

void Utils::HistUniform(int hist[], int T[])
{
    double sumdist[256];
    sumdist[0] = hist[0];
    for (int i = 1;i < 256;i++) {
        sumdist[i] = hist[i] + sumdist[i - 1];
    }
    for (int i = 0;i < 256;i++) {
        T[i] = (int)(sumdist[i] / sumdist[255] * 255 + 0.5);
        qDebug() << T[i];
    }
    qDebug() << 'n';
}

void Utils::HistUniform(int hist[], double T[])
{
    T[0] = hist[0];
    for (int i = 1;i < 256;i++) {
        T[i] = hist[i] + T[i - 1];
    }
    for (int i = 0;i < 256;i++) {
        T[i] = T[i] / T[255];
    }
}


void Utils::HistRegular(int hist[], int hist2[], int T[])
{
    double   Tr[256], G[256];
    HistUniform(hist, Tr);
    HistUniform(hist2, G);
    for (int r = 0; r < 256; r++) {
        double minL = 1.1;
        int start = 0;
        if (r > 0) start = T[r - 1];
        for (int z = start; z < 256; z++) {
            if (hist2[z] == 0) continue;  // 规格图像中没有的灰度级不考虑
            if (fabs(Tr[r] - G[z]) < minL) {
                T[r] = z;
                minL = fabs(Tr[r] - G[z]);
            }
            else break;
        }
    }
}

int Utils::mean(int a[], int n)
{
    int m = (int)(accumulate(a, a + n, 0) / (n + 0.0) + 0.5);
    return qBound(0, m, 255);
}

int Utils::sum(int a[], int n)
{
    return accumulate(a, a + n, 0);
}

int Utils::max(int a[], int n)
{
    return *max_element(a,a+n);
}

int Utils::min(int a[], int n)
{
    return *min_element(a,a+n);
}

int Utils::median(int a[], int n)
{
    int med;
    if (n % 2 == 0) {
        int med_a, med_b;
        nth_element(a, a + n / 2, a + n);
        med_a = a[n / 2 - 1];
        nth_element(a, a + n / 2+1, a + n);
        med_b = a[n / 2];
        med = (int)((med_a + med_b) / 2.0 + 0.5);
    }
    else {
        nth_element(a, a + n / 2 + 1, a + n);
        med = a[n / 2];
    }
    return qBound(0, med, 255);
}

int Utils::sobel(int a[], int n)
{
    int kernel_horiz[9] = { 1,2,1,0,0,0,-1,-2,-1 };
    int kernel_vert[9] = { 1,0,-1,2,0,-2,1,0,-1 };
    int res_horiz = 0, res_vert = 0;
    for (int i = 0;i < n;i++) {
        res_horiz += kernel_horiz[i] * a[i];
        res_vert += kernel_vert[i] * a[i];
    }
    //return qBound(0, (int)(res / 2.0 + 0.5), 255);
    return qBound(0, abs(res_horiz)+abs(res_vert) , 255);
}

int Utils::laplus(int a[], int n)
{
    int kernel[9] = { 0,1,0,1,-4,1,0,1,0 };
    int res = 0;
    for (int i = 0;i < n;i++) {
        res += kernel[i] * a[i];
    }
    return qBound(0, abs(res), 255);
}

int Utils::gauss(int a[], int n)
{
    int kernel[9] = { 1,2,1,2,4,2,1,2,1 };
    int res = 0;
    for (int i = 0;i < n;i++) {
        res += kernel[i] * a[i];
    }
    return qBound(0, (int)(res/16.0+0.5), 255);
}

QRgb Utils::gray2rgb(int gray)
{
    int r, g, b;
    if (gray <= 255 / 4)
    {
        r = 0;
        g = 4 * gray;
        b = 255;
    }
    else if (gray <= 255 / 2)
    {
        r = 0;
        g = 255;
        b = -4 * gray + 2 * 255;
    }
    else if (gray <= 255*3 / 4)
    {
        r = 4 * gray - 2 * 255;
        g = 255;
        b = 0;
    }
    else
    {
        r = 255;
        g = 4 * (255 - gray);
        b = 0;
    }
    return qRgb(r,g,b);
}

