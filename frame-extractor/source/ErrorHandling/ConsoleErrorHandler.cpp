#include "stdafx.h"
#include "ConsoleErrorHandler.h"

void ConsoleErrorHandler::onErrorOccurred(const char *error)
{
    std::printf("Error occurred: %s\n", error);
}
