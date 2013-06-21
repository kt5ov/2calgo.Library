using System.Text.RegularExpressions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Mq4Replacer
    {
         public static string ReplaceDateTimeToInt(this string code)
         {
             return ReplaceType(code, "datetime", "int");
         }

         public static string ReplaceColorToInt(this string code)
         {
             return ReplaceType(code, "color", "int");
         }

         public static string ReplaceDoubleToMq4Double(this string code)
         {
             return ReplaceType(code, "double", "Mq4Double");
         }

         public static string ReplaceIntToMq4Double(this string code)
         {
             return ReplaceType(code, "int", "Mq4Double");
         }

        private static string ReplaceType(string code, string from, string to)
        {
            var regex = new Regex(@"[^\w](?<from>" + from + @")[^\w]");
            while (true)
            {
                var match = regex.Match(code);
                if (!match.Success)
                    return code;

                var index = match.Groups["from"].Index;
                code = code.Remove(index, from.Length);
                code = code.Insert(index, to);
            }
        }
    }
}