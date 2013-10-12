using System.Text;

namespace _2calgo.Presenter
{
    public class AlgoBuilder
    {
        private readonly string _template;

        public AlgoBuilder(string template)
        {
            _template = template;
        }

        public readonly StringBuilder Parameters = new StringBuilder();
        public readonly StringBuilder DebugActions = new StringBuilder();
        public readonly StringBuilder HandleException = new StringBuilder();
        public readonly StringBuilder References = new StringBuilder();
        public string Fields { get; set; }
        public string Mq4Functions { get; set; }
        public string Mq4Code { get; set; }
        public string AlgoName { get; set; }

        public string Levels { get; set; }
        public readonly StringBuilder LinesDeclarations = new StringBuilder();
        public readonly StringBuilder InitialzeBuffers = new StringBuilder();
        public readonly StringBuilder InitialzeAllOutputDataSeries = new StringBuilder();
        public readonly StringBuilder InvertedBuffersDeclarations = new StringBuilder();
        public readonly StringBuilder BuffersSetCurrentIndex = new StringBuilder();
        public readonly StringBuilder ColorParameters = new StringBuilder();
        public readonly StringBuilder LevelParameters = new StringBuilder();
        public readonly StringBuilder WidthParameters = new StringBuilder();
        public string IsDrawingOnChartWindow { get; set; }

        public string Build()
        {
            var code = TemplateOptimizer.RemoveUnusedCode(_template, new Words(Mq4Code));

            code = code.Replace("#AlgoName_PLACE_HOLDER#", AlgoName);
            code = code.Replace("#Parameters_PLACE_HOLDER#", Parameters.ToString());
            code = code.Replace("#Mq4Functions_PLACE_HOLDER#", Mq4Functions);
            code = code.Replace("#Mq4Fields_PLACE_HOLDER#", Fields);
            code = code.Replace("#DebugActions_PLACE_HOLDER#", DebugActions.ToString());
            code = code.Replace("#HandleException_PLACE_HOLDER#", HandleException.ToString());
            code = code.Replace("#References_PLACE_HOLDER#", References.ToString());

            code = code.Replace("#Lines_declarations_PLACE_HOLDER#", LinesDeclarations.ToString());
            code = code.Replace("#Initialize_buffers_PLACE_HOLDER#", InitialzeBuffers.ToString());
            code = code.Replace("#Initialize_AllOutputDataSeries_PLACE_HOLDER#", InitialzeAllOutputDataSeries.ToString());
            code = code.Replace("#Inverted_buffers_declarations_PLACE_HOLDER#", InvertedBuffersDeclarations.ToString());
            code = code.Replace("#Buffers_SetCurrentIndex_PLACE_HOLDER#", BuffersSetCurrentIndex.ToString());
            code = code.Replace("#ColorParameters_PLACE_HOLDER#", ColorParameters.ToString());
            code = code.Replace("#IsDrawingOnChartWindow_PLACE_HOLDER#", IsDrawingOnChartWindow);
            code = code.Replace("#Levels_PLACE_HOLDER#", Levels);
            code = code.Replace("#LevelParameters_PLACE_HOLDER#", LevelParameters.ToString());
            code = code.Replace("#WidthParameters_PLACE_HOLDER#", WidthParameters.ToString());

            return code;
        }
    }
}