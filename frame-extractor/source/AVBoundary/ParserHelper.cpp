#include "stdafx.h"
#include "ParserHelper.h"

static const std::size_t gInteranlBufferSize = 64000;

ParserHelper::ParserHelper(DecoderHelper &decoder, std::ifstream &inputFile, IAVErrorHandler &errorHandler)
    : mDecoder(decoder)
    , mInputFile(inputFile)
    , mErrorHandler(errorHandler)
    , mParser(nullptr)
    , mInternalBuffer(gInteranlBufferSize)
{
}

ParserHelper::~ParserHelper()
{
    if (mParser)
    {
        ::av_parser_close(mParser);
    }
}

bool ParserHelper::attach(AVCodecID codecID)
{
    mParser = ::av_parser_init(codecID); 
    if (!mParser)
    {
        mErrorHandler.onErrorOccurred("Cannot create parser.");
        return false;
    }
    return true;
}

bool ParserHelper::nextFrame(FrameHelper &frame)
{
    bool needMore = false;
    while (!tryParseFrame(frame, needMore))
    {
        if (needMore && !readBuffer())
            return false;
    }
    return true;
}

bool ParserHelper::readBuffer()
{
    mInputFile.read((char *)&mInternalBuffer[0], mInternalBuffer.size());
    auto readBytes = mInputFile.gcount();

    if (readBytes) {
        mStreamBuffer.insert(mStreamBuffer.end(), mInternalBuffer.begin(), mInternalBuffer.begin() + readBytes);
    }

    return 0 != readBytes;
}

//enum class ParserStatus
//{
//    Success,
//    NeedsBytes,
//    ParseError,
//    DecodeError
//};

bool ParserHelper::tryParseFrame(FrameHelper &frame, bool &needsMoreBytes)
{
    static int headIndex = 0;
    needsMoreBytes = false;

    if (/*mStreamBuffer.empty()*/0 == ((int)mStreamBuffer.size() - (headIndex))) {
        needsMoreBytes = true;
        return false;
    }

    uint8_t *data = nullptr;
    int size = 0;
    //int usedBytes = ::av_parser_parse2(mParser, mDecoder.getContext(), &data, &size,
    //    &mStreamBuffer[0], (int)mStreamBuffer.size(), mParser->pts, mParser->dts, AV_NOPTS_VALUE);
    int usedBytes = ::av_parser_parse2(mParser, mDecoder.getContext(), &data, &size,
        &mStreamBuffer[headIndex], (int)mStreamBuffer.size() - (headIndex), mParser->pts, mParser->dts, AV_NOPTS_VALUE);

    if (usedBytes)
    {
        headIndex += usedBytes;
    }

    if (size == 0 && usedBytes >= 0) {
        needsMoreBytes = true;
        return false;
    }

    if (size) {
        bool result = mDecoder.decode(data, size, frame);
        // call after decode
        //if (usedBytes)
        //    mStreamBuffer.erase(mStreamBuffer.begin(), mStreamBuffer.begin() + usedBytes);
        return result;
    }

    return false;
}
