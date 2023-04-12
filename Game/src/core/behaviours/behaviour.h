#pragma once

// TODO : how to handle circular dependencies?
class Object;

class Behaviour
{
public:
	virtual void Update(Object* owner, float timeDelta) = 0;
};