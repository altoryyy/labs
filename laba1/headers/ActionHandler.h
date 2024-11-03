#ifndef ACTIONHANDLER_H
#define ACTIONHANDLER_H

#include <iostream>
#include <string>
#include <functional>

template <typename T>
class ActionHandler
{
public:
    void execute(const std::string &description, double amount, std::function<void(const std::string &, double)> action)
    {
        action(description, amount);
    }
};

#endif