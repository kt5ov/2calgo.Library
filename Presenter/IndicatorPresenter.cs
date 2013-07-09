using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using _2calgo.Model;
using _2calgo.Presenter.Extensions;

namespace _2calgo.Presenter
{
    public class IndicatorPresenter
    {
        public string GenerateCodeFrom(Indicator indicator)
        {
            var template = new IndicatorBuilder();

            foreach (var parameter in indicator.Parameters)
            {
                if (parameter.Type != "color")
                {
                    if (parameter.DefaultValue != null)
                        template.Parameters.AppendLine(string.Format("[Parameter(\"{0}\", DefaultValue = {1})]", parameter.Name, parameter.DefaultValue));
                    else
                        template.Parameters.AppendLine(string.Format("[Parameter(\"{0}\")]", parameter.Name));
                }
                template.Parameters.AppendLine(string.Format("public {0} {1} {2}", parameter.Type, parameter.Name + "_parameter", "{ get; set; }"));
                template.Parameters.AppendLine(string.Format("bool _{0}Got;", parameter.Name));
                template.Parameters.AppendLine(string.Format("{0} {1}_backfield;", parameter.BackfieldType, parameter.Name));
                template.Parameters.AppendLine(parameter.BackfieldType + " " + parameter.Name + " { get { if (!_" + parameter.Name + "Got) " + parameter.Name
                    + "_backfield = " + parameter.Name + "_parameter; return " + parameter.Name + "_backfield;	} set { " + parameter.Name + "_backfield = value; } }");
                template.Parameters.AppendLine();
            }

            for (var index = 0; index < indicator.Buffers.Length; index++)
            {
                var buffer = indicator.Buffers[index];

                AddLineDeclaration(indicator, template, index, buffer);
                
                template.InvertedBuffersDeclarations.AppendFormat("private Mq4OutputDataSeries {0};\n", buffer);
                template.BuffersSetCurrentIndex.AppendFormat("{0}.SetCurrentIndex(index);\n", buffer);
                template.InitialzeBuffers.AppendFormat("if ({0}_AlgoOutputDataSeries == null) {0}_AlgoOutputDataSeries = new IndicatorDataSeries();\n", buffer);

                var style = indicator.Styles[index];
                if (style != DrawingShapeStyle.Arrow && !IsLineVisible(indicator, index))
                    style =  DrawingShapeStyle.None;
                
                var color = indicator.Colors[index] != null ? "Colors." + indicator.Colors[index] : "null";

                template.InitialzeBuffers.AppendFormat("{0} = new Mq4OutputDataSeries(this, {0}_AlgoOutputDataSeries, _closeExtremums, ChartObjects, {1}, {2}, {3});\n", buffer, (int)style, index, color);
                template.InitialzeBuffers.AppendFormat("_allBuffers.Add({0});\n", buffer);
            }

            template.Fields = indicator.Code.FieldsDeclarations;
            template.Levels = string.Join(", ", indicator.Levels);
            template.IsDrawingOnChartWindow = indicator.IsDrawingOnChartWindow ? "true" : "false";
            template.Mq4Functions = GetFunctions(indicator.Code.Functions);

            return template.BuildIndicator();
        }

        private static string GetFunctions(IEnumerable<Function> functions)
        {
            var result = new StringBuilder();
            foreach (var function in functions.Where(f => f.Name != "deinit"))
            {
                var parameters = string.Join(", ", function.Parameters);
                result.AppendFormat("{0} {1}({2}){3}", function.ReturnType, function.Name, parameters, Environment.NewLine);
                result.AppendLine("{");
                result.Append(function.Body);
                result.AppendLine("}");
            }
            return result.ToString();
        }

        private static void AddLineDeclaration(Indicator indicator, IndicatorBuilder template, int bufferIndex, string bufferName)
        {
            if (indicator.Styles[bufferIndex] != DrawingShapeStyle.None && IsLineVisible(indicator, bufferIndex))
            {
                var colorPart = string.Empty;
                if (indicator.Colors[bufferIndex] != null)
                    colorPart = ", Color = Colors." + indicator.Colors[bufferIndex];

                var plotTypePart = string.Empty;
                if (indicator.Styles[bufferIndex] != DrawingShapeStyle.None)
                    plotTypePart = ", PlotType = PlotType." + indicator.Styles[bufferIndex].ToPlotTypeString();

                template.LinesDeclarations.AppendFormat("[Output(\"{0}\"{1}{2})]\n", bufferName, colorPart, plotTypePart);
            }
            template.LinesDeclarations.AppendFormat("public cAlgo.API.IndicatorDataSeries {0}_AlgoOutputDataSeries {1}\n", bufferName,
                                                    "{ get; set; }");
        }

        private static bool IsLineVisible(Indicator indicator, int bufferIndex)
        {
            return indicator.Colors[bufferIndex] != null && indicator.Styles[bufferIndex] != DrawingShapeStyle.Arrow;
        }
    }
}