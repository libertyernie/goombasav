#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "../goombasav.h"

#define error_msg(...) { GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, __VA_ARGS__); gtk_dialog_run(GTK_DIALOG(dialog)); gtk_widget_destroy(dialog); }
#define note_msg(...) { GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, __VA_ARGS__); gtk_dialog_run(GTK_DIALOG(dialog)); gtk_widget_destroy(dialog); }

const char* TITLE = "Goomba Save Manager";
const char* const sleeptxt[] = { "5min", "10min", "30min", "OFF" };
const char* const brightxt[] = { "I", "II", "III", "IIII", "IIIII" };

static char loaded_sram[GOOMBA_COLOR_SRAM_SIZE];
static char* _filePath = NULL;
static bool dirty;
static stateheader** headers = NULL;

static GtkWidget* window;
static GtkListStore* listStore;
static GtkTreeSelection* selection;
static GtkWidget* normal_rows[5];
static GtkWidget* cfg_rows[5]; // some hboxes will be in both arrays

static GtkWidget* lblSize;
static GtkWidget* lblType;
static GtkWidget* lblUncompSize;
static GtkWidget* lblBorder;
static GtkWidget* lblPalette;
static GtkWidget* lblSleep;
static GtkWidget* lblFramecount;
static GtkWidget* lblGamma;
static GtkWidget* lblAutostate;
static GtkWidget* lblChecksum;
static GtkWidget* lblTitle;

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

static void lblset(GtkWidget** variable, const char* initial_text, GtkWidget* box) {
	if (*variable == NULL) {
		*variable = gtk_label_new(initial_text);
		gtk_widget_show(*variable);
		gtk_misc_set_alignment(GTK_MISC(*variable), 0, 0.5);
		gtk_box_pack_start(GTK_BOX(box), *variable, TRUE, TRUE, 0);
	} else {
		gtk_label_set_text(GTK_LABEL(*variable), initial_text);
	}
}

static void set_all_labels() {
	lblset(&lblSize, "Size:", normal_rows[0]);
	lblset(&lblType, "Type:", normal_rows[0]);

	lblset(&lblUncompSize, "Uncompressed size:", normal_rows[1]);

	lblset(&lblBorder, "Border:", cfg_rows[1]);
	lblset(&lblPalette, "Palette:", cfg_rows[1]);
	lblset(&lblSleep, "Sleep:", cfg_rows[1]);

	lblset(&lblFramecount, "Frame count:", normal_rows[2]);

	lblset(&lblGamma, "Gamma:", cfg_rows[2]);
	lblset(&lblAutostate, "Autoload state:", cfg_rows[2]);

	lblset(&lblChecksum, "ROM checksum:", normal_rows[3]);

	lblset(&lblTitle, "Title:", normal_rows[4]);
}

static void update_titlebar(GtkWindow* window) {
	if (_filePath == NULL) {
		gtk_window_set_title(window, TITLE);
		return;
	}
	char* i1 = strrchr(_filePath, '/') + 1;
	char* i2 = strrchr(_filePath, '\\') + 1;
	char* filename = i1 > i2 ? i1 : i2;
	char* buf = (char*)malloc(strlen(TITLE) + 3 + strlen(filename) + 1);
	sprintf(buf, "%s - %s", TITLE, filename);
	gtk_window_set_title(window, buf);
	free(buf);
}

static void open_click(GtkWidget* widget, gpointer data) {
	GtkWidget* dialog = gtk_file_chooser_dialog_new("Open", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
	gint res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res != GTK_RESPONSE_ACCEPT) return;

	char* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	FILE* f = fopen(path, "rb");
	gtk_widget_destroy(dialog);

	if (f == NULL) {
		error_msg("Could not open file %s", path);
		return;
	}
	size_t total_bytes_read = 0;
	while (total_bytes_read < GOOMBA_COLOR_SRAM_SIZE) {
		size_t bytes_read = fread(loaded_sram, 1, GOOMBA_COLOR_SRAM_SIZE-total_bytes_read, f);
		total_bytes_read += bytes_read;
		if (bytes_read <= 0) {
			error_msg("Could only read %lu bytes from %s", (unsigned long)total_bytes_read, path);
			return;
		}
	}
	fseek(f, 0, SEEK_END);
	uint32_t filesize = (uint32_t)ftell(f);
	fclose(f);
	if (filesize > GOOMBA_COLOR_SRAM_SIZE) {
		GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
			"This file is more than %u bytes. If you overwrite the file, the last %u bytes will be discarded.",
			GOOMBA_COLOR_SRAM_SIZE, filesize - GOOMBA_COLOR_SRAM_SIZE);
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}

	char* cleaned = goomba_cleanup(loaded_sram);
	if (cleaned == NULL) {
		// this should not happen
		error_msg(goomba_last_error());
		g_free(path);
		return;
	} else if (cleaned != loaded_sram) {
		GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_NONE,
			"Uncompressed SRAM found at 0xE000. Would you like to move and compress it to the proper location? (Doing this is required to extract or replace the save data.)");
		gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", GTK_RESPONSE_CANCEL);
		gtk_dialog_add_button(GTK_DIALOG(dialog), "Skip", GTK_RESPONSE_NO);
		gtk_dialog_add_button(GTK_DIALOG(dialog), "Clean", GTK_RESPONSE_YES);
		gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_YES);
		gint res = gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		if (res == GTK_RESPONSE_YES) {
			memcpy(loaded_sram, cleaned, GOOMBA_COLOR_SRAM_SIZE);
			dirty = true;
		}
		free(cleaned);
		if (res == GTK_RESPONSE_CANCEL) {
			g_free(path);
			return;
		}
	} else {
		dirty = false;
	}

	if (_filePath != NULL) free(_filePath);
	_filePath = (char*)malloc(strlen(path) + 1);
	strcpy(_filePath, path);
	g_free(path);

	update_titlebar(GTK_WINDOW(window));

	gtk_list_store_clear(listStore);
	GtkTreeIter iter;
	if (headers != NULL) {
		free(headers);
	}
	headers = stateheader_scan(loaded_sram);
	int i;
	for (i = 0; headers[i] != NULL; i++) {
		gtk_list_store_append(listStore, &iter);
		gtk_list_store_set(listStore, &iter, 0, stateheader_summary_str(headers[i]), 1, headers[i], -1);
	}
	set_all_labels();
}

static void export_click(GtkWidget* widget, gpointer data) {
	GtkTreeIter iter;
	stateheader* sh;
	if (_filePath != NULL && gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), NULL, &iter)) {
		gtk_tree_model_get(GTK_TREE_MODEL(listStore), &iter, 1, &sh, -1);
		goomba_size_t len;
		void* gbcsav = goomba_extract(loaded_sram, sh, &len);
		if (gbcsav == NULL) {
			error_msg("Error: %s", goomba_last_error());
			return;
		}

		GtkWidget* dialog = gtk_file_chooser_dialog_new("Export", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
		gint res = gtk_dialog_run(GTK_DIALOG(dialog));

		if (res == GTK_RESPONSE_ACCEPT) {
			char* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			FILE* outfile = fopen(path, "wb");
			if (outfile == NULL) {
				error_msg("Could not open file: %s", strerror(errno));
			} else {
				fwrite(gbcsav, 1, len, outfile);
				fclose(outfile);
			}
		}
		gtk_widget_destroy(dialog);
		free(gbcsav);
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

		sprintf(buf, "Title: %.32s", ptr->title);
		gtk_label_set_text(GTK_LABEL(lblTitle), buf);

		if (ptr->type == GOOMBA_CONFIGSAVE) {
			show_configuration_rows();

			configdata* cd = (configdata*)ptr;
			sprintf(buf, "Border: %u", cd->bordercolor);
			gtk_label_set_text(GTK_LABEL(lblBorder), buf);
			sprintf(buf, "Palette: %u", cd->palettebank);
			gtk_label_set_text(GTK_LABEL(lblPalette), buf);
			sprintf(buf, "Sleep: %s", sleeptxt[cd->misc & 0x3]);
			gtk_label_set_text(GTK_LABEL(lblSleep), buf);
			sprintf(buf, "Gamma: %s", brightxt[(cd->misc & 0xE0) >> 5]);
			gtk_label_set_text(GTK_LABEL(lblGamma), buf);
			sprintf(buf, "Autoload state: %s", ((cd->misc & 0x10) >> 4) ? "ON" : "OFF");
			gtk_label_set_text(GTK_LABEL(lblAutostate), buf);

			sprintf(buf, "ROM checksum: %08X", cd->sram_checksum);
			gtk_label_set_text(GTK_LABEL(lblChecksum), buf);
		} else {
			show_standard_rows();

			sprintf(buf, "%sompressed size: %u bytes",
				ptr->uncompressed_size < ptr->size ? "C" : "Unc",
				ptr->uncompressed_size);
			gtk_label_set_text(GTK_LABEL(lblUncompSize), buf);

			sprintf(buf, "Frame count: %u", ptr->framecount);
			gtk_label_set_text(GTK_LABEL(lblFramecount), buf);

			sprintf(buf, "ROM checksum: %08X", ptr->checksum);
			gtk_label_set_text(GTK_LABEL(lblChecksum), buf);
		}
	}
}

static gboolean delete_event(GtkWidget* widget, GdkEvent* event, gpointer data) {
    return FALSE;
}

static void destroy(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

GtkWidget* build_window() {
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);

	g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

	GtkWidget* vbox1 = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox1);

	// construct menubar
	GtkWidget* menubar = gtk_menu_bar_new();
	GtkWidget* file_item = gtk_menu_item_new_with_label("File");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_item);

	GtkWidget* file_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
	GtkWidget* open_item = gtk_menu_item_new_with_label("Open");
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_item);
	g_signal_connect(open_item, "activate", G_CALLBACK(open_click), NULL);

	gtk_box_pack_start(GTK_BOX(vbox1), menubar, FALSE, FALSE, 0);
	gtk_widget_show_all(menubar);

	// construct/add top container, with list box and one other element
	GtkWidget* hbox1 = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_end(GTK_BOX(vbox1), hbox1, TRUE, TRUE, 0);

	// construct list
	listStore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);

	GtkWidget* treeView = gtk_tree_view_new();
	GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeView), -1, "Header", renderer, "text", 0, NULL);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeView), FALSE);
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeView), GTK_TREE_MODEL(listStore));

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeView));
	g_signal_connect(selection, "changed", G_CALLBACK(selection_changed), NULL);

	// add list to hbox1
	gtk_widget_set_size_request(treeView, 150, -1);
	gtk_box_pack_start(GTK_BOX(hbox1), treeView, FALSE, FALSE, 0);
	gtk_widget_show(treeView);

	// construct vbox for right/main section
	GtkWidget* vbox2 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox1), vbox2, TRUE, TRUE, 8);

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
		gtk_box_pack_start(GTK_BOX(vbox2), normal_rows[i], FALSE, FALSE, 4);
		if (normal_rows[i] != cfg_rows[i]) {
			gtk_box_pack_start(GTK_BOX(vbox2), cfg_rows[i], FALSE, FALSE, 4);
		}
	}

	set_all_labels();

	// bottom row for buttons
	GtkWidget* button_hbox = gtk_hbox_new(FALSE, 8);
	gtk_box_pack_end(GTK_BOX(vbox2), button_hbox, FALSE, FALSE, 0);
	gtk_widget_show(button_hbox);

	GtkWidget* btnReplace = gtk_button_new_with_label("Replace");
	gtk_widget_set_size_request(btnReplace, 80, -1);
	gtk_box_pack_start(GTK_BOX(button_hbox), btnReplace, FALSE, FALSE, 0);
	gtk_widget_show(btnReplace);
	GtkWidget* btnExport = gtk_button_new_with_label("Export");
	gtk_widget_set_size_request(btnExport, 80, -1);
	gtk_box_pack_start(GTK_BOX(button_hbox), btnExport, FALSE, FALSE, 0);
	gtk_widget_show(btnExport);
	g_signal_connect(btnExport, "clicked", G_CALLBACK(export_click), NULL);

	// show things
	gtk_widget_show(vbox2);
	gtk_widget_show(hbox1);
	gtk_widget_show(vbox1);
	gtk_widget_set_size_request(window, 425, 225);
	return window;
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    
	// construct window
	GtkWidget* window = build_window();
	gtk_widget_show(window);

	update_titlebar(GTK_WINDOW(window));
	show_standard_rows();

	gtk_main();

	return 0;
}
