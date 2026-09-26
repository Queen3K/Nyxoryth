#include <cmath>

double Add(double a,double b){return a+b;}
double Subtract(double a,double b){return a-b;}
double Multiply(double a,double b){return a*b;}
double Divide(double a,double b){return b==0?0:a/b;}

double Percentage(double value,double percent)
{
    return value*(percent/100.0);
}