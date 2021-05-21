#pragma once
#include "Event.h"
class PromotionEvent :public Event
{
	double ExMoney; // Extra money paid to promote the order to VIP
public:
	PromotionEvent(int Etime, int Oid, double ExMoney);
	~PromotionEvent();
	virtual void Execute(Restaurant* pRest);
};

