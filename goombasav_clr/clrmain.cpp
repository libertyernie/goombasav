#include "MainForm.h"

using namespace goombasav_clr; //the project name in your header file

[STAThreadAttribute]
int main(array<String ^> ^args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MainForm(args));
	return 0;
}