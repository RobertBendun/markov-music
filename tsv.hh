#pragma once

#include <ranges>
#include <algorithm>
#include <iostream>
#include <string>

void modify_tsv_column(unsigned column, auto &&callback)
{
	for (std::string line; std::getline(std::cin, line); ) {
		auto n = column;
		auto fst = true;

		for (auto const column_range : std::views::split(line, '\t')) {
			if (fst) {
				fst = false;
			} else {
				std::cout << '\t';
			}
			std::string column;
			std::ranges::copy(column_range, std::back_inserter(column));

			if (--n == 0) {
				callback(column);
			} else {
				std::cout << column;
			}
		}

		std::cout << '\n';
	}
}
