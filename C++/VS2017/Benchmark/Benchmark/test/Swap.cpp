#include "benchmark/benchmark.h"

class Vector
{
public:
	int x;
	int y;
	int z;


	Vector() : Vector(0, 0, 0)
	{
		
	}

	Vector(int x, int y, int z)
		: x(x),
		  y(y),
		  z(z)
	{
	}

	Vector(const Vector& other)
		: x(other.x),
		  y(other.y),
		  z(other.z)
	{
	}

	Vector(Vector&& other) noexcept
		: x(other.x),
		  y(other.y),
		  z(other.z)
	{
	}

	Vector& operator=(const Vector& other)
	{
		if (this == &other)
			return *this;
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vector& operator=(Vector&& other) noexcept
	{
		if (this == &other)
			return *this;
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}


	friend void swap(Vector& lhs, Vector& rhs) noexcept
	{
		using std::swap;
		swap(lhs.x, rhs.x);
		swap(lhs.y, rhs.y);
		swap(lhs.z, rhs.z);
	}
};

const int VecNum = 1000000;
Vector Vec[VecNum];
Vector Vec2[VecNum];

static void Swap1(benchmark::State& state) {
	for(auto _ : state)
	{
		for (int i = 0; i < VecNum; i++)
		{
			Vec[i] = Vec2[i];
		}
	}
	
}
BENCHMARK(Swap1);

static void Swap2(benchmark::State& state) {
	for (auto _ : state)
	{
		for (int i = 0; i < VecNum; i++)
		{
			std::swap(Vec[i], Vec2[i]);
		}
	}
}
BENCHMARK(Swap2);

BENCHMARK_MAIN();