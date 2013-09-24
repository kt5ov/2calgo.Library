using System.Collections.Generic;

namespace _2calgo.Parser.CodeAdapter
{
    public static class SupportedCSharpTypes
    {
        private static readonly List<string> SupportedTypes = new List<string>
            {
                "int",
                "double",
                "string",
                "bool",
                "Mq4Double",
                "Mq4String",
            };

         public static bool IsSupported(this string type)
         {
             return SupportedTypes.Contains(type);
         }

        public static string ToPublicType(this string type)
        {
            if (type == "Mq4Double")
                return "double";
            if (type == "Mq4String")
                return "string";
            return type;
        }
    }
}