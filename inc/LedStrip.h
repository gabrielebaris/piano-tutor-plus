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


#ifndef __LEDSTRIP_H__
#define __LEDSTRIP_H__

#include <exception>
#include <stdint.h>
#include <string>
#include <vector>

#include "ws2811.h" // from https://github.com/jgarff/rpi_ws281x

/**
 * Namespace to deal with color definitions. It allows to parse colors to and 
 * from string, manage parsing errors and obtain the list of available colors
 */
namespace LedColor {
    enum Color {
		RED 		= 0x00200000,
		ORANGE 		= 0x00201000,
		YELLOW 		= 0x00202000,
		GREEN 		= 0x00002000,
		LIGHTBLUE	= 0x00002020,
		BLUE 		= 0x00000020,
		PURPLE 		= 0x00100010,
		PINK 		= 0x00200010
	};

	/**
	 * Exception thrown dealing Color parsing
	 */
	class ColorNotFoundException : public std::exception {
		virtual const char* what() const throw() {
			return "ColorNotFoundException";
		}
	};

	/**
	 * Parse a string, obtaining the corresponding Color
	 * 
	 * @param	color	string representing the color
	 * 
	 * @return	corresponding Color value
	 */
	Color parse(std::string color);

	/**
	 * Return the name of the provided color
	 * 
	 * @param	color	Color value
	 * 
	 * @return	name of the color
	 */
	const char* toString(Color color);

	/**
	 * Return the list of all the available colors
	 * 
	 * @return	vector containing all the available colors
	 */
	std::vector<Color> getAllColors();
}

/**
 * Namespace to deal with strip type definitions. It allows to parse types to and 
 * from string, manage parsing errors and obtain the list of available types
 */
namespace StripType {
    enum Type {
		RGB	= 0x00100800,
		RBG	= 0x00100008,
		GRB	= 0x00081000,
		GBR	= 0x00080010,
		BRG	= 0x00001008,
		BGR	= 0x00000810
	};

	/**
	 * Exception thrown dealing StripType parsing
	 */
	class StripTypeNotFoundException : public std::exception {
		virtual const char* what() const throw() {
			return "StripTypeNotFoundException";
		}
	};

	/**
	 * Parse a string, obtaining the corresponding strip type
	 * 
	 * @param	type	string representing the strip type
	 * 
	 * @return	corresponding Type value
	 */
	Type parse(std::string type);

	/**
	 * Return the name of the provided strip type
	 * 
	 * @param	type	strip type
	 * 
	 * @return	name of the type
	 */
	const char* toString(Type type);

	/**
	 * Return the list of all the available strip types
	 * 
	 * @return	vector containing all the available strip types
	 */
	std::vector<Type> getAllStripTypes();
}

/**
 * Namespace to deal with led order definitions. It allows to parse orders to and 
 * from string, manage parsing errors and obtain the list of available orders
 */
namespace LedOrder {
    enum Order {
		DIR,
		INV
	};

	/**
	 * Exception thrown dealing led order parsing
	 */
	class LedOrderNotFoundException : public std::exception {
		virtual const char* what() const throw() {
			return "LedOrderNotFound";
		}
	};

	/**
	 * Parse a string, obtaining the corresponding LED order
	 * 
	 * @param	order	string representing the LED order
	 * 
	 * @return	corresponding Order value
	 */
	Order parse(std::string order);

	/**
	 * Return the name of the provided LED order
	 * 
	 * @param	order	LED order
	 * 
	 * @return	name of the order
	 */
	std::vector<Order> getAllLedOrders();

	/**
	 * Return the list of all the available LED orders
	 * 
	 * @return	vector containing all the available LED orders
	 */
	const char* toString(Order order);
}

/**
 * Simple class which wraps the functionalities offered by the rpi_ws2811 library,
 * allowing to interact with the LED strip
 */ 
class LedStrip {

    ws2811_t ledstring;

public:

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
    LedStrip(unsigned int freq, unsigned char dmaChannel, unsigned char gpioPin, StripType::Type stripType, unsigned char count);
   
	/**
	 * Destroy the LedStrip object and perfrom clean-up
	 */
    ~LedStrip();

	/**
	 * Set the brightness of a LED to the desired value
	 * 
	 * @param	intensity	desired intensity value
	 * 
	 * @return	a reference to the object
	 */
    LedStrip& setBrightness(unsigned char intensity);

	/**
	 * Set the color of the desired LED
	 * 
	 * @param	pos		position of the LED in the strip
	 * 
	 * @return	a reference to the object
	 */
    LedStrip& switchOn(unsigned char pos, LedColor::Color color);

	/**
	 * Switch off the desired LED
	 * 
	 * @param	pos		position of the LED in the strip
	 * 
	 * @return	a reference to the object
	 */
    LedStrip& switchOff(unsigned char pos);

	/**
	 * Switch off all the LEDs in the strip
	 * 
	 * @return	a reference to the object
	 */
    LedStrip& clearAll();

	/**
	 * Send commands to switch on/off the LEDs in the strip
	 */
    void render();
};

/**
 * Exception thrown dealing with the LED strip
 */
class LedStripException : public std::exception {
	virtual const char* what() const throw() {
		return "LedStripException";
	}
};


#endif
