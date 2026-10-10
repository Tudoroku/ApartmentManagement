#ifndef LAB6_7_OBSERVABLE_H
#define LAB6_7_OBSERVABLE_H

#include "Observer.h"
#include <algorithm>
#include <vector>

class Observable
{
private:
    std::vector<Observer*> observers;

public:
    void addObserver(Observer* obs)
    {
        if (obs == nullptr)
        {
            return;
        }
        if (std::find(observers.begin(), observers.end(), obs) == observers.end())
        {
            observers.push_back(obs);
        }
    }

    void removeObserver(Observer* obs)
    {
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }

    void notifyObservers()
    {
        for (auto* obs : observers)
        {
            if (obs != nullptr)
            {
                obs->update();
            }
        }
    }
};

#endif // LAB6_7_OBSERVABLE_H
