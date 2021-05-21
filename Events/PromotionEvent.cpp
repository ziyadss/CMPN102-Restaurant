#include "PromotionEvent.h"
#include "..\Rest\Restaurant.h"
PromotionEvent::PromotionEvent(int Etime, int Oid, double money) :Event(Etime, Oid)
{
	ExMoney = money;
}
PromotionEvent::~PromotionEvent()
{

}
void PromotionEvent::Execute(Restaurant* pRest)
{
	pRest->PromoteOrder(OrderID, ExMoney, EventTime);
}
