# Markov Chain Music

Repository related to topic in title on 'Muzyka algorytmiczna' (_Algorithmic Music_) course at the Adam Mickiewicz University in Poznań.

See also: [Music and Markov Chains on Wikipedia](https://en.wikipedia.org/wiki/Markov_chain#Music).

## Example usage

- Generate random sequence based on note X follows note Y frequencies in file `example.mid`
```console
$ ./midi/midi-get example.mid | ./random-markov-path 10 | ./key2music
D# 6
D# 6
D# 6
B 5
C# 5
F 5
B 5
F 5
B 5
B 5
B 5
```

## Programs

- `graph-markov.cc` - turns Markov chain described by TSV file into Graphviz DOT file
- `key2music.cc` - transforms key number into note and octave
- `midi/midi-get.go` - transforms MIDI file into markov chain based on probability of occuring key `X` after key `Y`
- `random-markov-path.cc` - takes length of path and produces sequence of keys based on probabilities in TSV

## Dependencies

- C++ Compiler (supporting C++17)
- Go
- Make
