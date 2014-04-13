#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <msclr\marshal.h>
#include "../goombasav.h"
#include "HeaderPtr.h"
#include "AboutForm.h"

namespace goombasav_clr {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	using msclr::interop::marshal_context;
	typedef Windows::Forms::DialogResult DR;

	const char* TITLE = "Goomba Save Manager";
	const char* const sleeptxt[] = { "5min", "10min", "30min", "OFF" };
	const char* const brightxt[] = { "I", "II", "III", "IIII", "IIIII" };

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{

	private:
		unsigned char* loaded_sram;
		String^ _filePath;
		 bool dirty;

		// Update status of Save and Save As items whenever File menu is opened
		void OnFileDropDownOpening(Object ^sender, EventArgs ^e) {
			saveToolStripMenuItem->Enabled = (_filePath != nullptr && dirty);
			saveAsToolStripMenuItem->Enabled = (_filePath != nullptr);
		}

		void OnClosing(Object ^sender, CancelEventArgs ^e) {
			if (_filePath != nullptr && dirty) {
				DR dr = MessageBox::Show("Save changes to " + Path::GetFileName(_filePath) + "?",
					gcnew String(TITLE), MessageBoxButtons::YesNoCancel);
				if (dr == DR::Yes) {
					save(_filePath);
				} else if (dr == DR::Cancel) {
					e->Cancel = true;
				}
			}
		}

		void OnDragEnter(Object ^sender, DragEventArgs ^e) {
			e->Effect = DragDropEffects::None;
			if (e->Data->GetDataPresent(DataFormats::FileDrop)) {
				array<String^>^ arr = (array<String^>^)e->Data->GetData(DataFormats::FileDrop);
				if (arr->Length == 1) {
					String^ pathname = arr[0];
					if (pathname->EndsWith(".sav", StringComparison::InvariantCultureIgnoreCase)) {
						FileInfo fi(pathname);
						if (fi.Length >= 64 * 1024) {
							e->Effect = DragDropEffects::Link;
						} else if (fi.Length <= 32*1024 && _filePath != nullptr) {
							e->Effect = DragDropEffects::Copy;
						}
					}
				}
			}
		}


		void OnDragDrop(Object ^sender, DragEventArgs ^e) {
			if (e->Effect == DragDropEffects::Link) {
				// try open file
				array<String^>^ arr = (array<String^>^)e->Data->GetData(DataFormats::FileDrop);
				load(arr[0]);
			} else if (e->Effect == DragDropEffects::Copy) {
				// try replace
				array<String^>^ arr = (array<String^>^)e->Data->GetData(DataFormats::FileDrop);
				replace(arr[0]);
			}
		}

	public:
		MainForm(void) {
			InitializeComponent();

			loaded_sram = new unsigned char[GOOMBA_COLOR_SRAM_SIZE];
			_filePath = nullptr;

			fileToolStripMenuItem->DropDownOpening += gcnew System::EventHandler(this, &goombasav_clr::MainForm::OnFileDropDownOpening);
			this->Closing += gcnew System::ComponentModel::CancelEventHandler(this, &goombasav_clr::MainForm::OnClosing);
			splitContainer1->Panel2->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &goombasav_clr::MainForm::OnDragEnter);
			splitContainer1->Panel2->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &goombasav_clr::MainForm::OnDragDrop);
			splitContainer1->Panel2->AllowDrop = true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm() {
			if (components) delete components;
			delete[] loaded_sram;
		}

#pragma region Windows Form Designer generated code
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
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

	private: System::Windows::Forms::Button^  btnExtract;
	private: System::Windows::Forms::Button^  btnReplace;
	private: System::Windows::Forms::FlowLayoutPanel^  flpButtons;

	private: System::ComponentModel::IContainer^  components;
	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	private: void InitializeComponent(void)
		{
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
				 this->listBox1 = (gcnew System::Windows::Forms::ListBox());
				 this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
				 this->flpButtons = (gcnew System::Windows::Forms::FlowLayoutPanel());
				 this->btnReplace = (gcnew System::Windows::Forms::Button());
				 this->btnExtract = (gcnew System::Windows::Forms::Button());
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
				 this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
				 this->splitContainer1->Panel1->SuspendLayout();
				 this->splitContainer1->Panel2->SuspendLayout();
				 this->splitContainer1->SuspendLayout();
				 this->flpButtons->SuspendLayout();
				 this->flowLayoutPanel1->SuspendLayout();
				 this->flpConfigdata->SuspendLayout();
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
				 this->listBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
				 this->listBox1->Name = L"listBox1";
				 this->listBox1->Size = System::Drawing::Size(130, 226);
				 this->listBox1->TabIndex = 0;
				 this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::listBox1_SelectedIndexChanged);
				 // 
				 // splitContainer1
				 // 
				 this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
				 this->splitContainer1->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
				 this->splitContainer1->Location = System::Drawing::Point(0, 28);
				 this->splitContainer1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
				 this->splitContainer1->Name = L"splitContainer1";
				 // 
				 // splitContainer1.Panel1
				 // 
				 this->splitContainer1->Panel1->Controls->Add(this->listBox1);
				 // 
				 // splitContainer1.Panel2
				 // 
				 this->splitContainer1->Panel2->Controls->Add(this->flpButtons);
				 this->splitContainer1->Panel2->Controls->Add(this->flowLayoutPanel1);
				 this->splitContainer1->Size = System::Drawing::Size(507, 226);
				 this->splitContainer1->SplitterDistance = 130;
				 this->splitContainer1->TabIndex = 1;
				 // 
				 // flpButtons
				 // 
				 this->flpButtons->AutoSize = true;
				 this->flpButtons->Controls->Add(this->btnReplace);
				 this->flpButtons->Controls->Add(this->btnExtract);
				 this->flpButtons->Dock = System::Windows::Forms::DockStyle::Bottom;
				 this->flpButtons->Location = System::Drawing::Point(0, 190);
				 this->flpButtons->Margin = System::Windows::Forms::Padding(4);
				 this->flpButtons->Name = L"flpButtons";
				 this->flpButtons->Size = System::Drawing::Size(373, 36);
				 this->flpButtons->TabIndex = 9;
				 // 
				 // btnReplace
				 // 
				 this->btnReplace->Enabled = false;
				 this->btnReplace->Location = System::Drawing::Point(4, 4);
				 this->btnReplace->Margin = System::Windows::Forms::Padding(4);
				 this->btnReplace->Name = L"btnReplace";
				 this->btnReplace->Size = System::Drawing::Size(100, 28);
				 this->btnReplace->TabIndex = 0;
				 this->btnReplace->Text = L"Replace";
				 this->btnReplace->UseVisualStyleBackColor = true;
				 this->btnReplace->Click += gcnew System::EventHandler(this, &MainForm::btnReplace_Click);
				 // 
				 // btnExtract
				 // 
				 this->btnExtract->Enabled = false;
				 this->btnExtract->Location = System::Drawing::Point(112, 4);
				 this->btnExtract->Margin = System::Windows::Forms::Padding(4);
				 this->btnExtract->Name = L"btnExtract";
				 this->btnExtract->Size = System::Drawing::Size(100, 28);
				 this->btnExtract->TabIndex = 1;
				 this->btnExtract->Text = L"Export";
				 this->btnExtract->UseVisualStyleBackColor = true;
				 this->btnExtract->Click += gcnew System::EventHandler(this, &MainForm::btnExtract_Click);
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
				 this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
				 this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
				 this->flowLayoutPanel1->Size = System::Drawing::Size(373, 226);
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
				 this->lblType->Size = System::Drawing::Size(51, 23);
				 this->lblType->TabIndex = 2;
				 this->lblType->Text = L"Type:";
				 // 
				 // lblTypeVal
				 // 
				 this->flowLayoutPanel1->SetFlowBreak(this->lblTypeVal, true);
				 this->lblTypeVal->Location = System::Drawing::Point(196, 0);
				 this->lblTypeVal->Margin = System::Windows::Forms::Padding(0);
				 this->lblTypeVal->Name = L"lblTypeVal";
				 this->lblTypeVal->Size = System::Drawing::Size(100, 23);
				 this->lblTypeVal->TabIndex = 3;
				 // 
				 // flpConfigdata
				 // 
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
				 this->flpConfigdata->Size = System::Drawing::Size(280, 48);
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
				 this->lblBorderVal->Size = System::Drawing::Size(29, 23);
				 this->lblBorderVal->TabIndex = 1;
				 this->lblBorderVal->Text = L"88";
				 // 
				 // lblPalette
				 // 
				 this->lblPalette->Location = System::Drawing::Point(89, 0);
				 this->lblPalette->Margin = System::Windows::Forms::Padding(0);
				 this->lblPalette->Name = L"lblPalette";
				 this->lblPalette->Size = System::Drawing::Size(60, 23);
				 this->lblPalette->TabIndex = 2;
				 this->lblPalette->Text = L"Palette:";
				 // 
				 // lblPaletteVal
				 // 
				 this->lblPaletteVal->Location = System::Drawing::Point(149, 0);
				 this->lblPaletteVal->Margin = System::Windows::Forms::Padding(0);
				 this->lblPaletteVal->Name = L"lblPaletteVal";
				 this->lblPaletteVal->Size = System::Drawing::Size(29, 23);
				 this->lblPaletteVal->TabIndex = 3;
				 this->lblPaletteVal->Text = L"88";
				 // 
				 // lblSleep
				 // 
				 this->lblSleep->Location = System::Drawing::Point(178, 0);
				 this->lblSleep->Margin = System::Windows::Forms::Padding(0);
				 this->lblSleep->Name = L"lblSleep";
				 this->lblSleep->Size = System::Drawing::Size(51, 23);
				 this->lblSleep->TabIndex = 9;
				 this->lblSleep->Text = L"Sleep:";
				 // 
				 // lblSleepVal
				 // 
				 this->flpConfigdata->SetFlowBreak(this->lblSleepVal, true);
				 this->lblSleepVal->Location = System::Drawing::Point(229, 0);
				 this->lblSleepVal->Margin = System::Windows::Forms::Padding(0);
				 this->lblSleepVal->Name = L"lblSleepVal";
				 this->lblSleepVal->Size = System::Drawing::Size(51, 23);
				 this->lblSleepVal->TabIndex = 10;
				 // 
				 // lblGamma
				 // 
				 this->lblGamma->Location = System::Drawing::Point(0, 23);
				 this->lblGamma->Margin = System::Windows::Forms::Padding(0);
				 this->lblGamma->Name = L"lblGamma";
				 this->lblGamma->Size = System::Drawing::Size(65, 25);
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
				 this->flpStateheader->Location = System::Drawing::Point(0, 71);
				 this->flpStateheader->Margin = System::Windows::Forms::Padding(0);
				 this->flpStateheader->Name = L"flpStateheader";
				 this->flpStateheader->Size = System::Drawing::Size(295, 48);
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
				 this->lblChecksum->Location = System::Drawing::Point(0, 119);
				 this->lblChecksum->Margin = System::Windows::Forms::Padding(0);
				 this->lblChecksum->Name = L"lblChecksum";
				 this->lblChecksum->Size = System::Drawing::Size(120, 23);
				 this->lblChecksum->TabIndex = 7;
				 this->lblChecksum->Text = L"ROM checksum:";
				 // 
				 // lblChecksumVal
				 // 
				 this->flowLayoutPanel1->SetFlowBreak(this->lblChecksumVal, true);
				 this->lblChecksumVal->Location = System::Drawing::Point(120, 119);
				 this->lblChecksumVal->Margin = System::Windows::Forms::Padding(0);
				 this->lblChecksumVal->Name = L"lblChecksumVal";
				 this->lblChecksumVal->Size = System::Drawing::Size(140, 23);
				 this->lblChecksumVal->TabIndex = 8;
				 // 
				 // lblTitle
				 // 
				 this->lblTitle->Location = System::Drawing::Point(0, 142);
				 this->lblTitle->Margin = System::Windows::Forms::Padding(0);
				 this->lblTitle->Name = L"lblTitle";
				 this->lblTitle->Size = System::Drawing::Size(45, 23);
				 this->lblTitle->TabIndex = 6;
				 this->lblTitle->Text = L"Title:";
				 // 
				 // lblTitleVal
				 // 
				 this->lblTitleVal->Location = System::Drawing::Point(45, 142);
				 this->lblTitleVal->Margin = System::Windows::Forms::Padding(0);
				 this->lblTitleVal->Name = L"lblTitleVal";
				 this->lblTitleVal->Size = System::Drawing::Size(247, 23);
				 this->lblTitleVal->TabIndex = 7;
				 // 
				 // menuStrip1
				 // 
				 this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
					 this->fileToolStripMenuItem,
						 this->helpToolStripMenuItem
				 });
				 this->menuStrip1->Location = System::Drawing::Point(0, 0);
				 this->menuStrip1->Name = L"menuStrip1";
				 this->menuStrip1->Padding = System::Windows::Forms::Padding(5, 2, 0, 2);
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
				 this->saveToolStripMenuItem->Enabled = false;
				 this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
				 this->saveToolStripMenuItem->Size = System::Drawing::Size(138, 24);
				 this->saveToolStripMenuItem->Text = L"Save";
				 this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveToolStripMenuItem_Click);
				 // 
				 // saveAsToolStripMenuItem
				 // 
				 this->saveAsToolStripMenuItem->Enabled = false;
				 this->saveAsToolStripMenuItem->Name = L"saveAsToolStripMenuItem";
				 this->saveAsToolStripMenuItem->Size = System::Drawing::Size(138, 24);
				 this->saveAsToolStripMenuItem->Text = L"Save As...";
				 this->saveAsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveAsToolStripMenuItem_Click);
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
				 this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::exitToolStripMenuItem_Click);
				 // 
				 // helpToolStripMenuItem
				 // 
				 this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->aboutToolStripMenuItem });
				 this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
				 this->helpToolStripMenuItem->Size = System::Drawing::Size(53, 24);
				 this->helpToolStripMenuItem->Text = L"Help";
				 // 
				 // aboutToolStripMenuItem
				 // 
				 this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
				 this->aboutToolStripMenuItem->Size = System::Drawing::Size(119, 24);
				 this->aboutToolStripMenuItem->Text = L"About";
				 this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::aboutToolStripMenuItem_Click);
				 // 
				 // MainForm
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(507, 254);
				 this->Controls->Add(this->splitContainer1);
				 this->Controls->Add(this->menuStrip1);
				 this->MainMenuStrip = this->menuStrip1;
				 this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
				 this->Name = L"MainForm";
				 this->Text = L"Goomba Save Manager";
				 this->splitContainer1->Panel1->ResumeLayout(false);
				 this->splitContainer1->Panel2->ResumeLayout(false);
				 this->splitContainer1->Panel2->PerformLayout();
				 (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
				 this->splitContainer1->ResumeLayout(false);
				 this->flpButtons->ResumeLayout(false);
				 this->flowLayoutPanel1->ResumeLayout(false);
				 this->flpConfigdata->ResumeLayout(false);
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
			if (arr->Length < GOOMBA_COLOR_SRAM_SIZE) {
				MessageBox::Show("This file has an incorrect size. Valid Goomba save files must be at least " + GOOMBA_COLOR_SRAM_SIZE + " bytes.");
				return;
			} else if (arr->Length > GOOMBA_COLOR_SRAM_SIZE) {
				MessageBox::Show("This file is more than " + GOOMBA_COLOR_SRAM_SIZE +
					" bytes. If you overwrite the file, the last " + (arr->Length - GOOMBA_COLOR_SRAM_SIZE) +
					" bytes will be discarded.", "Note", MessageBoxButtons::OK, MessageBoxIcon::Information);
			}
			pin_ptr<unsigned char> pin = &arr[0];
			char* cleaned = goomba_cleanup(pin);
			if (cleaned == NULL) {
				return;
			} else if (cleaned != (void*)pin) {
				DR r = MessageBox::Show("Uncompressed SRAM found at 0xE000. Would you like to move and compress it to the proper location? (Doing this is required to extract or replace the save data.)",
					"Clean SRAM", MessageBoxButtons::YesNoCancel);
				if (r == DR::Yes) {
					memcpy(loaded_sram, cleaned, GOOMBA_COLOR_SRAM_SIZE);
					dirty = true;
				} else if (r == DR::No) {
					memcpy(loaded_sram, pin, GOOMBA_COLOR_SRAM_SIZE);
					dirty = false;
				}
				free(cleaned);
				if (r == DR::Cancel) {
					return;
				}
			} else {
				memcpy(loaded_sram, pin, GOOMBA_COLOR_SRAM_SIZE);
				dirty = false;
			}
			
			_filePath = filename;
			this->Text = (filename == nullptr)
				? gcnew String(TITLE)
				: gcnew String(TITLE) + " - " + IO::Path::GetFileName(filename);

			headerScan();
		}

		void save(String^ path) {
			marshal_context context;
			FILE* f = fopen(context.marshal_as<const char*>(_filePath), "wb");
			if (f == NULL) {
				MessageBox::Show("Could not open file: " + gcnew String(strerror(errno)), "Error", MessageBoxButtons::OK, MessageBoxIcon::Information);
				return;
			}
			fwrite(loaded_sram, 1, GOOMBA_COLOR_SRAM_SIZE, f);
			fclose(f);
			_filePath = path;
			dirty = false;
		}

		void headerScan() {
			stateheader** headers = stateheader_scan(loaded_sram);
			if (headers == NULL) {
				MessageBox::Show(gcnew String(goomba_last_error()), "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			} else if (headers[0] == NULL) {
				listBox1->Items->Clear();
				MessageBox::Show("No headers were found in this file.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Information);
			} else {
				listBox1->Items->Clear();
				for (int i = 0; headers[i] != NULL; i++) {
					listBox1->Items->Add(gcnew HeaderPtr(headers[i]));
				}
				free(headers);
			}
		}

		Void openToolStripMenuItem_Click(Object^ sender, EventArgs^ e) {
			OpenFileDialog d;
			d.Filter = "Game Boy Advance save data (*.sav)|*.sav|All files (*.*)|*.*";
			if (d.ShowDialog() == DR::OK) {
				load(d.FileName);
			}
		}
		Void saveToolStripMenuItem_Click(Object^ sender, EventArgs^ e) {
			save(_filePath);
		}
		Void saveAsToolStripMenuItem_Click(Object^ sender, EventArgs^ e) {
			SaveFileDialog d;
			d.Filter = "Game Boy Advance save data (*.sav)|*.sav|All files (*.*)|*.*";
			d.AddExtension = true;
			if (d.ShowDialog() == DR::OK) {
				save(d.FileName);
			}
		}
		Void exitToolStripMenuItem_Click(Object^ sender, EventArgs^ e) {
			this->Close();
		}
	
		Void listBox1_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
			HeaderPtr^ p = (HeaderPtr^)listBox1->SelectedItem;
			stateheader* sh = p->sh_ptr();
			btnExtract->Enabled = sh->type == GOOMBA_SRAMSAVE;
			btnReplace->Enabled = sh->type == GOOMBA_SRAMSAVE;
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
				lblChecksumVal->Text = cd->sram_checksum.ToString("X8"); // The SRAM with this ROM checksum value is currently in 0xe000-0xffff
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

		void replace(String^ filename) {
			array<unsigned char>^ gbc_data_arr = System::IO::File::ReadAllBytes(filename);
			pin_ptr<unsigned char> gbc_data = &gbc_data_arr[0];
			HeaderPtr^ p = (HeaderPtr^)listBox1->SelectedItem;
			if (p == nullptr) {
				MessageBox::Show("No item is selected.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
			stateheader* sh = p->sh_ptr();
			void* new_data = goomba_new_sav(loaded_sram, sh, gbc_data, gbc_data_arr->Length);
			if (new_data == NULL) {
				MessageBox::Show(gcnew String(goomba_last_error()), "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
			memcpy(loaded_sram, new_data, GOOMBA_COLOR_SRAM_SIZE);
			dirty = true;
			free(new_data);

			int sel = listBox1->SelectedIndex;
			headerScan();
			listBox1->SelectedIndex = sel;
		}

		Void btnReplace_Click(Object^ sender, EventArgs^ e) {
			OpenFileDialog d;
			d.Title = btnReplace->Text;
			d.Filter = "Game Boy save data (*.sav)|*.sav|All files (*.*)|*.*";
			if (d.ShowDialog() == Windows::Forms::DialogResult::OK) {
				replace(d.FileName);
			}
		}
		Void btnExtract_Click(Object^ sender, EventArgs^ e) {
			HeaderPtr^ p = (HeaderPtr^)listBox1->SelectedItem;
			stateheader* sh = p->sh_ptr();
			goomba_size_t len;
			void* data = goomba_extract(loaded_sram, sh, &len);
			if (data == NULL) {
				MessageBox::Show(gcnew String(goomba_last_error()), "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}
			SaveFileDialog d;
			d.Title = btnExtract->Text;
			d.Filter = "Game Boy save data (*.sav)|*.sav|All files (*.*)|*.*";
			d.AddExtension = true;
			if (d.ShowDialog() == Windows::Forms::DialogResult::OK) {
				marshal_context context;
				FILE* outfile = fopen(context.marshal_as<const char*>(d.FileName), "wb");
				if (outfile == NULL) {
					MessageBox::Show("Could not open file: " + gcnew String(strerror(errno)), "Error", MessageBoxButtons::OK, MessageBoxIcon::Information);
				} else {
					fwrite(data, 1, len, outfile);
					fclose(outfile);
				}
			}
			free(data);
		}

		Void aboutToolStripMenuItem_Click(Object^ sender, EventArgs^ e) {
			AboutForm a;
			a.ShowDialog();
		}
};
}
