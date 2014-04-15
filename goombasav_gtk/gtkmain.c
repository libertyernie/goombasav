#include <stdlib.h>
#include <gtk/gtk.h>
#include "../goombasav.h"

#define error_msg(...) { GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, __VA_ARGS__); gtk_dialog_run(GTK_DIALOG(dialog)); gtk_widget_destroy(dialog); }

const char* TITLE = "Goomba Save Manager";
const char* const sleeptxt[] = { "5min", "10min", "30min", "OFF" };
const char* const brightxt[] = { "I", "II", "III", "IIII", "IIIII" };

static char loaded_data[GOOMBA_COLOR_SRAM_SIZE];
static stateheader** headers = NULL;

static GtkListStore* listStore;
static GtkWidget* normal_rows[5];
static GtkWidget* cfg_rows[5]; // some hboxes will be in both arrays

static GtkWidget* lblSize;
static GtkWidget* lblType;
static GtkWidget* lblUncompSize;
static GtkWidget* lblBorder;
static GtkWidget* lblPalette;
static GtkWidget* lblSleep;

static void show_standard_rows() {
	for (int i=0; i<5; i++) {
		gtk_widget_hide(cfg_rows[i]);
		gtk_widget_show(normal_rows[i]);
	}
}

static void show_configuration_rows() {
	for (int i=0; i<5; i++) {
		gtk_widget_hide(normal_rows[i]);
		gtk_widget_show(cfg_rows[i]);
	}
}

static void click(GtkWidget* widget, gpointer data) {
	FILE* f = fopen("regular.sav", "rb");
	if (f == NULL) {
		error_msg("Could not open file %s", "regular.sav");
		return;
	}
	size_t total_bytes_read = 0;
	while (total_bytes_read < GOOMBA_COLOR_SRAM_SIZE) {
		size_t bytes_read = fread(loaded_data, 1, GOOMBA_COLOR_SRAM_SIZE-total_bytes_read, f);
		total_bytes_read += bytes_read;
		if (bytes_read <= 0) {
			error_msg("Could only read %lu bytes from %s", (unsigned long)total_bytes_read, "regular.sav");
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
		gtk_list_store_set(listStore, &iter, 0, stateheader_summary_str(headers[i]), 1, headers[i], -1);
	}
}

static void selection_changed(GtkWidget* widget, gpointer data) {
	GtkTreeIter iter;
	stateheader* ptr;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), NULL, &iter)) {
		gtk_tree_model_get(GTK_TREE_MODEL(listStore), &iter, 1, &ptr, -1);

		char buf[256];

		sprintf(buf, "Size: %u bytes    ", ptr->size);
		gtk_label_set_text(GTK_LABEL(lblSize), buf);

		gtk_label_set_text(GTK_LABEL(lblType),
			ptr->type == GOOMBA_STATESAVE ? "Type: Savestate"
			: ptr->type == GOOMBA_SRAMSAVE ? "Type: SRAM"
			: ptr->type == GOOMBA_CONFIGSAVE ? "Type: Configuration"
			: "Type:");

		if (ptr->type == GOOMBA_CONFIGSAVE) {
			show_configuration_rows();

			configdata* cd = (configdata*)ptr;
			sprintf(buf, "Border: %u", cd->bordercolor);
			gtk_label_set_text(GTK_LABEL(lblBorder), buf);
			sprintf(buf, "Palette: %u", cd->palettebank);
			gtk_label_set_text(GTK_LABEL(lblPalette), buf);
			sprintf(buf, "Sleep: %s", sleeptxt[cd->misc & 0x3]);
			gtk_label_set_text(GTK_LABEL(lblSleep), buf);
		} else {
			show_standard_rows();

			sprintf(buf, "%sompressed size: %u bytes",
				ptr->uncompressed_size < ptr->size ? "C" : "Unc",
				ptr->uncompressed_size);
			gtk_label_set_text(GTK_LABEL(lblUncompSize), buf);
		}
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
	listStore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);
	GtkTreeIter iter;
	gtk_list_store_append(listStore, &iter);
	gtk_list_store_set(listStore, &iter, 0, "First row", 1, "Second row", -1);

	GtkWidget* treeView = gtk_tree_view_new();
	GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeView), -1, "Header", renderer, "text", 0, NULL);

	gtk_tree_view_set_model(GTK_TREE_VIEW(treeView), GTK_TREE_MODEL(listStore));

	GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeView));
	g_signal_connect(selection, "changed", G_CALLBACK(selection_changed), NULL);

	// add list to hbox1
	gtk_widget_set_size_request(treeView, 150, -1);
	gtk_box_pack_start(GTK_BOX(hbox1), treeView, FALSE, FALSE, 0);
	gtk_widget_show(treeView);

	// construct vbox for right/main section
	GtkWidget* vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox1), vbox, TRUE, TRUE, 8);

	// put together both sets of rows
	normal_rows[0] = gtk_hbox_new(TRUE, 0);
	normal_rows[1] = gtk_hbox_new(TRUE, 0);
	normal_rows[2] = gtk_hbox_new(TRUE, 0);
	normal_rows[3] = gtk_hbox_new(TRUE, 0);
	normal_rows[4] = gtk_hbox_new(TRUE, 0);
	cfg_rows[0] = normal_rows[0];
	cfg_rows[1] = gtk_hbox_new(TRUE, 0);
	cfg_rows[2] = gtk_hbox_new(TRUE, 0);
	cfg_rows[3] = normal_rows[3];
	cfg_rows[4] = normal_rows[4];
	for (int i=0; i<5; i++) {
		gtk_box_pack_start(GTK_BOX(vbox), normal_rows[i], FALSE, FALSE, 4);
		gtk_box_pack_start(GTK_BOX(vbox), cfg_rows[i], FALSE, FALSE, 4);
	}

	// row 1
	lblSize = gtk_label_new("Size:");
	gtk_widget_show(lblSize);
	gtk_misc_set_alignment(GTK_MISC(lblSize), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(normal_rows[0]), lblSize, TRUE, TRUE, 0);
	lblType = gtk_label_new("Type:");
	gtk_widget_show(lblType);
	gtk_misc_set_alignment(GTK_MISC(lblType), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(normal_rows[0]), lblType, TRUE, TRUE, 0);

	// row 2 (normal)
	lblUncompSize = gtk_label_new("Uncompressed size:");
	gtk_widget_show(lblUncompSize);
	gtk_misc_set_alignment(GTK_MISC(lblUncompSize), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(normal_rows[1]), lblUncompSize, TRUE, TRUE, 0);

	// row 2 (config)
	lblBorder = gtk_label_new("Border:");
	lblPalette = gtk_label_new("Palette:");
	lblSleep = gtk_label_new("Sleep:");
	GtkWidget* arr[3] = { lblBorder, lblPalette, lblSleep };
	for (int i=0; i<3; i++) {
		gtk_widget_show(arr[i]);
		gtk_misc_set_alignment(GTK_MISC(arr[i]), 0, 0.5);
		gtk_box_pack_start(GTK_BOX(cfg_rows[1]), arr[i], TRUE, TRUE, 0);
	}

	// construct/add button for testing
	GtkWidget* button1 = gtk_button_new_with_label("Read regular.sav");
	g_signal_connect(button1, "clicked", G_CALLBACK(click), NULL);
	gtk_box_pack_end(GTK_BOX(vbox), button1, FALSE, FALSE, 8);
	gtk_widget_show(button1);

	/*GtkWidget* button2 = gtk_button_new_with_label("regular");
	g_signal_connect(button2, "clicked", G_CALLBACK(show_standard_rows), NULL);
	gtk_box_pack_end(GTK_BOX(vbox), button2, FALSE, FALSE, 8);
	gtk_widget_show(button2);
	GtkWidget* button3 = gtk_button_new_with_label("cfg");
	g_signal_connect(button3, "clicked", G_CALLBACK(show_configuration_rows), NULL);
	gtk_box_pack_end(GTK_BOX(vbox), button3, FALSE, FALSE, 8);
	gtk_widget_show(button3);*/

	// show things
	gtk_widget_show(vbox);
	gtk_widget_show(hbox1);
    gtk_widget_show(window);
	show_standard_rows();
    
    gtk_main();
    
    return 0;
}
