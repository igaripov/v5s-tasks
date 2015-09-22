#pragma once

struct Params
{
    std::string inputFile;
    std::size_t framePos;
    std::size_t frameCount;
    std::string output;
};

struct ParamsParser
{
    static bool tryParseParams(int argc, char *argv[], /*out*/ Params &params);
private: 
    ParamsParser(int argc, char *argv[], Params &params);

    typedef bool(ParamsParser::*command)(int optionIndex);

    bool parseFramePosition(int optionIndex);
    bool parseFrameCount(int optionIndex);
    bool parseInputFile(int optionIndex);
    bool parseOutput(int optionIndex);

    bool checkIndex(int index);

    std::map<std::string, command> mOptions;
    Params &mParams;
    int mArgsCount;
    char **mArgs;
    std::istringstream mParser;
};

