#include <stdio.h>
#include <GL/glew.h>
#ifdef __APPLE__
#	define GLFW_INCLUDE_GLCOREARB
#endif
#include <glew-2.1.0/include/GL/glext.h>
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "demo.h"
#include "perf.h"
#include <vector>
#include <iostream>
#include<algorithm>
#include<math.h>
#include "common/Spline.h"
#include "common/TICPP.h"
//#include <list>
using namespace kb;

//locally global only to serve callbacks
Curve one;
Curve two;
Utilities temp;
void errorcb(int error, const char* desc)
{
	printf("GLFW error %d: %s\n", error, desc);
}


//callbacks
static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	NVG_NOTUSED(scancode);
	NVG_NOTUSED(mods);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		printf("Key_Pressed : %d", GLFW_KEY_SPACE);
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		printf("Key_Pressed : %d", GLFW_KEY_S);
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		printf("Key_Pressed : %d", GLFW_KEY_P);
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//printf("%f , %f \n",xpos, ypos);
}
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	NVG_NOTUSED(mods);
	unsigned char data[4] = {};
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double lowest_distance_to_one = 100, lowest_distance_to_two = 100;
		for (int idx = 0; idx < (one.x_data)->size(); idx++)
		{
			lowest_distance_to_one = std::min(lowest_distance_to_one, temp.ComputeDistance(mx, my, (one.x_data)->at(idx), (one.y_data)->at(idx)));
			lowest_distance_to_two = std::min(lowest_distance_to_two, temp.ComputeDistance(mx, my, (two.x_data)->at(idx), (two.y_data)->at(idx)));
		}
		//to prevent a negative distance because of curve width being 3.0f
		if (floor(lowest_distance_to_one) < 5)
		{
			printf(" \n Curve with id : %p Clicked \n", &one);
			one.color = nvgRGBA(rand() % 255, rand() % 255, rand() % 255, 255);
		}
		//to prevent a negative distance because of curve width being 3.0f
		else if (floor(lowest_distance_to_two) < 5) {
			printf(" \n Curve with id : %p Clicked \n", &two);
			two.color = nvgRGBA(rand() % 255, rand() % 255, rand() % 255, 255);
		}
		else
		{
			printf(" \n No Curve Clicked \n");
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double lowest_distance_to_one = 100, lowest_distance_to_two = 100;
		for (int idx = 0; idx < (one.x_data)->size(); idx++)
		{
			lowest_distance_to_one = std::min(lowest_distance_to_one, temp.ComputeDistance(mx, my, (one.x_data)->at(idx), (one.y_data)->at(idx)));
			lowest_distance_to_two = std::min(lowest_distance_to_two, temp.ComputeDistance(mx, my, (two.x_data)->at(idx), (two.y_data)->at(idx)));
		}
		if (lowest_distance_to_one < lowest_distance_to_two)
		{
			printf(" \n Curve id : %p \n", &one);
			one.color = nvgRGBA(rand() % 255, rand() % 255, rand() % 255, 255);
		}
		else {
			printf("\n Curve id :%p \n", &two);
			two.color = nvgRGBA(rand() % 255, rand() % 255, rand() % 255, 255);
		}
	}
}


//examples
void customDrawLinesFromExample(NVGcontext* vg, float x, float y, float w, float h, float t)
{
	int i, j;
	float pad = 5.0f, s = w / 9.0f - pad * 2;
	float pts[4 * 2], fx, fy;
	int joins[3] = { NVG_MITER, NVG_ROUND, NVG_BEVEL };
	int caps[3] = { NVG_BUTT, NVG_ROUND, NVG_SQUARE };
	NVG_NOTUSED(h);

	nvgSave(vg);
	pts[0] = -s * 0.25f + cosf(t * 0.3f) * s * 0.5f;
	pts[1] = sinf(t * 0.3f) * s * 0.5f;
	pts[2] = -s * 0.25;
	pts[3] = 0;
	pts[4] = s * 0.25f;
	pts[5] = 0;
	pts[6] = s * 0.25f + cosf(-t * 0.3f) * s * 0.5f;
	pts[7] = sinf(-t * 0.3f) * s * 0.5f;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			fx = x + s * 0.5f + (i * 3 + j) / 9.0f * w + pad;
			fy = y - s * 0.5f + pad;

			nvgLineCap(vg, caps[i]);
			nvgLineJoin(vg, joins[j]);

			nvgStrokeWidth(vg, s * 0.3f);
			nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 160));
			nvgBeginPath(vg);
			nvgMoveTo(vg, fx + pts[0], fy + pts[1]);
			nvgLineTo(vg, fx + pts[2], fy + pts[3]);
			nvgLineTo(vg, fx + pts[4], fy + pts[5]);
			nvgLineTo(vg, fx + pts[6], fy + pts[7]);
			nvgStroke(vg);

			//draws the outside line shadow
			nvgLineCap(vg, NVG_BUTT);
			nvgLineJoin(vg, NVG_BEVEL);

			nvgStrokeWidth(vg, 1.0f);
			nvgStrokeColor(vg, nvgRGBA(0, 192, 255, 255));
			nvgBeginPath(vg);
			nvgMoveTo(vg, fx + pts[0], fy + pts[1]);
			nvgLineTo(vg, fx + pts[2], fy + pts[3]);
			nvgLineTo(vg, fx + pts[4], fy + pts[5]);
			nvgLineTo(vg, fx + pts[6], fy + pts[7]);
			nvgStroke(vg);
		}
	}


}
int run_example()
{
	GLFWwindow* window;
	DemoData data;
	NVGcontext* vg = NULL;
	GPUtimer gpuTimer;
	PerfGraph fps, cpuGraph, gpuGraph;
	double prevt = 0, cpuTime = 0;

	if (!glfwInit()) {
		printf("Failed to init GLFW.");
		return -1;
	}

	initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");
	initGraph(&cpuGraph, GRAPH_RENDER_MS, "CPU Time");
	initGraph(&gpuGraph, GRAPH_RENDER_MS, "GPU Time");

	glfwSetErrorCallback(errorcb);
#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

#ifdef DEMO_MSAA
	glfwWindowHint(GLFW_SAMPLES, 4);
#endif
	//window = glfwCreateWindow(1000, 600, "NanoVG", NULL, NULL);
	window = glfwCreateWindow(1000, 600, "NanoVG", glfwGetPrimaryMonitor(), NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key);


	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("Could not init glew.\n");
		return -1;
	}
	// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
	glGetError();

	//vg = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG);
	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	if (vg == NULL) {
		printf("Could not init nanovg.\n");
		return -1;
	}

	if (loadDemoData(vg, &data) == -1)
		return -1;

	glfwSwapInterval(0);

	initGPUTimer(&gpuTimer);

	glfwSetTime(0);
	prevt = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		double mx, my, t, dt;
		int winWidth, winHeight;
		int fbWidth, fbHeight;
		float pxRatio;
		float gpuTimes[3];
		int i, n;

		t = glfwGetTime();
		dt = t - prevt;
		prevt = t;

		startGPUTimer(&gpuTimer);

		glfwGetCursorPos(window, &mx, &my);
		glfwGetWindowSize(window, &winWidth, &winHeight);
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		// Calculate pixel ration for hi-dpi devices.
		pxRatio = (float)fbWidth / (float)winWidth;

		// Update and render
		glViewport(0, 0, fbWidth, fbHeight);
		if (0)
			glClearColor(0, 0, 0, 0);
		else
			glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

		renderDemo(vg, mx, my, winWidth, winHeight, t, 1, &data);

		//works here
		//char sample[] = "Sample";
		//nvgBeginPath(vg);
		//nvgFontSize(vg, 18.0f);
		//nvgFontFace(vg, "sans");
		//nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		//nvgText(vg, 200, 200, sample, NULL);
		///*printf("%f, %f \n ", ((axisObject->start.x + axisObject->end.x) / 3.0F), (axisObject->start.y + axisObject->labelMargin));
		//*/
		//nvgFill(vg);

		renderGraph(vg, 5, 5, &fps);
		renderGraph(vg, 5 + 200 + 5, 5, &cpuGraph);
		if (gpuTimer.supported)
			renderGraph(vg, 5 + 200 + 5 + 200 + 5, 5, &gpuGraph);

		nvgEndFrame(vg);

		// Measure the CPU time taken excluding swap buffers (as the swap may wait for GPU)
		cpuTime = glfwGetTime() - t;

		updateGraph(&fps, dt);
		updateGraph(&cpuGraph, cpuTime);

		// We may get multiple results.
		n = stopGPUTimer(&gpuTimer, gpuTimes, 3);
		for (i = 0; i < n; i++)
			updateGraph(&gpuGraph, gpuTimes[i]);

		if (0) {
			saveScreenShot(fbWidth, fbHeight, 0, "dump.png");
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	freeDemoData(vg, &data);

	nvgDeleteGL3(vg);

	printf("Average Frame Time: %.2f ms\n", getGraphAverage(&fps) * 1000.0f);
	printf("          CPU Time: %.2f ms\n", getGraphAverage(&cpuGraph) * 1000.0f);
	printf("          GPU Time: %.2f ms\n", getGraphAverage(&gpuGraph) * 1000.0f);

	glfwTerminate();
	return 0;
}

//testing
int run_test()
{
	GLFWwindow* window;
	NVGcontext* vg = NULL;
	int iteration_Number = 0;
	//initializing GLFW
	if (!glfwInit()) {
		printf("Failed to init GLFW.");
		return -1;
	}

	glfwSetErrorCallback(errorcb);

	//creating a GL-Window
	window = glfwCreateWindow(1700, 560, "NanoVG", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	//glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetKeyCallback(window, key);
	//glfwSetCursorPosCallback(window, cursor_position_callback);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	
	// setting the current context

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("Could not init glew.\n");
		return -1;
	}
	// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
	glGetError();

	//initialize nanovg
	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	if (vg == NULL) {
		printf("Could not init nanovg.\n");
		return -1;
	}
	glfwSwapInterval(0);

	//write a function to initialize fonts - many
	int fontInit = nvgCreateFont(vg, "sans", ".\\example\\Roboto-Regular.ttf");
	if (fontInit == -1)
	{
		fprintf(stderr, "Could not initialize fonts");
	}

	fontInit = nvgCreateFont(vg, "sans_Mono_Medium", ".\\example\\RobotoMono-Medium.ttf");
	if (fontInit == -1)
	{
		fprintf(stderr, "Could not initialize fonts");
	}

	one.color = nvgRGBA(0, 192, 255, 255);
	one.thickeness = 3.0f;

	two.color = nvgRGBA(0, 192, 255, 255);
	two.thickeness = 3.0f;

	/*
	////seeding data into curve one and two
	//tk::spline spline = tk::spline();
	//std::vector<double> x_data_temp;
	//std::vector<double> y_data_temp;
	//for (int i = 0; i < 101; ++i)
	//{
	//	double push_point_x = i / 50.0 - 1;
	//	double push_point_y = push_point_x * push_point_x;
	//	x_data_temp.push_back(100 * (push_point_x + 2));
	//	//one->x_data->push_back(100 * (push_point_x + 2));
	//	//(two.x_data)->push_back(20 + (100 * (push_point_x + 2)));
	//	y_data_temp.push_back((0.5 * 758 * (push_point_y)) + 10);
	//	//(one.y_data)->push_back((0.5 * 758 * (push_point_y)) + 10);
	//	//(two.y_data)->push_back((0.5 * 758 * (push_point_y)) + 10);
	//}
	//one.x_data = &x_data_temp;
	//two.x_data = &x_data_temp;
	//one.y_data = &y_data_temp;
	//two.y_data = &y_data_temp;

	//std::sort(x_data_temp.begin(), x_data_temp.end());
	//std::sort(y_data_temp.begin(), y_data_temp.end());
	//spline.set_boundary(tk::spline::second_deriv, 0.0, tk::spline::first_deriv, 0.0, true);
	//spline.set_points(x_data_temp, y_data_temp);
	*/

	defaultColor = nvgRGBA(255, 255, 255, 255);
	defaultWidth = 1.0f;

	//std::vector<int> valuesforAxis = { 1,1,1,1,1,1,1,1,1,1 };
	std::vector<std::string> valuesforAxis , valuesforAxis1, valuesforAxis2;
	valuesforAxis.push_back("100");
	valuesforAxis.push_back("200");
	valuesforAxis.push_back("300");
	valuesforAxis.push_back("400");
	valuesforAxis.push_back("500");
	valuesforAxis.push_back("600");
	valuesforAxis.push_back("700");
	valuesforAxis.push_back("800");
	valuesforAxis.push_back("900");
	valuesforAxis.push_back("1000");

	char label[] = "X_Top";
	kb::Axis x_top;
	x_top.arrowScale = 5;
	x_top.axisType = kb::AxisType::X_Top;
	x_top.isReversed = true;
	x_top.stroke_width = 1.0;
	x_top.labelMargin = 20;
	x_top.AxisLabelText = label;
	x_top.axisvalues = &valuesforAxis;
	int i = 0;

	char label1[] = "X_Bottom";
	kb::Axis x_Bottom;
	x_Bottom.arrowScale = 5;
	x_Bottom.axisType = kb::AxisType::X_Bottom;
	x_Bottom.isReversed = true;
	x_Bottom.stroke_width = 1.0;
	x_Bottom.labelMargin = 20;
	x_Bottom.AxisLabelText = label1;
	x_Bottom.axisvalues = &valuesforAxis1;

	char label3[] = "Y_Left";
	Axis y_left;
	y_left.arrowScale = 5;
	y_left.axisType = AxisType::Y_Left;
	y_left.isReversed = true;
	y_left.stroke_width = 1.0;
	y_left.labelMargin = 10;
	y_left.AxisLabelText = label3;
	y_left.axisvalues = &valuesforAxis2;

	char label2[] = "Y_Right";
	Axis Y_Right;
	Y_Right.arrowScale = 5;
	Y_Right.axisType = AxisType::Y_Right;
	Y_Right.isReversed = true;
	Y_Right.stroke_width = 1.0;
	Y_Right.labelMargin = 10;
	Y_Right.AxisLabelText = label2;
	Y_Right.axisvalues = &valuesforAxis2;

	kb::Graph graphCentre;
	graphCentre.Background_Color = nvgRGBA(227, 205, 186, 201);
	graphCentre.position.x = 620;
	graphCentre.position.y = 50;
	graphCentre.Grid = true;
	graphCentre.Borders = true;
	graphCentre.Grid_Color = nvgRGBA(10, 0, 0, 255);
	graphCentre.Grid_Scale = 20;
	graphCentre.Size_X = 460;
	graphCentre.Size_Y = 460;
	graphCentre.Label = "Histogram Exp Growth";
	graphCentre.Border_Label_Padding = 15.0;
	graphCentre.Border_Padding = 40.0;
	graphCentre.Label_Position = Position::Centre;

	kb::Graph graphRight;
	graphRight.Background_Color = nvgRGBA(227, 205, 186, 201);
	graphRight.position.x = 1170;
	graphRight.position.y = 50;
	graphRight.Grid = true;
	graphRight.Borders = true;
	graphRight.Grid_Color = nvgRGBA(10, 0, 0, 255);
	graphRight.Grid_Scale = 20;
	graphRight.Size_X = 460;
	graphRight.Size_Y = 460;
	graphRight.Label = "Plotting Graph Right";
	graphRight.Border_Label_Padding = 15.0;
	graphRight.Border_Padding = 40.0;
	graphRight.Label_Position = Position::Right;

	kb::Graph graphLeft;
	graphLeft.Background_Color = nvgRGBA(227, 205, 186, 201);
	graphLeft.position.x = 70;
	graphLeft.position.y = 50;
	graphLeft.Grid = true;
	graphLeft.Borders = true;
	graphLeft.Grid_Color = nvgRGBA(10, 0, 0, 255);
	graphLeft.Grid_Scale = 20;
	graphLeft.Size_X = 460;
	graphLeft.Size_Y = 460;
	graphLeft.Label = "Plotting Graph Left";
	graphLeft.Border_Label_Padding = 15.0;
	graphLeft.Border_Padding = 40.0; 
	graphLeft.Label_Position = Position::Right;

	std::vector<double> hist_x, hist_y;
	for (int i =0; i< 20 ; i++)
	{
		valuesforAxis1.push_back(std::to_string(i));
		hist_x.push_back(i);
		hist_y.push_back(i*i);
		valuesforAxis2.push_back(std::to_string(i*19));
	}
	Histogram histogram1;
	histogram1.setData(hist_x, hist_y);
	histogram1.Bar_Color = nvgRGBA(54,54,54,240);
	histogram1.Bar_Width = 10;

	//generating data frop left
	std::vector<double> sample_x, y00, y11;
	for (int i = 0; i < 1000; ++i)
	{
		sample_x.push_back(i);
		y00.push_back(150 * (exp(-i / 150.0) * cos(i / 10.0)));
		y11.push_back(150 * (exp(-i / 150.0)));
	}
	// generate data for right
	std::vector<double> x0(500), y0(500);
	std::vector<double> x2(500), y2(500);
	std::vector<double> x3(500), y3(500);
	std::vector<double> x4(500), y4(500);
	for (int i = 0; i < 500; ++i) // data for graph 0
	{
		x0[i] = 3.0 * i / 500.0;
		y0[i] = exp(-x0[i] * x0[i] * 0.8) * (x0[i] * x0[i] + x0[i]);
	}
	for (int i = 0; i < 500; ++i) // data for graphs 2, 3 and 4
	{
		x2[i] = i / 500.0 * 3 * PI;
		x3[i] = x2[i];
		x4[i] = i / 500.0 * 100 - 50;
		y2[i] = sin(x2[i] * 12) * cos(x2[i]) * 10;
		y3[i] = cos(x3[i]) * 10;
		y4[i] = 0.01 * x4[i] * x4[i] + 1.5 * (rand() / (double)RAND_MAX - 0.5) + 1.5 * PI;
	}

	std::vector<double> line_1_x, line_2_x, line_3_x, line_4_x, line_5_x, line_6_x;
	std::vector<double> line_1_y, line_2_y, line_3_y, line_4_y, line_5_y, line_6_y;
	for (int j = 0; j < 150; j++)
	{
		line_1_x.push_back(j / 15.0 * 5 * 3.14 + 0.01);
		line_1_y.push_back(7 * tan(line_1_x[j]) / line_1_x[j]);
	}

	kb::Curve exponentialDecay;
	exponentialDecay.x_data = &sample_x;
	exponentialDecay.y_data = &y11;
	exponentialDecay.LineType = LineType::Line_Default;

	kb::Curve exponentialDecayCos;
	exponentialDecayCos.x_data = &sample_x;
	exponentialDecayCos.y_data = &y00;
	exponentialDecayCos.LineType = LineType::Line_Default;

	kb::Curve line_1;
	line_1.x_data = &line_1_x;
	line_1.y_data = &line_1_y;
	line_1.LineType = LineType::Impulse_Line_X_Bottom;

	kb::Curve line_2;
	line_2.x_data = &line_1_x;
	line_2.y_data = &line_1_y;
	line_2.LineType = LineType::Step_Centre_Line;
	line_2.y_offset = 30.0;

	kb::Curve line_2_1;
	line_2_1.x_data = &line_1_x;
	line_2_1.y_data = &line_1_y;
	line_2_1.LineType = LineType::Step_Left_Line;
	line_2_1.y_offset = 60.0;

	kb::Curve line_2_2;
	line_2_2.x_data = &line_1_x;
	line_2_2.y_data = &line_1_y;
	line_2_2.LineType = LineType::Step_Right_Line;
	line_2_2.y_offset = 90.0;

	kb::Curve line_2_3;
	line_2_3.x_data = &line_1_x;
	line_2_3.y_data = &line_1_y;
	line_2_3.LineType = LineType::Line_Default;
	line_2_3.y_offset = 120.0;

	kb::Curve line_3;
	line_3.x_data = &line_1_x;
	line_3.y_data = &line_1_y;
	line_3.LineType = LineType::Dotted_Line;
	line_3.y_offset = 150.0;

	kb::Curve x0y0;
	x0y0.x_data = &x0;
	x0y0.y_data = &y0;

	//kb::Curve x1y1;
	//x1y1.x_data = &scaledRanges_x1;
	//x1y1.y_data = &scaledRanges_y1;

	kb::Curve x2y2;
	x2y2.x_data = &x2;
	x2y2.y_data = &y2;

	kb::Curve x3y3;
	x3y3.x_data = &x3;
	x3y3.y_data = &y3;

	kb::Curve x4y4;
	x4y4.x_data = &x4;
	x4y4.y_data = &y4;


	Point2D start_1 = { 50,30 };
	Point2D start_2 = { 600,30 };

	//reading images
	char* inputFilename = "C:\\Users\\KirannBhavaraju\\Desktop\\ISG HIWI\\NanoVGSecond\\build\\sample.jpg";

	while (!glfwWindowShouldClose(window))
	{

		int winWidth, winHeight, pxRatio;
		int fbWidth, fbHeight;

		//x_start.print = print_test;
		//x_start.print(&iteration_Number);

		glfwGetCursorPos(window, &mx, &my);
		glfwGetWindowSize(window, &winWidth, &winHeight);
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		pxRatio = (float)fbWidth / (float)winWidth;
		glViewport(0, 0, winWidth, winHeight);
		//glViewport(i, 0, fbWidth, fbHeight);
		//i += 10;

		glClearColor(0.654, 0.666, 0.658, 0.5);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

		graphLeft.init(vg);
		if (iteration_Number % 100 == 0)
		{
			exponentialDecay.color = nvgRGBA(rand() % 101, rand() % 101, rand() % 100, 255);
			exponentialDecayCos.color = nvgRGBA(rand() % 101, rand() % 101, rand() % 100, 255);
		}
		graphLeft.DrawAxes(vg, &x_top);
		graphLeft.DrawAxes(vg, &Y_Right);
		graphLeft.DrawAxes(vg, &x_Bottom);
		graphLeft.DrawAxes(vg, &y_left);

		graphLeft.DrawCurve(vg, &exponentialDecay);
		graphLeft.DrawCurve(vg, &exponentialDecayCos);

		graphRight.init(vg);
		graphRight.DrawCurve(vg, &line_1);
		graphRight.DrawCurve(vg, &line_2);
		graphRight.DrawCurve(vg, &line_2_1);
		graphRight.DrawCurve(vg, &line_2_2);
		graphRight.DrawCurve(vg, &line_2_3);
		graphRight.DrawCurve(vg, &line_3);
		graphRight.DrawAxes(vg, &x_top);

		graphCentre.init(vg);
		//graphCentre.DrawCurve(vg, &x0y0);
		//graphCentre.DrawCurve(vg, &x2y2);
		//graphCentre.DrawCurve(vg, &x3y3);
		//graphCentre.DrawCurve(vg, &x4y4);
		graphCentre.DrawAxes(vg, &y_left);
		graphCentre.DrawAxes(vg, &x_Bottom);


		graphCentre.DrawHistogram(vg, &histogram1);

		nvgEndFrame(vg);

		glfwSwapBuffers(window);
		glfwPollEvents();
		iteration_Number++;
	}

	nvgDeleteGL3(vg);
	glfwTerminate();
	return 0;
}
int main()
{
	//run_example();
	//print_test();
	
	run_test();

	////chutzpah
	//std::list <int> list1;
	//list1.push_back(1);
	//list1.push_back(2);
	//list1.push_back(3);
	//std::list <int> list2;
	//list2 = list1;
	//list2.push_back(4);
	//printf("%p , %p \n", list1, list2);
	//printf("%d \n", *std::max_element(list1.begin(),list1.end()));
	//printf("%d \n", *std::min_element(list2.begin(), list2.end()));
	//printf("%d \n", *std::min_element(list2.begin(), list2.end()));


	//kb::Point2D newPoint = kb::Point2D(2.9, 3.9);
	//std::map<int, std::pair<double, double>> test;
	//test[1] = std::make_pair(2.9, 3.9);
	//printf("%f , %f", ((*test.find(0)).second).first, ((*test.find(0)).second).second);

}
