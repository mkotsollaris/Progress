#include "MyForm.h"
#include <opencv2\core\core.hpp>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "Utilities.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	ImageProcessingTool::MyForm form;
	Application::Run(%form);
}
