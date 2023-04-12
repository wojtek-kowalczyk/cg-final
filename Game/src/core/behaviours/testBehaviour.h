#pragma once

#include "behaviour.h"

class TestBehaviour : public Behaviour
{
public:
	void Update(Object* owner, float deltaTime) override;
};