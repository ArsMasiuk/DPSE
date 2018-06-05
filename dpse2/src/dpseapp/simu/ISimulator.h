/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include "CBranchEditorScene.h"


class ISimulator
{
public:
    virtual void setScene(const CBranchEditorScene& scene) = 0;
    virtual bool run() = 0;
	virtual bool stop() = 0;
};


class ISimulatorUI
{
public:
    virtual bool run(const CBranchEditorScene& scene) = 0;
};

