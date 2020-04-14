#ifndef TICPP_H
#define TICPP_H
#include "nanovg.h"
#include<vector>
#include<string>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#define PI 3.141592653;

namespace kb
{
	static double mx, my;
	static NVGcolor defaultColor = nvgRGBA(0,0,0,150);
	static float defaultWidth = 1.5;

	enum class AxisType {
		X_Top = 0,
		X_Bottom = 1,
		Y_Left = 2,
		Y_Right = 3
	};
	enum class PositionType{
		Top_Left = 0,
		Top_Right = 1,
		Bottom_Left = 2,
		Bottom_Right = 3
	};

	class Point2D
	{
	public:
		double x = 0.0;
		double y = 0.0;
		Point2D() {};
		Point2D(double a, double b);

		void ShowPoint();
	};

	class Point3D
	{
	public:
		double x, y, z;
		Point3D() {};
		Point3D(double a, double b, double c);

		void ShowPoint();
	};

	class Utilities
	{
	public:
		double ComputeDistance(double x1, double y1, double x2, double y2);
		double ComputeSimilarity2D(Point2D p1, Point2D p2);
		double ComputeSimilarity3D(Point3D p1, Point3D p2);
		void PopulateSamplesParabola2D(std::vector<double>* x_data, std::vector<double>* y_data);
		void PopulateSamplesCurve2D(std::vector<double>* x_data, std::vector<double>* y_data);
		void MapRanges(std::vector<double>* inData, double inDataMin, double inDataMax, std::vector<double>* outData, double outDataMin, double outDataMax);
	};

	class Curve {
	public:
		Curve() {};
		std::vector<double>* x_data;
		std::vector<double>* y_data;
		NVGcolor color = nvgRGBA(0, 0, 0, 150);
		int id;
		float thickeness;
	private:
		int a;
	};

	class Axis
	{
	public:
		AxisType axisType;
		float arrowScale;
		float stroke_width;
		bool isReversed;
		NVGcolor color = nvgRGBA(0, 0, 0, 200);

		const char* AxisLabelText;
		float labelMargin;
		std::vector<std::string>* axisvalues;
		double valueSeparators;
	};

	class Graph
	{
	public:
		Point2D position;
		double Size_X;
		double Size_Y;
		char* Label;
		NVGcolor Background_Color;
		bool Borders;
		bool Grid;
		int Grid_Scale;
		NVGcolor Grid_Color;
		float Border_Label_Padding;
		float Border_Padding;
		NVGcolor Border_Color;
		float Border_Width;

		void init(NVGcontext* vg);
		bool DrawAxes(NVGcontext* vg, Axis* axisObject);
		void DrawCurve(NVGcontext* vg, Curve* curve);
	private:

	};


	class Histogram
	{

	};

}

#endif // !TICPP_H
