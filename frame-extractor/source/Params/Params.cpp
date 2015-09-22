#include "stdafx.h"
#include "Params.h"

bool ParamsParser::tryParseParams(int argc, char *argv[], Params &params)
{
    if (argc > 8)
        return false;

    ParamsParser parser(argc, argv, params);
    auto &options = parser.mOptions;
    for (int i = 1; i < argc; ++i)
    {
        const char *option = argv[i];
        auto &find = options.find(option);
        if (options.end() != find)
        {
            if ((parser.*(find->second))(++i))
                continue;
            return false;
        }
        if (parser.parseInputFile(i))
            continue;
        return false;
    }
    return true;
}

ParamsParser::ParamsParser(int argc, char *argv[], Params &params)
    : mArgsCount(argc)
    , mArgs(argv)
    , mParams(params)
{
    mParams = { };
    mOptions.emplace("-pos", &ParamsParser::parseFramePosition);
    mOptions.emplace("-count", &ParamsParser::parseFrameCount);
    mOptions.emplace("-o", &ParamsParser::parseOutput);
}

bool ParamsParser::parseFramePosition(int optionIndex)
{
    if (!checkIndex(optionIndex))
        return false;
    mParser.clear();
    mParser.str(mArgs[optionIndex]);
    mParser >> mParams.framePos;
    bool result = !mParser.fail();
    if (result)
    {
        if (0 == mParams.frameCount)
            mParams.frameCount = 1;

    }

    return result;
}

bool ParamsParser::parseFrameCount(int optionIndex)
{
    if (!checkIndex(optionIndex))
        return false;
    mParser.clear();
    mParser.str(mArgs[optionIndex]);
    mParser >> mParams.frameCount;
    return !mParser.fail();
}

bool ParamsParser::parseInputFile(int optionIndex)
{
    if (!checkIndex(optionIndex))
        return false;
    mParams.inputFile = mArgs[optionIndex];
    return true;
}

bool ParamsParser::parseOutput(int optionIndex)
{
    if (!checkIndex(optionIndex))
        return false;
    mParams.output = mArgs[optionIndex];
    return true;
}

bool ParamsParser::checkIndex(int index)
{
    return index >= 0 && index < mArgsCount;
}
