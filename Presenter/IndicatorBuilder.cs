using System.Text;
using RazorEngine;

namespace _2calgo.Presenter
{
    public class IndicatorBuilder
    {
        public string Fields { get; set; }
        public string Mq4Functions { get; set; }
        public string Levels { get; set; }

        public readonly StringBuilder Parameters = new StringBuilder();
        public readonly StringBuilder LinesDeclarations = new StringBuilder();
        public readonly StringBuilder InitialzeBuffers = new StringBuilder();
        public readonly StringBuilder InvertedBuffersDeclarations = new StringBuilder();
        public readonly StringBuilder BuffersSetCurrentIndex = new StringBuilder();        
        public bool IsDrawingOnChartWindow { get; set; }
        public Words Words { get; set; }

        public string BuildIndicator()
        {
            var template = IndicatorTemplateProvider.GetTemplate();
            var code = Razor.Parse(template, this);

            return code;
        }   
    }
}