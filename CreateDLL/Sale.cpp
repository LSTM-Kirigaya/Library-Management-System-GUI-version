#include "library.h"

double Sale::taxRate = 0.06;

double Sale::calcuSaleMoney()
{
	return ((double)num * retail);
}

double Sale::calcuTax()
{
	return (calcuSaleMoney() * taxRate);
}

double Sale::calcuTotalMoney()
{
	return (calcuSaleMoney() + calcuTax());
}