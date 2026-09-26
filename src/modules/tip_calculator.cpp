double Tip(double total,double rate)
{
    return total*(rate/100.0);
}

double Split(double total,int people)
{
    if(people<=0) return 0;
    return total/people;
}