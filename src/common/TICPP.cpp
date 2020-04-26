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

//defaults


//graph
Axis* Graph::x_bottom = new Axis();
Axis* Graph::x_top = new Axis();
Axis* Graph::y_left = new Axis();
Axis* Graph::y_right = new Axis();

void kb::Graph::init(NVGcontext* vg)
{
	omnidir_axis_padding = 10.0;
	defaultColor = nvgRGBA(0, 0, 0, 200);
	defaultWidth = 1.5;
	nvgBeginPath(vg);
	nvgStrokeColor(vg, this->Border_Color);
	nvgStrokeWidth(vg, this->Border_Width);
	nvgRect(vg, this->position.x - this->Border_Padding, this->position.y - this->Border_Padding, (this->Size_X + 2 * this->Border_Padding), (this->Size_Y + 2 * this->Border_Padding));
	nvgFillPaint(vg, nvgBoxGradient(vg, this->position.x - this->Border_Padding, this->position.y - this->Border_Padding, (this->Size_X + 2 * this->Border_Padding), (this->Size_Y + 2 * this->Border_Padding), 1.0, 20, nvgRGBA(255, 255, 255, 150), nvgRGBA(20, 56, 56, 200)));
	nvgFill(vg);
	nvgStroke(vg);

	//nvgSave(vg);
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
	switch (this->Label_Position) {
	case(Position::Left):
		nvgBeginPath(vg);
		nvgFontSize(vg, 18.0);
		nvgFontFace(vg, "sans_Mono_Medium");
		nvgFillColor(vg, defaultColor);
		nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
		nvgText(vg, this->position.x, -omnidir_axis_padding - this->Border_Label_Padding + this->position.y, this->Label, NULL);
		nvgFill(vg);
		break;
	case(Position::Centre):
		nvgBeginPath(vg);
		nvgFontSize(vg, 19.0);
		nvgFontFace(vg, "sans_Mono_Medium");
		nvgFillColor(vg, defaultColor);
		nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
		nvgText(vg, 0.3 * this->Size_X + this->position.x, -this->Border_Label_Padding + this->position.y, this->Label, NULL);
		nvgFill(vg);
		break;
	case(Position::Right):
		nvgBeginPath(vg);
		nvgFontSize(vg, 18.0);
		nvgFontFace(vg, "sans_Mono_Medium");
		nvgFillColor(vg, defaultColor);
		nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
		nvgText(vg, 0.65 * this->Size_X + this->position.x, -omnidir_axis_padding - this->Border_Label_Padding + this->position.y, this->Label, NULL);
		nvgFill(vg);
		break;
	default:
		nvgBeginPath(vg);
		nvgFontSize(vg, 18.0);
		nvgFontFace(vg, "sans_Mono_Medium");
		nvgFillColor(vg, defaultColor);
		nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
		nvgText(vg, 0.35 * this->Size_X + this->position.x, -omnidir_axis_padding - this->Border_Label_Padding + this->position.y, this->Label, NULL);
		nvgFill(vg);
		break;
	}

	//nvgRestore(vg);

}

bool kb::Graph::DrawAxes(NVGcontext* vg, Axis* axisObject)
{
	switch (axisObject->axisType)
	{
	case(AxisType::X_Top):
		this->x_top = axisObject;
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

			std::vector<double> x_values, y_values, scaledRanges_x, scaledRanges_y;
			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				x_values.push_back((i * scale) + this->position.x);
				y_values.push_back(this->position.y);
			}
			MapRanges(&x_values, *std::min_element(x_values.begin(), x_values.end()), *std::max_element(x_values.begin(), x_values.end()), &scaledRanges_x, this->position.x + 1.5 + omnidir_axis_padding, (this->position.x + this->Size_X) - 1.5 - omnidir_axis_padding);
			MapRanges(&y_values, *std::min_element(y_values.begin(), y_values.end()), *std::max_element(y_values.begin(), y_values.end()), &scaledRanges_y, this->position.y, this->Size_Y + this->position.y + omnidir_axis_padding);

			// write in code to plot axis values according to top and bottom
			for (int i = (axisObject->axisvalues)->size() - 1; i >= 0; i--)
			{
				int x_padding = strnlen((axisObject->axisvalues)->at(i).c_str(), 5) * (7.0 / 3.0);
				nvgBeginPath(vg);
				nvgMoveTo(vg, scaledRanges_x.at(i), scaledRanges_y.at(i) - 4.0);
				nvgLineTo(vg, scaledRanges_x.at(i), scaledRanges_y.at(i) + 4.0);
				nvgStroke(vg);
				nvgBeginPath(vg);
				nvgFontSize(vg, 12.0f);
				nvgFillColor(vg, axisObject->color);
				nvgFontFace(vg, "sans_Mono_Medium");
				nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
				int text_index = abs((int)(i - ((axisObject->axisvalues)->size() - 1)));
				nvgText(vg, scaledRanges_x.at(i) - x_padding, scaledRanges_y.at(i) - y_padding, (axisObject->axisvalues)->at(text_index).c_str(), NULL);
				nvgFill(vg);
			}
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

			std::vector<double> x_values, y_values, scaledRanges_x, scaledRanges_y;
			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				x_values.push_back((i * scale) + this->position.x);
				y_values.push_back(this->position.y);
			}
			MapRanges(&x_values, *std::min_element(x_values.begin(), x_values.end()), *std::max_element(x_values.begin(), x_values.end()), &scaledRanges_x, this->position.x + 1.5 + omnidir_axis_padding, (this->position.x + this->Size_X) - 1.5 - omnidir_axis_padding);
			MapRanges(&y_values, *std::min_element(y_values.begin(), y_values.end()), *std::max_element(y_values.begin(), y_values.end()), &scaledRanges_y, this->position.y, this->Size_Y + this->position.y + omnidir_axis_padding);

			// write in code to plot axis values according to top and bottom
			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				int x_padding = strnlen((axisObject->axisvalues)->at(i).c_str(), 5) * (7.0 / 3.0);
				nvgBeginPath(vg);
				nvgMoveTo(vg, scaledRanges_x.at(i), scaledRanges_y.at(i) - 4.0);
				nvgLineTo(vg, scaledRanges_x.at(i), scaledRanges_y.at(i) + 4.0);
				nvgStroke(vg);
				nvgBeginPath(vg);
				nvgFontSize(vg, 12.0f);
				nvgFillColor(vg, axisObject->color);
				nvgFontFace(vg, "sans_Mono_Medium");
				nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
				nvgText(vg, scaledRanges_x.at(i) - x_padding, scaledRanges_y.at(i) - y_padding, (axisObject->axisvalues)->at(i).c_str(), NULL);
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
		this->x_bottom = axisObject;
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
			nvgStroke(vg);

			// code to mark axis label according to top and bottom
			int scale = (this->Size_X) / (axisObject->axisvalues)->size();
			int y_padding = (this->Size_Y * 0.021);
			nvgBeginPath(vg);
			nvgFontSize(vg, 15.0);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, ((start_x + end_x) / 2) - 15.0, (start_y + (2.5 * y_padding)), axisObject->AxisLabelText, NULL);
			nvgFill(vg);

			std::vector<double> x_values, y_values, scaledRanges_x, scaledRanges_y;
			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				x_values.push_back((i * scale) + this->position.x);
				y_values.push_back(this->Size_Y + this->position.y);
			}
			MapRanges(&x_values, *std::min_element(x_values.begin(), x_values.end()), *std::max_element(x_values.begin(), x_values.end()), &scaledRanges_x, this->position.x + 1.5 + omnidir_axis_padding, (this->position.x + this->Size_X) - 1.5 - omnidir_axis_padding);
			MapRanges(&y_values, *std::min_element(y_values.begin(), y_values.end()), *std::max_element(y_values.begin(), y_values.end()), &scaledRanges_y, this->Size_Y + this->position.y, this->position.y + omnidir_axis_padding);

			// write in code to plot axis values according to top and bottom
			for (int i = (axisObject->axisvalues)->size() - 1; i >= 0; i--)
			{
				int x_padding = strnlen((axisObject->axisvalues)->at(i).c_str(), 5) * (7.0 / 3.0);
				nvgBeginPath(vg);
				nvgMoveTo(vg, scaledRanges_x.at(i), scaledRanges_y.at(i) - 4.0);
				nvgLineTo(vg, scaledRanges_x.at(i), scaledRanges_y.at(i) + 4.0);
				nvgStroke(vg);
				nvgBeginPath(vg);
				nvgFontSize(vg, 12.0f);
				nvgFillColor(vg, axisObject->color);
				nvgFontFace(vg, "sans_Mono_Medium");
				nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
				int text_index = abs((int)(i - ((axisObject->axisvalues)->size() - 1)));
				nvgText(vg, scaledRanges_x.at(i) - x_padding, scaledRanges_y.at(i) + y_padding, (axisObject->axisvalues)->at(text_index).c_str(), NULL);
				nvgFill(vg);
			}

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
			nvgStroke(vg);

			// code to mark axis label according to top and bottom
			int scale = (this->Size_X) / (axisObject->axisvalues)->size();
			int y_padding = (this->Size_Y * 0.021);
			nvgBeginPath(vg);
			nvgFontSize(vg, 15.0f);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, ((start_x + end_x) / 2) - 15.0, (start_y + (2.5 * y_padding)), axisObject->AxisLabelText, NULL);
			nvgFill(vg);

			std::vector<double> x_values, y_values, scaledRanges_x, scaledRanges_y;
			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				x_values.push_back((i * scale) + this->position.x);
				y_values.push_back(this->Size_Y + this->position.y);
			}
			MapRanges(&x_values, *std::min_element(x_values.begin(), x_values.end()), *std::max_element(x_values.begin(), x_values.end()), &scaledRanges_x, this->position.x + 1.5 + omnidir_axis_padding, (this->position.x + this->Size_X) - 1.5 - omnidir_axis_padding);
			MapRanges(&y_values, *std::min_element(y_values.begin(), y_values.end()), *std::max_element(y_values.begin(), y_values.end()), &scaledRanges_y, this->Size_Y + this->position.y, this->position.y + omnidir_axis_padding);

			// write in code to plot axis values according to top and bottom
			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				int x_padding = strnlen((axisObject->axisvalues)->at(i).c_str(), 5) * (7.0 / 3.0);
				nvgBeginPath(vg);
				nvgMoveTo(vg, scaledRanges_x.at(i), scaledRanges_y.at(i) - 4.0);
				nvgLineTo(vg, scaledRanges_x.at(i), scaledRanges_y.at(i) + 4.0);
				nvgStroke(vg);
				nvgBeginPath(vg);
				nvgFontSize(vg, 12.0f);
				nvgFillColor(vg, axisObject->color);
				nvgFontFace(vg, "sans_Mono_Medium");
				nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
				//nvgText(vg, ((1.0 * i * scale) + this->position.x) - 2.0 , this->position.y - 5.0 , std::to_string((axisObject->axisvalues)->at(i)).c_str(), NULL);
				nvgText(vg, scaledRanges_x.at(i) - x_padding, scaledRanges_y.at(i) + y_padding, (axisObject->axisvalues)->at(i).c_str(), NULL);
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
	case(AxisType::Y_Left):
		this->y_left = axisObject;
		if (axisObject->isReversed == false)
		{
			double x_start = this->position.x;
			double x_end = this->position.x;

			double y_start = this->position.y + this->Size_Y;
			double y_end = this->position.y;

			nvgBeginPath(vg);
			nvgStrokeColor(vg, axisObject->color);
			nvgStrokeWidth(vg, axisObject->stroke_width);
			nvgMoveTo(vg, x_start, y_start);
			nvgLineTo(vg, x_end, y_end);
			nvgLineTo(vg, x_end + (axisObject->arrowScale / 2.0), y_end);
			nvgLineTo(vg, x_end, y_end - (axisObject->arrowScale));
			nvgLineTo(vg, x_end - (axisObject->arrowScale / 2.0), y_end);
			nvgLineTo(vg, x_end, y_end);
			nvgStroke(vg);

			// code to mark axis label according to top and bottom
			int scale = (this->Size_Y) / (axisObject->axisvalues)->size();
			int x_padding = 1.8 * (this->Size_X * 0.021);
			nvgSave(vg);
			nvgRotate(vg, -1.570796);
			nvgBeginPath(vg);
			nvgFontSize(vg, 15.0f);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, -((y_start + y_end) / 1.75F), (x_start - axisObject->labelMargin - x_padding), axisObject->AxisLabelText, NULL);
			nvgFill(vg);
			nvgRestore(vg);

			std::vector<double> x_values, y_values, scaledRanges_x, scaledRanges_y, x1_padding, scaled_x1;
			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				x_values.push_back(this->position.x);
				y_values.push_back(this->Size_Y + this->position.y - (i * scale));
				x1_padding.push_back(strnlen((axisObject->axisvalues)->at(i).c_str(), 5));
			}
			MapRanges(&x_values, *std::min_element(x_values.begin(), x_values.end()), *std::max_element(x_values.begin(), x_values.end()), &scaledRanges_x, this->position.x, this->position.x - omnidir_axis_padding);
			MapRanges(&y_values, *std::min_element(y_values.begin(), y_values.end()), *std::max_element(y_values.begin(), y_values.end()), &scaledRanges_y, this->position.y + omnidir_axis_padding, this->Size_Y + this->position.y - omnidir_axis_padding);
			MapRanges(&x1_padding, *std::min_element(x1_padding.begin(), x1_padding.end()), *std::max_element(x1_padding.begin(), x1_padding.end()), &scaled_x1, 12, 20);

			// write in code to plot axis values according to left and Right
			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				//int x1_padding = (10.0 * strnlen((axisObject->axisvalues)->at(i).c_str(), 5));
				nvgBeginPath(vg);
				nvgMoveTo(vg, scaledRanges_x.at(i) - 4.0, scaledRanges_y.at(i));
				nvgLineTo(vg, scaledRanges_x.at(i) + 4.0, scaledRanges_y.at(i));
				nvgStroke(vg);
				nvgBeginPath(vg);
				nvgFontSize(vg, 12.0f);
				nvgFillColor(vg, axisObject->color);
				nvgFontFace(vg, "sans_Mono_Medium");
				nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
				//nvgText(vg, ((1.0 * i * scale) + this->position.x) - 2.0 , this->position.y - 5.0 , std::to_string((axisObject->axisvalues)->at(i)).c_str(), NULL);
				nvgText(vg, scaledRanges_x.at(i) - scaled_x1.at(i), scaledRanges_y.at(i), (axisObject->axisvalues)->at(i).c_str(), NULL);
				nvgFill(vg);
			}


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
			nvgStroke(vg);

			// code to mark axis label according to top and bottom
			int scale = (this->Size_Y) / (axisObject->axisvalues)->size();
			int x_padding = 1.8 * (this->Size_X * 0.021);
			nvgSave(vg);
			nvgRotate(vg, -1.570796);
			nvgBeginPath(vg);
			nvgFontSize(vg, 15.0f);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, -((y_start + y_end) / 1.75F), (x_start - axisObject->labelMargin - x_padding), axisObject->AxisLabelText, NULL);
			nvgFill(vg);
			nvgRestore(vg);

			std::vector<double> x_values, y_values, scaledRanges_x, scaledRanges_y, x1_padding, scaled_x1;
			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				x_values.push_back(this->position.x);
				y_values.push_back(this->Size_Y + this->position.y - (i * scale));
				x1_padding.push_back(strnlen((axisObject->axisvalues)->at(i).c_str(), 5));
			}
			MapRanges(&x_values, *std::min_element(x_values.begin(), x_values.end()), *std::max_element(x_values.begin(), x_values.end()), &scaledRanges_x, this->position.x, this->position.x - omnidir_axis_padding);
			MapRanges(&y_values, *std::min_element(y_values.begin(), y_values.end()), *std::max_element(y_values.begin(), y_values.end()), &scaledRanges_y, this->Size_Y + this->position.y - omnidir_axis_padding, this->position.y + omnidir_axis_padding);
			MapRanges(&x1_padding, *std::min_element(x1_padding.begin(), x1_padding.end()), *std::max_element(x1_padding.begin(), x1_padding.end()), &scaled_x1, 12, 20);

			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				nvgBeginPath(vg);
				nvgMoveTo(vg, scaledRanges_x.at(i) - 4.0, scaledRanges_y.at(i));
				nvgLineTo(vg, scaledRanges_x.at(i) + 4.0, scaledRanges_y.at(i));
				nvgStroke(vg);
				nvgBeginPath(vg);
				nvgFontSize(vg, 12.0f);
				nvgFillColor(vg, axisObject->color);
				nvgFontFace(vg, "sans_Mono_Medium");
				nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
				//int text_index = abs((int)(i - ((axisObject->axisvalues)->size() - 1)));
				nvgText(vg, scaledRanges_x.at(i) - scaled_x1.at(i), scaledRanges_y.at(i), (axisObject->axisvalues)->at(i).c_str(), NULL);
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
	case(AxisType::Y_Right):
		this->y_right = axisObject;
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
			nvgStroke(vg);

			// code to mark axis label according to left and right
			int scale = (this->Size_Y) / (axisObject->axisvalues)->size();
			int x_padding = 1.8 * (this->Size_X * 0.021);
			nvgSave(vg);
			nvgRotate(vg, 1.570796);
			nvgBeginPath(vg);
			nvgFontSize(vg, 15.0);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, ((y_start + y_end) / 2.25F), -(x_start + axisObject->labelMargin + x_padding), axisObject->AxisLabelText, NULL);
			nvgFill(vg);
			nvgRestore(vg);

			std::vector<double> x_values, y_values, scaledRanges_x, scaledRanges_y, x1_padding/* ,scaled_x1*/;
			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				x_values.push_back(this->position.x + this->Size_X);
				y_values.push_back(this->Size_Y + this->position.y - (i * scale));
				//x1_padding.push_back(strnlen((axisObject->axisvalues)->at(i).c_str(), 5));
			}
			MapRanges(&x_values, *std::min_element(x_values.begin(), x_values.end()), *std::max_element(x_values.begin(), x_values.end()), &scaledRanges_x, this->position.x + this->Size_X , this->position.x + this->Size_X );
			MapRanges(&y_values, *std::min_element(y_values.begin(), y_values.end()), *std::max_element(y_values.begin(), y_values.end()), &scaledRanges_y, this->position.y + omnidir_axis_padding, this->Size_Y + this->position.y - omnidir_axis_padding);
			//MapRanges(&x1_padding, *std::min_element(x1_padding.begin(), x1_padding.end()), *std::max_element(x1_padding.begin(), x1_padding.end()), &scaled_x1, 12, 20);

			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				nvgBeginPath(vg);
				nvgMoveTo(vg, scaledRanges_x.at(i) - 4.0, scaledRanges_y.at(i));
				nvgLineTo(vg, scaledRanges_x.at(i) + 4.0, scaledRanges_y.at(i));
				nvgStroke(vg);
				nvgBeginPath(vg);
				nvgFontSize(vg, 12.0f);
				nvgFillColor(vg, axisObject->color);
				nvgFontFace(vg, "sans_Mono_Medium");
				nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
				//int text_index = abs((int)(i - ((axisObject->axisvalues)->size() - 1)));
				nvgText(vg, scaledRanges_x.at(i) + ( omnidir_axis_padding / 2.0 )/*scaled_x1.at(i)*/, scaledRanges_y.at(i), (axisObject->axisvalues)->at(i).c_str(), NULL);
				nvgFill(vg);
			}
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
			nvgStroke(vg);

			// code to mark axis label according to left and right
			int scale = (this->Size_Y) / (axisObject->axisvalues)->size();
			int x_padding = 1.8 * (this->Size_X * 0.021);
			nvgSave(vg);
			nvgRotate(vg, 1.570796);
			nvgBeginPath(vg);
			nvgFontSize(vg, 15.0f);
			nvgFontFace(vg, "sans_Mono_Medium");
			nvgFillColor(vg, axisObject->color);
			nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
			nvgText(vg, ((y_start + y_end) / 2.25F), -(x_start + axisObject->labelMargin + x_padding), axisObject->AxisLabelText, NULL);
			nvgFill(vg);
			nvgRestore(vg);

			std::vector<double> x_values, y_values, scaledRanges_x, scaledRanges_y, x1_padding/* ,scaled_x1*/;
			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				x_values.push_back(this->position.x + this->Size_X);
				y_values.push_back(this->Size_Y + this->position.y - (i * scale));
				//x1_padding.push_back(strnlen((axisObject->axisvalues)->at(i).c_str(), 5));
			}
			MapRanges(&x_values, *std::min_element(x_values.begin(), x_values.end()), *std::max_element(x_values.begin(), x_values.end()), &scaledRanges_x, this->position.x + this->Size_X, this->position.x + this->Size_X);
			MapRanges(&y_values, *std::min_element(y_values.begin(), y_values.end()), *std::max_element(y_values.begin(), y_values.end()), &scaledRanges_y,  this->Size_Y + this->position.y - omnidir_axis_padding, this->position.y + omnidir_axis_padding);
			//MapRanges(&x1_padding, *std::min_element(x1_padding.begin(), x1_padding.end()), *std::max_element(x1_padding.begin(), x1_padding.end()), &scaled_x1, 12, 20);

			for (int i = 0; i < (axisObject->axisvalues)->size(); i++)
			{
				nvgBeginPath(vg);
				nvgMoveTo(vg, scaledRanges_x.at(i) - 4.0, scaledRanges_y.at(i));
				nvgLineTo(vg, scaledRanges_x.at(i) + 4.0, scaledRanges_y.at(i));
				nvgStroke(vg);
				nvgBeginPath(vg);
				nvgFontSize(vg, 12.0f);
				nvgFillColor(vg, axisObject->color);
				nvgFontFace(vg, "sans_Mono_Medium");
				nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_MIDDLE);
				//int text_index = abs((int)(i - ((axisObject->axisvalues)->size() - 1)));
				nvgText(vg, scaledRanges_x.at(i) + (omnidir_axis_padding / 2.0)/*scaled_x1.at(i)*/, scaledRanges_y.at(i), (axisObject->axisvalues)->at(i).c_str(), NULL);
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
	default:
		break;
	}


}

void kb::Graph::DrawCurve(NVGcontext* vg, Curve* curve)
{
	nvgSave(vg);
	std::vector<double> scaledRanges_x, scaledRanges_y;
	//mapping values from input data to graph area 
	// to fix translation is done explicitly via offsets... incorrect :: can be seen as scrolling wrt default x , y axis with 0,0 at left bottom
	MapRanges(curve->x_data, *std::min_element((*(curve->x_data)).begin(), (*(curve->x_data)).end()), *std::max_element((*(curve->x_data)).begin(), (*(curve->x_data)).end()), &scaledRanges_x, this->position.x + 1.5 + curve->x_offset + omnidir_axis_padding, (this->position.x + this->Size_X) - 1.5 - omnidir_axis_padding);
	MapRanges(curve->y_data, *std::min_element((*(curve->y_data)).begin(), (*(curve->y_data)).end()), *std::max_element((*(curve->y_data)).begin(), (*(curve->y_data)).end()), &scaledRanges_y, this->Size_Y + this->position.y - 1.5 - curve->y_offset - omnidir_axis_padding, this->position.y + 1.5 + omnidir_axis_padding);
	switch (curve->LineType)
	{
	case(LineType::Line_Default):
		nvgBeginPath(vg);
		nvgStrokeWidth(vg, 1.0);
		nvgStrokeColor(vg, curve->color);
		nvgLineCap(vg, NVG_BEVEL);
		nvgLineJoin(vg, NVG_BEVEL);
		for (int i = 0; i < scaledRanges_x.size(); i++)
		{
			if (i == 0) {
				nvgMoveTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			}
			else
			{
				nvgLineTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			}
		}
		nvgStroke(vg);
		break;
	case(LineType::Dotted_Line):
		for (int i = 0; i < scaledRanges_x.size(); i++)
		{
			nvgBeginPath(vg);
			nvgFillColor(vg, curve->color);
			nvgCircle(vg, scaledRanges_x[i], scaledRanges_y[i], 1.5f);
			nvgFill(vg);
		}
		break;
	case(LineType::Impulse_Line_X_Bottom):
		for (int i = 0; i < scaledRanges_x.size(); i++)
		{
			nvgBeginPath(vg);
			nvgFillColor(vg, curve->color);
			nvgCircle(vg, scaledRanges_x[i], scaledRanges_y[i], 1.5f);
			nvgFill(vg);

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, 0.8f);
			nvgStrokeColor(vg, curve->color);
			nvgMoveTo(vg, scaledRanges_x[i], this->position.y + this->Size_Y - omnidir_axis_padding);
			nvgLineTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			nvgStroke(vg);
		}
		break;
	case(LineType::Impulse_Line_X_Top):
		for (int i = 0; i < scaledRanges_x.size(); i++)
		{
			nvgBeginPath(vg);
			nvgFillColor(vg, curve->color);
			nvgCircle(vg, scaledRanges_x[i], scaledRanges_y[i], 1.5f);
			nvgFill(vg);

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, 0.8f);
			nvgStrokeColor(vg, curve->color);
			nvgMoveTo(vg, scaledRanges_x[i], this->position.y);
			nvgLineTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			nvgStroke(vg);
		}
		break;
	case(LineType::Impulse_Line_Y_Left):
		for (int i = 0; i < scaledRanges_x.size(); i++)
		{
			nvgBeginPath(vg);
			nvgFillColor(vg, curve->color);
			nvgCircle(vg, scaledRanges_x[i], scaledRanges_y[i], 1.5f);
			nvgFill(vg);

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, 0.8f);
			nvgStrokeColor(vg, curve->color);
			nvgMoveTo(vg, this->position.x, scaledRanges_y[i]);
			nvgLineTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			nvgStroke(vg);
		}
		break;
	case(LineType::Impulse_Line_Y_Right):
		for (int i = 0; i < scaledRanges_x.size(); i++)
		{
			nvgBeginPath(vg);
			nvgFillColor(vg, curve->color);
			nvgCircle(vg, scaledRanges_x[i], scaledRanges_y[i], 1.5f);
			nvgFill(vg);

			nvgBeginPath(vg);
			nvgStrokeWidth(vg, 0.8f);
			nvgStrokeColor(vg, curve->color);
			nvgMoveTo(vg, this->position.x + this->Size_X, scaledRanges_y[i]);
			nvgLineTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			nvgStroke(vg);
		}
		break;
	case(LineType::Step_Right_Line):
		nvgBeginPath(vg);
		nvgStrokeWidth(vg, 1.0);
		nvgStrokeColor(vg, curve->color);
		nvgLineCap(vg, NVG_BEVEL);
		nvgLineJoin(vg, NVG_BEVEL);
		for (int i = 0; i < scaledRanges_x.size(); i++)
		{
			if (i == 0) {
				nvgMoveTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			}
			else
			{
				nvgLineTo(vg, scaledRanges_x[i], scaledRanges_y[i - 1]);
				nvgLineTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			}
		}
		nvgStroke(vg);
		break;
	case(LineType::Step_Left_Line):
		nvgBeginPath(vg);
		nvgStrokeWidth(vg, 1.0);
		nvgStrokeColor(vg, curve->color);
		nvgLineCap(vg, NVG_BEVEL);
		nvgLineJoin(vg, NVG_BEVEL);
		for (int i = 0; i < scaledRanges_x.size(); i++)
		{
			if (i == 0) {
				nvgMoveTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			}
			else
			{
				nvgLineTo(vg, scaledRanges_x[i - 1], scaledRanges_y[i]);
				nvgLineTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			}
		}
		nvgStroke(vg);
		break;
	case(LineType::Step_Centre_Line):
		nvgBeginPath(vg);
		nvgStrokeWidth(vg, 1.0);
		nvgStrokeColor(vg, curve->color);
		nvgLineCap(vg, NVG_BEVEL);
		nvgLineJoin(vg, NVG_BEVEL);
		for (int i = 0; i < scaledRanges_x.size(); i++)
		{
			if (i == 0) {
				nvgMoveTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			}
			else
			{
				nvgLineTo(vg, (scaledRanges_x[i - 1] + scaledRanges_x[i]) / 2, scaledRanges_y[i - 1]);
				nvgLineTo(vg, (scaledRanges_x[i - 1] + scaledRanges_x[i]) / 2, scaledRanges_y[i]);
				nvgLineTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
			}
		}
		nvgStroke(vg);
		break;
	case(LineType::Dashed_Line):
		int no_of_dashes = 0 + (scaledRanges_x.size() * 0.05);
		int values_each_dash = (scaledRanges_x.size() / no_of_dashes);
		for (int dash = 0; dash < no_of_dashes; dash++)
		{
			bool flag = true;
			nvgBeginPath(vg);
			nvgStrokeWidth(vg, 1.0);
			nvgStrokeColor(vg, curve->color);
			nvgLineCap(vg, NVG_BEVEL);
			nvgLineJoin(vg, NVG_BEVEL);
			for (int i = (dash * values_each_dash); i < values_each_dash * (dash + 1) % (scaledRanges_x.size()); i++)
			{
				if (flag == true)
				{
					nvgMoveTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
					flag = false;
				}
				else {
					nvgLineTo(vg, scaledRanges_x[i], scaledRanges_y[i]);
				}
			}
			nvgStroke(vg);
			flag = true;
		}
		break;
	}
	nvgRestore(vg);
}

void kb::Graph::DrawHistogram(NVGcontext* vg, Histogram* histogram)
{
	nvgSave(vg);
	std::vector<int> keys = histogram->getKeys();
	std::vector<double> x_values, y_values;
	for (int i = 0; i < keys.size(); i++)
	{
		x_values.push_back(histogram->getData(keys[i]).first);
		y_values.push_back(histogram->getData(keys[i]).second);
	}
	std::vector<double> scaledRanges_x, scaledRanges_y;
	int size = histogram->Bar_Width / 2;
	int base = this->position.y + this->Size_Y - omnidir_axis_padding;
	MapRanges(&x_values, *std::min_element(x_values.begin(), x_values.end()), *std::max_element(x_values.begin(), x_values.end()), &scaledRanges_x, this->position.x + 1.5 + omnidir_axis_padding, (this->position.x + this->Size_X) - 1.5 - omnidir_axis_padding);
	MapRanges(&y_values, *std::min_element(y_values.begin(), y_values.end()), *std::max_element(y_values.begin(), y_values.end()), &scaledRanges_y, this->Size_Y + this->position.y - 1.5 - omnidir_axis_padding, this->position.y + 1.5 + omnidir_axis_padding);

	for (int i = 0; i < scaledRanges_x.size(); i++)
	{
		nvgBeginPath(vg);
		nvgStrokeColor(vg, histogram->Bar_Color);
		nvgRoundedRect(vg, scaledRanges_x.at(i) - size, base, histogram->Bar_Width, -(base - scaledRanges_y.at(i)), 1.0f);
		nvgFillColor(vg, histogram->Bar_Color);
		nvgFill(vg);
		nvgStroke(vg);
	}
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
	double slope;
	if (inDataMax - inDataMin == 0)
	{
		slope = 0;
	}
	else
	{
		slope = 1.0 * (outDataMax - outDataMin) / (inDataMax - inDataMin);
	}

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


//Histogram
std::vector<int> kb::Histogram::getKeys()
{
	int i = 0;
	std::vector<int> keys;
	while (bars.find(i) != bars.end())
	{
		keys.push_back((*bars.find(i)).first);
		i++;
	}
	return keys;
}

std::pair<double, double> kb::Histogram::getData(int key)
{
	return (*bars.find(key)).second;
}

void kb::Histogram::setData(std::vector<double> x_inData, std::vector<double> y_inData)
{
	this->generateBars(&x_inData, &y_inData);
}

void kb::Histogram::addData(int key, double value)
{
	//not implemented
}


//Bar
void kb::Bar::generateBars(std::vector<double>* x_Data, std::vector<double>* y_Data)
{
	for (int i = 0; i < x_Data->size(); i++)
	{
		bars[i] = std::make_pair(x_Data->at(i), y_Data->at(i));
	}
}
