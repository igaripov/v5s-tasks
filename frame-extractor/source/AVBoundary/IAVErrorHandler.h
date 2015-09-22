#pragma once

struct IAVErrorHandler
{
    virtual void onErrorOccurred(const char *error) = 0;
protected:
    virtual ~IAVErrorHandler() {}
};
