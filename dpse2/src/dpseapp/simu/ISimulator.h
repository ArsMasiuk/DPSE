/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

class IGraphInterface;


struct SimuParams
{
	int simuTime;	// s, <= 0 - infinite
	float deltaX;
	int method;
};


class ISimulator
{
public:
    virtual void setGraph(const IGraphInterface& scene) = 0;
    virtual bool analyse() = 0;
    virtual bool run(const SimuParams& params) = 0;
    virtual bool stop() = 0;
};
