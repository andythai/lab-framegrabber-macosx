#pragma once

#include "stdafx.h"
using namespace std;

// Interval to hold data and intervals to grab frames from in the video
class Interval
{
public:
	// Constructors
	Interval();
	Interval(string intervalName);
	Interval(string intervalName, double intervalStart, double intervalEnd);
	~Interval();

	// Getter methods
	double getStartTimeSeconds();
	double getEndTimeSeconds();
	double getLengthSeconds();
	double getStartTimeMs();
	double getEndTimeMs();
	double getLengthMs();
	string getName();

	// Setter methods
	void setStartTime(double newStartTime);
	void setEndTime(double newEndTime);
	void setTimes(double newStartTime, double newEndTime);
	void setName(string newName);

	// Calculations
	void calculateLength();

private:
	// Variables
	string name;
	double startTime;
	double endTime;
	double length;
};