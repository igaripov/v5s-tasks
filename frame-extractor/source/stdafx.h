// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _WIN32
#include "targetver.h"
#endif

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#include <cstdio>
#include <sstream>
#include <cinttypes>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "AVBoundary/IAVErrorHandler.h"
#include "AVBoundary/DecoderHelper.h"
#include "AVBoundary/EncoderHelper.h"
#include "AVBoundary/PacketHelper.h"
#include "AVBoundary/ParserHelper.h"
#include "AVBoundary/FrameHelper.h"
