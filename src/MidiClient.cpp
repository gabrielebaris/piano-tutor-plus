/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2018, Gabriele Baris
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <algorithm>
#include <alsa/asoundlib.h>
#include <regex>
#include <stdlib.h>
#include <string>

#include "debug.h"
#include "MidiClient.h"

/**
 * Open the MIDI sequencer in non-blocking mode, creates a client and a port, subscribing to it.
 * If something goes wrong, trows a MidiDeviceException()
 * 
 * @param	clientName		name of the MIDI client
 * @param	portName		name of the MIDI port
 */
MidiClient::MidiClient(const char* clientName, const char* portName)
{
	int port;

	if (snd_seq_open(&(this->seq_handle), "default", SND_SEQ_OPEN_INPUT, SND_SEQ_NONBLOCK) < 0) {
		throw MidiDeviceException();
    }
	dprintf("Open sequential handler: correct");


	snd_seq_set_client_name(this->seq_handle, clientName);

    if ((port = snd_seq_create_simple_port(seq_handle, portName,
			SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
			SND_SEQ_PORT_TYPE_APPLICATION | SND_SEQ_PORT_TYPE_MIDI_GENERIC)) < 0)  {
		throw MidiDeviceException();
    }
	dprintf("Create simple port: correct");

    if (snd_seq_connect_from(this->seq_handle, port, SND_SEQ_CLIENT_SYSTEM,
				SND_SEQ_PORT_SYSTEM_ANNOUNCE) < 0) {
		throw MidiDeviceException();
    }
	dprintf("Subscribe to port events: correct");
}

/**
 * Close the MIDI sequencer
 */
MidiClient::~MidiClient()
{
	dprintf("Closing sequencer");
	snd_seq_close(this->seq_handle);
}

/**
 * Return a MidiEvent of type
 *  - NO_EVENT if no event is present (the semantics is non-blocking)
 *  - NOTE_ON if a key has been pressed
 *  - NOTE_OFF if a key has been released
 *  - UNKNOWN otherwise (all of them are meaningless for this applicaton)
 * When needed, note and hand are correctly set
 */
MidiEvent MidiClient::getEvent()
{
	MidiEvent ret;
	snd_seq_event_t *ev = NULL;
	
	if(snd_seq_event_input(this->seq_handle, &ev) != -EAGAIN) {
		if((ev->type == SND_SEQ_EVENT_NOTEON) || (ev->type == SND_SEQ_EVENT_NOTEOFF)) {
			
			ret.note = ev->data.note.note;

			if(ev->type == SND_SEQ_EVENT_NOTEOFF || (ev->type == SND_SEQ_EVENT_NOTEON && ev->data.note.velocity == 0))
				ret.type = MidiEvent::Type::NOTE_OFF;
			else
				ret.type = MidiEvent::Type::NOTE_ON;


			ret.hand = ev->data.control.channel == 0 ? MidiEvent::Hand::RIGHT : MidiEvent::Hand::LEFT;
		} else {
			ret.type = MidiEvent::Type::UNKNOWN;
		}
		snd_seq_free_event(ev);
	} else {
		ret.type = MidiEvent::Type::NO_EVENT;
	}

	return ret;

}

/**
 * Return a string representing the provided midi note
 * 
 * @param	midi	value of the midi note
 * 
 * @return	string encoding corresponding note
 */
std::string MidiClient::midi2note(int midi) {
    const char* notes[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    int note = midi % 12;
    int octave = midi / 12 - 1;
	std::string ret = notes[note];
	return ret + std::to_string(octave);
}

/**
 * Return the MIDI value representing the provided note
 * 
 * @param	note	string encoding a note
 * 
 * @return	integer representing corresponding MIDI value
 */
int MidiClient::note2midi(std::string note) {
	std::transform(note.begin(), note.end(), note.begin(), ::toupper);

	int ret = -1;

	std::regex expr("^([A-G]|[A|C|D|F|G][\\#])([0-9])$");
	std::cmatch cm;
	if(std::regex_match(note.c_str(), cm, expr)) {
		if(cm[1] == "C")
			ret = 0;
		else if(cm[1] == "C#")
			ret = 1;
		else if(cm[1] == "D")
			ret = 2;
		else if(cm[1] == "D#")
			ret = 3;
		else if(cm[1] == "E")
			ret = 4;
		else if(cm[1] == "F")
			ret = 5;
		else if(cm[1] == "F#")
			ret = 6;
		else if(cm[1] == "G")
			ret = 7;
		else if(cm[1] == "G#")
			ret = 8;
		else if(cm[1] == "A")
			ret = 9;
		else if(cm[1] == "A#")
			ret = 10;
		else if(cm[1] == "B")
			ret = 11;

		ret += (std::stoi(cm[2]) + 1) * 12;

	}

	return ret;

}