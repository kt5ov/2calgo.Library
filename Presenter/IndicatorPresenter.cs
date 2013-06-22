using System.Collections.Generic;
using System.Text;
using System.Linq;
using _2calgo.Model;
using _2calgo.Parser.CodeAdapter;
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
                
                template.InvertedBuffersDeclarations.AppendFormat("private Mq4DataSeries {0};\n", buffer);
                template.BuffersSetCurrentIndex.AppendFormat("{0}.SetCurrentIndex(index);\n", buffer);
                template.InitialzeBuffers.AppendFormat("if ({0}_Converted == null) {0}_Converted = new NormalIndicatorDataSeries();\n", buffer);
                template.InitialzeBuffers.AppendFormat("{0} = new Mq4DataSeries({0}_Converted, _closeExtremums);\n", buffer);
                template.InitialzeBuffers.AppendFormat("_allBuffers.Add({0});\n", buffer);
            }

            template.StartFunction = GetStartFunction(indicator.Code);
            template.InitFunction = GenerateInitFunction(indicator.Code);
            template.Fields = indicator.Code.FieldsDeclarations;
            template.IsDrawingOnChartWindow = indicator.IsDrawingOnChartWindow ? "true" : "false";
            template.Mq4Functions = GetFunctions(indicator.Code.Functions);

            return template.BuildIndicator();
        }

        private static string GetFunctions(IEnumerable<Function> functions)
        {
            var result = new StringBuilder();
            foreach (var function in functions.Where(f => f.Name != "start" && f.Name != "init" && f.Name != "deinit"))
            {
                result.AppendLine(function.Declaration);
                result.AppendLine("{");
                result.Append(function.Body);
                result.AppendLine("}");
            }
            return result.ToString();
        }

        private static void AddLineDeclaration(Indicator indicator, IndicatorBuilder template, int bufferIndex, string bufferName)
        {
            if (indicator.Styles[bufferIndex] != DrawingShapeStyle.None && bufferIndex < indicator.Colors.Length)
            {
                var colorPart = string.Empty;
                if (indicator.Colors[bufferIndex] != null)
                    colorPart = ", Color = Colors." + indicator.Colors[bufferIndex];

                var plotTypePart = string.Empty;
                if (indicator.Styles[bufferIndex] != DrawingShapeStyle.None)
                    plotTypePart = ", PlotType = PlotType." + indicator.Styles[bufferIndex].ToPlotTypeString();

                template.LinesDeclarations.AppendFormat("[Output(\"{0}\"{1}{2})]\n", bufferName, colorPart, plotTypePart);
            }
            template.LinesDeclarations.AppendFormat("public IndicatorDataSeries {0}_Converted {1}\n", bufferName,
                                                    "{ get; set; }");
        }

        private static string GenerateInitFunction(IndicatorCode code)
        {
            var initFunction = code.Functions.FirstOrDefault(function => function.Name == "init");
            if (initFunction == null)
                return string.Empty;

            return initFunction.Body
                .RemoveReturnStatements();
        }

        private static string GetStartFunction(IndicatorCode code)
        {
            var startFunction = code.Functions.FirstOrDefault(function => function.Name == "start");
            if (startFunction == null)
                return string.Empty;

            return startFunction.Body;
        }
    }
}