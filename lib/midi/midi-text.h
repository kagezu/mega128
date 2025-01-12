#pragma once
#include <avr.h>

static const char *MIDI_pgm[] PROGMEM = {
"Instrument",
"Acoustic Grand Piano",           // Index 1
"Bright Acoustic Piano",
"Electric Grand Piano",
"Honky - tonk Piano",
"Rhodes Piano",
"Chorused Piano",
"Harpsichord",
"Clavi",
"Celesta",
"Glockenspiel",
"Music Box",
"Vibraphone",
"Marimba",
"Xylophone",
"Tubular Bells",
"Dulcimer",
"Drawbar Organ",
"Percussive Organ",
"Rock Organ",
"Church Organ",
"Reed Organ",
"Accordion",
"Harmonica",
"Tango Accordion",
"Acoustic Guitar(nylon)",
"Acoustic Guitar(steel)",
"Electric Guitar(jazz)",
"Electric Guitar(clean)",
"Electric Guitar(muted)",
"Overdriven Guitar",
"Distortion Guitar",
"Guitar Harmonics",
"Acoustic Bass",
"Electric Bass(finger)",
"Electric Bass(pick)",
"Fretless Bass",
"Slap Bass 1",
"Slap Bass 2",
"Synth Bass 1",
"Synth Bass 2",
"Violin",
"Viola",
"Cello",
"Contrabass",
"Tremolo Strings",
"Pizzicato Strings",
"Orchestral Harp",
"Timpani",
"String Ensemble 1",
"String Ensemble 2",
"SynthStrings 1",
"SynthStrings 2",
"Choir Aahs",
"Voice Oohs",
"Synth Voice",
"Orchestra Hit",
"Trumpet",
"Trombone",
"Tuba",
"Muted Trumpet",
"French Horn",
"Brass Section",
"Synth Brass 1",
"Synth Brass 2",
"Soprano Sax",
"Alto Sax",
"Tenor Sax",
"Baritone Sax",
"Oboe",
"English Horn",
"Bassoon",
"Clarinet",
"Piccolo",
"Flute",
"Recorder",
"Pan Flute",
"Blown Bottle",
"Shakuhachi",
"Whistle",
"Ocarina",
"Lead 1 (square)",
"Lead 2 (sawtooth)",
"Lead 3 (calliope)",
"Lead 4 (chiff)",
"Lead 5 (charang)",
"Lead 6 (voice)",
"Lead 7 (fifths)",
"Lead 8 (bass + lead)",
"Pad 1 (new age)",
"Pad 2 (warm)",
"Pad 3 (polysynth)",
"Pad 4 (choir)",
"Pad 5 (bowed)",
"Pad 6 (metallic)",
"Pad 7 (halo)",
"Pad 8 (sweep)",
"FX 1 (rain)",
"FX 2 (soundtrack)",
"FX 3 (crystal)",
"FX 4 (atmosphere)",
"FX 5 (brightness)",
"FX 6 (goblins)",
"FX 7 (echoes)",
"FX 8 (sci - fi)",
"Sitar",
"Banjo",
"Shamisen",
"Koto",
"Kalimba",
"Bag pipe",
"Fiddle",
"Shanai",
"Tinkle Bell",
"Agogo",
"Steel Drums",
"Woodblock",
"Taiko Drum",
"Melodic Tom",
"Synth Drum",
"Reverse Cymbal",
"Guitar Fret Noise",
"Breath Noise",
"Seashore",
"Bird Tweet",
"Telephone Ring",
"Helicopter",
"Applause",
"Gunshot"
};

static const char *MIDI_pgm_group[] PROGMEM = {
"Instrument group",
"Piano",                          // Index 1
"Chromatic percussion",
"Organ",
"Guitar",
"Bass",
"Strings",
"Ensemble",
"Brass",
"ProgNo",
"Instrument group",
"Reed",
"Pipe",
"Synth Lead",
"Synth Pad",
"Synth Effects",
"Ethnic",
"Percussive",
"Sound Effects"
};

static const char *MIDI_percussion[] PROGMEM = {
"Acoustic Bass Drum",             // Index 35
"Bass Drum 1",
"Side Stick",
"Acoustic Snare",
"Hand Clap",
"Electric Snare",
"Low Floor Tom",
"Closed Hi Hat",
"High Floor Tom",
"Pedal Hi-Hat",
"Low Tom",
"Open Hi-Hat",
"Low-Mid Tom",
"Hi Mid Tom",
"Crash Cymbal 1",
"High Tom",
"Ride Cymbal 1",
"Chinese Cymbal",
"Ride Bell",
"Tambourine",
"Splash Cymbal",
"Cowbell",
"Crash Cymbal 2",
"Vibraslap",
"Ride Cymbal 2",
"Hi Bongo",
"Low Bongo",
"Mute Hi Conga",
"Open Hi Conga",
"Low Conga",
"High Timbale",
"Low Timbale",
"High Agogo",
"Low Agogo",
"Cabasa",
"Maracas",
"Short Whistle",
"Long Whistle",
"Short Guiro",
"Claves",
"Hi Wood Block",
"Low Wood Block",
"Mute Cuica",
"Open Cuica",
"Mute Triangle",
"Open Triangle"
};
