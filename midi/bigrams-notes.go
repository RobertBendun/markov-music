package main

import (
	"fmt"
	"log"
	"os"
	"path"

	"gitlab.com/gomidi/midi/reader"
)

type Note = uint8
type AbsoluteTicks = uint64

type entry struct {
	note    Note
	channel uint8
}

type tracker struct {
	noteStartTime map[entry]struct{}
	track         [][]uint8
}

func (nt *tracker) appendToTrack() {
	chord := []uint8{}
	for entry := range nt.noteStartTime {
		isUnique := true
		for _, s := range chord {
			if s == entry.note {
				isUnique = false
				break
			}
		}
		if isUnique {
			chord = append(chord, entry.note)
		}
	}

	if len(chord) > 0 {
		nt.track = append(nt.track, chord)
	}
}

func (nt tracker) noteOn(p *reader.Position, channel, key, vel uint8) {
	if _, ok := nt.noteStartTime[entry{key, channel}]; !ok {
		nt.noteStartTime[entry{key, channel}] = struct{}{}
	}
}

func (nt *tracker) noteOff(p *reader.Position, channel, key, vel uint8) {
	mapKey := entry{key, channel}
	_, ok := nt.noteStartTime[mapKey]
	if !ok {
		return
		panic("Note is off that has not appeared before")
	}

	nt.appendToTrack()
	delete(nt.noteStartTime, mapKey)
}

type pair struct {
	fst uint8
	snd uint8
}

func bigramsFrequency(input [][]uint8) map[pair]float32 {
	result := make(map[pair]float32)
	count := 0
	for i := 1; i < len(input); i++ {
		for _, snd := range input[i] {
			for _, fst := range input[i-1] {
				key := pair{fst, snd}
				if v, ok := result[key]; ok {
					result[key] = v + 1
				} else {
					result[key] = 1
				}
				count++
			}
		}
	}

	for key, val := range result {
		result[key] = val / float32(count)
	}

	return result
}

func main() {
	var nt tracker
	nt.noteStartTime = make(map[entry]struct{})

	rd := reader.New(
		reader.NoLogger(),
		reader.NoteOn(nt.noteOn),
		reader.NoteOff(nt.noteOff),
	)

	if len(os.Args) == 1 {
		fmt.Printf("usage: %s midifile...\n", path.Base(os.Args[0]))
	}

	for _, file := range os.Args[1:] {
		err := reader.ReadSMFFile(rd, file)
		if err != nil {
			log.Fatalln(err)
		}
	}

	for key, val := range bigramsFrequency(nt.track) {
		fmt.Printf("%d\t%d\t%f\n", key.fst, key.snd, val)
	}
}
