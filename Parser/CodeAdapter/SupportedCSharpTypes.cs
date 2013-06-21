using System.Collections.Generic;

namespace _2calgo.Parser.CodeAdapter
{
    public static class SupportedCSharpTypes
    {
        private static readonly List<string> SupportedTypes = new List<string>
            {
                "int",
                "Mq4Double",
                "string",
                "bool"
            };

         public static bool IsSupported(this string type)
         {
             return SupportedTypes.Contains(type);
         }

        public static string ToPublicType(this string type)
        {
            if (type == "Mq4Double")
                return "double";
            return type;
        }
    }
}