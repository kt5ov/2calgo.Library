using System.Text;

namespace _2calgo.Presenter
{
    public class IndicatorBuilder
    {
        public string Fields { get; set; }
        public string Mq4Functions { get; set; }
        public string Levels { get; set; }
        public string Mq4Code { get; set; }

        public readonly StringBuilder Parameters = new StringBuilder();
        public readonly StringBuilder LinesDeclarations = new StringBuilder();
        public readonly StringBuilder InitialzeBuffers = new StringBuilder();
        public readonly StringBuilder InitialzeAllOutputDataSeries = new StringBuilder();
        public readonly StringBuilder InvertedBuffersDeclarations = new StringBuilder();
        public readonly StringBuilder BuffersSetCurrentIndex = new StringBuilder();        
        public readonly StringBuilder ColorParameters = new StringBuilder();
        public readonly StringBuilder LevelParameters = new StringBuilder();        
        public readonly StringBuilder WidthParameters = new StringBuilder();        
        public string IsDrawingOnChartWindow { get; set; }

        public string BuildIndicator()
        {
            var code = IndicatorTemplateProvider.GetTemplate();
            code = TemplateOptimizer.RemoveUnusedCode(code, new Words(Mq4Code));

            code = code.Replace("#Parameters_PLACE_HOLDER#", Parameters.ToString());
            code = code.Replace("#Lines_declarations_PLACE_HOLDER#", LinesDeclarations.ToString());
            code = code.Replace("#Initialize_buffers_PLACE_HOLDER#", InitialzeBuffers.ToString());
            code = code.Replace("#Initialize_AllOutputDataSeries_PLACE_HOLDER#", InitialzeAllOutputDataSeries.ToString());
            code = code.Replace("#Inverted_buffers_declarations_PLACE_HOLDER#", InvertedBuffersDeclarations.ToString());
            code = code.Replace("#Buffers_SetCurrentIndex_PLACE_HOLDER#", BuffersSetCurrentIndex.ToString());
            code = code.Replace("#ColorParameters_PLACE_HOLDER#", ColorParameters.ToString());
            code = code.Replace("#Mq4Functions_PLACE_HOLDER#", Mq4Functions);

            code = code.Replace("#IsDrawingOnChartWindow_PLACE_HOLDER#", IsDrawingOnChartWindow);
            code = code.Replace("#Mq4Fields_PLACE_HOLDER#", Fields);
            code = code.Replace("#Levels_PLACE_HOLDER#", Levels);
            code = code.Replace("#LevelParameters_PLACE_HOLDER#", LevelParameters.ToString());
            code = code.Replace("#WidthParameters_PLACE_HOLDER#", WidthParameters.ToString());

            return code;
        }   
    }
}