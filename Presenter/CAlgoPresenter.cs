using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;
using System.Text.RegularExpressions;
using _2calgo.Model;
using _2calgo.Parser;
using _2calgo.Presenter.Extensions;

namespace _2calgo.Presenter
{
    public class CAlgoPresenter
    {
        public string GenerateCodeFrom(Algo algo)
        {
            var template = new IndicatorBuilder();
            template.Mq4Code = algo.Mq4Code;

            template.AlgoName = GetIndicatorName(algo.Mq4Code);
            foreach (var parameter in algo.Parameters)
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
            template.ColorParameters.AppendFormat("int indicator_buffers = {0};\n", algo.BuffersCount);

            for (var index = 0; index < algo.Colors.Length; index++)
            {
                if (algo.Colors[index] != null)
                    template.ColorParameters.AppendFormat("int indicator_color{0} = {1};\n", index + 1, algo.Colors[index]);
            }
            for (var index = 0; index < algo.Buffers.Length; index++)
            {
                var buffer = algo.Buffers[index];

                AddLineDeclaration(algo, template, index, buffer);
                
                template.InvertedBuffersDeclarations.AppendFormat("private Mq4OutputDataSeries {0};\n", buffer);
                template.BuffersSetCurrentIndex.AppendFormat("{0}.SetCurrentIndex(index);\n", buffer);
                template.InitialzeBuffers.AppendFormat("if ({0}_AlgoOutputDataSeries == null) {0}_AlgoOutputDataSeries = CreateDataSeries();\n", buffer);

                var style = algo.Styles[index];
                if (style != DrawingShapeStyle.Arrow && style != DrawingShapeStyle.Histogram && !IsLineVisible(algo, index))
                    style =  DrawingShapeStyle.None;
                
                var color = algo.Colors[index] != null ? "Colors." + algo.Colors[index] : "null";
                var lineWidth = algo.Widths[index];

                template.InitialzeBuffers.AppendFormat("{0} = new Mq4OutputDataSeries(this, {0}_AlgoOutputDataSeries, ChartObjects, {1}, {2}, () => CreateDataSeries(), {3}, {4});\n", buffer, (int)style, index, lineWidth, color);
                template.InitialzeBuffers.AppendFormat("AllBuffers.Add({0});\n", buffer);
            }

            template.Fields = algo.Code.FieldsDeclarations;
            template.Levels = string.Join(", ", algo.Levels);
            for (var i = 0; i < algo.Levels.Count; i++)
            {
                template.LevelParameters.AppendLine(string.Format("Mq4Double indicator_level{0} = {1};", i + 1,
                                                                  algo.Levels[i]));
            }
            for (var i = 0; i < algo.Widths.Length; i++)
            {
                template.WidthParameters.AppendLine(string.Format("Mq4Double indicator_width{0} = {1};", i + 1,
                                                                  algo.Widths[i]));
            }
            template.IsDrawingOnChartWindow = algo.IsDrawingOnChartWindow ? "true" : "false";
            template.Mq4Functions = GetFunctions(algo.Code.Functions);
            foreach (var customIndicator in algo.CustomIndicators)
            {
                template.References.AppendLine(string.Format("//#reference: {0}.algo", customIndicator));
            }
#if DEBUG
            template.DebugActions.AppendLine("Debug.Activate();");
            template.DebugActions.AppendLine("Debug.Initialize(m => Print(m));");
            template.HandleException.AppendLine(@"

			var exceptionReportDir = @""D:\2calgo"";
			Directory.CreateDirectory(exceptionReportDir);
			var fileName = Path.Combine(exceptionReportDir, ""last exception.txt"");
			File.WriteAllText(fileName, e.ToString());
            ");
#endif

            return template.Build();
        }

        private static readonly Regex IndicatorNameRegex = new Regex(@"(?<name>.*)\.mq4");
        private string GetIndicatorName(string mq4Code)
        {
            var match = IndicatorNameRegex.Match(mq4Code);
            if (!match.Success)
                return "ConvertedIndicator";

            return AlgoNameProvider.GetSimplifiedName(match.Groups["name"].Value);
        }

        private static string GetFunctions(IEnumerable<Function> functions)
        {
            var result = new StringBuilder();
            foreach (var function in functions)
            {
                var parameters = string.Join(", ", function.Parameters.Select(PresentParameter));
                result.AppendFormat("{0} {1}({2}){3}", function.ReturnType, function.Name, parameters, Environment.NewLine);
                result.AppendLine("{");
                result.Append(function.Body);
                result.AppendLine("}");
            }
            return result.ToString();
        }

        private static string PresentParameter(FunctionParameter functionParameter)
        {
            var parameter = string.Format("{0} {1}", functionParameter.Type, functionParameter.Name);
            if (functionParameter.ByReference)
                parameter = "ref " + parameter;
            if (functionParameter.DefaultValue != string.Empty)
                parameter = string.Format("{0} = {1}", parameter, functionParameter.DefaultValue);

            return parameter;
        }

        private static void AddLineDeclaration(Algo algo, IndicatorBuilder template, int bufferIndex, string bufferName)
        {
            if (algo.Styles[bufferIndex] != DrawingShapeStyle.None && IsLineVisible(algo, bufferIndex))
            {
                var colorPart = string.Empty;
                if (algo.Colors[bufferIndex] != null)
                    colorPart = ", Color = Colors." + algo.Colors[bufferIndex];

                var plotTypePart = string.Empty;
                if (algo.Styles[bufferIndex] != DrawingShapeStyle.None)
                    plotTypePart = ", PlotType = PlotType." + algo.Styles[bufferIndex].ToPlotTypeString();

                template.LinesDeclarations.AppendFormat("[Output(\"{0}\"{1}{2})]\n", bufferName, colorPart, plotTypePart);
            }
            template.LinesDeclarations.AppendFormat("public IndicatorDataSeries {0}_AlgoOutputDataSeries {1}\n", bufferName,
                                                    "{ get; set; }");

            template.InitialzeAllOutputDataSeries.AppendLine(string.Format("AllOutputDataSeries.Add({0}_AlgoOutputDataSeries);", bufferName));
        }

        private static bool IsLineVisible(Algo algo, int bufferIndex)
        {
            return algo.Colors[bufferIndex] != null 
                && algo.Styles[bufferIndex] != DrawingShapeStyle.Arrow 
                && !(algo.IsDrawingOnChartWindow && algo.Styles[bufferIndex] == DrawingShapeStyle.Histogram);
        }
    }
}