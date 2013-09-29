using System.Text;

namespace _2calgo.Presenter
{
    public class IndicatorBuilder : AlgoBuilder
    {
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

        public override string Build()
        {
            var template = new IndicatorTemplateProvider().GetTemplate();
            var code = Build(template);

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