/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


#include "stdafx.h"
#include "LogWriter.h"

std::shared_ptr<spdlog::logger> CLogWriter::Log;

void CLogWriter::prepare() {
	Log = spdlog::daily_logger_mt("update_log", "logs/update", 0, 0, true);
	Log->set_pattern("** [%T %z] [%l] >> %v");
}
