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


#ifndef __ARGPARSER_H__
#define __ARGPARSER_H__

#include <functional>
#include <getopt.h>
#include <map>
#include <string>

/**
 * Simple command-line argument parser wrapping the getopt_long() function.
 * It provides a builder-alike interface for maximum flexibility.
 */
class ArgParser {

public:

	enum ArgumentType {
		NO_ARGUMENT = no_argument,
		OPTIONAL = optional_argument,
		REQUIRED = required_argument
	};

private:

	struct Option {
		const char* longOpt;
		char shortOpt;
		ArgumentType type;
		std::function<void(char*)> handler;
	};

	std::map<char, Option> options;

public:

	/**
	 * Add an option to the parser
	 * 
	 * @param	longOpt		extended option, used with double dash (ex. --help)
	 * @param	shortOpt	short option, used with single dash (ex. -h)
	 * @param	type		type of the argument (required, optional or no argument)
	 * @param	handler		callback to execute when parsing that option
	 * 
	 * @return	reference to the object
	 * 
	 */
	ArgParser& addOption(const char* longOpt, char shortOpt, ArgumentType type, std::function<void(char*)> handler);

	/**
	 * Parse the provided vector of arguments (the one of the main function),
	 * invoking the callbacks
	 * 
	 * @param	argc	number of arguments
	 * @param	argv	vector of arguments
	 */
	void parse(int argc, char* argv[]);

};

#endif
