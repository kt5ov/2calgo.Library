using System;
using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Model;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Functions
    {
        private static readonly Regex ReturnRegex = new Regex(@"return\s*(?<value>[^;]*);");

        public static Function FixReturnValue(this Function function)
        {
            var body = function.Body;
            bool wasFixed;
            do
            {
                wasFixed = false;
                var matches = ReturnRegex.Matches(body);
                foreach (var match in matches.OfType<Match>())
                {
                    var value = match.Groups["value"].Value.Trim();
                    if (value == string.Empty && function.ReturnType != "void")
                    {
                        wasFixed = true;
                        body = body.Insert(match.Groups["value"].Index, " 0");
                        break;
                    }
                }
            } while (wasFixed);

            if (!ReturnRegex.IsMatch(body))
            {
                switch (function.ReturnType)
                {
                    case "void":
                        body += "return;";
                        break;
                    case "bool":
                        body += "return true;";
                        break;
                    default:
                        body += "return 0;";
                        break;
                }
            }

            return new Function(function.ReturnType, function.Name, function.Parameters, body);
        }
    }
}