using System;
using _2calgo.Model;

namespace _2calgo.Presenter.Extensions
{
    public static class DrawingShapeStyleExtensions
    {
        public static string ToPlotTypeString(this DrawingShapeStyle drawingShapeStyle)
         {
             switch (drawingShapeStyle)
             {
                 case DrawingShapeStyle.Line:
                     return "Line";
                 case DrawingShapeStyle.Histogram:
                     return "Histogram";
                 case DrawingShapeStyle.Arrow:
                     return "Points";
                 default:
                     throw new ArgumentOutOfRangeException("drawingShapeStyle");
             }
         }
    }
}