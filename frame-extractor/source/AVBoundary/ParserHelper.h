#pragma once

class FrameHelper;

class ParserHelper
{
public:
    ParserHelper(DecoderHelper &decoder, std::ifstream &inputFile, IAVErrorHandler &errorHandler);
    ~ParserHelper();

    bool attach(AVCodecID codecID);
    bool nextFrame(/*out*/ FrameHelper &frame);
private:
    bool tryParseFrame(/*out*/ FrameHelper &frame, /*out*/ bool &needsMoreBytes);
    bool readBuffer();

    DecoderHelper &mDecoder;
    std::ifstream &mInputFile;
    IAVErrorHandler &mErrorHandler;
    AVCodecParserContext *mParser;
    std::vector<uint8_t> mStreamBuffer;
    std::vector<uint8_t> mInternalBuffer;
};

