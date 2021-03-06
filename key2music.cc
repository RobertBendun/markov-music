#include <iostream>
#include <charconv>
#include <cstring>
#include <cassert>
#include <ranges>
#include <algorithm>

#include "tsv.hh"

int main(int argc, char **argv)
{
	unsigned column = 0;
	if (argc >= 2) {
		auto [p, e] = std::from_chars(argv[1], argv[1] + std::strlen(argv[1]), column);
		if (p == argv[1]) {
			std::cerr << "cannot parse column number '" << argv[1] << "'\n";
			return 1;
		}
	}

	constexpr static char const* notes[] = {
		"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "B"
	};

	if (column == 0) {
		while (std::cin) {
			unsigned key;
			std::cin >> key;
			auto octave = key / 12, note = key % 12;
			std::cout << notes[note] << '\t' << octave << '\n';
		}
		return 0;
	}

	modify_tsv_column(column, [&](std::string const& column) {
		unsigned key = 0;
		auto [ptr, _] = std::from_chars(column.data(), column.data() + column.size(), key);
		assert(ptr != column.data());

		auto octave = key / 12, note = key % 12;
		std::cout << notes[note] << '\t' << octave;
	});
}
