#include <iostream>

int main()
{
	constexpr static char const* notes[] = {
		"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "B"
	};

	while (std::cin) {
		unsigned key;
		std::cin >> key;

		auto octave = key / 12;
		auto note = key % 12;

		std::cout << notes[note] << '\t' << octave << '\n';
	}
}
