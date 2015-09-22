// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Params/Params.h"
#include "ErrorHandling/ConsoleErrorHandler.h"

static void usage(const char *name);
static void saveFrame(PacketHelper &packet, const std::string &output, int frameIndex);

int main(int argc, char *argv[])
{
    Params params;
    if (!ParamsParser::tryParseParams(argc, argv, /*out*/ params))
    {
        usage(argv[0]);
        return -1;
    }

    std::ifstream inputHandler(params.inputFile, std::ios::binary);

    ::avcodec_register_all();

    ConsoleErrorHandler errorHandler;

    DecoderHelper decoder(errorHandler);
    if (!decoder.attach(AV_CODEC_ID_H264))
        return -1;
    ParserHelper parser(decoder, inputHandler, errorHandler);
    if(!parser.attach(AV_CODEC_ID_H264))
        return -1;

    EncoderHelper encoder(errorHandler);
    if(!encoder.attach(AV_CODEC_ID_MJPEG))
        return -1;

    AVCodecContext *encoderContext = encoder.getContext();
    encoderContext->pix_fmt = PIX_FMT_YUVJ420P;
    //encoderContext->flags |= CODEC_FLAG_QSCALE;
    encoderContext->time_base = ::av_d2q(1, 1);
    encoderContext->qmin = encoderContext->qmax = 3;
    encoderContext->mb_lmin = encoderContext->lmin = encoderContext->qmin * FF_QP2LAMBDA;
    encoderContext->mb_lmax = encoderContext->lmax = encoderContext->qmax * FF_QP2LAMBDA;
    encoderContext->me_method = 1;
    //encoderContext->width = 1600;
    //encoderContext->height = 1200;

    PacketHelper packet;
    FrameHelper frame;
    uint64_t frameIndex = 0;
    while (parser.nextFrame(/*out*/ frame))
    {
        ++frameIndex;

        if (frameIndex < params.framePos)
            continue; // skip

        if (0 != params.frameCount && frameIndex >= params.framePos + params.frameCount)
            break;

        if(encoder.encodeFrame(frame, /*out*/ packet))
            saveFrame(packet, params.output, (int)frameIndex);
    }

    //worked();
    return 0;
}

static void usage(const char *name)
{
    std::printf("Usage: %s <inputfile> [-pos <frame-position> [-count <frame-count>]] [-o <outmask>]"
        "\n\t(where <inputfile> is a \"example.264\" file path,"
        "\n\t<frame-position> is an index of output frame,"
        "\n\t<frame-count> is count of output frames,"
        "\n\t<outmask> is an output name of file)\n", name);
}

static void saveFrame(PacketHelper &packet, const std::string &output, int frameIndex)
{
    std::vector<char> newName(255);
    std::copy(output.begin(), output.end(), newName.begin());
    ::av_get_frame_filename(&*newName.begin() + output.size(),
        (int)newName.size() - (int)output.size(), "%d.jpg", frameIndex);
    std::ofstream outputHandler(&newName[0], std::ios::binary);
    outputHandler.write((char *)packet.getPkt()->data, packet.getPkt()->size);
}
