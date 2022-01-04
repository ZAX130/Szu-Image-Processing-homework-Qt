#include "Utils.h"

int Utils::plus(double a, int x, double b, int y)
{
	int res = (int)(a * x + b * y+0.5);
	return qBound(0,res,255);
}

int Utils::minus(int x, int y)
{
	return qBound(0, abs(x-y), 255);
}

int Utils::multiply(int x, int k)
{
	return k>0?x:0;
}

int Utils::divide(int x, double C)
{
	return qBound(0, (int)(x/C+0.5), 255);
}

int Utils::andOp(int x, int y)
{
	int res = x > 0 && y > 0 ? 255 : 0;
	return res;
}


int Utils::orOp(int x, int y)
{
	int res = x > 0 || y > 0 ? 255 : 0;
	return res;
}

int Utils::xorOp(int x, int y)
{
	int res = x > 0 ^ y > 0 ? 255 : 0;
	return res;
}

int Utils::notOp(int x)
{
	int res = 255 - x;
	return res;
}
