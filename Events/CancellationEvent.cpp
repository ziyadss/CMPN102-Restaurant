#include "CancellationEvent.h"
#include "..\Rest\Restaurant.h"
CancellationEvent::CancellationEvent(int time, int id) :Event(time, id)
{

}
CancellationEvent::~CancellationEvent()
{

}
void CancellationEvent::Execute(Restaurant* pRest)
{
	pRest->CancelOrder(OrderID);
}