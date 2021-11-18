#include <cassert>
#include <iostream>
#include <charconv>
#include <numeric>
#include <cstring>

#include "tsv.hh"

int main(int argc, char **argv)
{
	unsigned column = 0;
	if (argc == 2) {
		auto [p, e] = std::from_chars(argv[1], argv[1] + std::strlen(argv[1]), column);
		assert(p != argv[1]);
	}

	modify_tsv_column(column, [](std::string const& column) {
		unsigned dur;
		auto [p, c] = std::from_chars(column.data(), column.data() + column.size(), dur);
		assert(p != column.data());

		auto gcd = std::gcd(dur, 64);

		std::cout << (dur / gcd) << " / " << (64 / gcd);
	});
}
