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
#include <math.h>
#include <algorithm>
double mx, my;

void errorcb(int error, const char* desc)
{
	printf("GLFW error %d: %s\n", error, desc);
}

int blowup = 0;
int screenshot = 0;
int premult = 0;

#define PI 3.141592653;
typedef struct
{
	double x, y, z;
}Point;

typedef struct
{
	double x, y;
}Point2D;

typedef struct {
	std::vector<double> x_data;
	std::vector<double> y_data;
	NVGcolor color;
	int id;
	float thickeness;
}Curve;

NVGcolor defaultColor;
float defaultWidth;

Curve one = {};
Curve two = {};


//computes eucledian distance
//unit tested
double ComputeDistance(double x1, double y1, double x2,double y2)
{
	double square_difference_x = (x2 - x1) * (x2 - x1);
	double square_difference_y = (y2 - y1) * (y2 - y1);
	double sum = square_difference_x + square_difference_y;
	double value = sqrt(sum);
	return value;
}

//computes cosine similarity
//unit tested
double ComputeSimilarity2D(Point2D p1, Point2D p2)
{
	double dotProduct = (p1.x * p2.x) + (p1.y * p2.y);
	double magnitudep1 = (p1.x * p1.x) + (p1.y * p1.y);
	double magnitudep2 = (p2.x * p2.x) + (p2.y * p2.y);
	double radians = (dotProduct / (sqrt(magnitudep1) * sqrt(magnitudep2))) * PI;
	return cos(radians / 180.0);
}

//computes cosine similarity
//unit tested
double ComputeSimilarity3D(Point p1, Point p2)
{
	double dotProduct = (p1.x * p2.x) + (p1.y * p2.y) + (p1.z * p2.z);
	double magnitudep1 = (p1.x * p1.x) + (p1.y * p1.y) + (p1.z * p1.z);
	double magnitudep2 = (p2.x * p2.x) + (p2.y * p2.y) + (p2.z * p2.z);
	double radians = (dotProduct / (sqrt(magnitudep1) * sqrt(magnitudep2))) * PI;
	return cos(radians / 180.0);
}

Curve DrawCurve(NVGcontext* vg, GLFWwindow* window)
{
	Curve Curve1;
	return Curve1;
}

//unit tested
void GenerateBorders(NVGcontext* vg, float x, float y, float w, float h, NVGcolor color, float Stroke_Width)
{
	nvgBeginPath(vg);
	nvgStrokeColor(vg, color);
	nvgStrokeWidth(vg, Stroke_Width);
	nvgRect(vg, x, y, w, h);
	nvgStroke(vg);
}
//unit tested
void GenerateDefaultBorders(NVGcontext* vg, GLFWwindow* window)
{
	int winWidth, winHeight;
	glfwGetWindowSize(window, &winWidth, &winHeight);
	nvgBeginPath(vg);
	nvgStrokeColor(vg, defaultColor);
	nvgStrokeWidth(vg, defaultWidth);
	nvgRect(vg, 10, 10, winWidth -20, winHeight - 20);
	nvgStroke(vg);
}
//Not implemented
void DrawDottedLines(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data)
{

}
//Not implemented
void DrawDashedLines(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data)
{

}
//Not implemented
void DrawStepLeftLines(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data)
{

}
//Not implemented
void DrawStepRightLines(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data)
{

}
//Not implemented
void DrawStepCentreLines(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data)
{

}
//Not implemented
void DrawImpulseinX(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data, float base_x)
{

}
//Not implemented
void DrawImpulseinY(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data, float base_y)
{

}

//c represent x axis and h represent y axis
//unit tested
void DrawAxes(NVGcontext* vg, Point2D x_start, Point2D x_end, Point2D y_start, Point2D y_end, NVGcolor color, float stroke_width, NVGlineCap lineCap, NVGlineCap lineJoin)
{
	nvgBeginPath(vg);
	nvgStrokeWidth(vg, stroke_width);
	nvgStrokeColor(vg, color);
	nvgLineCap(vg, lineCap);
	nvgLineJoin(vg, lineJoin);
	nvgMoveTo(vg, x_start.x, x_start.y);
	nvgLineTo(vg, x_end.x, x_end.y);
	nvgStroke(vg);

	nvgBeginPath(vg);
	nvgStrokeWidth(vg, stroke_width);
	nvgStrokeColor(vg, color);
	nvgLineCap(vg, lineCap);
	nvgLineJoin(vg, lineJoin);
	nvgMoveTo(vg, y_start.x, y_start.y);
	nvgLineTo(vg, y_end.x, y_end.y);
	nvgStroke(vg);

}
//Not implemented
void DrawBoxAxes(NVGcontext* vg, GLFWwindow* window)
{

}
//unit tested
void PopulateSamplesParabola2D(std::vector<double>* x_data, std::vector<double>* y_data)
{
	printf(" \n Populating a 100 sample points for a parabola");
	for (int i = 0; i < 101; ++i)
	{
		double push_point_x = i / 50.0 - 1;
		double push_point_y = push_point_x * push_point_x;
		x_data -> push_back(100 * (push_point_x + 2));
		y_data -> push_back((0.5 * 758 * (push_point_y)) + 10);
		/*y_data -> push_back(push_point_x);
		y_data -> push_back(push_point_x * push_point_x);*/

	}
	printf(" \n Populating Complete");
}
//unit tested
void PopulateSamplesCurve2D(std::vector<double>* x_data, std::vector<double>* y_data)
{
	printf(" \n Populating a 100 sample points for a Curve");
	for (int i = 0; i < 101; ++i)
	{
		double push_point_x = i/15.0 * 5 *3.14 + 0.01;
		double push_point_y = sin(push_point_x) / push_point_x;
		x_data->push_back(100 * (push_point_x + 2));
		y_data->push_back((0.5 * 758 * (push_point_y)) + 10);
	}
	printf(" \n Populating Complete");
}
//unit tested
void EnableGrid(NVGcontext* vg, GLFWwindow* window,unsigned int scale, NVGcolor color)
{
	int winHeight, winWidth;
	glfwGetWindowSize(window, &winWidth, &winHeight);
	float step_x = winWidth / scale;
	float step_y = winHeight / scale;
	for (int i=0; i<scale; i++)
	{
		nvgBeginPath(vg);
		nvgStrokeWidth(vg, 0.5f);
		nvgStrokeColor(vg, color);
		nvgMoveTo(vg, (step_x * i), 0.0);
		nvgLineTo(vg, (step_x * i), winHeight);
		nvgStroke(vg);

		nvgBeginPath(vg);
		nvgStrokeWidth(vg, 0.5f);
		nvgStrokeColor(vg, color);
		nvgMoveTo(vg, 0.0 , (step_y * i));
		nvgLineTo(vg, winWidth, (step_y * i));
		nvgStroke(vg);
	}

	for (int i = 0; i < scale; i++)
	{
		for (int j = 0; j < scale; j++)
		{
			nvgBeginPath(vg);
			nvgCircle(vg, (step_x * i), (step_y * j), 1.0f);
			nvgStroke(vg);
		}
	}


}
//unit tested
static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	NVG_NOTUSED(scancode);
	NVG_NOTUSED(mods);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		blowup = !blowup;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		screenshot = 1;
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		premult = !premult;
}
//unit tested
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//printf("%f , %f \n",xpos, ypos);
}
//unit tested
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	NVG_NOTUSED(mods);
	unsigned char data[4] = {};
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double lowest_distance_to_one = 100, lowest_distance_to_two = 100;
		for (int idx = 0; idx < one.x_data.size(); idx++)
		{
			lowest_distance_to_one = std::min(lowest_distance_to_one, ComputeDistance(mx, my, one.x_data[idx], one.y_data[idx]));
			lowest_distance_to_two = std::min(lowest_distance_to_two, ComputeDistance(mx, my, two.x_data[idx], two.y_data[idx]));
		}
		//to prevent a negative distance because of curve width being 3.0f
		if (floor(lowest_distance_to_one) < 5)
		{
			printf(" \n Curve with id : %p Clicked \n", &one);
			one.color = nvgRGBA(rand() % 255, rand() % 255, rand() % 255, 255);
		}
		//to prevent a negative distance because of curve width being 3.0f
		else if(floor(lowest_distance_to_two) < 5) {
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
		double lowest_distance_to_one = 100 , lowest_distance_to_two = 100;
		for (int idx = 0; idx < one.x_data.size(); idx++)
		{
			lowest_distance_to_one = std::min(lowest_distance_to_one , ComputeDistance(mx,my,one.x_data[idx],one.y_data[idx]));
			lowest_distance_to_two = std::min(lowest_distance_to_two , ComputeDistance(mx,my,two.x_data[idx],two.y_data[idx]));
		}
		if (lowest_distance_to_one < lowest_distance_to_two)
		{
			printf(" \n Curve id : %p \n", &one);
			one.color = nvgRGBA(rand()%255, rand() % 255, rand() % 255, 255);
		}
		else {
			printf("\n Curve id :%p \n", &two);
			two.color = nvgRGBA(rand() % 255, rand() % 255, rand() % 255, 255);
		}
	}
}
//example
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
//example
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
		if (premult)
			glClearColor(0, 0, 0, 0);
		else
			glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

		renderDemo(vg, mx, my, winWidth, winHeight, t, blowup, &data);

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

		if (screenshot) {
			screenshot = 0;
			saveScreenShot(fbWidth, fbHeight, premult, "dump.png");
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

int run_test()
{
	GLFWwindow* window;
	NVGcontext* vg = NULL;

	//initializing GLFW
	if (!glfwInit()) {
		printf("Failed to init GLFW.");
		return -1;
	}

	//creating a GL-Window
	window = glfwCreateWindow(1366, 768, "NanoVG", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	//glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetKeyCallback(window, key);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
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

	one.id = 1;
	one.color = nvgRGBA(0, 192, 255, 255);
	one.thickeness = 3.0f;

	two.id = 1;
	two.color = nvgRGBA(0, 192, 255, 255);
	two.thickeness = 3.0f;

	//seeding data into curve one and two
	for (int i = 0; i < 101; ++i)
	{
		double push_point_x = i / 50.0 - 1;
		double push_point_y = push_point_x * push_point_x;
		one.x_data.push_back(100 * (push_point_x + 2));
		two.x_data.push_back(20 + (100 * (push_point_x + 2)));
		one.y_data.push_back((0.5 * 758 * (push_point_y)) + 10);
		two.y_data.push_back((0.5 * 758 * (push_point_y)) + 10);
	}

	Curve sample;
	sample.color = defaultColor;
	sample.thickeness = 2.0f;
	PopulateSamplesCurve2D(&sample.x_data, &sample.y_data);
	int iteration_Number = 0;
	while (!glfwWindowShouldClose(window))
	{
		defaultColor = nvgRGBA(255,255,255,255);
		defaultWidth = 3.0f;
		GenerateDefaultBorders(vg, window); // here
		GenerateBorders(vg, 200,200,100,100, defaultColor, 2.0f); // here
		
		EnableGrid(vg, window, 50, defaultColor); // here

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.003f ,0.09f , 0.09f , 0.74f);
		double t, dt;
		int winWidth, winHeight, pxRatio;
		int fbWidth, fbHeight;

		Point2D x_start = { 50.0 , one.y_data[0] + 10 };
		Point2D x_end = { 350, one.y_data[0] + 10 };
		Point2D y_start = { one.x_data[0] - 10.0, 10.0 };
		Point2D y_end = { one.x_data[0] - 10.0, one.y_data[0] + 20 };
		
		glfwGetCursorPos(window, &mx, &my);
		glfwGetWindowSize(window, &winWidth, &winHeight);
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		pxRatio = (float)fbWidth / (float)winWidth;

		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);


		DrawAxes(vg, x_start, x_end, y_start, y_end, defaultColor, 3.0f , NVG_BUTT, NVG_BEVEL); // here

		// drawing the parabola  for 1
		nvgLineCap(vg, NVG_BUTT);
		nvgLineJoin(vg, NVG_BEVEL);
		nvgStrokeWidth(vg, one.thickeness);
		nvgStrokeColor(vg, one.color);
		nvgBeginPath(vg);
		nvgMoveTo(vg, one.x_data[0] , one.y_data[0]);
		for (int idx = 1; idx < one.x_data.size(); idx++)
		{
			nvgLineTo(vg, one.x_data[idx], one.y_data[idx]);
		}
		nvgStroke(vg);

		// drawing the parabola 2
		nvgLineCap(vg, NVG_BUTT);
		nvgLineJoin(vg, NVG_BEVEL);
		nvgStrokeWidth(vg, two.thickeness);
		nvgStrokeColor(vg, two.color);
		nvgBeginPath(vg);
		nvgMoveTo(vg, two.x_data[0], two.y_data[0]);
		for (int idx = 1; idx < two.x_data.size(); idx+=27)
		{
			nvgLineTo(vg, two.x_data[idx], two.y_data[idx]);
		}
		nvgStroke(vg);

		drawGraph(vg, 0, winHeight / 2, winWidth, winHeight / 2, 2);
		nvgEndFrame(vg);
		glfwSwapBuffers(window);
		glfwPollEvents();
		iteration_Number++;

	}
	
	nvgDeleteGL3(vg);
	glfwTerminate();
}

int main()
{
	//run_example();
	run_test();
}