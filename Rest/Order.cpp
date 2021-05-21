#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type)
{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
	type = r_Type;
	status = WAIT;
}

Order::~Order()
{
}

int Order::getID()
{
	return ID;
}

ORD_TYPE Order::getType() const
{
	return type;
}

void Order::setSize(int s)
{
	Size = s > 0 ? s : 0;
}

int Order::getSize() const
{
	return Size;
}

void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::setServTime(int t)
{
	ServTime = t;
}

void Order::setFinishTime(int t)
{
	FinishTime = t;
}


int Order::getArrTime()
{
	return ArrTime;
}
int Order::getServTime()
{
	return ServTime;
}
int Order::getFinTime()
{
	return FinishTime;
}
int Order::autoPromoted = 0;
int Order::getAPcount()
{
	return autoPromoted;
}
void Order::Promote(int m,int ts)
{
	if (m == 0)
		autoPromoted++;
	type = TYPE_VIP;
	totalMoney += m;
	VIPTime = ts;
}

double Order::gettotalMoney()
{
	return totalMoney;
}

void Order::settotalMoney(double m)
{
	totalMoney = m;
}

Order::Order(int id, ORD_TYPE r_Type, double Money, int Osize, int ArriTime)
{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	totalMoney = Money;
	Size = Osize;
	ArrTime = ArriTime;
	if (r_Type == TYPE_VIP)
		VIPTime = ArriTime;
}

int Order::getPriority() const
{
	if (status == SRV || status == DELAYED)
		return -FinishTime * 1000 + ServTime;
	return -2 * ArrTime + totalMoney / 100 - Size; //change
	
}

int Order::getVIPTime() const
{
	return VIPTime;
}