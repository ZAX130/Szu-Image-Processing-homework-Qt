#pragma once
#include<algorithm>
#include<QtGlobal>
#include<cmath>
#include<qdebug.h>
#include<Qcolor>
using namespace std;
class Utils
{
public :
	static void LogTrans(int T[], double a, int b);
	static void PowerTrans(int T[], double r, int b);
	static void ExpTrans(int T[], double c, int a);
	static void NegTrans(int T[]);
	static void SegTrans(int T[], int x1, int y1, int x2, int y2);
	static void HistUniform(int hist[], int T[]);
	static void HistUniform(int hist[], double T[]);
	static void HistRegular(int hist[],int hist2[], int T[]);
	static int mean(int a[], int n);
	static int sum(int a[], int n);
	static int max(int a[], int n);
	static int min(int a[], int n);
	static int median(int a[], int n);
	static int sobel(int a[], int n);
	static int laplus(int a[], int n);
	static int gauss(int a[], int n);
	static QRgb gray2rgb(int gray);
};

