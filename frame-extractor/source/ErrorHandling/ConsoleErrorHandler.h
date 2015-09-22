#pragma once

class ConsoleErrorHandler : public IAVErrorHandler
{
public:
    // Inherited via IAVErrorHandler
    virtual void onErrorOccurred(const char * error) override;
};

