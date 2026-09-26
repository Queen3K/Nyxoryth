double calculateTip(double total,double percentage)
{
    return total*(percentage/100.0);
}

double splitBill(double total,int people)
{
    return people<=0?0:total/people;
}