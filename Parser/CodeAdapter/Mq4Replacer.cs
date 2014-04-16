using System.Text.RegularExpressions;
using _2calgo.Model;
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

        public static string ReplaceSimpleTypesToMq4Types(this string code)
        {
            return code
                .ReplaceType("double", "Mq4Double")
                .ReplaceType("int", "Mq4Double")
                .ReplaceType("bool", "Mq4Double")
                .ReplaceType("string", "Mq4String");
        }

        public static void ReplaceSimpleTypesToMq4Types(this Code code)
        {
            code.FieldsDeclarations = code.FieldsDeclarations.ReplaceSimpleTypesToMq4Types();
            foreach (var function in code.Functions)
            {
                function.ReturnType = function.ReturnType.ReplaceSimpleTypesToMq4Types();
                function.Body = function.Body.ReplaceSimpleTypesToMq4Types();
                for (var i = 0; i < function.Parameters.Length; i++)
                {
                    if (function.Parameters[i].DefaultValue == string.Empty)
                        function.Parameters[i].Type = function.Parameters[i].Type.ReplaceSimpleTypesToMq4Types();
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

        public static void AddMq4InitFunctionIfDoesNotExist(this Code code)
        {
            if (!code.Functions.Any(f => f.Name == "Mq4Init"))
            {
                code.Functions.Add(new Function("void", "Mq4Init", new FunctionParameter[0], string.Empty));
            }
        }

        public static void RenameStandardFunctions(this Code code)
        {
            var startInvocationRegex = new Regex(@"(?<!\w)start\s*\(\s*\)");
            var initInvocationRegex = new Regex(@"(?<!\w)init\s*\(\s*\)");
            foreach (var function in code.Functions)
            {
                switch (function.Name)
                {
                    case "start":
                        function.Name = "Mq4Start";
                        break;
                    case "init":
                        function.Name = "Mq4Init";
                        break;
                }
                function.Body = startInvocationRegex.Replace(function.Body, "Mq4Start()");
                function.Body = initInvocationRegex.Replace(function.Body, "Mq4Init()");
            }
        }


        private static readonly string[] _calgoKeywords = new []
            {
                "TimeFrame",
                "Debug",
                "Stop",
                "Trade",
                "TimeSpan",
                "Math",
                "Account",
                "PendingOrders",
                "Positions",
                "History",
                "Server",
                "MarketSeries",
                "MarketData",
                "DateTime",
            };

        public static string ReplaceCAlgoKeyWords(this string code)
        {
            foreach (var calgoKeyword in _calgoKeywords)
            {
                var regex = new Regex(@"(?<!\w)" + calgoKeyword + @"(?!\w)");
                code = regex.Replace(code, calgoKeyword + "Mq4");
            }
            return code;
        }

        public static string ReplacePrintToMq4Print(this string code)
        {
            var printInvocationRegex = new Regex(@"(?<!\w)Print\s*\(");
            return printInvocationRegex.Replace(code, "Mq4Print(");
        }
    }
}