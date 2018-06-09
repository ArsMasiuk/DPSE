/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

class IGraphInterface;


class ISimulator
{
public:
    virtual void setGraph(const IGraphInterface& scene) = 0;
    virtual bool analyse() = 0;
    virtual bool run() = 0;
    virtual bool stop() = 0;
};
