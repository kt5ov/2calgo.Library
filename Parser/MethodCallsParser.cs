using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Model;
using _2calgo.Parser.Extensions;

namespace _2calgo.Parser
{
    public static class MethodCallsParser
    {
        private static readonly Regex MethodCallRegex = new Regex(@"(?<name>\w+)\s*(?<open>\()");

        public static IEnumerable<MethodCall> Parse(string code)
        {
            foreach (var match in MethodCallRegex.Matches(code).OfType<Match>())
            {
                var openBracketIndex = match.Groups["open"].Index;
                var parameters = new StringBuilder();
                var openedBracketsCount = 0;
                int j;
                for (j = openBracketIndex; j < code.Length; j++)
                {
                    parameters.Append(code[j]);
                    if (code[j] == '(')
                        openedBracketsCount++;
                    if (code[j] == ')')
                        openedBracketsCount--;
                    if (openedBracketsCount == 0)
                        break;
                }

                var parametersString = parameters.ToString();
                var parametersStringWithoutSurroundingBrackets = parametersString.Substring(1, parametersString.Length - 2);
                var parametersCollection = parametersStringWithoutSurroundingBrackets.SplitByComma().ToArray();

                var methodName = match.Groups["name"].Value;
                var originalText = code.SafeSubstring(match.Index, j - match.Index + 1);
                var methodCall = new MethodCall(methodName, parametersCollection, originalText);

                yield return methodCall;
            }
        }
    }
}