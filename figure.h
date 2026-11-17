#pragma once

class Figure {
public:
    virtual ~Figure() = default;
    virtual void draw() = 0;
    virtual void step() = 0;
    virtual bool isFinished() const = 0;
    virtual void erase() = 0;
};