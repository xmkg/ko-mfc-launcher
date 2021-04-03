/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


#pragma once
#include <spdlog\spdlog.h>
class CLogWriter
{
public:
	static std::shared_ptr<spdlog::logger> Log;
	static void prepare();
};

