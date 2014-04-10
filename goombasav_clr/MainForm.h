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
			goomba_onerror = show_error_box;
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
	private: System::Windows::Forms::PropertyGrid^  propertyGrid1;


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
			this->propertyGrid1 = (gcnew System::Windows::Forms::PropertyGrid());
			this->SuspendLayout();
			// 
			// listBox1
			// 
			this->listBox1->Dock = System::Windows::Forms::DockStyle::Left;
			this->listBox1->FormattingEnabled = true;
			this->listBox1->IntegralHeight = false;
			this->listBox1->ItemHeight = 16;
			this->listBox1->Location = System::Drawing::Point(0, 0);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(120, 253);
			this->listBox1->TabIndex = 0;
			this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::listBox1_SelectedIndexChanged);
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->button1->Location = System::Drawing::Point(126, 218);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// propertyGrid1
			// 
			this->propertyGrid1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->propertyGrid1->Location = System::Drawing::Point(126, 12);
			this->propertyGrid1->Name = L"propertyGrid1";
			this->propertyGrid1->Size = System::Drawing::Size(344, 200);
			this->propertyGrid1->TabIndex = 2;
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(482, 253);
			this->Controls->Add(this->propertyGrid1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->listBox1);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->ResumeLayout(false);

		}
#pragma endregion

	private:
		void load(String^ filename) {
			array<unsigned char>^ arr = System::IO::File::ReadAllBytes(filename);
			if (arr->Length != GOOMBA_COLOR_SRAM_SIZE) {
				MessageBox::Show("Incorrect file size: must be " + GOOMBA_COLOR_SRAM_SIZE + " bytes.");
				return;
			}
			pin_ptr<unsigned char> pin = &arr[0];
			memcpy(loaded_sram, pin, GOOMBA_COLOR_SRAM_SIZE);

			stateheader* first = (stateheader*)(loaded_sram + 4);
			stateheader** headers = stateheader_scan(first, 31);
			if (headers != NULL) {
				listBox1->Items->Clear();
				for (int i = 0; headers[i] != NULL; i++) {
					listBox1->Items->Add(HeaderPtr::FromPtr(headers[i]));
				}
				free(headers);
			}
		}
		
		Void button1_Click(Object^ sender, EventArgs^ e) {
			OpenFileDialog d;
			if (d.ShowDialog() == Windows::Forms::DialogResult::OK) {
				load(d.FileName);
			}
		}
	
		Void listBox1_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
			HeaderPtr^ p = (HeaderPtr^)listBox1->SelectedItem;
			Console::Write((uint32_t)loaded_sram);
			Console::Write(" ");
			Console::WriteLine((uint32_t)p->sh_ptr());
			propertyGrid1->SelectedObject = p->plausible() ? (Object^)p : this;
		}
	};
}
