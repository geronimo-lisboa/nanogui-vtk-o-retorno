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
	glfwInit();
	glfwSetTime(0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(800, 800, "example3", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Create a nanogui screen and pass the glfw pointer to initialize
	Screen *screen = new Screen();
	screen->initialize(window, true);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glfwSwapInterval(0);
	glfwSwapBuffers(window);

	FormHelper *gui = new FormHelper(screen);
	ref<Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");
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
	nanoguiWindow->center();
	//-----------NEW - COISAS DA VTK----------------
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderWindowInteractor> interactor;
	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->InitializeFromCurrentContext();
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


	// Game loop
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		renderWindow->Render();
		// Draw nanogui
		screen->drawContents();
		screen->drawWidgets();


		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;

	nanogui::shutdown();
	return EXIT_SUCCESS;
}