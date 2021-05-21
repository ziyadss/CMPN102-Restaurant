#pragma once

enum ORD_TYPE	//types of orders
{
	TYPE_NRM,	//normal order
	TYPE_VGAN,	//vegan
	TYPE_VIP,	//VIP
	TYPE_CNT	//Number of types

};

enum ORD_STATUS	//order status
{
	WAIT,	//waiting to be served
	SRV,	//In-service but not delivered (not finished) yet
	DONE,	//Delivered (finished) to its destination
	DELAYED, // for delayed orders due to cook injury 
	ORD_STATUS_CNT
};

enum COOK_STATUS // current status of the cook
{
	AVAILABLE, // Ready to take order at anytime
	BUSY, // assigned with order and working on it 
	BREAK,
	PREINJURY,
	INJURED,
	COOK_STATUS_CNT
};

enum PROG_MODE	//mode of the program interface
{
	MODE_INTR,	//interactive mode
	MODE_STEP,	//Step-by-step mode
	MODE_SLNT,	//Silent mode
	MODE_CNT	//number of possible modes
};

#define MaxPossibleOrdCnt 999	//max possible order count (arbitrary value)
#define MaxPossibleMcCnt  100	//max possible cook count (arbitrary value)