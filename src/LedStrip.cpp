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
#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "LedStrip.h"

/**
 * Parse a string, obtaining the corresponding Color
 * 
 * @param	color	string representing the color
 * 
 * @return	corresponding Color value
 */
LedColor::Color LedColor::parse(std::string color) {

	std::transform(color.begin(), color.end(), color.begin(), ::tolower);

	if(color == "red")
		return LedColor::Color::RED;
	else if(color == "orange")
		return LedColor::Color::ORANGE;
	else if(color == "yellow")
		return LedColor::Color::YELLOW;
	else if(color == "green")
		return LedColor::Color::GREEN;
	else if(color == "lightblue")
		return LedColor::Color::LIGHTBLUE;
	else if(color == "blue")
		return LedColor::Color::BLUE;
	else if(color == "purple")
		return LedColor::Color::PURPLE;
	else if(color == "pink")
		return LedColor::Color::PINK;
	else
		throw LedColor::ColorNotFoundException();
}

/**
 * Return the name of the provided color
 * 
 * @param	color	Color value
 * 
 * @return	name of the color
 */
const char* LedColor::toString(LedColor::Color color) {
	switch(color){
		case LedColor::Color::RED:
			return "RED";
		case LedColor::Color::ORANGE:
			return "ORANGE";
		case LedColor::Color::YELLOW:
			return "YELLOW";
		case LedColor::Color::GREEN:
			return "GREEN";
		case LedColor::Color::LIGHTBLUE:
			return "LIGHTBLUE";
		case LedColor::Color::BLUE:
			return "BLUE";
		case LedColor::Color::PURPLE:
			return "PURPLE";
		case LedColor::Color::PINK:
			return "PINK";
		default:
			return nullptr;

	}
}

/**
 * Return the list of all the available colors
 * 
 * @return	vector containing all the available colors
 */
std::vector<LedColor::Color> LedColor::getAllColors() {
	return std::vector<LedColor::Color>({RED, ORANGE, YELLOW, GREEN, LIGHTBLUE, BLUE, PURPLE, PINK});
}

/**
 * Parse a string, obtaining the corresponding strip type
 * 
 * @param	type	string representing the strip type
 * 
 * @return	corresponding Type value
 */
StripType::Type StripType::parse(std::string type) {
	std::transform(type.begin(), type.end(), type.begin(), ::toupper);

	if(type == "RGB")
		return RGB;
	else if(type == "RBG")
		return RBG;
	else if(type == "GRB")
		return GRB;
	else if(type == "GBR")
		return GBR;
	else if(type == "BRG")
		return BRG;
	else if(type == "BGR")
		return BGR;
	else
		throw StripType::StripTypeNotFoundException();
}

/**
 * Return the name of the provided strip type
 * 
 * @param	type	strip type
 * 
 * @return	name of the type
 */
const char* StripType::toString(StripType::Type type) {
	switch(type) {
		case RGB:
			return "RGB";
		case RBG:
			return "RBG";
		case GRB:
			return "GRB";
		case GBR:
			return "GBR";
		case BRG:
			return "BRG";
		case BGR:
			return "BGR";
		default:
			return nullptr;
	}
}

/**
 * Return the list of all the available strip types
 * 
 * @return	vector containing all the available strip types
 */
std::vector<StripType::Type> StripType::getAllStripTypes() {
	return std::vector<StripType::Type>({RGB, RBG, GRB, GBR, BRG, BGR});
}

/**
 * Parse a string, obtaining the corresponding LED order
 * 
 * @param	order	string representing the LED order
 * 
 * @return	corresponding Order value
 */
LedOrder::Order LedOrder::parse(std::string order) {

	std::transform(order.begin(), order.end(), order.begin(), ::toupper);

	if(order == "DIR")
		return LedOrder::Order::DIR;
	else if(order == "INV")
		return LedOrder::Order::INV;
	else
		throw LedOrder::LedOrderNotFoundException();
}

/**
 * Return the name of the provided LED order
 * 
 * @param	order	LED order
 * 
 * @return	name of the order
 */
const char* LedOrder::toString(LedOrder::Order order) {
	switch(order){
		case LedOrder::Order::DIR:
			return "DIR";
		case LedOrder::Order::INV:
			return "INV";
		default:
			return nullptr;
	}
}

/**
 * Return the list of all the available LED orders
 * 
 * @return	vector containing all the available LED orders
 */
std::vector<LedOrder::Order> LedOrder::getAllLedOrders() {
	return std::vector<LedOrder::Order>({DIR, INV});
}

/**
 * Initialize the LED strip starting from the provided parameters. In case of error a
 * LedStripException is thrown
 * 
 * @param	freq		driving frequency
 * @param	dmaChannel	number of the DMA channel
 * @param	gpioPin		number of the GPIO pin
 * @param	stripType	type of the LED strip
 * @param	count		number of LEDs in the strip
 */
LedStrip::LedStrip(unsigned int freq, unsigned char dmaChannel, unsigned char gpioPin, StripType::Type stripType, unsigned char count) {

    memset(&ledstring, 0, sizeof(ledstring));

    ledstring.freq = freq;
    ledstring.dmanum = dmaChannel;

    ledstring.channel[0].gpionum = gpioPin;
    ledstring.channel[0].count = count;
    ledstring.channel[0].invert = 0;
    ledstring.channel[0].brightness = 255;
    ledstring.channel[0].strip_type = stripType;

    if (ws2811_init(&ledstring) != WS2811_SUCCESS)
		throw LedStripException();

}

/**
 * Destroy the LedStrip object and perfrom clean-up
 */
LedStrip::~LedStrip() {
    dprintf("LED strip clean-up");
    clearAll();
    render();
    ws2811_fini(&ledstring);
}

/**
 * Set the brightness of a LED to the desired value
 * 
 * @param	intensity	desired intensity value
 * 
 * @return	a reference to the object
 */
LedStrip& LedStrip::setBrightness(unsigned char intensity){
	dprintf("Set brightness to %d", intensity);
    ledstring.channel[0].brightness = intensity;
    return *this;
}

/**
 * Set the color of the desired LED
 * 
 * @param	pos		position of the LED in the strip
 * @param	color	desired color
 * 
 * @return	a reference to the object
 */
LedStrip& LedStrip::switchOn(unsigned char pos, LedColor::Color color) {
    dprintf("Set color %s to LED %d", LedColor::toString(color), pos);
    ledstring.channel[0].leds[pos] = color;
    return *this;
}

/**
 * Switch off the desired LED
 * 
 * @param	pos		position of the LED in the strip
 * 
 * @return	a reference to the object
 */
LedStrip& LedStrip::switchOff(uint8_t pos) {
    dprintf("Switch off LED %d", pos);
    ledstring.channel[0].leds[pos] = 0;
    return *this;
}

/**
 * Switch off all the LEDs in the strip
 * 
 * @return	a reference to the object
 */
LedStrip& LedStrip::clearAll()
{
    memset(ledstring.channel[0].leds, 0, sizeof(ws2811_led_t) * ledstring.channel[0].count);
    return *this;
}

/**
 * Send commands to switch on/off the LEDs in the strip
 */
void LedStrip::render()
{
    ws2811_render(&ledstring);
}
