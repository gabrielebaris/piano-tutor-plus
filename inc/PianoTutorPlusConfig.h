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


#ifndef __PIANOTUTORPLUSCONFIG_H__
#define __PIANOTUTORPLUSCONFIG_H__

#define KEY_FREQ		"FREQUENCY"
#define KEY_GPIO_PIN	"GPIO_PIN"
#define KEY_DMA_CHANNEL	"DMA_CHANNEL"
#define KEY_LED_COUNT	"LED_COUNT"
#define KEY_LED_ORDER	"LED_ORDER"
#define KEY_LED_TYPE	"LED_TYPE"
#define KEY_LED_PER_KEY	"LED_PER_KEY"
#define KEY_COLOR_RIGHT	"COLOR_RIGHT_HAND"
#define KEY_COLOR_LEFT	"COLOR_LEFT_HAND"
#define KEY_KEYBOARD_MIN_NOTE	"KEYBOARD_MIN_NOTE"
#define KEY_KEYBOARD_MAX_NOTE	"KEYBOARD_MAX_NOTE"

#include <string>

#include "LedStrip.h"

/**
 * Simple class used to parse the configuration file and retrieve
 * the desired settings
 */ 
class PianoTutorPlusConfig {
	
	unsigned int freq;
	unsigned short gpioPin;
	unsigned short dmaChannel;
	unsigned short ledCount;
	LedOrder::Order ledOrder;
	StripType::Type stripType;
	float ledPerKey;
	LedColor::Color colorRightHand;
	LedColor::Color colorLeftHand;
	unsigned char keyboardMinNote;
	unsigned char keyboardMaxNote;

	/**
	 * Throw a ParsingException, showing the provided error message
	 * 
	 * @param	msg		message to show
	 */
	void throwParsingError(const std::string& msg);

public:

	/**
	 * Build the object by parsing the provided file and initializing all the
	 * internal variables. Throw a ParsingException if somethig goes wrong
	 * 
	 * @param	filename	name of the configuration file
	 */
    PianoTutorPlusConfig(const std::string& filename);

	// list of getters
	unsigned int getFreq() { return freq; }
	unsigned short getGpioPin() { return gpioPin; }
	unsigned short getDmaChannel() { return dmaChannel; }
	unsigned short getLedCount() { return ledCount; }
	LedOrder::Order getLedOrder() { return ledOrder; }
	StripType::Type getStripType() { return stripType; }
	float getLedPerKey() { return ledPerKey; }
	LedColor::Color getColorRightHand() { return colorRightHand; }
	LedColor::Color getColorLeftHand() { return colorLeftHand; }
	unsigned char getKeyboardMinNote() { return keyboardMinNote; }
	unsigned char getKeyboardMaxNote() { return keyboardMaxNote; }

};

#endif
