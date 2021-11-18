package main

import (
	"os"
	"fmt"
	"path"
	"log"

	"gitlab.com/gomidi/midi/reader"
	"gitlab.com/gomidi/midi/smf"
)

type Note = uint8
type AbsoluteTicks = uint64

type entry struct {
	note    Note
	channel uint8
}

type duration struct {
	note uint8
	start, end AbsoluteTicks
	time uint32
}

type tracker struct {
	noteStartTime map[entry]AbsoluteTicks
	track         []duration
}

func (t *tracker) appendToTrack(e entry, start, end AbsoluteTicks) {
	t.track = append(t.track, duration { e.note, start, end, 0 })
}

func (nt tracker) noteOn(p *reader.Position, channel, key, vel uint8) {
	if _, ok := nt.noteStartTime[entry{key, channel}]; !ok  {
		nt.noteStartTime[entry{key, channel}] = p.AbsoluteTicks
	}
}

func (nt *tracker) noteOff(p *reader.Position, channel, key, vel uint8) {
	mapKey := entry{key, channel}
	start, ok := nt.noteStartTime[mapKey]
	if !ok {
		return
		panic("Note is off that has not appeared before")
	}
	nt.appendToTrack(mapKey, start, p.AbsoluteTicks)
	delete(nt.noteStartTime, mapKey)
}

/*
note dur probability
*/

func main() {
	var track []duration

	if len(os.Args) == 1 {
		fmt.Printf("usage: %s midifile...\n", path.Base(os.Args[0]))
	}

	for _, file := range os.Args[1:] {
		var t tracker
		t.noteStartTime = make(map[entry]uint64)

		rd := reader.New(
			reader.NoLogger(),
			reader.NoteOn(t.noteOn),
			reader.NoteOff(t.noteOff),
		)

		err := reader.ReadSMFFile(rd, file)
		if err != nil {
			log.Fatalln(err)
		}

		mt := rd.Header().TimeFormat.(smf.MetricTicks)
		for i, v := range t.track {
			t.track[i].time = mt.In64ths(uint32(v.end - v.start))
		}

		track = append(track, t.track...)
	}

	type key struct { note Note; dur uint32 }
	durations := make(map[key]uint32)
	durationsOptionsCount := make(map[Note]uint32)

	for _, dur := range track {
		k := key{dur.note, dur.time}
		if _, ok := durations[k]; ok {
			durations[k]++
		} else {
			durations[k] = 1
		}

		durationsOptionsCount[dur.note]++
	}

	for key, v := range durations {
		fmt.Printf("%d\t%d\t%f\n", key.note, key.dur, float32(v) / float32(durationsOptionsCount[key.note]))
	}
}
