#pragma once
#include"Event.h"
class CancellationEvent :public Event
{

public:
	CancellationEvent(int Etime, int Oid);
	~CancellationEvent();
	virtual void Execute(Restaurant* pRest);
};