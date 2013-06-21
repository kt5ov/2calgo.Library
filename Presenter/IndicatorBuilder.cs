using System.Text;

namespace _2calgo.Presenter
{
    public class IndicatorBuilder
    {
        public string StartFunction { get; set; }
        public string InitFunction { get; set; }
        public string Fields { get; set; }
        public string Mq4Functions { get; set; }

        public readonly StringBuilder Parameters = new StringBuilder();
        public readonly StringBuilder LinesDeclarations = new StringBuilder();
        public readonly StringBuilder InitialzeBuffers = new StringBuilder();
        public readonly StringBuilder InvertedBuffersDeclarations = new StringBuilder();
        public readonly StringBuilder BuffersSetCurrentIndex = new StringBuilder();
        public string IsDrawingOnChartWindow { get; set; }

        public string BuildIndicator()
        {
            var code = TemplatesReader.Read("IndicatorTemplate.c");

            code = code.Replace("#Parameters_PLACE_HOLDER#", Parameters.ToString());
            code = code.Replace("#Lines_declarations_PLACE_HOLDER#", LinesDeclarations.ToString());
            code = code.Replace("#Initialize_buffers_PLACE_HOLDER#", InitialzeBuffers.ToString());
            code = code.Replace("#Inverted_buffers_declarations_PLACE_HOLDER#", InvertedBuffersDeclarations.ToString());
            code = code.Replace("#Buffers_SetCurrentIndex_PLACE_HOLDER#", BuffersSetCurrentIndex.ToString());
            code = code.Replace("#Mq4Functions_PLACE_HOLDER#", Mq4Functions);

            code = code.Replace("#Start_function_PLACE_HOLDER#", StartFunction);
            code = code.Replace("#Init_function_PLACE_HOLDER#", InitFunction);
            code = code.Replace("#IsDrawingOnChartWindow_PLACE_HOLDER#", IsDrawingOnChartWindow);
            code = code.Replace("#Mq4Fields_PLACE_HOLDER#", Fields);

            return code;
        }   
    }
}