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


#ifndef __MIDICLIENT_H__
#define __MIDICLIENT_H__

#include <alsa/asoundlib.h>
#include <exception>
#include <string>

/**
 * Custom struct for storing the MIDI event informations meaningfull for
 * the project
 */
struct MidiEvent {

    enum Type {
        NOTE_ON,
        NOTE_OFF,
        UNKNOWN,
        NO_EVENT
    };

    enum Hand {
        RIGHT,
        LEFT
    };

    unsigned char note;
    Type type;
    Hand hand;

};

/**
 * Exception thrown dealing with MIDI errors
 */
class MidiDeviceException : public std::exception {
    virtual const char* what() const throw() {
        return "MidiDeviceException";
    }
};

/**
 * Simple class to mask the interaction with the ALSA MIDI sequencer
 */
class MidiClient {

    snd_seq_t *seq_handle;

public:

    /**
     * Open the MIDI sequencer in non-blocking mode, creates a client and a port, subscribing to it.
     * If something goes wrong, trows a MidiDeviceException()
     * 
     * @param	clientName		name of the MIDI client
     * @param	portName		name of the MIDI port
     */
    MidiClient(const char* clientName, const char* portName);

    /**
     * Close the MIDI sequencer
     */
    ~MidiClient();

    /**
     * Return a MidiEvent of type
     *  - NO_EVENT if no event is present (the semantics is non-blocking)
     *  - NOTE_ON if a key has been pressed
     *  - NOTE_OFF if a key has been released
     *  - UNKNOWN otherwise (all of them are meaningless for this applicaton)
     * When needed, note and hand are correctly set
     */
    MidiEvent getEvent();

    /**
     * Return a string representing the provided midi note
     * 
     * @param	midi	value of the midi note
     * 
     * @return	string encoding corresponding note
     */
    static std::string midi2note(int midi);

    /**
     * Return the MIDI value representing the provided note
     * 
     * @param	note	string encoding a note
     * 
     * @return	integer representing corresponding MIDI value
     */
    static int note2midi(std::string note);
};

#endif