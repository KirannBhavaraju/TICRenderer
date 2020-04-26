#ifndef TICPP_H
#define TICPP_H
#include "nanovg.h"
#include<vector>
#include<string>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <list>
#include <map>
#define PI 3.141592653;

namespace kb
{
	static double mx, my;
	static double omnidir_axis_padding;
	static NVGcolor defaultColor;
	static float defaultWidth;

	enum class Position {
		Left = 0,
		Centre = 1,
		Right = 2
	};

	enum class LineType {
		Line_Default = 0,
		Dotted_Line = 1,
		Dashed_Line = 2,
		Step_Left_Line = 3,
		Step_Right_Line = 4,
		Step_Centre_Line = 5,
		Impulse_Line_X_Bottom = 6,
		Impulse_Line_X_Top = 7,
		Impulse_Line_Y_Left = 8,
		Impulse_Line_Y_Right = 9,
	};

	enum class AxisType {
		X_Top = 0,
		X_Bottom = 1,
		Y_Left = 2,
		Y_Right = 3
	};
	enum class PositionType {
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
		double x = 0.0, y = 0.0, z = 0.0;
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
		double x_offset = 0.0;
		double y_offset = 0.0;
		NVGcolor color = nvgRGBA(0, 0, 0, 150);
		float thickeness;
		LineType LineType = LineType::Line_Default;
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
		NVGcolor color = nvgRGBA(0, 0, 0, 150);

		const char* AxisLabelText;
		float labelMargin;
		std::vector<std::string>* axisvalues;
		double valueSeparators;
	};

	class Bar
	{
	protected:
		std::map<int, std::pair<double, double>> bars;
		void generateBars(std::vector<double>* x_Data, std::vector<double>* y_Data);
	};
	class Histogram : protected Bar, public Utilities
	{
	public:
		//std::vector<double> x_Data;
		//std::vector<double> y_Data;
		double Bar_Width;
		NVGcolor Bar_Color;
		std::vector<int> getKeys();
		std::pair<double, double> getData(int key);
		void setData(std::vector<double> x_inData, std::vector<double> y_inData);
		void addData(int key, double value);
	};

	class Graph : private Utilities
	{
	public:
		Graph() {};
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
		Position Label_Position;
		float Border_Padding;
		NVGcolor Border_Color;
		float Border_Width;

		void init(NVGcontext* vg);
		bool DrawAxes(NVGcontext* vg, Axis* axisObject);
		void DrawCurve(NVGcontext* vg, Curve* curve);
		void DrawHistogram(NVGcontext* vg, Histogram* histogram);

	private:
		static Axis* x_top;
		static Axis* x_bottom;
		static Axis* y_left;
		static Axis* y_right;
	};
}

#endif // !TICPP_H
