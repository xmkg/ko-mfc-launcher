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

enum KOOpcodes : uint8
{
    LS_VERSION_REQ				= 0x01,
    LS_DOWNLOADINFO_REQ			= 0x02,
    WIZ_COMPRESS_PACKET         = 0x42,
};

