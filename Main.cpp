#include <gtk/gtk.h>
#include <glade/glade.h>

int main(int argc, char **argv)
{
	GladeXML *main_window;
	GtkWidget *widget;
	
	gtk_init (&argc, &argv);

	/* Load the Iterface */
	main_window = glade_xml_new("Interface.glade", NULL, NULL);

	widget = glade_xml_get_widget(main_window, "MainWindow");

	g_signal_connect (G_OBJECT (widget), "delete_event", G_CALLBACK (gtk_main_quit), NULL);
	gtk_widget_show(widget);	

	printf("Starting\n");
	gtk_main();
	return 0;
}
