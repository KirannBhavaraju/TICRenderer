#ifndef TIC_H
#define TIC_H
#include "nanovg.h"
#include<vector>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#define PI 3.141592653;

// objects used for constructing the return references
typedef struct {
	std::vector<double> x_data;
	std::vector<double> y_data;
	NVGcolor color;
	int id;
	float thickeness;
}Curve;
typedef struct
{
	double x, y, z;
}Point3D;
typedef struct
{
	double x, y;
}Point2D;
typedef struct {
	Point2D start;
	Point2D end;
	float stroke_width;
	NVGcolor color;
	bool axis_switch;
	char* AxisLabel;
	bool isReversed;
	int scale;
	std::vector<double>* axisvalues;
}Axis;
typedef struct {
	Axis* x;
	Axis* y;
	int* bins;
	std::vector<double>* binRange;
	std::vector<int>* counts;
}Histogram;


//globals
static double mx, my;
static int blowup = 0; // not needed included for example
static int screenshot = 0; // not needed included for example
static int premult = 0; // not needed included for example

static NVGcolor defaultColor; 
static float defaultWidth;

//function declarations
void print_test();
void draw_custom_spline_example();

double ComputeDistance(double x1, double y1, double x2, double y2);
double ComputeSimilarity2D(Point2D p1, Point2D p2);
Curve DrawCurve(NVGcontext* vg, GLFWwindow* window);
void GenerateBorders(NVGcontext* vg, float x, float y, float w, float h, NVGcolor color, float Stroke_Width);
void GenerateDefaultBorders(NVGcontext* vg, GLFWwindow* window);
void DrawDottedLines(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data);
void DrawDashedLines(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data);
void DrawStepLeftLines(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data);
void DrawStepRightLines(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data);
void DrawStepCentreLines(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data);
void DrawImpulseinX(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data, float base_x);
void DrawImpulseinY(NVGcontext* vg, std::vector<double>* x_data, std::vector<double>* y_data, float base_y);
void DrawAxes(NVGcontext* vg, Point2D x_start, Point2D x_end, Point2D y_start, Point2D y_end, NVGcolor color, float stroke_width, NVGlineCap lineCap, NVGlineCap lineJoin);
void DrawBoxAxes(NVGcontext* vg, GLFWwindow* window);
void PopulateSamplesParabola2D(std::vector<double>* x_data, std::vector<double>* y_data);
void PopulateSamplesCurve2D(std::vector<double>* x_data, std::vector<double>* y_data);
void EnableGrid(NVGcontext* vg, GLFWwindow* window, unsigned int scale, NVGcolor color);

#endif // !TIC_H