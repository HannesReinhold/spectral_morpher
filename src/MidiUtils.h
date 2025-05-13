#pragma once
namespace MidiUtils {

	int keyToNote(char key) {
		switch (key) {
		case 'y':
			return 60;// C4
		case 'x':
			return 62;// D4
		case 'c':
			return 64;// E4
		case 'v':
			return 65;// F4
		case 'b':
			return 67;// G4
		case 'n':
			return 69;// A4
		case 'm':
			return 71;// B4
		case ',':
			return 72;// C5
		case '.':
			return 74;// D5
		case '-':
			return 76;// E5
		case 's':
			return 61;// C#4 -----
		case 'd':
			return 63;// D#4
		case 'g':
			return 66;// F#4
		case 'h':
			return 68;// G#4
		case 'j':
			return 70;// A#4
		case 'l':
			return 60;// C#5
		case 'ä':
			return 60;// D#5
		case 'q':
			return 72;// C5 -----
		case 'w':
			return 74;// D5
		case 'e':
			return 76;// E5
		case 'r':
			return 77;// F5
		case 't':
			return 79;// G5
		case 'z':
			return 81;// A5
		case 'u':
			return 83;// B5
		case 'i':
			return 84;// C6
		case 'o':
			return 86;// D6
		case 'p':
			return 88;// E6
		case 'ü':
			return 89;// C6
		case '2':
			return 73;// C#5 -----
		case '3':
			return 75;// D#5
		case '5':
			return 78;// F#5
		case '6':
			return 80;// G#5
		case '7':
			return 82;// A#5
		case '9':
			return 85;// C#6
		case '0':
			return 87;// D#6
		default:
			return -1000;
		}
	}

	/// <summary>
	/// Tries to add note to pool, if not already contained
	/// </summary>
	/// <param name="note"></param>
	/// <param name="noteArray"></param>
	/// <returns>index of note in pool, -1 if full or already in array</returns>
	int addNoteToPool(int note, int* noteArray) {
		int firstEmpty = -1;
		for (int i = 0; i < 8; i++) {
			if (noteArray[i] == -1 && firstEmpty == -1) firstEmpty = i;
			if (noteArray[i] == note) return -1;
		}
		if (firstEmpty != -1) noteArray[firstEmpty] = note;
		return firstEmpty;
	}

	/// <summary>
	/// Tries to remove note to pool, if contained
	/// </summary>
	/// <param name="note"></param>
	/// <param name="noteArray"></param>
	/// <returns>index of removed note in pool, -1 if not found</returns>
	int removeNoteFromPool(int note, int* noteArray) {
		for (int i = 0; i < 8; i++) {
			if (noteArray[i] == note) {
				noteArray[i] = -1;
				return i;
			}
		}
		return -1;
	}

	float midiToFreq(int note) {
		return std::powf(2, ((float)note - 69.0f) / 12.0f) * 440.0f;
	}

	int freqToMidi(float freq) {
		return (int)(12.0f * std::log2(freq / 440.0f)) + 69;
	}

}