#pragma once
#include <cstdint>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>

class PrimeFactors {
public:
	PrimeFactors(uint64_t);
	void Factorize();
	std::string ToString() const;
	~PrimeFactors() = default;
private:
	const uint64_t number;
	std::vector<std::pair<int, unsigned> > factors;
};