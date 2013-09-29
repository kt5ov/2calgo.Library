using System.Collections.Generic;

namespace _2calgo.Model
{
    public class Algo
    {
        public string Mq4Code { get; set; }

        public bool IsDrawingOnChartWindow { get; set; }

        public int BuffersCount { get; set; }

        public string[] Buffers = new string[0];

        public readonly List<double> Levels = new List<double>();

        public IEnumerable<Parameter> Parameters { get; set; } 

        public readonly string[] Colors = new string[8];

        public readonly DrawingShapeStyle[] Styles;
        
        public readonly int[] Widths;

        public Code Code { get; private set; }

        public string[] CustomIndicators { get; set; }

        public Algo()
        {
            Code = new Code();

            Styles = new DrawingShapeStyle[8];
            Widths = new int[8];
            for (var i = 0; i < 8; i++)
            {
                Styles[i] = DrawingShapeStyle.Line;
                Widths[i] = 1;
            }


            Parameters = new Parameter[0];
        }
    }
}