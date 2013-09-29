using _2calgo.Model;
using _2calgo.Presenter.Extensions;

namespace _2calgo.Presenter
{
    public class IndicatorPresenter : AlgoPresenter
    {
        public string GenerateCodeFrom(Algo algo)
        {
            var builder = new IndicatorBuilder();
            InitializeAlgoBuilder(algo, builder);

            foreach (var parameter in algo.Parameters)
            {
                if (parameter.Type != "color")
                {
                    if (parameter.DefaultValue != null)
                        builder.Parameters.AppendLine(string.Format("[Parameter(\"{0}\", DefaultValue = {1})]", parameter.Name, parameter.DefaultValue));
                    else
                        builder.Parameters.AppendLine(string.Format("[Parameter(\"{0}\")]", parameter.Name));
                }
                builder.Parameters.AppendLine(string.Format("public {0} {1} {2}", parameter.Type, parameter.Name + "_parameter", "{ get; set; }"));
                builder.Parameters.AppendLine(string.Format("bool _{0}Got;", parameter.Name));
                builder.Parameters.AppendLine(string.Format("{0} {1}_backfield;", parameter.BackfieldType, parameter.Name));
                builder.Parameters.AppendLine(parameter.BackfieldType + " " + parameter.Name + " { get { if (!_" + parameter.Name + "Got) " + parameter.Name
                    + "_backfield = " + parameter.Name + "_parameter; return " + parameter.Name + "_backfield;	} set { " + parameter.Name + "_backfield = value; } }");
                builder.Parameters.AppendLine();
            }
            builder.ColorParameters.AppendFormat("int indicator_buffers = {0};\n", algo.BuffersCount);

            for (var index = 0; index < algo.Colors.Length; index++)
            {
                if (algo.Colors[index] != null)
                    builder.ColorParameters.AppendFormat("int indicator_color{0} = {1};\n", index + 1, algo.Colors[index]);
            }
            for (var index = 0; index < algo.Buffers.Length; index++)
            {
                var buffer = algo.Buffers[index];

                AddLineDeclaration(algo, builder, index, buffer);
                
                builder.InvertedBuffersDeclarations.AppendFormat("private Mq4OutputDataSeries {0};\n", buffer);
                builder.BuffersSetCurrentIndex.AppendFormat("{0}.SetCurrentIndex(index);\n", buffer);
                builder.InitialzeBuffers.AppendFormat("if ({0}_AlgoOutputDataSeries == null) {0}_AlgoOutputDataSeries = CreateDataSeries();\n", buffer);

                var style = algo.Styles[index];
                if (style != DrawingShapeStyle.Arrow && style != DrawingShapeStyle.Histogram && !IsLineVisible(algo, index))
                    style =  DrawingShapeStyle.None;
                
                var color = algo.Colors[index] != null ? "Colors." + algo.Colors[index] : "null";
                var lineWidth = algo.Widths[index];

                builder.InitialzeBuffers.AppendFormat("{0} = new Mq4OutputDataSeries(this, {0}_AlgoOutputDataSeries, ChartObjects, {1}, {2}, () => CreateDataSeries(), {3}, {4});\n", buffer, (int)style, index, lineWidth, color);
                builder.InitialzeBuffers.AppendFormat("AllBuffers.Add({0});\n", buffer);
            }

            builder.Levels = string.Join(", ", algo.Levels);
            for (var i = 0; i < algo.Levels.Count; i++)
            {
                builder.LevelParameters.AppendLine(string.Format("Mq4Double indicator_level{0} = {1};", i + 1,
                                                                  algo.Levels[i]));
            }
            for (var i = 0; i < algo.Widths.Length; i++)
            {
                builder.WidthParameters.AppendLine(string.Format("Mq4Double indicator_width{0} = {1};", i + 1,
                                                                  algo.Widths[i]));
            }
            builder.IsDrawingOnChartWindow = algo.IsDrawingOnChartWindow ? "true" : "false";
           
            return builder.Build();
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