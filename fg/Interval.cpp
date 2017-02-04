#include "Interval.h"

// No-arg constructor
Interval::Interval()
{
}

// One-arg constructor with name
Interval::Interval(string intervalName)
{
	name = intervalName;
}

// Three-arg constructor for full data
Interval::Interval(string intervalName, double intervalStart, double intervalEnd)
{
	name = intervalName;
	startTime = intervalStart;
	endTime = intervalEnd;
	calculateLength();
}

// Destructor
Interval::~Interval()
{
}

// Getter method for starting time
double Interval::getStartTimeSeconds()
{
	return startTime;
}

// Getter method for ending time
double Interval::getEndTimeSeconds()
{
	return endTime;
}

// Getter method for interval length
double Interval::getLengthSeconds()
{
	return length;
}

// Getter method for starting time
double Interval::getStartTimeMs()
{
	return startTime * 1000;
}

// Getter method for ending time
double Interval::getEndTimeMs()
{
	return endTime * 1000;
}

// Getter method for interval length
double Interval::getLengthMs()
{
	return length * 1000;
}

// Getter method for name
string Interval::getName()
{
	return name;
}

// Setter method for start time
void Interval::setStartTime(double newStartTime)
{
	startTime = newStartTime;
	calculateLength();
}

// Setter method for end time
void Interval::setEndTime(double newEndTime)
{
	endTime = newEndTime;
	calculateLength();
}

// Setter method for both times
void Interval::setTimes(double newStartTime, double newEndTime)
{
	startTime = newStartTime;
	endTime = newEndTime;
	calculateLength();
}

// Setter method for name
void Interval::setName(string newName)
{
	name = newName;
}

// Setter method for interval length
void Interval::calculateLength()
{
	length = endTime - startTime;
}