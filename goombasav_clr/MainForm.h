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

	char *const sleeptxt[] = { "5min", "10min", "30min", "OFF" };
	char *const brightxt[] = { "I", "II", "III", "IIII", "IIIII" };

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
	private: System::Windows::Forms::FlowLayoutPanel^  flpConfigdata;
	private: System::Windows::Forms::Label^  lblBorder;
	private: System::Windows::Forms::Label^  lblBorderVal;

	private: System::Windows::Forms::Label^  lblPalette;
	private: System::Windows::Forms::Label^  lblPaletteVal;
	private: System::Windows::Forms::Label^  lblSleep;
	private: System::Windows::Forms::Label^  lblSleepVal;
	private: System::Windows::Forms::Label^  lblAutostate;
	private: System::Windows::Forms::Label^  lblAutostateVal;
	private: System::Windows::Forms::Label^  lblGamma;
	private: System::Windows::Forms::Label^  lblGammaVal;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;


	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


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
			this->flpConfigdata = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->lblBorder = (gcnew System::Windows::Forms::Label());
			this->lblBorderVal = (gcnew System::Windows::Forms::Label());
			this->lblPalette = (gcnew System::Windows::Forms::Label());
			this->lblPaletteVal = (gcnew System::Windows::Forms::Label());
			this->lblSleep = (gcnew System::Windows::Forms::Label());
			this->lblSleepVal = (gcnew System::Windows::Forms::Label());
			this->lblGamma = (gcnew System::Windows::Forms::Label());
			this->lblGammaVal = (gcnew System::Windows::Forms::Label());
			this->lblAutostate = (gcnew System::Windows::Forms::Label());
			this->lblAutostateVal = (gcnew System::Windows::Forms::Label());
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
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->flowLayoutPanel1->SuspendLayout();
			this->flpConfigdata->SuspendLayout();
			this->flpStateheader->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
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
			this->listBox1->Size = System::Drawing::Size(130, 201);
			this->listBox1->TabIndex = 0;
			this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::listBox1_SelectedIndexChanged);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
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
			this->splitContainer1->Size = System::Drawing::Size(507, 201);
			this->splitContainer1->SplitterDistance = 130;
			this->splitContainer1->TabIndex = 1;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Controls->Add(this->lblSize);
			this->flowLayoutPanel1->Controls->Add(this->lblSizeVal);
			this->flowLayoutPanel1->Controls->Add(this->lblType);
			this->flowLayoutPanel1->Controls->Add(this->lblTypeVal);
			this->flowLayoutPanel1->Controls->Add(this->flpConfigdata);
			this->flowLayoutPanel1->Controls->Add(this->flpStateheader);
			this->flowLayoutPanel1->Controls->Add(this->lblChecksum);
			this->flowLayoutPanel1->Controls->Add(this->lblChecksumVal);
			this->flowLayoutPanel1->Controls->Add(this->lblTitle);
			this->flowLayoutPanel1->Controls->Add(this->lblTitleVal);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(373, 201);
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
			// flpConfigdata
			// 
			this->flpConfigdata->AutoSize = true;
			this->flpConfigdata->Controls->Add(this->lblBorder);
			this->flpConfigdata->Controls->Add(this->lblBorderVal);
			this->flpConfigdata->Controls->Add(this->lblPalette);
			this->flpConfigdata->Controls->Add(this->lblPaletteVal);
			this->flpConfigdata->Controls->Add(this->lblSleep);
			this->flpConfigdata->Controls->Add(this->lblSleepVal);
			this->flpConfigdata->Controls->Add(this->lblGamma);
			this->flpConfigdata->Controls->Add(this->lblGammaVal);
			this->flpConfigdata->Controls->Add(this->lblAutostate);
			this->flpConfigdata->Controls->Add(this->lblAutostateVal);
			this->flpConfigdata->Location = System::Drawing::Point(0, 23);
			this->flpConfigdata->Margin = System::Windows::Forms::Padding(0);
			this->flpConfigdata->Name = L"flpConfigdata";
			this->flpConfigdata->Size = System::Drawing::Size(280, 47);
			this->flpConfigdata->TabIndex = 8;
			this->flpConfigdata->Visible = false;
			// 
			// lblBorder
			// 
			this->lblBorder->Location = System::Drawing::Point(0, 0);
			this->lblBorder->Margin = System::Windows::Forms::Padding(0);
			this->lblBorder->Name = L"lblBorder";
			this->lblBorder->Size = System::Drawing::Size(60, 23);
			this->lblBorder->TabIndex = 0;
			this->lblBorder->Text = L"Border:";
			// 
			// lblBorderVal
			// 
			this->lblBorderVal->Location = System::Drawing::Point(60, 0);
			this->lblBorderVal->Margin = System::Windows::Forms::Padding(0);
			this->lblBorderVal->Name = L"lblBorderVal";
			this->lblBorderVal->Size = System::Drawing::Size(30, 23);
			this->lblBorderVal->TabIndex = 1;
			this->lblBorderVal->Text = L"88";
			// 
			// lblPalette
			// 
			this->lblPalette->Location = System::Drawing::Point(90, 0);
			this->lblPalette->Margin = System::Windows::Forms::Padding(0);
			this->lblPalette->Name = L"lblPalette";
			this->lblPalette->Size = System::Drawing::Size(60, 23);
			this->lblPalette->TabIndex = 2;
			this->lblPalette->Text = L"Palette:";
			// 
			// lblPaletteVal
			// 
			this->lblPaletteVal->Location = System::Drawing::Point(150, 0);
			this->lblPaletteVal->Margin = System::Windows::Forms::Padding(0);
			this->lblPaletteVal->Name = L"lblPaletteVal";
			this->lblPaletteVal->Size = System::Drawing::Size(30, 23);
			this->lblPaletteVal->TabIndex = 3;
			this->lblPaletteVal->Text = L"88";
			// 
			// lblSleep
			// 
			this->lblSleep->Location = System::Drawing::Point(180, 0);
			this->lblSleep->Margin = System::Windows::Forms::Padding(0);
			this->lblSleep->Name = L"lblSleep";
			this->lblSleep->Size = System::Drawing::Size(50, 23);
			this->lblSleep->TabIndex = 9;
			this->lblSleep->Text = L"Sleep:";
			// 
			// lblSleepVal
			// 
			this->flpConfigdata->SetFlowBreak(this->lblSleepVal, true);
			this->lblSleepVal->Location = System::Drawing::Point(230, 0);
			this->lblSleepVal->Margin = System::Windows::Forms::Padding(0);
			this->lblSleepVal->Name = L"lblSleepVal";
			this->lblSleepVal->Size = System::Drawing::Size(50, 23);
			this->lblSleepVal->TabIndex = 10;
			// 
			// lblGamma
			// 
			this->lblGamma->Location = System::Drawing::Point(0, 23);
			this->lblGamma->Margin = System::Windows::Forms::Padding(0);
			this->lblGamma->Name = L"lblGamma";
			this->lblGamma->Size = System::Drawing::Size(65, 24);
			this->lblGamma->TabIndex = 13;
			this->lblGamma->Text = L"Gamma:";
			// 
			// lblGammaVal
			// 
			this->lblGammaVal->Location = System::Drawing::Point(65, 23);
			this->lblGammaVal->Margin = System::Windows::Forms::Padding(0);
			this->lblGammaVal->Name = L"lblGammaVal";
			this->lblGammaVal->Size = System::Drawing::Size(60, 23);
			this->lblGammaVal->TabIndex = 14;
			// 
			// lblAutostate
			// 
			this->lblAutostate->Location = System::Drawing::Point(125, 23);
			this->lblAutostate->Margin = System::Windows::Forms::Padding(0);
			this->lblAutostate->Name = L"lblAutostate";
			this->lblAutostate->Size = System::Drawing::Size(105, 23);
			this->lblAutostate->TabIndex = 11;
			this->lblAutostate->Text = L"Autoload state:";
			// 
			// lblAutostateVal
			// 
			this->flpConfigdata->SetFlowBreak(this->lblAutostateVal, true);
			this->lblAutostateVal->Location = System::Drawing::Point(230, 23);
			this->lblAutostateVal->Margin = System::Windows::Forms::Padding(0);
			this->lblAutostateVal->Name = L"lblAutostateVal";
			this->lblAutostateVal->Size = System::Drawing::Size(40, 23);
			this->lblAutostateVal->TabIndex = 12;
			// 
			// flpStateheader
			// 
			this->flpStateheader->Controls->Add(this->lblUncompressedSize);
			this->flpStateheader->Controls->Add(this->lblUncompressedSizeVal);
			this->flpStateheader->Controls->Add(this->lblFramecount);
			this->flpStateheader->Controls->Add(this->lblFramecountVal);
			this->flowLayoutPanel1->SetFlowBreak(this->flpStateheader, true);
			this->flpStateheader->Location = System::Drawing::Point(0, 70);
			this->flpStateheader->Margin = System::Windows::Forms::Padding(0);
			this->flpStateheader->Name = L"flpStateheader";
			this->flpStateheader->Size = System::Drawing::Size(295, 46);
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
			this->lblChecksum->Location = System::Drawing::Point(0, 116);
			this->lblChecksum->Margin = System::Windows::Forms::Padding(0);
			this->lblChecksum->Name = L"lblChecksum";
			this->lblChecksum->Size = System::Drawing::Size(120, 23);
			this->lblChecksum->TabIndex = 7;
			this->lblChecksum->Text = L"ROM checksum:";
			// 
			// lblChecksumVal
			// 
			this->flowLayoutPanel1->SetFlowBreak(this->lblChecksumVal, true);
			this->lblChecksumVal->Location = System::Drawing::Point(120, 116);
			this->lblChecksumVal->Margin = System::Windows::Forms::Padding(0);
			this->lblChecksumVal->Name = L"lblChecksumVal";
			this->lblChecksumVal->Size = System::Drawing::Size(140, 23);
			this->lblChecksumVal->TabIndex = 8;
			// 
			// lblTitle
			// 
			this->lblTitle->Location = System::Drawing::Point(0, 139);
			this->lblTitle->Margin = System::Windows::Forms::Padding(0);
			this->lblTitle->Name = L"lblTitle";
			this->lblTitle->Size = System::Drawing::Size(45, 23);
			this->lblTitle->TabIndex = 6;
			this->lblTitle->Text = L"Title:";
			// 
			// lblTitleVal
			// 
			this->lblTitleVal->Location = System::Drawing::Point(45, 139);
			this->lblTitleVal->Margin = System::Windows::Forms::Padding(0);
			this->lblTitleVal->Name = L"lblTitleVal";
			this->lblTitleVal->Size = System::Drawing::Size(247, 23);
			this->lblTitleVal->TabIndex = 7;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(507, 28);
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
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripStatusLabel1 });
			this->statusStrip1->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::Flow;
			this->statusStrip1->Location = System::Drawing::Point(0, 229);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(507, 24);
			this->statusStrip1->TabIndex = 9;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(0, 0);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(507, 253);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainForm";
			this->Text = L"Goomba Save Manager";
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->flowLayoutPanel1->ResumeLayout(false);
			this->flowLayoutPanel1->PerformLayout();
			this->flpConfigdata->ResumeLayout(false);
			this->flpStateheader->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
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
				if (cleaned != (void*)pin) {
					toolStripStatusLabel1->Text = "Cleaned 0xE000-0xFFFF. To skip this step, run with --no-clean.";
				} else {
					statusStrip1->Text = "SRAM is clean - no data in 0xE000-0xFFFF range";
				}
				memcpy(loaded_sram, cleaned, GOOMBA_COLOR_SRAM_SIZE);
			}

			stateheader** headers = stateheader_scan(loaded_sram);
			if (headers != NULL) {
				listBox1->Items->Clear();
				for (int i = 0; headers[i] != NULL; i++) {
					listBox1->Items->Add(gcnew HeaderPtr(headers[i]));
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
			if (sh->type == GOOMBA_CONFIGSAVE) {
				flpConfigdata->Visible = true;
				flpStateheader->Visible = false;

				configdata* cd = p->cd_ptr();
				lblBorderVal->Text = cd->bordercolor.ToString();
				lblPaletteVal->Text = cd->palettebank.ToString();
				lblSleepVal->Text = gcnew String(sleeptxt[cd->misc & 0x3]);
				lblAutostateVal->Text = ((cd->misc & 0x10) >> 4) ? "ON" : "OFF";
				lblGammaVal->Text = gcnew String(brightxt[(cd->misc & 0xE0) >> 5]);
				lblChecksumVal->Text = cd->sram_checksum.ToString("X8");
			} else {
				flpConfigdata->Visible = false;
				flpStateheader->Visible = true;
				lblUncompressedSize->Text = (sh->uncompressed_size >= sh->size) ? "Uncompressed size:" : "Compressed size:";
				lblUncompressedSizeVal->Text = sh->uncompressed_size.ToString() + " bytes";
				lblFramecountVal->Text = sh->framecount.ToString();
				lblChecksumVal->Text = sh->checksum.ToString("X8");
			}
			lblTitleVal->Text = gcnew String(sh->title);
		}
};
}
