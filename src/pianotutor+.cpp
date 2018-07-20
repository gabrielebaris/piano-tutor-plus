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


#include <iostream>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ArgParser.h"
#include "Config.h"
#include "debug.h"
#include "LedStrip.h"
#include "MidiClient.h"
#include "PianoTutorPlusConfig.h"


#define PROGRAM		"pianotutor+"
#define VERSION		"1.0.0"
#define DESCRIPTION	"ALSA component which receives MIDI events \
                    and switches on/off corresponding keyboard LEDs"

#define MIDI_CLIENT_NAME    "PianoTutor+"
#define MIDI_PORT_NAME      "PianoTutor+ MIDI input"

#define ERR_OPEN_FILE	-1
#define ERR_PARSE_FILE	-2
#define ERR_MIDI_DEVICE -3


/**
 * Utility function to print help messages about the usage of this program
 */
static inline void printUsage() {
	std::cout << std::endl;
	std::cout << DESCRIPTION << std::endl;
	std::cout << std::endl;
    std::cout << "Usage:" << std::endl;
	std::cout << "    " << PROGRAM << " (-f | --file) <name>" << std::endl;
	std::cout << "    " << PROGRAM << " (-v | --version)" << std::endl;
	std::cout << "    " << PROGRAM << " (-h | --help)" << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "    " << "-f <name>, --file <name>\tLoad configurations from file named <name>" << std::endl;
	std::cout << "    " << "-h, --help\t\t\tShow this screen" << std::endl;
	std::cout << "    " << "-v, --version\t\tShow program version" << std::endl;

	std::cout << std::endl;
}


bool run = true;

/**
 * Custom SIGINT handler
 * 
 * @param   signum      number of the signal (unused)
 */
static inline void sigintHandler(int signum) {
    dprintf("Invoking SIGINT handler");
    run = false;
}

/**
 * Program entry-point. It parses the command-line arguments, retrieves the name
 * of the configuration file and parse it. Then, depending on the MIDI note caught,
 * it computes the right LED to light up
 * 
 * @param	argc	number of arguments
 * @param	argv	vector of arguments
 */
int main(int argc, char* argv[]) {

    signal(SIGINT, sigintHandler);

	std::string configFile;

    try {

		ArgParser parser;
		parser.addOption("help", 'h', ArgParser::ArgumentType::NO_ARGUMENT, [](const char* arg) {
			printUsage();
			exit(EXIT_SUCCESS);
		})
		.addOption("version", 'v', ArgParser::ArgumentType::NO_ARGUMENT, [](const char* arg) {
			std::cout << "Version: " << VERSION << std::endl;
			exit(EXIT_SUCCESS);
		})
		.addOption("file", 'f', ArgParser::ArgumentType::REQUIRED, [&configFile](const char* arg) {
			dprintf("Loading configuration from %s", arg);
			configFile = std::string(arg);
		})
		.parse(argc, argv);

		if(configFile == "") {
			printUsage();
			exit(ERR_OPEN_FILE);
		}

        PianoTutorPlusConfig config(configFile);
        dprintf("Parse configuration file: correct");

        LedStrip strip(config.getFreq(),
    				config.getDmaChannel(), 
                    config.getGpioPin(),
                    config.getStripType(),
                    config.getLedCount());

        MidiClient midi(MIDI_CLIENT_NAME, MIDI_PORT_NAME);


        MidiEvent midiEvent;
        int pin = 0;
        std::string note;

        while(run) {

            midiEvent = midi.getEvent();
            if(midiEvent.type != MidiEvent::Type::UNKNOWN && midiEvent.type != MidiEvent::Type::NO_EVENT) {

                note = MidiClient::midi2note(midiEvent.note);
                dprintf("[%c] %s %s",
						midiEvent.hand == MidiEvent::Hand::RIGHT ? 'R' : 'L',
						note.c_str(),
						midiEvent.type == MidiEvent::Type::NOTE_ON ? "ON" : "OFF");

                switch(config.getLedOrder()) {
                    case LedOrder::Order::DIR:
                        pin = round((midiEvent.note - config.getKeyboardMinNote()) * config.getLedPerKey());
                        break;
                    case LedOrder::Order::INV:
                        pin = round((config.getKeyboardMaxNote() - midiEvent.note) * config.getLedPerKey());
                        break;
                }

                if(midiEvent.type == MidiEvent::Type::NOTE_ON) {
                    LedColor::Color color = midiEvent.hand == MidiEvent::Hand::RIGHT ? config.getColorRightHand() : config.getColorLeftHand();
                    strip.switchOn(pin, color);
                } else {
                    strip.switchOff(pin);
                }

                strip.render();
            }

            usleep(10000);
        }

    } catch(OpenFileException& e) {
        std::cerr << "Error opening the configuration file " << std::endl << std::flush;
        exit(ERR_OPEN_FILE);
    } catch(ParsingException& e) {
        std::cerr << "Error parsing the configuration file" << std::endl  << std::flush;
        exit(ERR_PARSE_FILE);
    } catch(MidiDeviceException& e) {
		std::cerr << "Error accessing the MIDI device" << std::endl  << std::flush;
        exit(ERR_MIDI_DEVICE);
    }

    return 0;
}