// Copyright (c) 2013 Ryan Houdek

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

#include <vector>
#include <string>

class cSettings
{
	private:
	// Members
	std::vector<std::string> m_cpufreqs;
	int m_cpufreq;
	bool m_SupportsPWM;
			
	// Functions
	void LoadCPUFreqs();
	void GetDevice();
	public:
	cSettings();
	~cSettings();

	// Public Functions
	// Returns false on failure
	bool SetCPUFreq(unsigned int index);
	// Returns index in the CPU Freq
	unsigned int GetCPUFreq();
	// Returns false on failure
	bool SetCPUGovernor(bool performance);
	// Returns -1 on error, 0 on ondemand, 1 on performance
	int GetCPUGovernor();
	// Returns CPU frequencies
	std::vector<std::string> GetCPUFreqs() { return m_cpufreqs; }
	// Returns if it supports the PWM fan or not
	bool SupportsPWM() { return m_SupportsPWM; }
	// Checks if PWM is enabled or not
	bool isPWMEnabled();
	// Enable or disable the PWM fan
	bool SetPWM(const bool Enable);
	// Set's PWM value from 0 to 255. 0 being the least amount of fan movement
	bool SetPWMValue(const int Value); 
	// Returns the PWM value, 0 to 255
	unsigned int GetPWMValue();
};

extern cSettings Settings;
