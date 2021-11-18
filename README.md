# Markov Chain Music

Repository related to topic in title on 'Muzyka algorytmiczna' (_Algorithmic Music_) course at the Adam Mickiewicz University in Poznań.

## Programs

- `graph-markov.cc` - turns Markov chain described by TSV file into Graphviz DOT file
- `key2music.cc` - transforms key number into note and octave
- `midi/midi-get.go` - transforms MIDI file into markov chain based on probability of occuring key `X` after key `Y`
- `random-markov-path.cc` - takes length of path and produces sequence of keys based on probabilities in TSV

## Dependencies

- C++ Compiler (supporting C++17)
- Go
- Make
