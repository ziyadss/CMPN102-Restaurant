#pragma once

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int Size;	// Number of dishes in order

	double totalMoney;	//Total order money

	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times

	int VIPTime;

	static int autoPromoted;

public:
	Order(int ID, ORD_TYPE r_Type);
	virtual ~Order();

	static int getAPcount();

	int getID();

	ORD_TYPE getType() const;

	void setSize(int s);
	int getSize() const;
	void settotalMoney(double m);

	double gettotalMoney();
	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;

	void setServTime(int t);
	void setFinishTime(int t);
	int getArrTime();
	int getServTime();
	int getFinTime();
	void Promote(int m,int ts);
	int getPriority() const;
	Order(int ID, ORD_TYPE r_Type, double Money, int Osize, int ArrivalTime);

	int getVIPTime() const;
};