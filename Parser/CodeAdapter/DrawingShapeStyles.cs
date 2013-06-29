using _2calgo.Model;

namespace _2calgo.Parser.CodeAdapter
{
    public static class DrawingShapeStyles
    {
         public static DrawingShapeStyle ToDrawingShapeStyle(this string str)
         {
             switch (str)
             {
                 case "DRAW_LINE":
                     return DrawingShapeStyle.Line;
                 case "DRAW_SECTION":
                     return DrawingShapeStyle.Section;
                 case "DRAW_HISTOGRAM":
                     return DrawingShapeStyle.Histogram;
                 case "DRAW_NONE":
                     return DrawingShapeStyle.None;
                 default:
                     return DrawingShapeStyle.Line;
             }
         }
    }
}