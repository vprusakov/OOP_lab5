#include "PrimeFactors.h"

PrimeFactors::PrimeFactors(uint64_t number) : number(number) {
	Factorize();
}
void PrimeFactors::Factorize() {
	uint64_t curr_number = number;
	int probe;
	unsigned power;
	for (probe = 2; probe <= curr_number; ++probe) {
		power = 0;
		while (curr_number % probe == 0) {
			++power;
			curr_number /= probe;
		}
		if (power != 0) {
			factors.push_back(std::make_pair(probe, power));
		}
	}
}

std::string PrimeFactors::ToString() const {
	uint64_t temp = 1;
	std::string result(std::to_string(number) + " = 1");
	unsigned l = factors.size();
	for (unsigned i = 0; i != l; ++i) {
		if (factors[i].second == 1) {
			result += " * " + std::to_string(factors[i].first);
		}
		else {
			result += " * " + std::to_string(factors[i].first) + '^' + std::to_string(factors[i].second);
		}
	}
	return result;
}