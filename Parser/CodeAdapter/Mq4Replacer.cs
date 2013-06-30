using System.Text.RegularExpressions;
using _2calgo.Model;
using _2calgo.Parser.Extensions;
using System.Linq;

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
                function.ReturnType = function.ReturnType.ReplaceSimpleTypesToMq4Double();
                function.Body = function.Body.ReplaceSimpleTypesToMq4Double();
                for (var i = 0; i < function.Parameters.Length; i++)
                {
                    if (!function.Parameters[i].Contains("="))
                        function.Parameters[i] = function.Parameters[i].ReplaceSimpleTypesToMq4Double();
                }
            }
        }

        private static string ReplaceType(this string code, string from, string to)
        {
            code = " " + code + " ";
            var regex = new Regex(@"[^\w](?<from>" + from + @")[^\w]");
            while (true)
            {
                var match = regex.Match(code);
                if (!match.Success)
                    return code.Trim();

                var index = match.Groups["from"].Index;
                code = code.Remove(index, from.Length);
                code = code.Insert(index, to);
            }
        }

        public static void AddMq4InitFunctionIfDoesNotExist(this IndicatorCode indicatorCode)
        {
            if (!indicatorCode.Functions.Any(f => f.Name == "Mq4Init"))
            {
                indicatorCode.Functions.Add(new Function("void", "Mq4Init", new string[0], string.Empty));
            }
        }
    }
}