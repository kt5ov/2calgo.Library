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
                     return "DiscontinuousLine";
                 case DrawingShapeStyle.Section:
                     return "Line";
                 case DrawingShapeStyle.Histogram:
                     return "Histogram";
                 default:
                     throw new ArgumentOutOfRangeException("drawingShapeStyle");
             }
         }
    }
}