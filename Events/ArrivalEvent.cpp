#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"

ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType) :Event(eTime, oID)
{
	OrdType = oType;
}

ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int Osize, double Omoney) : Event(eTime, oID)
{
	OrdType = oType;
	OrdSize = Osize;
	OrdMoney = Omoney;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	Order* pOrd = new Order(OrderID, OrdType, OrdMoney, OrdSize, EventTime);
	pRest->OrderArrival(pOrd);
}
