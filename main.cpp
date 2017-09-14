#include <gl/glew.h>
#include <nanogui/nanogui.h>
#include <nanogui/formhelper.h>
#include <iostream>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkCubeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"

using namespace nanogui;

bool bvar = true;
std::string strval = "A string";
int ivar = 12345678;
double dvar = 3.1415926;
float fvar = (float)dvar;
enum test_enum {
	Item1 = 0,
	Item2,
	Item3
};
test_enum enumval = Item2;
Color colval(0.5f, 0.5f, 0.7f, 1.f);


int main(int argc, char ** argv)
{
	nanogui::init();
	Screen * screen = new Screen(Vector2i(500, 700), "NanoGUI test");
	FormHelper *gui = new FormHelper(screen);
	ref<Window> window = gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");
	gui->addGroup("Basic types");
	gui->addVariable("bool", bvar);
	gui->addVariable("string", strval);

	gui->addGroup("Validating fields");
	gui->addVariable("int", ivar);
	gui->addVariable("float", fvar);
	gui->addVariable("double", dvar);

	gui->addGroup("Complex types");
	gui->addVariable("Enumeration", enumval, true)->setItems({ "Item 1", "Item 2", "Item 3" });
	gui->addVariable("Color", colval);

	gui->addGroup("Other widgets");
	gui->addButton("A button", []() { std::cout << "Button pressed." << std::endl; });

	screen->setVisible(true);
	screen->performLayout();
	window->center();
	//-----------NEW - COISAS DA VTK----------------
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindowInteractor> interactor;
	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	vtkSmartPointer<vtkCubeSource> testeCubeSource = vtkSmartPointer<vtkCubeSource>::New();
	testeCubeSource->SetCenter(0, 0, 0);
	testeCubeSource->SetXLength(1);
	testeCubeSource->SetYLength(1);
	testeCubeSource->SetZLength(1);
	vtkSmartPointer<vtkPolyDataMapper> testeCubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	testeCubeMapper->SetInputConnection(testeCubeSource->GetOutputPort());
	vtkSmartPointer<vtkActor> testeCubeActor = vtkSmartPointer<vtkActor>::New();
	testeCubeActor->SetMapper(testeCubeMapper);
	testeCubeActor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
	testeCubeActor->RotateX(30.0);
	testeCubeActor->RotateY(-45.0);
	renderer->AddActor(testeCubeActor);
	renderer->ResetCamera();
	renderWindow->Render();


	nanogui::mainloop();//Esse m�todo blocka

	nanogui::shutdown();
	return EXIT_SUCCESS;
}