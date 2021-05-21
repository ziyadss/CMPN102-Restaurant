#include "Cook.h"

Cook::Cook()
{
}

Cook::~Cook()
{
}

int Cook::getID() const
{
	return ID;
}

ORD_TYPE Cook::getType() const
{
	return type;
}

void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}
int Cook::breakOrders = 10;
void Cook::setBO(int b)
{
	breakOrders = b;
}
int Cook::injuriesCount = 0;
int Cook::getInjCount()
{
	return injuriesCount;
}
////////////////////////////////////////////////////////////////////////////////////////////
void Cook::setBreakTime(int BT)
{
	BreakTime = BT > 0 ? BT : 0;
}

int Cook::getBreakTime() const
{
	return BreakTime;
}

void Cook::setSpeed(int sp)
{
	speed = sp > 0 ? sp : 0;
}

float Cook::getSpeed() const
{
	if (status == INJURED || status == PREINJURY)
		return speed / 2;
	return speed;
}

void Cook::giveOrder(Order* o)
{
	if (status == AVAILABLE)
		status = BUSY;
	currOrder = o;
	availTS = o->getFinTime();
}

Cook::Cook(int iD, int sped, int breaktime, ORD_TYPE Otype)
{
	ID = (iD > 0 && iD < 1000) ? iD : 0;	//1<ID<999
	speed = sped > 0 ? sped : 0;
	BreakTime = breaktime > 0 ? breaktime : 0;
	type = Otype;
	status = AVAILABLE;
	availTS = 0;
}
Order* Cook::getOrder() const
{
	return currOrder;
}
COOK_STATUS Cook::getStatus() const
{
	return status;
}
float Cook::getPriority() const
{
	if (status == AVAILABLE)
		return speed;
	return -availTS;
}
bool Cook::checkForBreak(int TS)
{
	if (servedOrders == breakOrders)
	{
		status = BREAK;
		servedOrders = 0;
		availTS = TS + BreakTime;
		return true;
	}
	return false;
}
void Cook::Injure(int TS)
{
	if (status == INJURED || status == PREINJURY)
		return;
	injuriesCount++;
	status = PREINJURY;
	servedOrders = 0; // Why?? To prevent double breaks since he is already going to take a break due to his injury 
	int remainingOrderSize = currOrder->getSize() - speed * (TS - currOrder->getServTime());
	availTS = TS + ceil(remainingOrderSize / getSpeed());
	currOrder->setFinishTime(availTS); // do i need to change the order position InService queue??
	currOrder->setStatus(DELAYED);  // for orders that has been assigned to an injured cook and it's finish time has been changed to reposition order in (InService Queue)
}

void Cook::setStatus(COOK_STATUS C_Status)
{
	status = C_Status;
}

int Cook::getNextAvailable() const
{
	return availTS;
}
void Cook::setNextAvailable(int t)
{
	availTS = t;
}