double CalculateTip(double total,double percent)
{
    return total*(percent/100.0);
}

double Split(double total,int people)
{
    return people ? total/people : 0;
}