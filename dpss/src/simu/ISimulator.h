#pragma once

#include "CBranchEditorScene.h"


class ISimulator
{
public:
    virtual void setScene(const CBranchEditorScene& scene) = 0;
    virtual bool run() = 0;
};


class ISimulatorUI
{
public:
    virtual bool run(const CBranchEditorScene& scene) = 0;
};

