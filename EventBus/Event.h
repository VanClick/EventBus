#pragma once
struct Event
{
public:
	Event() {}
	virtual ~Event() {}

	virtual int GetId() = 0;
};