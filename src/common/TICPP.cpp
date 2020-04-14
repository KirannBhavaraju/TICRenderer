#include "TICPP.h"
using namespace kb;
#include<stdio.h>
#include <math.h>
#include <algorithm>
#include "nanovg.h"
#include <GLFW\glfw3.h>
#include<common/Spline.h>
#include<stdlib.h>
#include <sstream>

//custom types
Point2D::Point2D(double a, double b)
{
	this->x = a;
	this->y = b;
}

void Point2D::ShowPoint()
{
	printf("%f, %f", this->x, this->y);
}

kb::Point3D::Point3D(double a, double b, double c)
{
	this->x = a;
	this->y = b;
	this->z = c;
}

void kb::Point3D::ShowPoint()
{
	printf("%f, %f, %f", this->x, this->y, this->z);
}

//graph
void kb::Graph::init(NVGcontext* vg)
{
	nvgBeginPath(vg);
	nvgStrokeColor(vg, this->Border_Color);
	nvgStrokeWidth(vg, this->Border_Width);
	nvgRect(vg, this->position.x - this->Border_Padding, this->position.y - this->Border_Padding, (this->Size_X + 2*this->Border_Padding), (this->Size_Y + 2 * this->Border_Padding));
	nvgFillPaint(vg, nvgBoxGradient(vg, this->position.x - this->Border_Padding, this->position.y - this->Border_Padding, (this->Size_X + 2 * this->Border_Padding), (this->Size_Y + 2 * this->Border_Padding), 1.0, 20, nvgRGBA(255, 255, 255, 150), nvgRGBA(20, 56, 56, 200)));
	nvgFill(vg);
	nvgStroke(vg);

	nvgSave(vg);
	//write boundary checks here
	nvgBeginPath(vg);
	nvgLineCap(vg, NVG_BUTT);
	nvgLineJoin(vg, NVG_BEVEL);
	nvgMoveTo(vg, this->position.x, this->position.y);
	nvgLineTo(vg, this->position.x + this->Size_X, this->position.y);
	nvgLineTo(vg, this->position.x + this->Size_X, this->position.y + this->Size_Y);
	nvgLineTo(vg, this->position.x, this->position.y + this->Size_Y);
	nvgLineTo(vg, this->position.x, this->position.y);
	nvgFillColor(vg, this->Background_Color);
	nvgFill(vg);

	//drawing the borders
	if (this->Borders)
	{
		nvgBeginPath(vg);
		nvgStrokeWidth(vg, 1.0);
		nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 150));
		nvgRoundedRect(vg, this->position.x, this->position.y, this->Size_X, this->Size_Y, 3);
		nvgStroke(vg);
	}

	//drawing the grid
	if (this->Grid)
	{
		int scale = this->Grid_Scale;
		//glfwGetWindowSize(window, &winWidth, &winHeight);
		float step_x = this->Size_X / this->Grid_Scale;
		float step_y = this->Size_Y / this->Grid_Scale;
		for (int i = 0; i < scale; i++)
		{
			nvgBeginPath(vg);
			nvgStrokeWidth(vg, 0.3f);
			nvgStrokeColor(vg, this->Grid_Color);
			nvgMoveTo(vg, this->position.x + (step_x * i), this->position.y);
			nvgLineTo(vg, this->position.x + (step_x * i), this->position.y + this->Size_Y);
			nvgStroke(vg);

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, 0.3f);
			nvgStrokeColor(vg, this->Grid_Color);
			nvgMoveTo(vg, this->position.x, this->position.y + (step_y * i));
			nvgLineTo(vg, this->position.x + this->Size_X, this->position.y + (step_y * i));
			nvgStroke(vg);
		}

		for (int i = 0; i < scale; i++)
		{
			for (int j = 0; j < scale; j++)
			{
				nvgBeginPath(vg);
				nvgCircle(vg, this->position.x * (step_x * i), this->position.y * (step_y * j), 1.0f);
				nvgStroke(vg);
			}
		}
	}

	//drawing the labels
	nvgBeginPath(vg);
	nvgFontSize(vg, 18.0);
	nvgFontFace(vg, "sans_Mono_Medium");
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 150));
	nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
	nvgText(vg, 0.30 * this->Size_X + this->position.x, this->Border_Label_Padding + this->position.y, this->Label, NULL);
	nvgFill(vg);
	nvgRestore(vg);

}

bool kb::Graph::DrawAxes(NVGcontext* vg, Axis* axisObject)
{
	switch (axisObject->axisType)
	{
	case(AxisType::X_Top):
		if (axisObject->isReversed)
		{
			double start_x = this->position.x + this->Size_X;
			double end_x = this->position.x;

			double start_y = this->position.y;
			double end_y = this->position.y;

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, axisObject->stroke_width);
			nvgStrokeColor(vg, axisObject->color);
			nvgMoveTo(vg, start_x, start_y);
			nvgLineTo(vg, end_x, end_y);
			//drawing the arrows
			nvgLineTo(vg, end_x, end_y + (axisObject->arrowScale / 2.0));
			nvgLineTo(vg, end_x - (axisObject->arrowScale), end_y);
			nvgLineTo(vg, end_x, end_y - (axisObject->arrowScale / 2.0));
			nvgLineTo(vg, end_x, end_y);
			nvgFill(vg);
			nvgStroke(vg);

			// code to mark axis label according to top and bottom
			nvgBeginPath(vg);
			nvgFontSize(vg, 11.0f);
			nvgFillColor(vg, axisObject->color);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, ((start_x + end_x) / 2.5F), (start_y - axisObject->labelMargin), axisObject->AxisLabelText, NULL);
			nvgFill(vg);


			return true;
		}
		else if (axisObject->isReversed == false)
		{
			double end_x = this->position.x + this->Size_X;
			double start_x = this->position.x;

			double end_y = this->position.y;
			double start_y = this->position.y;

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, axisObject->stroke_width);
			nvgStrokeColor(vg, axisObject->color);
			nvgMoveTo(vg, start_x, start_y);
			nvgLineTo(vg, end_x, end_y);
			//drawing the arrows
			nvgLineTo(vg, end_x, end_y + (axisObject->arrowScale / 2.0));
			nvgLineTo(vg, end_x + (axisObject->arrowScale), end_y);
			nvgLineTo(vg, end_x, end_y - (axisObject->arrowScale / 2.0));
			nvgLineTo(vg, end_x, end_y);
			nvgFill(vg);
			nvgStroke(vg);

			// code to mark axis label according to top and bottom
			int scale = (this->Size_X) / (axisObject->axisvalues)->size();
			int y_padding = (this->Size_Y * 0.021);
			nvgBeginPath(vg);
			nvgFontSize(vg, 15.0);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, ((start_x + end_x) / 2) - 15.0, (start_y - (2.5 * y_padding)), axisObject->AxisLabelText, NULL);
			nvgFill(vg);
			

			// write in code to plot axis values according to top and bottom
			for (int i = 1; i < (axisObject->axisvalues)->size(); i++)
			{
				int x_padding = strnlen((axisObject->axisvalues)->at(i).c_str(), 5) * (7.0 / 3.0);
				nvgBeginPath(vg);
				nvgMoveTo(vg, (i * scale) + this->position.x, this->position.y);
				nvgLineTo(vg, (i * scale) + this->position.x, this->position.y + 5);
				nvgStroke(vg);
				nvgBeginPath(vg);
				nvgFontSize(vg, 12.0f);
				nvgFillColor(vg, axisObject->color);
				nvgFontFace(vg, "sans_Mono_Medium");
				nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
				//nvgText(vg, ((1.0 * i * scale) + this->position.x) - 2.0 , this->position.y - 5.0 , std::to_string((axisObject->axisvalues)->at(i)).c_str(), NULL);
				nvgText(vg, ((1.0 * i * scale) + this->position.x) - x_padding, this->position.y - y_padding, (axisObject->axisvalues)->at(i).c_str(), NULL);
				nvgFill(vg);
			}
			
			
			
			return true;


		}
		else
		{
			fprintf(stderr, "Error!");
			return false;
		}
		break;
	case(AxisType::X_Bottom):
		if (axisObject->isReversed)
		{
			double start_x = this->position.x + this->Size_X;
			double end_x = this->position.x;

			double start_y = this->position.y + this->Size_Y;
			double end_y = this->position.y + this->Size_Y;

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, axisObject->stroke_width);
			nvgStrokeColor(vg, axisObject->color);
			nvgMoveTo(vg, start_x, start_y);
			nvgLineTo(vg, end_x, end_y);
			//drawing the arrows
			nvgLineTo(vg, end_x, end_y + (axisObject->arrowScale / 2.0));
			nvgLineTo(vg, end_x - (axisObject->arrowScale), end_y);
			nvgLineTo(vg, end_x, end_y - (axisObject->arrowScale / 2.0));
			nvgLineTo(vg, end_x, end_y);
			nvgFill(vg);
			nvgStroke(vg);

			// code to mark axis label according to top and bottom
			nvgBeginPath(vg);
			nvgFontSize(vg, 11.0f);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, ((start_x + end_x) / 2.5F), (start_y - axisObject->labelMargin), axisObject->AxisLabelText, NULL);
			nvgFill(vg);

			// write in code to plot axis values according to top and bottom

			return true;
		}
		else if (axisObject->isReversed == false)
		{
			double end_x = this->position.x + this->Size_X;
			double start_x = this->position.x;

			double end_y = this->position.y + this->Size_Y;
			double start_y = this->position.y + this->Size_Y;

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, axisObject->stroke_width);
			nvgStrokeColor(vg, axisObject->color);
			nvgMoveTo(vg, start_x, start_y);
			nvgLineTo(vg, end_x, end_y);
			//drawing the arrows
			nvgLineTo(vg, end_x, end_y + (axisObject->arrowScale / 2.0));
			nvgLineTo(vg, end_x + (axisObject->arrowScale), end_y);
			nvgLineTo(vg, end_x, end_y - (axisObject->arrowScale / 2.0));
			nvgLineTo(vg, end_x, end_y);
			nvgFill(vg);
			nvgStroke(vg);

			// code to mark axis label according to top and bottom
			nvgBeginPath(vg);
			nvgFontSize(vg, 11.0f);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, ((start_x + end_x) / 2.5F), (start_y + axisObject->labelMargin), axisObject->AxisLabelText, NULL);
			nvgFill(vg);

			// write in code to plot axis values according to top and bottom

			return true;
		}
		else
		{
			fprintf(stderr, "Error!");
			return false;
		}
		break;
	case(AxisType::Y_Left):
		if (axisObject->isReversed == false)
		{
			double x_start = this->position.x;
			double x_end = this->position.x;

			double y_start = this->position.y + this->Size_Y;
			double y_end = this->position.y;

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, axisObject->stroke_width);
			nvgStrokeColor(vg, axisObject->color);
			// write in code to plot axis values
			// write in code to mark axis label
			// write in code for arrows
			nvgMoveTo(vg, x_start, y_start);
			nvgLineTo(vg, x_end, y_end);
			nvgLineTo(vg, x_end + (axisObject->arrowScale / 2.0), y_end);
			nvgLineTo(vg, x_end, y_end - (axisObject->arrowScale));
			nvgLineTo(vg, x_end - (axisObject->arrowScale / 2.0), y_end);
			nvgLineTo(vg, x_end, y_end);
			nvgFill(vg);
			nvgStroke(vg);

			// code to mark axis label according to top and bottom
			nvgSave(vg);
			nvgRotate(vg, -1.570796);
			nvgBeginPath(vg);
			nvgFontSize(vg, 11.0f);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, -((y_start + y_end) / 1.75F), (x_start - axisObject->labelMargin), axisObject->AxisLabelText, NULL);
			nvgFill(vg);
			nvgRestore(vg);

			return true;
		}
		else if (axisObject->isReversed)
		{
			double x_end = this->position.x;
			double x_start = this->position.x;

			double y_end = this->position.y + this->Size_Y;
			double y_start = this->position.y;

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, axisObject->stroke_width);
			nvgStrokeColor(vg, axisObject->color);
			// write in code to plot axis values
			// write in code to mark axis label
			// write in code for arrows
			nvgMoveTo(vg, x_start, y_start);
			nvgLineTo(vg, x_end, y_end);
			nvgLineTo(vg, x_end + (axisObject->arrowScale / 2.0), y_end);
			nvgLineTo(vg, x_end, y_end + (axisObject->arrowScale));
			nvgLineTo(vg, x_end - (axisObject->arrowScale / 2.0), y_end);
			nvgLineTo(vg, x_end, y_end);
			nvgFill(vg);
			nvgStroke(vg);

			// code to mark axis label according to top and bottom
			nvgSave(vg);
			nvgRotate(vg, -1.570796);
			nvgBeginPath(vg);
			nvgFontSize(vg, 11.0f);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, -((y_start + y_end) / 1.75F), (x_start - axisObject->labelMargin), axisObject->AxisLabelText, NULL);
			nvgFill(vg);
			nvgRestore(vg);
			return true;
		}
		else
		{
			fprintf(stderr, "Error!");
			return false;
		}
		break;
	case(AxisType::Y_Right):
		if (axisObject->isReversed == false)
		{
			double x_start = this->position.x + this->Size_X;
			double x_end = this->position.x + this->Size_X;

			double y_start = this->position.y + this->Size_Y;
			double y_end = this->position.y;

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, axisObject->stroke_width);
			nvgStrokeColor(vg, axisObject->color);
			// write in code to plot axis values
			// write in code to mark axis label
			// write in code for arrows
			nvgMoveTo(vg, x_start, y_start);
			nvgLineTo(vg, x_end, y_end);
			nvgLineTo(vg, x_end + (axisObject->arrowScale / 2.0), y_end);
			nvgLineTo(vg, x_end, y_end - (axisObject->arrowScale));
			nvgLineTo(vg, x_end - (axisObject->arrowScale / 2.0), y_end);
			nvgLineTo(vg, x_end, y_end);
			nvgFill(vg);
			nvgStroke(vg);

			// code to mark axis label according to left and right
			nvgSave(vg);
			nvgRotate(vg, 1.570796);
			nvgBeginPath(vg);
			nvgFontSize(vg, 11.0f);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, ((y_start + y_end) / 2.5F), -(x_start + axisObject->labelMargin), axisObject->AxisLabelText, NULL);
			nvgFill(vg);
			nvgRestore(vg);

			return true;
		}
		else if (axisObject->isReversed)
		{
			double x_end = this->position.x + this->Size_X;
			double x_start = this->position.x + this->Size_X;

			double y_end = this->position.y + this->Size_Y;
			double y_start = this->position.y;
			nvgBeginPath(vg);
			nvgStrokeWidth(vg, axisObject->stroke_width);
			nvgStrokeColor(vg, axisObject->color);
			// write in code to plot axis values

			nvgMoveTo(vg, x_start, y_start);
			nvgLineTo(vg, x_end, y_end);
			nvgLineTo(vg, x_end + (axisObject->arrowScale / 2.0), y_end);
			nvgLineTo(vg, x_end, y_end + (axisObject->arrowScale));
			nvgLineTo(vg, x_end - (axisObject->arrowScale / 2.0), y_end);
			nvgLineTo(vg, x_end, y_end);
			nvgFill(vg);
			nvgStroke(vg);

			// code to mark axis label according to left and right
			nvgSave(vg);
			nvgRotate(vg, 1.570796);
			nvgBeginPath(vg);
			nvgFontSize(vg, 11.0f);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, ((y_start + y_end) / 2.5F), -(x_start + axisObject->labelMargin), axisObject->AxisLabelText, NULL);
			nvgFill(vg);
			nvgRestore(vg);


			return true;
		}
		else
		{
			fprintf(stderr, "Error!");
			return false;
		}
		break;
	default:
		break;
	}


}

void kb::Graph::DrawCurve(NVGcontext* vg, Curve* curve)
{
	nvgSave(vg);
	nvgBeginPath(vg);
	nvgStrokeWidth(vg, 1.0);
	nvgStrokeColor(vg, curve->color);
	nvgLineCap(vg,NVG_BEVEL);
	nvgLineJoin(vg,NVG_BEVEL);
	for (int i = 0; i < (curve->x_data)->size(); i++)
	{
		if (i == 0) {
			nvgMoveTo(vg, (*(curve->x_data))[i] , (*(curve->y_data))[i]);
		}
		else
		{
			nvgLineTo(vg, (*(curve->x_data))[i], (*(curve->y_data))[i]);
		}
	}
	nvgStroke(vg);
	nvgRestore(vg);
}


//utilities
double kb::Utilities::ComputeDistance(double x1, double y1, double x2, double y2)
{
	double square_difference_x = (x2 - x1) * (x2 - x1);
	double square_difference_y = (y2 - y1) * (y2 - y1);
	double sum = square_difference_x + square_difference_y;
	double value = sqrt(sum);
	return value;
}

double kb::Utilities::ComputeSimilarity2D(Point2D p1, Point2D p2)
{
	double dotProduct = (p1.x * p2.x) + (p1.y * p2.y);
	double magnitudep1 = (p1.x * p1.x) + (p1.y * p1.y);
	double magnitudep2 = (p2.x * p2.x) + (p2.y * p2.y);
	double radians = (dotProduct / (sqrt(magnitudep1) * sqrt(magnitudep2))) * PI;
	return cos(radians / 180.0);
}

double kb::Utilities::ComputeSimilarity3D(Point3D p1, Point3D p2)
{
	double dotProduct = (p1.x * p2.x) + (p1.y * p2.y) + (p1.z * p2.z);
	double magnitudep1 = (p1.x * p1.x) + (p1.y * p1.y) + (p1.z * p1.z);
	double magnitudep2 = (p2.x * p2.x) + (p2.y * p2.y) + (p2.z * p2.z);
	double radians = (dotProduct / (sqrt(magnitudep1) * sqrt(magnitudep2))) * PI;
	return cos(radians / 180.0);
}

void kb::Utilities::PopulateSamplesCurve2D(std::vector<double>* x_data, std::vector<double>* y_data)
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

void kb::Utilities::MapRanges(std::vector<double>* inData, double inDataMin, double inDataMax, std::vector<double>* outData, double outDataMin, double outDataMax)
{
	double slope = 1.0 * (outDataMax - outDataMin) / (inDataMax - inDataMin);
	for (int i = 0; i < inData->size(); i++)
	{
		//double calculatedOutValue = outDataMin + slope * (inData->at(i) - inDataMin);
		outData->push_back(outDataMin + slope * (inData->at(i) - inDataMin));
		//printf("%f -> %f \n", inData->at(i), calculatedOutValue);
	}
}

void kb::Utilities::PopulateSamplesParabola2D(std::vector<double>* x_data, std::vector<double>* y_data)
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
