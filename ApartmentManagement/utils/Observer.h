#ifndef LAB6_7_OBSERVER_H
#define LAB6_7_OBSERVER_H

class Observer
{
public:
    virtual void update() = 0;
    virtual ~Observer() = default;
};

#endif // LAB6_7_OBSERVER_H
