using System.Collections.Generic;

namespace _2calgo.Model
{
    public class Indicator
    {
        public bool IsDrawingOnChartWindow { get; set; }

        public int BuffersCount { get; set; }

        public string[] Buffers = new string[0];

        public IEnumerable<Parameter> Parameters { get; set; } 

        public readonly string[] Colors = new string[8];

        public readonly DrawingShapeStyle[] Styles;

        public IndicatorCode Code { get; private set; }

        public Indicator()
        {
            Code = new IndicatorCode();

            Styles = new DrawingShapeStyle[8];
            for (var i = 0; i < 8; i++)
                Styles[i] = DrawingShapeStyle.Line;

            Parameters = new Parameter[0];
        }
    }
}