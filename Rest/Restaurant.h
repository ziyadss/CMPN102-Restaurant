#pragma once

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include"..\Generic_DS\PriorityQueue.h"
#include"..\Generic_DS\LinkedList.h"
#include "..\Events\Event.h"
#include <fstream>

#include "Order.h"

// it is the maestro of the project
class Restaurant
{
private:
	GUI* pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file

	PriorityQueue<Cook*> NormalCooks;
	PriorityQueue<Cook*> VeganCooks;
	PriorityQueue<Cook*> VIPCooks;

	PriorityQueue<Cook*> BusyCooks;
	Queue<Cook*> InjuredCooks;
	PriorityQueue<Cook*> OnBreakCooks;


	LinkedList<Order*>NormalOrders;
	Queue<Order*> VeganOrders;
	PriorityQueue<Order*> VIPOrders;
	Queue<Order*> UrgentOrders;

	int ServedVIP = 0;
	int ServedVegan = 0;
	int ServedNormal = 0;

	Queue<Order*> Finished;
	PriorityQueue<Order*>InService;
	LinkedList<string>TypeID;
	int urgent = 0;
	string fname;
	int inVar[21];
	//	inVar elements are {N,G,V,SN_min,SN_max,SG_min,SG_max,SV_min,SV_max,BO,BN_min,BN_max,BG_min,BG_max,BV_min,BV_max,InjProb*100,RstPrd,AutoP,VIP_WT,M}
	//yet to use	InjProb	=	inVar[16] / 100
	//				RstPrd	=	inVar[17]
	//				AutoP	=	inVar[18]
	//				VIP_WT	=	inVar[19]

public:

	Restaurant();
	~Restaurant();

	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();

	void PromoteOrder(int id, int m,int ts);
	void CancelOrder(int id);
	void OrderArrival(Order* pOrd);


	void CooksCreator(); //	Creates and places cooks in their respective queues.
	void LoadFile(); //	Loads input file, reads variables and events.
	void ReadArrivalEvent(ifstream& file); //	Reads an ArrivalEvent.
	void ReadCancellationEvent(ifstream& file); //	Reads an CancellationEvent.
	void ReadPromotionEvent(ifstream& file); //	Reads an PromotionEvent.

	void ServeNormalOrders(int ts);
	void ServeVeganOrders(int ts);
	void ServeVIPOrders(int ts);
	void CheckandServeUrgent(int ts);

	void Serve(Order* o, Cook* c, int ts);
	void FinishOrders(int ts);
	void InjuryCheck(int ts);

	void OutputFile();
	void AutoPromotion(int ts);
	void FillDrawingList();

	void RestoreCooks(int ts);
	string stringPrep(Cook* c, Order* o);
	void Run(PROG_MODE m);
};