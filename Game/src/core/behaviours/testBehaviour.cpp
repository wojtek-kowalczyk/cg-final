#include "testBehaviour.h"
#include "../object.h"

#include <cmath>

void TestBehaviour::Update(Object* owner, float timeDelta)
{
	// Test animation
	static float timer = 0.0f;
	timer += timeDelta;
	float sinvalue = sinf(timer);
	owner->Position.y = 1.0f + sinvalue;
}
