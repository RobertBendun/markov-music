CXX=g++
CXXFLAGS=-Wall -Wextra -O3 -std=c++17
OUTPUT=graph-markov key2music random-markov-path midi/bigrams-notes midi/bigrams-dur

.PHONY: all
all: $(OUTPUT)

midi/%: midi/%.go
	make -C ./midi $(notdir $@)

%: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OUTPUT)
	make -C ./midi clean
