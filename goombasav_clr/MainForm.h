#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../goombasav.h"

namespace goombasav_clr {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// listBox1
			// 
			this->listBox1->Dock = System::Windows::Forms::DockStyle::Left;
			this->listBox1->FormattingEnabled = true;
			this->listBox1->ItemHeight = 16;
			this->listBox1->Location = System::Drawing::Point(0, 0);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(120, 253);
			this->listBox1->TabIndex = 0;
			this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::listBox1_SelectedIndexChanged);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(143, 51);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(143, 105);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(46, 17);
			this->label1->TabIndex = 2;
			this->label1->Text = L"label1";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(282, 253);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->listBox1);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	Void button1_Click(Object^ sender, EventArgs^ e) {
		OpenFileDialog d;
		if (d.ShowDialog() == Windows::Forms::DialogResult::OK) {
			array<unsigned char>^ arr = IO::File::ReadAllBytes(d.FileName);
			if (arr->Length != GOOMBA_COLOR_SRAM_SIZE) {
				MessageBox::Show("Incorrect file size (must be " + GOOMBA_COLOR_SRAM_SIZE + " bytes.)");
				return;
			}
			pin_ptr<unsigned char> pin = &arr[0];
			memcpy(loaded_sram, pin, GOOMBA_COLOR_SRAM_SIZE);

			listBox1->Items->Clear();
			stateheader* first = (stateheader*)(pin + 4);
			stateheader** headers = stateheader_scan(first, 31);
			if (headers == NULL) {
				MessageBox::Show("An error occurred. See the console for more information.");
			} else {
				for (int i = 0; headers[i] != NULL; i++) {
					listBox1->Items->Add(gcnew IntPtr(headers[i]));
				}
				free(headers);
			}
		}
	}
	
	Void listBox1_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
		IntPtr^ intptr = (IntPtr^)listBox1->SelectedItem;
		stateheader* sh = (stateheader*)intptr->ToPointer();
		if (stateheader_plausible(sh)) label1->Text = gcnew String(sh->title);
	}
};
}
