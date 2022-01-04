#include<algorithm>
#include<QtGlobal>
#include<cmath>
#include<qdebug.h>
#include<Qcolor>
using namespace std;
class Utils
{
public:
	static int plus(double a, int x, double b, int y);
	static int minus(int x, int y);
	static int multiply(int x, int k);
	static int divide(int x, double C);
	static int andOp (int x, int y);
	static int orOp (int x, int y);
	static int xorOp (int x, int y);
	static int notOp (int x);
};

