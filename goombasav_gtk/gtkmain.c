#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "../goombasav.h"

#define error_msg(...) { GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, __VA_ARGS__); gtk_dialog_run(GTK_DIALOG(dialog)); gtk_widget_destroy(dialog); }
#define note_msg(...) { GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, __VA_ARGS__); gtk_dialog_run(GTK_DIALOG(dialog)); gtk_widget_destroy(dialog); }

const char* TITLE = "Goomba Save Manager";

const char* goomba_basename(const char* c) {
	const char* i1 = strrchr(c, '/') + 1;
	const char* i2 = strrchr(c, '\\') + 1;
	return i1 > i2 ? i1 : i2;
}

const char* GPL_NOTICE = "Goomba Save Manager (GTK frontend)\n"
"Copyright (C) 2014 libertyernie\n"
"http://github.com/libertyernie/goombasav\n"
"\n"
"This program is free software: you can redistribute it and/or modify\n"
"it under the terms of the GNU General Public License as published by\n"
"the Free Software Foundation, either version 2 of the License, or\n"
"(at your option) any later version.\n"
"\n"
"This program is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the\n"
"GNU General Public License for more details.\n"
"\n"
"You should have received a copy of the GNU General Public License\n"
"along with this program. If not, see <http://www.gnu.org/licenses/>.\n";

#pragma region window-level variables
static char loaded_sram[GOOMBA_COLOR_SRAM_SIZE];
static char* _filePath = NULL;
static bool dirty;
static stateheader** headers = NULL;

static GtkWidget* window;
static GtkListStore* listStore;
static GtkTreeSelection* selection;
static GtkWidget* normal_rows[5];
static GtkWidget* cfg_rows[5]; // some hboxes will be in both arrays

static GtkWidget* save_item;
static GtkWidget* save_as_item;
static GtkWidget* btnExport;
static GtkWidget* btnReplace;

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

static GtkWidget* dataHashColor;
static GtkWidget* lblDataHash;
#pragma endregion

#pragma region gtk helper functions
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

	if (lblDataHash != NULL) gtk_label_set_text(GTK_LABEL(lblDataHash), "");
	if (dataHashColor != NULL) gtk_widget_hide(dataHashColor);
}

static void update_titlebar(GtkWindow* window) {
	if (_filePath == NULL) {
		gtk_window_set_title(window, TITLE);
		return;
	}
	const char* filename = goomba_basename(_filePath);
	char* buf = (char*)malloc(strlen(TITLE) + 3 + strlen(filename) + 1);
	sprintf(buf, "%s - %s", TITLE, filename);
	gtk_window_set_title(window, buf);
	free(buf);
}

static void header_scan() {
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
#pragma endregion

#pragma region event handlers
static void save(const char* path) {
	FILE* f = fopen(path, "wb");
	if (f == NULL) {
		error_msg("Could not open file: %s", strerror(errno));
		return;
	}
	fwrite(loaded_sram, 1, GOOMBA_COLOR_SRAM_SIZE, f);
	fclose(f);
	if (path != _filePath) {
		g_free(_filePath);
		_filePath = (char*)g_malloc(strlen(path) + 1);
		strcpy(_filePath, path);
		update_titlebar(GTK_WINDOW(window));
	}
	dirty = false;
}

// Update status of Save and Save As items whenever File menu is opened
static void file_click(GtkWidget* widget, gpointer data) {
	gtk_widget_set_sensitive(save_item, (_filePath != NULL && dirty));
	gtk_widget_set_sensitive(save_as_item, (_filePath != NULL));
}

static void load(const char* path) {
	FILE* f = fopen(path, "rb");
	if (f == NULL) {
		error_msg("Could not open file %s", path);
		return;
	}
	size_t total_bytes_read = 0;
	while (total_bytes_read < GOOMBA_COLOR_SRAM_SIZE) {
		size_t bytes_read = fread(loaded_sram, 1, GOOMBA_COLOR_SRAM_SIZE - total_bytes_read, f);
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
			return;
		}
	} else {
		dirty = false;
	}

	if (_filePath != NULL) free(_filePath);
	_filePath = (char*)malloc(strlen(path) + 1);
	strcpy(_filePath, path);

	update_titlebar(GTK_WINDOW(window));

	header_scan();
}

static void open_click(GtkWidget* widget, gpointer data) {
	GtkWidget* dialog = gtk_file_chooser_dialog_new("Open", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
	gint res = gtk_dialog_run(GTK_DIALOG(dialog));
	if (res != GTK_RESPONSE_ACCEPT) {
		gtk_widget_destroy(dialog);
		return;
	}

	char* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	gtk_widget_destroy(dialog);
	load(path);
	g_free(path);
}

static void save_click(GtkWidget* widget, gpointer data) {
	save(_filePath);
}

static void save_as_click(GtkWidget* widget, gpointer data) {
	GtkWidget* dialog = gtk_file_chooser_dialog_new("Save As", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
	gint res = gtk_dialog_run(GTK_DIALOG(dialog));

	if (res == GTK_RESPONSE_ACCEPT) {
		char* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		save(path);
		g_free(path);
	}
	gtk_widget_destroy(dialog);
}

static void export_click(GtkWidget* widget, gpointer data) {
	GtkTreeIter iter;
	stateheader* sh;
	if (_filePath != NULL && gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), NULL, &iter)) {
		gtk_tree_model_get(GTK_TREE_MODEL(listStore), &iter, 1, &sh, -1);
		goomba_size_t len;
		void* gbcsav = goomba_extract(loaded_sram, sh, &len);
		if (gbcsav == NULL) {
			error_msg("%s", goomba_last_error());
			return;
		}

		GtkWidget* dialog = gtk_file_chooser_dialog_new("Export", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
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
			g_free(path);
		}
		gtk_widget_destroy(dialog);
		free(gbcsav);
	}
}

static void replace_click(GtkWidget* widget, gpointer data) {
	GtkTreeIter iter;
	stateheader* sh;
	if (_filePath != NULL && gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), NULL, &iter)) {
		gtk_tree_model_get(GTK_TREE_MODEL(listStore), &iter, 1, &sh, -1);

		GtkWidget* dialog = gtk_file_chooser_dialog_new("Replace", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
		gint res = gtk_dialog_run(GTK_DIALOG(dialog));
		if (res != GTK_RESPONSE_ACCEPT) {
			gtk_widget_destroy(dialog);
			return;
		}

		char* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		FILE* f = fopen(path, "rb");
		gtk_widget_destroy(dialog);

		fseek(f, 0, SEEK_END);
		size_t len = ftell(f);
		if (len > 65536) len = 65536; // prevent using too much memory on opening big file by accident
		char* gbc_data = (char*)malloc(len);
		fseek(f, 0, SEEK_SET);
		size_t total_bytes_read = 0;
		while (total_bytes_read < len) {
			size_t bytes_read = fread(gbc_data, 1, len - total_bytes_read, f);
			total_bytes_read += bytes_read;
			if (bytes_read <= 0) {
				error_msg("Could only read %lu bytes from %s", (unsigned long)total_bytes_read, path); 
				free(gbc_data);
				g_free(path);
				fclose(f);
				return;
			}
		}

		g_free(path);
		fclose(f);

		// only first X bytes will be used, where X is uncompressed size of current data
		void* new_data = goomba_new_sav(loaded_sram, sh, gbc_data, len);
		free(gbc_data);
		if (new_data == NULL) {
			error_msg(goomba_last_error());
		} else {
			memcpy(loaded_sram, new_data, GOOMBA_COLOR_SRAM_SIZE);
			dirty = true;
			free(new_data);

			header_scan();
		}
	}
}

static void about_click(GtkWidget* widget, gpointer data) {
	gtk_show_about_dialog(GTK_WINDOW(window),
		"copyright", "Copyright (C) 2014 libertyernie",
		"license", GPL_NOTICE,
		"program-name", "Goomba Save Manager",
		"website", "http://github.com/libertyernie/goombasav",
		"comments", "version 2014-05-23",
		NULL);
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

		if (ptr->size > sizeof(stateheader)) {
			uint64_t ck = goomba_compressed_data_checksum(ptr, 3);
			sprintf(buf, "Hash of compressed data: %6X", ck);
			gtk_label_set_text(GTK_LABEL(lblDataHash), buf);

			GdkColor color;
			color.red = 0x101 * ((ck >> 16) & 0xFF);
			color.green = 0x101 * ((ck >> 8) & 0xFF);
			color.blue = 0x101 * (ck & 0xFF);
			gtk_widget_show(dataHashColor);
			gtk_widget_modify_bg(dataHashColor, GTK_STATE_NORMAL, &color);
		} else {
			gtk_label_set_text(GTK_LABEL(lblDataHash), "");
			gtk_widget_hide(dataHashColor);
		}

		gtk_widget_set_sensitive(btnExport, ptr->type == GOOMBA_SRAMSAVE);
		gtk_widget_set_sensitive(btnReplace, ptr->type == GOOMBA_SRAMSAVE);

		if (ptr->type == GOOMBA_CONFIGSAVE) {
			show_configuration_rows();

			configdata* cd = (configdata*)ptr;
			sprintf(buf, "Border: %u", cd->bordercolor);
			gtk_label_set_text(GTK_LABEL(lblBorder), buf);
			sprintf(buf, "Palette: %u", cd->palettebank);
			gtk_label_set_text(GTK_LABEL(lblPalette), buf);
			configdata_misc_strings strs = configdata_get_misc(cd->misc);
			sprintf(buf, "Sleep: %s", strs.sleep);
			gtk_label_set_text(GTK_LABEL(lblSleep), buf);
			sprintf(buf, "Gamma: %s", strs.gamma);
			gtk_label_set_text(GTK_LABEL(lblGamma), buf);
			sprintf(buf, "Autoload state: %s", strs.autoload_state);
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
#pragma endregion

#pragma region window closing
static gboolean delete_event(GtkWidget* widget, GdkEvent* event, gpointer data) {
	if (_filePath != NULL && dirty) {
		GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_NONE,
			"Save changes to %s?", goomba_basename(_filePath));
		gtk_dialog_add_button(GTK_DIALOG(dialog), "Close without Saving", GTK_RESPONSE_NO);
		gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", GTK_RESPONSE_CANCEL);
		gtk_dialog_add_button(GTK_DIALOG(dialog), "Save", GTK_RESPONSE_YES);
		gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_YES);
		gint res = gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		if (res == GTK_RESPONSE_YES) {
			save(_filePath);
		} else if (res == GTK_RESPONSE_CANCEL) {
			return TRUE;
		}
	}
	return FALSE;
}

static void destroy(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

static void exit_click(GtkWidget* widget, gpointer data) {
	if (!delete_event(window, NULL, NULL)) gtk_widget_destroy(window);
}
#pragma endregion

GtkWidget* make_menu_item(const char* label, GtkMenuShell* add_to, GCallback callback) {
	GtkWidget* w = gtk_menu_item_new_with_label(label);
	gtk_menu_shell_append(add_to, w);
	g_signal_connect(w, "activate", callback, NULL);
	return w;
}

GtkWidget* build_menubar() {
	GtkWidget* menubar = gtk_menu_bar_new();

	GtkWidget* file_item = gtk_menu_item_new_with_label("File");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_item);
	g_signal_connect(file_item, "activate", G_CALLBACK(file_click), NULL);
	GtkWidget* file_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);

	GtkWidget* open_item = make_menu_item("Open", GTK_MENU_SHELL(file_menu), G_CALLBACK(open_click));
	save_item = make_menu_item("Save", GTK_MENU_SHELL(file_menu), G_CALLBACK(save_click));
	save_as_item = make_menu_item("Save As...", GTK_MENU_SHELL(file_menu), G_CALLBACK(save_as_click));

	GtkWidget* sep = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), sep);

	GtkWidget* exit_item = make_menu_item("Close", GTK_MENU_SHELL(file_menu), G_CALLBACK(exit_click));

	GtkWidget* help_item = gtk_menu_item_new_with_label("Help");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help_item);
	GtkWidget* help_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_item), help_menu);

	GtkWidget* about_item = make_menu_item("About", GTK_MENU_SHELL(help_menu), G_CALLBACK(about_click));

	gtk_widget_show_all(menubar);
	return menubar;
}

GtkWidget* build_window() {
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);

	g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

	GtkWidget* vbox1 = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox1);

	// construct menubar
	GtkWidget* menubar = build_menubar();
	gtk_box_pack_start(GTK_BOX(vbox1), menubar, FALSE, FALSE, 0);

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

	btnReplace = gtk_button_new_with_label("Replace");
	gtk_widget_set_size_request(btnReplace, 80, -1);
	gtk_box_pack_start(GTK_BOX(button_hbox), btnReplace, FALSE, FALSE, 0);
	gtk_widget_show(btnReplace);
	g_signal_connect(btnReplace, "clicked", G_CALLBACK(replace_click), NULL);
	btnExport = gtk_button_new_with_label("Export");
	gtk_widget_set_size_request(btnExport, 80, -1);
	gtk_box_pack_start(GTK_BOX(button_hbox), btnExport, FALSE, FALSE, 0);
	gtk_widget_show(btnExport);
	g_signal_connect(btnExport, "clicked", G_CALLBACK(export_click), NULL);

	// little hash
	lblDataHash = gtk_label_new("");
	gtk_misc_set_alignment(GTK_MISC(lblDataHash), 0, 0.5);

	dataHashColor = gtk_event_box_new();

	GtkWidget* hash_hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hash_hbox), lblDataHash, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hash_hbox), dataHashColor, TRUE, TRUE, 8);

	gtk_box_pack_end(GTK_BOX(vbox2), hash_hbox, FALSE, FALSE, 0);
	gtk_widget_show_all(hash_hbox);

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

	if (argc >= 2) {
		load(argv[1]);
	}

	gtk_main();

	return 0;
}
