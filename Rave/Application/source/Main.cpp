#include <iostream>
#include <functional>
#include "Engine/Engine.h"

static constexpr int foo()
{
	rv::Vector<2, int> vec1 = rv::Vector<2, int>(2, 3);
	rv::Vector<2, int> vec2 = rv::Vector<2, int>(3, 7);

	vec1 += vec2.array[0];

	return (vec1 + vec2.array[0]).array[1];
}

struct A
{
	static constexpr int i = foo();
};

rv::Result<void> rave_main()
{
	int i = A::i;
	return rv_success;
}