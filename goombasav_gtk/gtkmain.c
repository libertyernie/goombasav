#include <stdlib.h>
#include <gtk/gtk.h>
#include "../goombasav.h"

static char loaded_data[GOOMBA_COLOR_SRAM_SIZE];
static stateheader** headers = NULL;

static GtkListStore* listStore;

static void click(GtkWidget* widget, gpointer data) {
	FILE* f = fopen("pokemon.sav", "rb");
	if (f == NULL) {
		GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Could not open file %s", "pokemon.sav");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}
	size_t total_bytes_read = 0;
	while (total_bytes_read < GOOMBA_COLOR_SRAM_SIZE) {
		size_t bytes_read = fread(loaded_data, 1, GOOMBA_COLOR_SRAM_SIZE-total_bytes_read, f);
		total_bytes_read += bytes_read;
		printf("read %zu bytes\n", bytes_read);
		if (bytes_read <= 0) {
			GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Could only read %lu bytes from %s", (unsigned long)total_bytes_read, "pokemon.sav");
			gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
			return;
		}
	}
	fclose(f);

	gtk_list_store_clear(listStore);
	GtkTreeIter iter;
	if (headers != NULL) {
		free(headers);
		printf("freed headers\n");
	}
	headers = stateheader_scan(loaded_data);
	int i;
	for (i = 0; headers[i] != NULL; i++) {
		gtk_list_store_append(listStore, &iter);
		gtk_list_store_set(listStore, &iter, 0, stateheader_summary_str(headers[i]), -1);
	}
}
static gboolean delete_event(GtkWidget* widget, GdkEvent* event, gpointer data) {
    return FALSE;
}

static void destroy(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    
	// construct window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show(window);

	g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

	// construct/add top container, with list box and one other element
	GtkWidget* hbox1 = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), hbox1);

	// construct list
	listStore = gtk_list_store_new(1, G_TYPE_STRING);
	GtkTreeIter iter;
	gtk_list_store_append(listStore, &iter);
	gtk_list_store_set(listStore, &iter, 0, "First row", -1);

	GtkWidget* treeView = gtk_tree_view_new();
	GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeView), -1, "Header", renderer, "text", 0, NULL);

	gtk_tree_view_set_model(GTK_TREE_VIEW(treeView), GTK_TREE_MODEL(listStore));

	// add list to hbox1
	gtk_widget_set_size_request(treeView, 256, -1);
	gtk_box_pack_start(GTK_BOX(hbox1), treeView, FALSE, FALSE, 0);
	gtk_widget_show(treeView);

	// construct vbox for right/main section
	GtkWidget* vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_end(GTK_BOX(hbox1), vbox, FALSE, TRUE, 8);

	// construct/add button for testing
	GtkWidget* button1 = gtk_button_new_with_label("Read pokemon.sav");
	g_signal_connect(button1, "clicked", G_CALLBACK(click), NULL);
	gtk_box_pack_end(GTK_BOX(vbox), button1, FALSE, FALSE, 8);
	gtk_widget_show(button1);

	// show things
	gtk_widget_show(vbox);
	gtk_widget_show(hbox1);
    gtk_widget_show(window);
    
    gtk_main();
    
    return 0;
}
