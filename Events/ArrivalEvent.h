#pragma once
#include "Event.h"

//class for the arrival event
class ArrivalEvent : public Event
{
	//info about the order ralted to arrival event
	ORD_TYPE OrdType;		//order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money
	/////////////////////////////////////////////////
	int OrdSize;
	////////////////////////////////////////////////
public:
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType);
	//Add more constructors if needed
	//////////////////////////////////////////////
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int Osize, double Omoney);
	/////////////////////////////////////////////
	virtual void Execute(Restaurant* pRest);	//override execute function
};