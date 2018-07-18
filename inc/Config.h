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


#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <map>
#include <string>

/**
 * Simple class to parse a configuration file with key=value pairs. Comments are
 * identified by #. Values can then be accessed using [] operator, indexed by the key
 */
class Config {

	std::map<std::string, std::string> options;

public:

	/**
	 * Parse the content of the provided file, storing all the (key, value) pairs.
	 * In case of error opening the file, a OpenFileException is thrown
	 * 
	 * @param	filename	name of the file to parse
	 * 
	 * @return	a Config object containing all the parsed (key, value) pairs
	 */
	static Config parse(const std::string& filename);

	/**
	 * Provide the [] interface to access the Config object
	 * 
	 * @param	key		key of the pair
	 * 
	 * @return	the value associated to that key
	 */
	std::string operator[](const std::string& key);

	/**
	 * Parse the provided string, returning the integer value
	 * 
	 * @parm	str		string representing an integer
	 * 
	 * @return	integer value
	 */
    static int parseInt(const std::string& str);

	/**
	 * Parse the provided string, returning the float value
	 * 
	 * @parm	str		string representing a float
	 * 
	 * @return	float value
	 */
    static float parseFloat(const std::string& str);

	/**
	 * Parse the provided string, returning the double value
	 * 
	 * @parm	str		string representing a double
	 * 
	 * @return	double value
	 */
    static double parseDouble(const std::string& str);

	/**
	 * Parse the provided string, returning the boolean value
	 * 
	 * @parm	str		string representing a boolean
	 * 
	 * @return	boolean value
	 */
    static bool parseBoolean(const std::string& str);

};

/**
 * Exception thrown dealing with file opening
 */
class OpenFileException : public std::exception {
public:
	virtual const char* what() const throw() {
		return "OpenFileException";
	}
};

/**
 * Exception thrown dealing with parsing
 */
class ParsingException : public std::exception {
public:
	virtual const char* what() const throw() {
		return "ParsingException";
	}
};

#endif
