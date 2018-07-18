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
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include "Config.h"
#include "LedStrip.h"


/**
 * Simple helper function to trim whitespaces at the beginning of a string. The result
 * is stored inside the same string
 * 
 * @param	s	string to be trimmed
 */
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

/**
 * Simple helper function to trim whitespaces at the end of a string. The result
 * is stored inside the same string
 * 
 * @param	s	string to be trimmed
 */
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

/**
 * Simple helper function to trim whitespaces both at the beginning and at the end
 * of a string. The result is stored inside the same string
 * 
 * @param	s	string to be trimmed
 */
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

/**
 * Parse the content of the provided file, storing all the (key, value) pairs.
 * In case of error opening the file, a OpenFileException is thrown
 * 
 * @param	filename	name of the file to parse
 * 
 * @return	a Config object containing all the parsed (key, value) pairs
 */
Config Config::parse(const std::string& filename) {
	std::ifstream configFile;

	Config config;

	configFile.open(filename);
	if(!configFile.is_open())
		throw OpenFileException();

	std::string line;
	std::string value;
	std::string key;

	std::size_t comment_pos;
	std::size_t equal_pos;

	while(std::getline(configFile, line)) {

		ltrim(line);

		if (line[0] == '#')
			continue;

		comment_pos = line.find('#');
		equal_pos = line.find('=');

		if(equal_pos != std::string::npos) {
			key = line.substr(0, equal_pos);
			if(comment_pos != std::string::npos)
				value = line.substr(equal_pos + 1, (comment_pos - equal_pos - 1));
			else
				value = line.substr(equal_pos + 1);

			trim(key);
			trim(value);
            
			config.options[key] = value;
		}
	}

	if(configFile.is_open())
		configFile.close();

	return config;
}

/**
 * Provide the [] interface to access the Config object
 * 
 * @param	key		key of the pair
 * 
 * @return	the value associated to that key
 */
std::string Config::operator[](const std::string &key) {
	return this->options[key];
}

/**
 * Parse the provided string, returning the integer value
 * 
 * @parm	str		string representing an integer
 * 
 * @return	integer value
 */
int Config::parseInt(const std::string& str) {
    return std::stoi(str);
}

/**
 * Parse the provided string, returning the float value
 * 
 * @parm	str		string representing a float
 * 
 * @return	float value
 */
float Config::parseFloat(const std::string& str) {
    return std::stof(str);
}

/**
 * Parse the provided string, returning the double value
 * 
 * @parm	str		string representing a double
 * 
 * @return	double value
 */
double Config::parseDouble(const std::string& str) {
    return std::stod(str);
}

/**
 * Parse the provided string, returning the boolean value
 * 
 * @parm	str		string representing a boolean
 * 
 * @return	boolean value
 */
bool Config::parseBoolean(const std::string& str) {
    std::string val;
    std::transform(str.begin(), str.end(), val.begin(), ::tolower);
    return str == "true";
}

