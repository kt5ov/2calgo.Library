using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using _2calgo.Model;
using _2calgo.Parser;
using _2calgo.Presenter.Extensions;

namespace _2calgo.Presenter
{
    public class AlgoPresenter
    {
        private readonly AlgoBuilder _algoBuilder;

        public AlgoPresenter(AlgoBuilder algoBuilder)
        {
            _algoBuilder = algoBuilder;
        }

        public string GenerateCodeFrom(Algo algo)
        {
            _algoBuilder.Mq4Code = algo.Mq4Code;
            _algoBuilder.AlgoName = GetMq4Name(algo.Mq4Code, algo.AlgoType);
            _algoBuilder.Fields = algo.Code.FieldsDeclarations;

            foreach (var parameter in algo.Parameters)
            {
                if (parameter.Type != "color")
                {
                    if (parameter.DefaultValue != null)
                        _algoBuilder.Parameters.AppendLine(string.Format("[Parameter(\"{0}\", DefaultValue = {1})]", parameter.Name, parameter.DefaultValue));
                    else
                        _algoBuilder.Parameters.AppendLine(string.Format("[Parameter(\"{0}\")]", parameter.Name));
                }
                _algoBuilder.Parameters.AppendLine(string.Format("public {0} {1} {2}", parameter.Type, parameter.Name + "_parameter", "{ get; set; }"));
                _algoBuilder.Parameters.AppendLine(string.Format("bool _{0}Got;", parameter.Name));
                _algoBuilder.Parameters.AppendLine(string.Format("{0} {1}_backfield;", parameter.BackfieldType, parameter.Name));
                _algoBuilder.Parameters.AppendLine(parameter.BackfieldType + " " + parameter.Name + " { get { if (!_" + parameter.Name + "Got) " + parameter.Name
                                              + "_backfield = " + parameter.Name + "_parameter; return " + parameter.Name + "_backfield;	} set { " + parameter.Name + "_backfield = value; } }");
                _algoBuilder.Parameters.AppendLine();
            }

            _algoBuilder.Mq4Functions = GetFunctions(algo.Code.Functions);
            foreach (var customIndicator in algo.CustomIndicators)
            {
                _algoBuilder.References.AppendLine(string.Format("//#reference: {0}.algo", customIndicator));
            }
#if DEBUG
            _algoBuilder.DebugActions.AppendLine("Debug.Activate();");
            _algoBuilder.DebugActions.AppendLine("Debug.Initialize(m => Print(m));");
            _algoBuilder.HandleException.AppendLine(@"

			var exceptionReportDir = @""D:\2calgo"";
			Directory.CreateDirectory(exceptionReportDir);
			var fileName = Path.Combine(exceptionReportDir, ""last exception.txt"");
			File.WriteAllText(fileName, e.ToString());
            ");
#endif

            _algoBuilder.ColorParameters.AppendFormat("int indicator_buffers = {0};\n", algo.BuffersCount);

            for (var index = 0; index < algo.Colors.Length; index++)
            {
                if (algo.Colors[index] != null)
                    _algoBuilder.ColorParameters.AppendFormat("int indicator_color{0} = {1};\n", index + 1, algo.Colors[index]);
            }
            for (var index = 0; index < algo.Buffers.Length; index++)
            {
                var buffer = algo.Buffers[index];

                AddLineDeclaration(algo, _algoBuilder, index, buffer);
                
                _algoBuilder.InvertedBuffersDeclarations.AppendFormat("private Mq4OutputDataSeries {0};\n", buffer);
                _algoBuilder.BuffersSetCurrentIndex.AppendFormat("{0}.SetCurrentIndex(index);\n", buffer);
                _algoBuilder.InitialzeBuffers.AppendFormat("if ({0}_AlgoOutputDataSeries == null) {0}_AlgoOutputDataSeries = CreateDataSeries();\n", buffer);

                var style = algo.Styles[index];
                if (style != DrawingShapeStyle.Arrow && style != DrawingShapeStyle.Histogram && !IsLineVisible(algo, index))
                    style =  DrawingShapeStyle.None;
                
                var color = algo.Colors[index] != null ? "Colors." + algo.Colors[index] : "null";
                var lineWidth = algo.Widths[index];

                _algoBuilder.InitialzeBuffers.AppendFormat("{0} = new Mq4OutputDataSeries(this, {0}_AlgoOutputDataSeries, ChartObjects, {1}, {2}, () => CreateDataSeries(), {3}, {4});\n", buffer, (int)style, index, lineWidth, color);
                _algoBuilder.InitialzeBuffers.AppendFormat("AllBuffers.Add({0});\n", buffer);
            }

            _algoBuilder.Levels = string.Join(", ", algo.Levels);
            for (var i = 0; i < algo.Levels.Count; i++)
            {
                _algoBuilder.LevelParameters.AppendLine(string.Format("Mq4Double indicator_level{0} = {1};", i + 1,
                                                                 algo.Levels[i]));
            }
            for (var i = 0; i < algo.Widths.Length; i++)
            {
                _algoBuilder.WidthParameters.AppendLine(string.Format("Mq4Double indicator_width{0} = {1};", i + 1,
                                                                 algo.Widths[i]));
            }
            _algoBuilder.IsDrawingOnChartWindow = algo.IsDrawingOnChartWindow ? "true" : "false";
           
            return _algoBuilder.Build();
        }

        private static readonly Regex Mq4NameRegex = new Regex(@"(?<name>.*)\.mq4");

        private static string GetMq4Name(string mq4Code, AlgoType algoType)
        {
            var match = Mq4NameRegex.Match(mq4Code);
            if (!match.Success)
                return "Converted" + algoType.ToString();

            return AlgoNameProvider.GetSimplifiedName(match.Groups["name"].Value, algoType);
        }

        private static string GetFunctions(IEnumerable<Function> functions)
        {
            var result = new StringBuilder();
            foreach (var function in functions)
            {
                var parameters = String.Join((string)", ", (IEnumerable<string>)function.Parameters.Select(PresentParameter));
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

        private static void AddLineDeclaration(Algo algo, AlgoBuilder template, int bufferIndex, string bufferName)
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