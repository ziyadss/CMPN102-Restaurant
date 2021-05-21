#pragma once

#include "..\Defs.h"
#include "Order.h"
#include <cmath>

#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	float speed;		//dishes it can prepare in one clock tick (in one timestep)
	int BreakTime; // BreakTime for each cook after certain n time step
	static int breakOrders; //Orders after which cook will need a break
	static int injuriesCount;
	int availTS; //to be calculated when cook given break/order, is next timestep he will be available on
	COOK_STATUS status;
	Order* currOrder;
	int servedOrders; // number of served orders (resets after each break)

public:
	Cook();
	Cook(int ID, int speed, int breaktime, ORD_TYPE Otype);
	virtual ~Cook();
	int getID() const;
	ORD_TYPE getType() const;
	void setID(int);
	void setType(ORD_TYPE);
	static void setBO(int b);
	static int getInjCount();
	////////////////////////////////////////////////////////
	void setBreakTime(int BT);
	int getBreakTime() const;
	void setSpeed(int sp);
	float getSpeed() const;
	void giveOrder(Order* o);
	Order* getOrder() const;
	COOK_STATUS getStatus() const;
	float getPriority() const;
	int getNextAvailable() const;
	void setNextAvailable(int t);

	bool checkForBreak(int TimeStep);
	void Injure(int Timestep);
	void setStatus(COOK_STATUS cstatus);
};
