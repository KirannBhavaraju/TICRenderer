#include "TIC.h"
#include<stdio.h>
#include <math.h>
#include <algorithm>
#include "nanovg.h"
#include <GLFW\glfw3.h>
#include<common/Spline.h>

//unit test complete
void print_test()
{
	printf("Hello_World \n");
	printf("Printing from the header");
}
void draw_custom_spline_example()
{

}



//computes eucledian distance
//unit tested
double ComputeDistance(double x1, double y1, double x2, double y2)
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
double ComputeSimilarity3D(Point3D p1, Point3D p2)
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
	nvgRect(vg, 10, 10, winWidth - 20, winHeight - 20);
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
		x_data->push_back(100 * (push_point_x + 2));
		y_data->push_back((0.5 * 758 * (push_point_y)) + 10);
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
		double push_point_x = i / 15.0 * 5 * 3.14 + 0.01;
		double push_point_y = sin(push_point_x) / push_point_x;
		x_data->push_back(100 * (push_point_x + 2));
		y_data->push_back((0.5 * 758 * (push_point_y)) + 10);
	}
	printf(" \n Populating Complete");
}
//unit tested
void EnableGrid(NVGcontext* vg, GLFWwindow* window, unsigned int scale, NVGcolor color)
{
	int winHeight, winWidth;
	glfwGetWindowSize(window, &winWidth, &winHeight);
	float step_x = winWidth / scale;
	float step_y = winHeight / scale;
	for (int i = 0; i < scale; i++)
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
		nvgMoveTo(vg, 0.0, (step_y * i));
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
