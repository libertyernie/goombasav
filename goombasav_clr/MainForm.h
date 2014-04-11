#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../goombasav.h"
#include "HeaderPtr.h"

namespace goombasav_clr {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	typedef Windows::Forms::DialogResult DR;

	// Defined outside the class so we can pass a pointer to it.
	void show_error_box(const char* c) {
		String^ s = gcnew String(c);
		MessageBox::Show(s);
	}

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();

			loaded_sram = new unsigned char[GOOMBA_COLOR_SRAM_SIZE];
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components) delete components;
			delete[] loaded_sram;
		}

	private: unsigned char* loaded_sram;
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveAsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::Label^  lblType;
	private: System::Windows::Forms::Label^  lblSizeVal;

	private: System::Windows::Forms::Label^  lblTypeVal;

	private: System::Windows::Forms::Label^  lblUncompressedSize;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::Label^  lblUncompressedSizeVal;
	private: System::Windows::Forms::FlowLayoutPanel^  flpStateheader;
	private: System::Windows::Forms::Label^  lblSize;
	private: System::Windows::Forms::Label^  lblFramecount;
	private: System::Windows::Forms::Label^  lblFramecountVal;
	private: System::Windows::Forms::Label^  lblChecksum;
	private: System::Windows::Forms::Label^  lblChecksumVal;

	private: System::Windows::Forms::Label^  lblTitle;
	private: System::Windows::Forms::Label^  lblTitleVal;








	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->lblSize = (gcnew System::Windows::Forms::Label());
			this->lblSizeVal = (gcnew System::Windows::Forms::Label());
			this->lblType = (gcnew System::Windows::Forms::Label());
			this->lblTypeVal = (gcnew System::Windows::Forms::Label());
			this->flpStateheader = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->lblUncompressedSize = (gcnew System::Windows::Forms::Label());
			this->lblUncompressedSizeVal = (gcnew System::Windows::Forms::Label());
			this->lblFramecount = (gcnew System::Windows::Forms::Label());
			this->lblFramecountVal = (gcnew System::Windows::Forms::Label());
			this->lblChecksum = (gcnew System::Windows::Forms::Label());
			this->lblChecksumVal = (gcnew System::Windows::Forms::Label());
			this->lblTitle = (gcnew System::Windows::Forms::Label());
			this->lblTitleVal = (gcnew System::Windows::Forms::Label());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->flowLayoutPanel1->SuspendLayout();
			this->flpStateheader->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// listBox1
			// 
			this->listBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listBox1->FormattingEnabled = true;
			this->listBox1->IntegralHeight = false;
			this->listBox1->ItemHeight = 16;
			this->listBox1->Location = System::Drawing::Point(0, 0);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(160, 225);
			this->listBox1->TabIndex = 0;
			this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::listBox1_SelectedIndexChanged);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 28);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->listBox1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->flowLayoutPanel1);
			this->splitContainer1->Size = System::Drawing::Size(482, 225);
			this->splitContainer1->SplitterDistance = 160;
			this->splitContainer1->TabIndex = 1;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Controls->Add(this->lblSize);
			this->flowLayoutPanel1->Controls->Add(this->lblSizeVal);
			this->flowLayoutPanel1->Controls->Add(this->lblType);
			this->flowLayoutPanel1->Controls->Add(this->lblTypeVal);
			this->flowLayoutPanel1->Controls->Add(this->flpStateheader);
			this->flowLayoutPanel1->Controls->Add(this->lblTitle);
			this->flowLayoutPanel1->Controls->Add(this->lblTitleVal);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(318, 225);
			this->flowLayoutPanel1->TabIndex = 5;
			// 
			// lblSize
			// 
			this->lblSize->Location = System::Drawing::Point(0, 0);
			this->lblSize->Margin = System::Windows::Forms::Padding(0);
			this->lblSize->Name = L"lblSize";
			this->lblSize->Size = System::Drawing::Size(45, 23);
			this->lblSize->TabIndex = 0;
			this->lblSize->Text = L"Size:";
			// 
			// lblSizeVal
			// 
			this->lblSizeVal->Location = System::Drawing::Point(45, 0);
			this->lblSizeVal->Margin = System::Windows::Forms::Padding(0);
			this->lblSizeVal->Name = L"lblSizeVal";
			this->lblSizeVal->Size = System::Drawing::Size(100, 23);
			this->lblSizeVal->TabIndex = 1;
			// 
			// lblType
			// 
			this->lblType->Location = System::Drawing::Point(145, 0);
			this->lblType->Margin = System::Windows::Forms::Padding(0);
			this->lblType->Name = L"lblType";
			this->lblType->Size = System::Drawing::Size(50, 23);
			this->lblType->TabIndex = 2;
			this->lblType->Text = L"Type:";
			// 
			// lblTypeVal
			// 
			this->flowLayoutPanel1->SetFlowBreak(this->lblTypeVal, true);
			this->lblTypeVal->Location = System::Drawing::Point(195, 0);
			this->lblTypeVal->Margin = System::Windows::Forms::Padding(0);
			this->lblTypeVal->Name = L"lblTypeVal";
			this->lblTypeVal->Size = System::Drawing::Size(100, 23);
			this->lblTypeVal->TabIndex = 3;
			// 
			// flpStateheader
			// 
			this->flpStateheader->Controls->Add(this->lblUncompressedSize);
			this->flpStateheader->Controls->Add(this->lblUncompressedSizeVal);
			this->flpStateheader->Controls->Add(this->lblFramecount);
			this->flpStateheader->Controls->Add(this->lblFramecountVal);
			this->flpStateheader->Controls->Add(this->lblChecksum);
			this->flpStateheader->Controls->Add(this->lblChecksumVal);
			this->flowLayoutPanel1->SetFlowBreak(this->flpStateheader, true);
			this->flpStateheader->Location = System::Drawing::Point(0, 23);
			this->flpStateheader->Margin = System::Windows::Forms::Padding(0);
			this->flpStateheader->Name = L"flpStateheader";
			this->flpStateheader->Size = System::Drawing::Size(265, 69);
			this->flpStateheader->TabIndex = 5;
			// 
			// lblUncompressedSize
			// 
			this->lblUncompressedSize->Location = System::Drawing::Point(0, 0);
			this->lblUncompressedSize->Margin = System::Windows::Forms::Padding(0);
			this->lblUncompressedSize->Name = L"lblUncompressedSize";
			this->lblUncompressedSize->Size = System::Drawing::Size(140, 23);
			this->lblUncompressedSize->TabIndex = 0;
			this->lblUncompressedSize->Text = L"Uncompressed size:";
			// 
			// lblUncompressedSizeVal
			// 
			this->flpStateheader->SetFlowBreak(this->lblUncompressedSizeVal, true);
			this->lblUncompressedSizeVal->Location = System::Drawing::Point(140, 0);
			this->lblUncompressedSizeVal->Margin = System::Windows::Forms::Padding(0);
			this->lblUncompressedSizeVal->Name = L"lblUncompressedSizeVal";
			this->lblUncompressedSizeVal->Size = System::Drawing::Size(120, 23);
			this->lblUncompressedSizeVal->TabIndex = 4;
			// 
			// lblFramecount
			// 
			this->lblFramecount->Location = System::Drawing::Point(0, 23);
			this->lblFramecount->Margin = System::Windows::Forms::Padding(0);
			this->lblFramecount->Name = L"lblFramecount";
			this->lblFramecount->Size = System::Drawing::Size(95, 23);
			this->lblFramecount->TabIndex = 5;
			this->lblFramecount->Text = L"Frame count:";
			// 
			// lblFramecountVal
			// 
			this->flpStateheader->SetFlowBreak(this->lblFramecountVal, true);
			this->lblFramecountVal->Location = System::Drawing::Point(95, 23);
			this->lblFramecountVal->Margin = System::Windows::Forms::Padding(0);
			this->lblFramecountVal->Name = L"lblFramecountVal";
			this->lblFramecountVal->Size = System::Drawing::Size(165, 23);
			this->lblFramecountVal->TabIndex = 6;
			// 
			// lblChecksum
			// 
			this->lblChecksum->Location = System::Drawing::Point(0, 46);
			this->lblChecksum->Margin = System::Windows::Forms::Padding(0);
			this->lblChecksum->Name = L"lblChecksum";
			this->lblChecksum->Size = System::Drawing::Size(120, 23);
			this->lblChecksum->TabIndex = 7;
			this->lblChecksum->Text = L"ROM checksum:";
			// 
			// lblChecksumVal
			// 
			this->lblChecksumVal->Location = System::Drawing::Point(120, 46);
			this->lblChecksumVal->Margin = System::Windows::Forms::Padding(0);
			this->lblChecksumVal->Name = L"lblChecksumVal";
			this->lblChecksumVal->Size = System::Drawing::Size(140, 23);
			this->lblChecksumVal->TabIndex = 8;
			// 
			// lblTitle
			// 
			this->lblTitle->Location = System::Drawing::Point(0, 92);
			this->lblTitle->Margin = System::Windows::Forms::Padding(0);
			this->lblTitle->Name = L"lblTitle";
			this->lblTitle->Size = System::Drawing::Size(45, 23);
			this->lblTitle->TabIndex = 6;
			this->lblTitle->Text = L"Title:";
			// 
			// lblTitleVal
			// 
			this->lblTitleVal->Location = System::Drawing::Point(48, 92);
			this->lblTitleVal->Name = L"lblTitleVal";
			this->lblTitleVal->Size = System::Drawing::Size(247, 23);
			this->lblTitleVal->TabIndex = 7;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(482, 28);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->openToolStripMenuItem,
					this->saveToolStripMenuItem, this->saveAsToolStripMenuItem, this->toolStripMenuItem1, this->exitToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(44, 24);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(138, 24);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(138, 24);
			this->saveToolStripMenuItem->Text = L"Save";
			// 
			// saveAsToolStripMenuItem
			// 
			this->saveAsToolStripMenuItem->Name = L"saveAsToolStripMenuItem";
			this->saveAsToolStripMenuItem->Size = System::Drawing::Size(138, 24);
			this->saveAsToolStripMenuItem->Text = L"Save As...";
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(135, 6);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(138, 24);
			this->exitToolStripMenuItem->Text = L"Exit";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(482, 253);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->flowLayoutPanel1->ResumeLayout(false);
			this->flpStateheader->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		void load(String^ filename) {
			array<unsigned char>^ arr = System::IO::File::ReadAllBytes(filename);
			if (arr->Length != GOOMBA_COLOR_SRAM_SIZE) {
				MessageBox::Show("Incorrect file size - must be " + GOOMBA_COLOR_SRAM_SIZE + " bytes.");
				return;
			}
			pin_ptr<unsigned char> pin = &arr[0];
			char* cleaned = goomba_cleanup(pin);
			if (cleaned != NULL) {
				memcpy(loaded_sram, cleaned, GOOMBA_COLOR_SRAM_SIZE);
			}

			stateheader** headers = stateheader_scan(loaded_sram);
			if (headers != NULL) {
				listBox1->Items->Clear();
				for (int i = 0; headers[i] != NULL; i++) {
					listBox1->Items->Add(HeaderPtr::FromPtr(headers[i]));
				}
				free(headers);
			}
		}

		Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			OpenFileDialog d;
			if (d.ShowDialog() == Windows::Forms::DialogResult::OK) {
				load(d.FileName);
			}
		}
	
		Void listBox1_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
			HeaderPtr^ p = (HeaderPtr^)listBox1->SelectedItem;
			stateheader* sh = p->sh_ptr();
			lblSizeVal->Text = sh->size.ToString() + " bytes";
			lblTypeVal->Text = sh->type == GOOMBA_STATESAVE ? "Savestate"
				: sh->type == GOOMBA_SRAMSAVE ? "SRAM"
				: sh->type == GOOMBA_CONFIGSAVE ? "Settings"
				: "Unknown";
			lblUncompressedSize->Text = (sh->uncompressed_size >= sh->size) ? "Uncompressed size:" : "Compressed size:";
			lblUncompressedSizeVal->Text = sh->uncompressed_size.ToString() + " bytes";
			lblFramecountVal->Text = sh->framecount.ToString();
			lblChecksumVal->Text = sh->checksum.ToString("X8");
			lblTitleVal->Text = gcnew String(sh->title);
		}
};
}
