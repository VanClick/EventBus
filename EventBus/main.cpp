#include "Event.h"
#include "EventBus.h"

#include <iostream>
using namespace std;

struct Event1 : public Event
{
	int a;

	static const int ID = 1;
	virtual int GetId()
	{
		return ID;
	}
};

struct Event2 : public Event
{
	int b;

	static const int ID = 2;
	virtual int GetId()
	{
		return ID;
	}
};

class MyClass1
{
public:
	void Register(EventBus* eb)
	{
		eb->RegisterHandler(Event1Handler, this);
		eb->RegisterHandler(Event2Handler, this);
	}

	void Unregister(EventBus* eb)
	{
		eb->UnregisterHandler(Event1Handler, this);
		eb->UnregisterHandler(Event2Handler, this);
	}

private:
	static int Event1Handler(Event1* evt, MyClass1* self)
	{
		cout << "MyClass1 " << self->a << " Event1Handler " << evt->a << " called" << endl;
		return 0;
	}

	static int Event2Handler(Event2* evt, MyClass1* self)
	{
		cout << "MyClass1 " << self->a << " Event2Handler " << evt->b << " called" << endl;
		return 0;
	}

private:
	int a = 345;
};

class MyClass2
{
public:
	void Register(EventBus* eb)
	{
		eb->RegisterHandler(Event1Handler, this);
		eb->RegisterHandler(Event2Handler, this);
	}

	void Unregister(EventBus* eb)
	{
		eb->UnregisterHandler(Event1Handler, this);
		//eb->UnregisterHandler(Event2Handler, this);
	}

private:
	static int Event1Handler(Event1* evt, MyClass2* self)
	{
		cout << "MyClass2 " << self->a << " Event1Handler " << evt->a << " called" << endl;
		return 0;
	}

	static int Event2Handler(Event2* evt, MyClass2* self)
	{
		cout << "MyClass2 " << self->a << " Event2Handler " << evt->b << " called" << endl;
		return 0;
	}

private:
	int a = 678;
};

int main()
{
	EventBus* eb = new EventBus();
	MyClass1* c1 = new MyClass1();
	MyClass2* c2 = new MyClass2();

	{
		printf("Register:\n");

		c1->Register(eb);
		c1->Register(eb);
		c2->Register(eb);
		c1->Register(eb);
		c1->Register(eb);

		eb->PrintHandlers();
	}
	{
		printf("SendEvent:\n");

		Event1 evt1;
		evt1.a = 1111;

		Event2 evt2;
		evt2.b = 2222;

		eb->SendEvent(evt1);
		eb->SendEvent(evt2);
	}
	{
		printf("PushEvent:\n");

		Event1* pEvt1 = new Event1();
		pEvt1->a = 1111;

		Event2* pEvt2 = new Event2();
		pEvt2->b = 2222;

		eb->PushEvent(pEvt1);
		eb->PushEvent(pEvt2);

		printf("Dispatch:\n");

		eb->Dispatch();
	}
	{
		printf("Unregister:\n");

		c1->Unregister(eb);
		c2->Unregister(eb);

		eb->PrintHandlers();
	}
	{
		printf("SendEvent:\n");

		Event1 evt1;
		evt1.a = 1111;

		Event2 evt2;
		evt2.b = 2222;

		eb->SendEvent(evt1);
		eb->SendEvent(evt2);
	}

	char end = getchar();
}

