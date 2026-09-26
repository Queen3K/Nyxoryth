#include "calculator.h"

double add(double a,double b){return a+b;}
double subtract(double a,double b){return a-b;}
double multiply(double a,double b){return a*b;}
double divide(double a,double b){return b==0?0:a/b;}
double percent(double value,double p){return value*(p/100.0);}