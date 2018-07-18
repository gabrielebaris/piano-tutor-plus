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


#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Config.h"
#include "LedStrip.h"
#include "MidiClient.h"
#include "PianoTutorPlusConfig.h"

/**
 * Throw a ParsingException, showing the provided error message
 * 
 * @param	msg		message to show
 */
void PianoTutorPlusConfig::throwParsingError(const std::string& msg) {
	std::cerr << "[Parsing Error] - " << msg << std::endl;
	throw ParsingException();

}

/**
 * Build the object by parsing the provided file and initializing all the
 * internal variables. Throw a ParsingException if somethig goes wrong
 * 
 * @param	filename	name of the configuration file
 */
PianoTutorPlusConfig::PianoTutorPlusConfig(const std::string &filename) {
	
    Config conf = Config::parse(filename);

	try {

		this->freq = (unsigned int) Config::parseInt(conf[KEY_FREQ]);
		if (this->freq <= 0)
			this->throwParsingError("The frequency must be a non-null positive integer");

		this->gpioPin = (unsigned short) Config::parseInt(conf[KEY_GPIO_PIN]);
		if (this->gpioPin <= 0)
			this->throwParsingError("The GPIO pin must be a non-null positive integer");

		this->dmaChannel = (unsigned short) Config::parseInt(conf[KEY_DMA_CHANNEL]);
		if (this->dmaChannel <= 0)
			this->throwParsingError("The DMA channel must be a non-null positive integer");

        this->ledCount = (unsigned short) Config::parseInt(conf[KEY_LED_COUNT]);
		if (this->ledCount <= 0)
			this->throwParsingError("The LED count must be a non-null positive integer");

		this->ledPerKey = Config::parseFloat(conf[KEY_LED_PER_KEY]);
		if(this->ledPerKey <= 0)
			this->throwParsingError("The number of LED(s) per key must be a non-negative real number");

		this->keyboardMinNote = (unsigned char) MidiClient::note2midi(conf[KEY_KEYBOARD_MIN_NOTE]);
		if(this->keyboardMinNote <= 0)
			this->throwParsingError("The min keyboard note has to be a proper note, such as C2");
		
		this->keyboardMaxNote = (unsigned char) MidiClient::note2midi(conf[KEY_KEYBOARD_MAX_NOTE]);
		if(this->keyboardMaxNote <= 0)
			this->throwParsingError("The max keyboard note has to be a proper note, such as C7");

		try {
			this->ledOrder = LedOrder::parse(conf[KEY_LED_ORDER]);
			this->stripType = StripType::parse(conf[KEY_LED_TYPE]);
			this->colorRightHand = LedColor::parse(conf[KEY_COLOR_RIGHT]);
			this->colorLeftHand = LedColor::parse(conf[KEY_COLOR_LEFT]);
		}catch(LedColor::ColorNotFoundException& e) {
			std::vector<LedColor::Color> colors = LedColor::getAllColors();
			std::string s = "";
			for(auto c : colors)
				s += std::string(LedColor::toString(c)) + " ";
			this->throwParsingError("Available colors: " + s);
		}catch(StripType::StripTypeNotFoundException& e) {
			std::vector<StripType::Type> types = StripType::getAllStripTypes();
			std::string s = "";
			for(auto t : types)
				s += std::string(StripType::toString(t)) + " ";
			this->throwParsingError("Available types: " + s);
		}catch(LedOrder::LedOrderNotFoundException& e) {
			std::vector<LedOrder::Order> orders = LedOrder::getAllLedOrders();
			std::string s = "";
			for(auto o : orders)
				s += std::string(LedOrder::toString(o)) + " ";
			this->throwParsingError("Available orders: " + s);
		}

	} catch (std::exception& e) {
		throw ParsingException();
	}

}
