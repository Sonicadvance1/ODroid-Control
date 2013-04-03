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

#include <algorithm>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include "Settings.h"

GladeXML *main_window;

GtkWidget *FreqLabel;
GtkWidget *FreqHScale;

GtkWidget *GovPerformance;

GtkWidget *PWMToggle;
GtkWidget *PWMPanel;
GtkWidget *PWMScale;

void MessageBox(const char *Text, const char *Caption = 0)
{
	GtkWidget *msgbox;
	msgbox = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, Text);

	if (Caption != NULL)
		gtk_window_set_title(GTK_WINDOW(msgbox), Caption);
	else
		gtk_window_set_title(GTK_WINDOW(msgbox), "Woops!");
	
	gint result = gtk_dialog_run(GTK_DIALOG(msgbox));
	gtk_widget_destroy(GTK_WIDGET(msgbox));
}

gboolean MaxFreqChangeEvent(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer user_data)
{
	std::vector<std::string> Freqs;
	Freqs = Settings.GetCPUFreqs();
	int NewVal = std::min((int)value, (int)Freqs.size() - 1);
	
	std::string LabelString;
	LabelString = "Frequency: " + Freqs[NewVal];
	gtk_label_set_text(GTK_LABEL(FreqLabel), LabelString.c_str());

	Settings.SetCPUFreq(NewVal);
	return false;
}

void GovToggled(GtkToggleButton *but, gpointer user_data)
{
	bool bPerfToggled;
	bPerfToggled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(GovPerformance));
	gtk_button_set_label(GTK_BUTTON(GovPerformance), bPerfToggled ? "Performance" : "OnDemand");
}

void PWMToggled(GtkToggleButton *but, gpointer user_data)
{
	bool bPWMEnabled = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(PWMToggle));
	Settings.SetPWM(bPWMEnabled);
	gtk_button_set_label(GTK_BUTTON(PWMToggle), bPWMEnabled ? "Enabled" : "Disabled");
}

gboolean PWMChangeEvent(GtkRange *range, GtkScrollType scroll, gdouble value, gpointer user_data)
{
	int PWMVal = (int)value;
	Settings.SetPWMValue(PWMVal);
	return false;
}

void SetFreqs()
{
	std::vector<std::string> Freqs;
	Freqs = Settings.GetCPUFreqs();

	FreqHScale = glade_xml_get_widget(main_window, "FreqHScale");
	FreqLabel = glade_xml_get_widget(main_window, "FreqLabel");

	std::string LabelString;
	unsigned int FreqIndex = Settings.GetCPUFreq();
	LabelString = "Frequency: " + Freqs[FreqIndex];
	gtk_label_set_text(GTK_LABEL(FreqLabel), LabelString.c_str());

	g_signal_connect(G_OBJECT(FreqHScale), "change-value", G_CALLBACK(MaxFreqChangeEvent), NULL);
	gtk_range_set_range(GTK_RANGE(FreqHScale), 0, Freqs.size());
	gtk_range_set_increments(GTK_RANGE(FreqHScale), 1, 1);
	gtk_range_set_value(GTK_RANGE(FreqHScale), FreqIndex);
}

void SetGovernor()
{
	GovPerformance = glade_xml_get_widget(main_window, "GovPerformance");

	g_signal_connect(G_OBJECT(GovPerformance), "toggled", G_CALLBACK(GovToggled), NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(GovPerformance), Settings.GetCPUGovernor());
	gtk_button_set_label(GTK_BUTTON(GovPerformance), Settings.GetCPUGovernor() ? "Performance" : "OnDemand");
}

void SetPWM()
{
	PWMToggle = glade_xml_get_widget(main_window, "PWMToggle");
	PWMPanel = glade_xml_get_widget(main_window, "PWMFrame");
	PWMScale = glade_xml_get_widget(main_window, "PWMScale");

	if (!Settings.SupportsPWM())
	{
		gtk_widget_hide(PWMPanel);
		return;
	}
	g_signal_connect(G_OBJECT(PWMToggle), "toggled", G_CALLBACK(PWMToggled), NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(PWMToggle), Settings.isPWMEnabled());
	gtk_button_set_label(GTK_BUTTON(PWMToggle), Settings.isPWMEnabled() ? "Enabled" : "Disabled");

	g_signal_connect(G_OBJECT(PWMScale), "change-value", G_CALLBACK(PWMChangeEvent), NULL);
	gtk_range_set_value(GTK_RANGE(PWMScale), Settings.GetPWMValue());
}

int main(int argc, char **argv)
{
	GtkWidget *widget;
	
	gtk_init (&argc, &argv);

	/* Load the Iterface */
	main_window = glade_xml_new("Interface.glade", NULL, NULL);

	widget = glade_xml_get_widget(main_window, "MainWindow");

	g_signal_connect (G_OBJECT (widget), "delete_event", G_CALLBACK (gtk_main_quit), NULL);
	gtk_widget_show(widget);	

	if (getuid())
	{
		MessageBox(	"This application needs to be run as root to work correctly.\n"
					"I'll be exiting now.");
		return 0;
	}

	SetFreqs();
	SetGovernor();
	SetPWM();

	gtk_main();

	return 0;
}
