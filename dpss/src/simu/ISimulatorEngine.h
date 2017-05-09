/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <stdio.h>
#include <vector>

using namespace std;


class ISimulatorCallback
{
public:
	// text		any text to output
	// status	optional severity id (for example, 0 - ok, 1 - error etc)
	virtual void log(const char* text, int status = 0) = 0;

	// time		 time stamp of the step
	// step		iteration number (0, 1, ...)
	// qvec		vector of Q values calculated for every branch (from 1 to N)
	virtual void stepResult(double time, int step, std::vector<double>& qvec) = 0;
};



class ISimulatorEngine
{
public:
	struct SimulationParams
	{
		double duration;	// in seconds; <= 0 - until forced stop
		double timeStep;

		SimulationParams()
		{
			duration = 120;
			timeStep = 0.001;
		}
	};

	// Starts simulation.
	// inputFileName	path to file (DDS format?)
	// params			simulation paramters
	// out				callback to log process and pass results
	// return			0 - ok, else failed
	virtual int runSimulation(const char* inputFileName, const SimulationParams& params, ISimulatorCallback& out) = 0;

	// Forces simulation to stop.
	// out				callback to log process and pass results
	// return			0 - ok, else failed
	virtual int stopSimulation(ISimulatorCallback& out) = 0;
};


