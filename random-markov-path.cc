#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <cassert>
#include <cstring>
#include <charconv>
#include <algorithm>

using Note = unsigned;

struct Val
{
	std::vector<Note>   next;
	std::vector<double> prob;
};

std::map<Note, Val> markov;

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cout << "usage: " << basename(argv[0]) << " <length>\n";
		std::cout << "  receives at STDIN TSV data with columns `src<TAB>dst<TAB>prob<LF>` and produces random sequence\n";
		std::cout << "  based on probabilites from this TSV. Header is not expected so trim it if you have it\n";
		return 1;
	}

	unsigned length = 0;
	{
		auto arg1 = std::string_view(argv[1]);
		auto [end, ec] = std::from_chars(arg1.data(), arg1.data() + arg1.size(), length);
		if (end == arg1.data()) {
			std::cout << basename(argv[0]) << ": error: '" << arg1 << "' is not an integer\n";
			return 1;
		}
	}

	while (std::cin) {
		unsigned fst, snd;
		float prob;
		std::cin >> fst >> snd >> prob;

		auto &curr = markov[fst];
		curr.next.push_back(snd);
		curr.prob.push_back(prob);
	}

	std::random_device rd;
	// Make room for simple device change
	auto &device = rd;

	std::uniform_int_distribution<decltype(markov.size())> markov_dist(0, markov.size() - 1);

	// choose first note randomly from distribution
	auto index = markov_dist(device);
	for (unsigned i = 0; i < length; ++i) {
		auto const &[note, val] = *std::next(std::cbegin(markov), index);
		std::cout << note << '\n';

		// choose next based on probabilites from markov chain
		std::discrete_distribution<decltype(markov.size())> next_dist(std::cbegin(val.prob), std::cend(val.prob));
		auto const next_note = *std::next(std::cbegin(val.next), next_dist(device));
		index = std::distance(std::cbegin(markov), std::find_if(std::cbegin(markov), std::cend(markov), [next_note](auto const& p) { return p.first == next_note; }));
		assert(index < markov.size());
	}
}
