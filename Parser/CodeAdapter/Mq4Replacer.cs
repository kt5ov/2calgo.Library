using System.Text.RegularExpressions;
using _2calgo.Model;
using _2calgo.Parser.Extensions;

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

        public static string ReplaceSimpleTypesToMq4Double(this string code)
        {
            return code
                .ReplaceType("double", "Mq4Double")
                .ReplaceType("int", "Mq4Double")
                .ReplaceType("bool", "Mq4Double");
        }

        public static void ReplaceSimpleTypesToMq4Double(this IndicatorCode indicatorCode)
        {
            indicatorCode.FieldsDeclarations = indicatorCode.FieldsDeclarations.ReplaceSimpleTypesToMq4Double();
            foreach (var function in indicatorCode.Functions)
            {
                function.Body = function.Body.ReplaceSimpleTypesToMq4Double();
                var declarationParts = function.Declaration.SplitByComma();
                for (var i = 0; i < declarationParts.Length; i++)
                {
                    if (!declarationParts[i].Contains("="))
                        declarationParts[i] = declarationParts[i].ReplaceSimpleTypesToMq4Double();
                }
                function.Declaration = string.Join(",", declarationParts);
            }
        }

        private static string ReplaceType(this string code, string from, string to)
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