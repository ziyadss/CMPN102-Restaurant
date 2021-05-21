#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h"
#include "..\Events\PromotionEvent.h"

Restaurant::Restaurant()
{
	pGUI = NULL;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();

	Run(mode);
}

void Restaurant::CooksCreator()
{
	srand(time(nullptr));
	Cook* cook;
	for (int i = 0; i < inVar[0]; i++)
	{
		cook = new Cook(i + 1, rand() % (inVar[4] - inVar[3] + 1) + inVar[3], rand() % (inVar[11] - inVar[10] + 1) + inVar[10], TYPE_NRM);
		NormalCooks.enqueue(cook, cook->getPriority());
	}

	for (int i = 0; i < inVar[1]; i++)
	{
		cook = new Cook(i + inVar[0] + 1, rand() % (inVar[6] - inVar[5] + 1) + inVar[5], rand() % (inVar[13] - inVar[12] + 1) + inVar[12], TYPE_VGAN);
		VeganCooks.enqueue(cook, cook->getPriority());
	}

	for (int i = 0; i < inVar[2]; i++)
	{
		cook = new Cook(i + inVar[1] + inVar[0] + 1, rand() % (inVar[8] - inVar[7] + 1) + inVar[7], rand() % (inVar[15] - inVar[14] + 1) + inVar[14], TYPE_VIP);
		VIPCooks.enqueue(cook, cook->getPriority());
	}
	Cook::setBO(inVar[9]);
}

void Restaurant::LoadFile()
{
	ifstream InFile;
	do
	{
		pGUI->PrintMessage("Enter filename to load from: ");
		fname = pGUI->GetString() + ".txt";
		InFile.open(fname, ios::in);
	} while (!(InFile.is_open()));
	//	Loop ends when a file is opened.
	float var;
	for (int i = 0; i < 21; i++)
	{
		InFile >> var;
		if (i == 16)
			var = var * 100;
		inVar[i] = var;

	}
	//	Reads variables from file.
	//	inVar elements are {N,G,V,SN_min,SN_max,SG_min,SG_max,SV_min,SV_max,BO,BN_min,BN_max,BG_min,BG_max,BV_min,BV_max,InjProb*100,RstPrd,AutoP,VIP_WT,M}
	CooksCreator();

	char type;
	for (int i = 0; i < inVar[20]; i++)
	{
		InFile >> type;
		switch (type)
		{
		case 'R':
			ReadArrivalEvent(InFile);
			break;
		case 'X':
			ReadCancellationEvent(InFile);
			break;
		case 'P':
			ReadPromotionEvent(InFile);
			break;
		default:
			break;
		}
	}
	InFile.close();
}

void Restaurant::ReadArrivalEvent(ifstream& file)
{
	char t;
	ORD_TYPE TYP;
	file >> t;
	switch (t)
	{
	case 'N':
		TYP = TYPE_NRM;
		break;
	case 'G':
		TYP = TYPE_VGAN;
		break;
	case 'V':
		TYP = TYPE_VIP;
		break;
	default:
		break;
	}
	int TS, ID, SIZE, MONY;
	file >> TS >> ID >> SIZE >> MONY;
	Event* e = new ArrivalEvent(TS, ID, TYP, SIZE, MONY);
	EventsQueue.enqueue(e);
}

void Restaurant::ReadCancellationEvent(ifstream& file)
{
	int TS, ID;
	file >> TS >> ID;
	Event* e = new CancellationEvent(TS, ID);
	EventsQueue.enqueue(e);
}

void Restaurant::ReadPromotionEvent(ifstream& file)
{
	int TS, ID, ExMony;
	file >> TS >> ID >> ExMony;
	Event* e = new PromotionEvent(TS, ID, ExMony);
	EventsQueue.enqueue(e);
}

void Restaurant::ServeNormalOrders(int ts)
{
	Order* o;
	Cook* c;
	while (!(NormalOrders.isEmpty()))
	{
		if (NormalCooks.dequeue(c))
		{
			NormalOrders.dequeue(o);
			Serve(o, c, ts);
		}
		else if (VIPCooks.dequeue(c))
		{
			NormalOrders.dequeue(o);
			Serve(o, c, ts);
		}
		else
			break;
	}
}

void Restaurant::ServeVeganOrders(int ts)
{
	Order* o;
	Cook* c;
	while (!(VeganOrders.isEmpty()) && VeganCooks.dequeue(c))
	{
		VeganOrders.dequeue(o);
		Serve(o, c, ts);
	}

}

void Restaurant::ServeVIPOrders(int ts)
{
	Order* o;
	Cook* c;
	while (!(VIPOrders.isEmpty()))
	{
		if (VIPCooks.dequeue(c))
		{
			VIPOrders.dequeue(o);
			Serve(o, c, ts);
		}
		else if (NormalCooks.dequeue(c))
		{
			VIPOrders.dequeue(o);
			Serve(o, c, ts);
		}
		else if (VeganCooks.dequeue(c))
		{
			VIPOrders.dequeue(o);
			Serve(o, c, ts);

		}
		else
			break;
	}
}

void Restaurant::CheckandServeUrgent(int ts)
{
	Order* o;
	Queue<Order*> L;
	while (VIPOrders.dequeue(o))
	{
		if (ts - o->getVIPTime() > inVar[19])
		{
			UrgentOrders.enqueue(o);
			urgent++;
		}
		else
			L.enqueue(o);
	}

	while (L.dequeue(o))
		VIPOrders.enqueue(o, o->getPriority());

	Cook* c;
	while (!(UrgentOrders.isEmpty()))
	{
		if (VIPCooks.dequeue(c))
		{
			UrgentOrders.dequeue(o);
			Serve(o, c, ts);
		}
		else if (NormalCooks.dequeue(c))
		{
			UrgentOrders.dequeue(o);
			Serve(o, c, ts);
		}
		else if (VeganCooks.dequeue(c))
		{
			UrgentOrders.dequeue(o);
			Serve(o, c, ts);

		}
		else if (OnBreakCooks.dequeue(c))
		{
			UrgentOrders.dequeue(o);
			Serve(o, c, ts);

		}
		else if (InjuredCooks.dequeue(c))
		{
			UrgentOrders.dequeue(o);
			Serve(o, c, ts);

		}
		else
			break;
	}
}

void Restaurant::Serve(Order* o, Cook* c, int ts)
{
	o->setStatus(SRV);
	o->setServTime(ts);

	int time = ceil(o->getSize() / c->getSpeed());
	o->setFinishTime(ts + time);
	c->giveOrder(o);
	InService.enqueue(o, o->getPriority());
	BusyCooks.enqueue(c, c->getPriority());

	TypeID.enqueue(stringPrep(c, o));
}

string Restaurant::stringPrep(Cook* c, Order* o)
{
	char ct, ot;
	switch (c->getType())
	{
	case TYPE_NRM:
		ct = 'N';
		break;
	case TYPE_VGAN:
		ct = 'G';
		break;
	case TYPE_VIP:
		ct = 'V';
	}
	switch (o->getType())
	{
	case TYPE_NRM:
		ot = 'N';
		break;
	case TYPE_VGAN:
		ot = 'G';
		break;
	case TYPE_VIP:
		ot = 'V';
	}
	return ct + to_string(c->getID()) + "(" + ot + to_string(o->getID()) + ")";
}

void Restaurant::FinishOrders(int ts)
{
	Order* o;
	while (InService.peekFront(o))
	{
		if (o->getStatus() == DELAYED)
		{
			InService.dequeue(o);
			o->setStatus(SRV);
			InService.enqueue(o, o->getPriority());
		}
		else if (o->getFinTime() == ts)
		{
			InService.dequeue(o);
			o->setStatus(DONE);
			Finished.enqueue(o);
			if (o->getType() == TYPE_NRM)
				ServedNormal++;
			else if (o->getType() == TYPE_VGAN)
				ServedVegan++;
			else if (o->getType() == TYPE_VIP)
				ServedVIP++;
		}
		else break;
	}
}

void Restaurant::InjuryCheck(int ts)
{
	Cook* c;
	if (rand() % 101 < inVar[16] && BusyCooks.dequeue(c))
	{
		c->Injure(ts);
		BusyCooks.enqueue(c, c->getPriority());
	}
}

void Restaurant::OutputFile()
{
	ofstream OutFile;
	OutFile.open("Out_" + fname, ios::out);

	OutFile << "FT ID AT WT ST" << endl;
	Order* o;
	int N = 0, G = 0, V = 0, WTsum = 0, STsum = 0, AP = Order::getAPcount();
	while (Finished.dequeue(o))
	{
		OutFile << o->getFinTime() << " " << o->getID() << " " << o->getArrTime() << " " << o->getServTime() - o->getArrTime() << " " << o->getFinTime() - o->getServTime() << endl;
		WTsum += (o->getServTime() - o->getArrTime());
		STsum += (o->getFinTime() - o->getServTime());
		switch (o->getType())
		{
		case TYPE_NRM:
			N++;
			break;
		case TYPE_VGAN:
			G++;
			break;
		case TYPE_VIP:
			V++;
			break;
		}
	}
	OutFile << "Orders: " << N + G + V << " [Normal:" << N << ", Vegan:" << G << ", VIP:" << V << "]" << endl;
	OutFile << "Cooks: " << inVar[0] + inVar[1] + inVar[2] << " [Normal:" << inVar[0] << ", Vegan:" << inVar[1] << ", VIP:" << inVar[2] << ", Injuries:" << Cook::getInjCount() << "]" << endl;
	OutFile << "Average waiting time = " << WTsum / (inVar[0] + inVar[1] + inVar[2]) << ", Average serving time = " << STsum / (inVar[0] + inVar[1] + inVar[2]) << endl;
	OutFile << "Urgent orders: " << urgent << ", Auto-promoted orders: " << 100 * AP / (AP + N) << "%" << endl;
	OutFile.close();
}

//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}
//-------------------------Events execution-----------------------------------//
void Restaurant::OrderArrival(Order* pOrd)
{
	switch (pOrd->getType())
	{
	case(TYPE_VIP):
		VIPOrders.enqueue(pOrd, pOrd->getPriority());
		break;
	case(TYPE_NRM):
		NormalOrders.enqueue(pOrd);
		break;
	case(TYPE_VGAN):
		VeganOrders.enqueue(pOrd);
		break;
	default:
		break;
	}
}

void Restaurant::PromoteOrder(int id, int m, int ts)
{
	Order* o;
	if (NormalOrders.remove(id, o))
	{
		o->Promote(m,ts);
		VIPOrders.enqueue(o, o->getPriority());
	}
}

void Restaurant::CancelOrder(int id)
{
	Order* o;
	if (NormalOrders.remove(id, o))
		delete o;
}

Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
}

void Restaurant::FillDrawingList()
{
	Cook** c = NormalCooks.toArray();
	for (int i = 0; i < NormalCooks.getCount(); i++)
		pGUI->AddToDrawingList(c[i]);
	c = VeganCooks.toArray();
	for (int i = 0; i < VeganCooks.getCount(); i++)
		pGUI->AddToDrawingList(c[i]);
	c = VIPCooks.toArray();
	for (int i = 0; i < VIPCooks.getCount(); i++)
		pGUI->AddToDrawingList(c[i]);
	Order** o = NormalOrders.toArray();
	for (int i = 0; i < NormalOrders.getCount(); i++)
		pGUI->AddToDrawingList(o[i]);
	o = VeganOrders.toArray();
	for (int i = 0; i < VeganOrders.getCount(); i++)
		pGUI->AddToDrawingList(o[i]);
	o = UrgentOrders.toArray();
	for (int i = 0; i < UrgentOrders.getCount(); i++)
		pGUI->AddToDrawingList(o[i]);
	o = VIPOrders.toArray();
	for (int i = 0; i < VIPOrders.getCount(); i++)
		pGUI->AddToDrawingList(o[i]);
	o = InService.toArray();
	for (int i = 0; i < InService.getCount(); i++)
		pGUI->AddToDrawingList(o[i]);
	o = Finished.toArray();
	for (int i = 0; i < Finished.getCount(); i++)
		pGUI->AddToDrawingList(o[i]);
}
void Restaurant::AutoPromotion(int ts)
{
	Order* o;
	while (NormalOrders.peekFront(o) && ts - o->getArrTime() == inVar[18])
	{
		NormalOrders.dequeue(o);
		o->Promote(0,ts);
		VIPOrders.enqueue(o, o->getPriority());
	}
}

void Restaurant::RestoreCooks(int ts)
{
	Cook* c;
	while (BusyCooks.peekFront(c) && c->getNextAvailable() == ts)
	{
		BusyCooks.dequeue(c);
		if (c->getStatus() == PREINJURY)
		{
			c->setNextAvailable(ts + inVar[17]);
			InjuredCooks.enqueue(c);
		}
		else if (c->checkForBreak(ts))
			OnBreakCooks.enqueue(c, c->getPriority());
		else
		{
			switch (c->getType())
			{
			case TYPE_NRM:
				NormalCooks.enqueue(c, c->getPriority());
				break;
			case TYPE_VGAN:
				VeganCooks.enqueue(c, c->getPriority());
				break;
			case TYPE_VIP:
				VIPCooks.enqueue(c, c->getPriority());
			}
			if (c->getStatus() == BUSY)
				c->setStatus(AVAILABLE);
		}
	}
	while (InjuredCooks.peekFront(c) && c->getNextAvailable() == ts)
	{
		InjuredCooks.dequeue(c);
		c->setStatus(AVAILABLE);
		switch (c->getType())
		{
		case TYPE_NRM:
			NormalCooks.enqueue(c, c->getPriority());
			break;
		case TYPE_VGAN:
			VeganCooks.enqueue(c, c->getPriority());
			break;
		case TYPE_VIP:
			VIPCooks.enqueue(c, c->getPriority());
		}
	}
	while (OnBreakCooks.peekFront(c) && c->getNextAvailable() == ts)
	{
		OnBreakCooks.dequeue(c);
		c->setStatus(AVAILABLE);
		switch (c->getType())
		{
		case TYPE_NRM:
			NormalCooks.enqueue(c, c->getPriority());
			break;
		case TYPE_VGAN:
			VeganCooks.enqueue(c, c->getPriority());
			break;
		case TYPE_VIP:
			VIPCooks.enqueue(c, c->getPriority());
		}
	}
}

void Restaurant::Run(PROG_MODE m)
{
	LoadFile();
	int ts = 0;
	pGUI->PrintMessage("Click anywhere to start simulation.");
	pGUI->waitForClick();
	while (!EventsQueue.isEmpty() || !InService.isEmpty() || !VIPOrders.isEmpty() || !VeganOrders.isEmpty() || !NormalOrders.isEmpty())
	{
		ts++;
		ExecuteEvents(ts);
		FinishOrders(ts);
		RestoreCooks(ts);

		CheckandServeUrgent(ts);
		ServeVIPOrders(ts);
		ServeVeganOrders(ts);
		ServeNormalOrders(ts);


		InjuryCheck(ts);
		AutoPromotion(ts);
		if (m != MODE_SLNT)
		{
			FillDrawingList();
			pGUI->UpdateInterface();
			pGUI->ResetDrawingList();

			string st1 = "Timestep: " + to_string(ts);
			string st2 = "Waiting orders, available cooks, and served orders: ";
			string st3 = "VIP: " + to_string(UrgentOrders.getCount()+VIPOrders.getCount()) + " waiting orders, " + to_string(VIPCooks.getCount()) + " available cooks, and " + to_string(ServedVIP) + " served orders.";
			string st4 = "Vegan: " + to_string(VeganOrders.getCount()) + " waiting orders, " + to_string(VeganCooks.getCount()) + " available cooks, and " + to_string(ServedVegan) + " served orders.";
			string st5 = "Normal: " + to_string(NormalOrders.getCount()) + " waiting orders, " + to_string(NormalCooks.getCount()) + " available cooks, and " + to_string(ServedNormal) + " served orders.";

			string st, st6;
			if (TypeID.dequeue(st))
				st6 += st;
			while (TypeID.dequeue(st))
				st6 += ", " + st;

			pGUI->PrintMessage(st1, st2, st3, st4, st5, st6);
		}
		if (m == MODE_STEP)
			Sleep(1000);
		else if (m == MODE_INTR)
			pGUI->waitForClick();
	}
	OutputFile();
	pGUI->PrintMessage("Simulation ended and output file produced as " "Out_" + fname, "Click anywhere to exit.");
	pGUI->waitForClick();
}