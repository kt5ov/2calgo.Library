using System;
using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Model;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Functions
    {
        private static readonly Regex TypeRegex = new Regex(@"(?<type>\w+).*");
        private static readonly Regex ReturnRegex = new Regex(@"return\s*(?<value>[^;]*);");

        public static Function FixReturnValue(this Function function)
        {
            var typeMatch = TypeRegex.Match(function.Declaration.Trim());
            if (!typeMatch.Success)
                throw new Exception("Function without type");

            var type = typeMatch.Groups["type"].Value.Trim();

            var body = function.Body;
            bool wasFixed;
            do
            {
                wasFixed = false;
                var matches = ReturnRegex.Matches(body);
                foreach (var match in matches.OfType<Match>())
                {
                    var value = match.Groups["value"].Value.Trim();
                    if (value == string.Empty && type != "void")
                    {
                        wasFixed = true;
                        body = body.Insert(match.Groups["value"].Index, " 0");
                        break;
                    }
                }
            } while (wasFixed);

            if (!ReturnRegex.IsMatch(body))
            {
                switch (type)
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

            return new Function(function.Name, function.Declaration, body);
        }
    }
}