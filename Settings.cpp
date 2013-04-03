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

#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include "Settings.h"

cSettings Settings;

const char *g_fileavailcpufreq = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies";
const char *g_filecpufreq = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq";
const char *g_filecpugov = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor";
const char *g_filecurrentcpufreq = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq";
const char *g_filepwmenable = "/sys/devices/platform/odroid-fan/pwm_enable";
const char *g_filepwmvalue = "/sys/devices/platform/odroid-fan/pwm_duty";

const char *procfile = "/proc/cpuinfo";

void ExecuteCommand(const char *command, char *out)
{
	FILE *fp;
	fp = popen(command, "r");
	
	if (fp == NULL)
	{
		printf("Couldn't open '%s' for reading\n", g_fileavailcpufreq);
		return;
	}
	while ( fgets(out, 1024, fp) != NULL ){}
	pclose(fp);

}
void cSettings::LoadCPUFreqs()
{
	char freqs[1024];
	int size;

	sprintf(freqs, "cat %s", g_fileavailcpufreq);
	ExecuteCommand(freqs, freqs);
	
	char *pch;
	pch = strtok(freqs, " ");
	while (pch != NULL)
	{
		if (strlen(pch) > 1)
			m_cpufreqs.insert(m_cpufreqs.begin(), std::string(pch));
		pch = strtok(NULL, " ");
	}
}
bool cSettings::SetCPUFreq(unsigned int index)
{
	if (index != m_cpufreq)
	{
		m_cpufreq = index;
		char command[1024];
		sprintf(command, "echo '%s' > %s",  m_cpufreqs[index].c_str(), g_filecpufreq);

		ExecuteCommand(command, command);

		if (!strstr(command, "denied"))
			return false;
	}
	return true;
}
unsigned int cSettings::GetCPUFreq()
{
	char freq[1024];
	int size;

	sprintf(freq, "cat %s", g_filecurrentcpufreq);
	ExecuteCommand(freq, freq);
	for (int a = 0; a < m_cpufreqs.size(); ++a)
		if (atoi(freq) == atoi(m_cpufreqs[a].c_str()))
			return a;
	printf("Returned %s\n", freq);
	printf("Couldn't get current CPU freq\n");
	return 0;
}
bool cSettings::SetCPUGovernor(bool performance)
{
	const char *governers[] = { "ondemand", "performance" };
	char command[1024];

	sprintf(command, "echo '%s' > %s",  governers[performance], g_filecpugov);

	ExecuteCommand(command, command);

	if (!strstr(command, "denied"))
		return false;

	return true;
}

int cSettings::GetCPUGovernor()
{
	char freqs[1024];
	int size;

	sprintf(freqs, "cat %s", g_filecpugov);

	ExecuteCommand(freqs, freqs);
	
	if (strstr(freqs, "performance"))
		return 1;
	return 0;
}

void cSettings::GetDevice()
{
	const char marker[] = "Hardware\t: ";
	char *cpu_string = 0;
	// Count the number of processor lines in /proc/cpuinfo
	char buf[1024];

	FILE *fp = fopen(procfile, "r");
	if (!fp)
		return;
	
	while (fgets(buf, sizeof(buf), fp))
	{
		if (strncmp(buf, marker, sizeof(marker) - 1))
			continue;
		cpu_string = buf + sizeof(marker) - 1;
		cpu_string = strndup(cpu_string, strlen(cpu_string) - 1); // Strip the newline
		break;
	}

	fclose(fp);
	
	if (strstr(cpu_string, "ODROIDU2"))
		m_SupportsPWM = true;
	else
		m_SupportsPWM = false;	
}

bool cSettings::isPWMEnabled()
{
	char freqs[1024];
	int size;

	sprintf(freqs, "cat %s", g_filepwmenable);

	ExecuteCommand(freqs, freqs);
	
	if (strstr(freqs, "1"))
		return 1;
	return 0;

}

bool cSettings::SetPWM(const bool Enable)
{
	char command[1024];
	sprintf(command, "echo '%d' > %s", Enable, g_filepwmenable);

	ExecuteCommand(command, command);

	if (!strstr(command, "denied"))
		return false;

	return true;
}

bool cSettings::SetPWMValue(const int value)
{
	static int PWMval;
	if (PWMval == value)
		return true;
	PWMval = value;

	char command[1024];
	sprintf(command, "echo '%d' > %s", value, g_filepwmvalue);

	ExecuteCommand(command, command);

	if (!strstr(command, "denied"))
		return false;

	return true;
}
unsigned int cSettings::GetPWMValue()
{
	char command[1024];
	sprintf(command, "cat %s", g_filepwmvalue);

	ExecuteCommand(command, command);

	return atoi(command);
}

cSettings::cSettings()
{
	LoadCPUFreqs();
	GetDevice();
}

cSettings::~cSettings()
{
}
