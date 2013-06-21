using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Parser.Extensions;
using _2calgo.Parser.InnerStructures;

namespace _2calgo.Parser.CodeAdapter.Switch
{
    public static class SwitchOperator
    {
        private static readonly Regex SwitchHeaderRegex = new Regex(@"(?<=[^\w])switch(?=[^\w])", RegexOptions.Compiled);

        public static string FixSwitches(this string code)
        {
            bool wasFixed;
            do
            {
                wasFixed = false;
                var switchHeaders = SwitchHeaderRegex.Matches(code);
                foreach (var switchHeader in switchHeaders.OfType<Match>())
                {
                    var switchBody = code
                        .GetBodyWithAroundingBrackets(switchHeader.Index)
                        .RemoveSurroundingBrackets()
                        .Trim();
                    var switchElements = GetElements(switchBody).ToList();
                    if (switchElements[switchElements.Count - 1] != "break;")
                    {
                        switchElements.Add("break;");
                        var fixedSwitchBody = ComposeSwitchBody(switchElements);
                        code = code.Replace(switchBody, fixedSwitchBody);
                        wasFixed = true;
                        break;
                    }
                }
            } while (wasFixed);
            return code;
        }

        private static IEnumerable<string> GetElements(string switchBody)
        {
            const string caseString = "case";
            const string breakString = "break";
            const string defaultString = "default";
            
            var innerStructures = new InnerStructuresHandler();
            int? caseStartIndex = null;
            int? caseBodyStartIndex = null;
            for (var i = 0; i < switchBody.Length; i++)
            {
                innerStructures.Handle(switchBody[i]);
                if (innerStructures.IsInsideSomeStructure)
                    continue;
                if (caseString.IsSubstring(switchBody, i) || defaultString.IsSubstring(switchBody, i))
                {
                    caseStartIndex = i;
                }
                if (":".IsSubstring(switchBody, i) && caseStartIndex != null)
                {
                    yield return switchBody.Substring(caseStartIndex.Value, i - caseStartIndex.Value + 1).Trim();
                    caseStartIndex = null;
                    caseBodyStartIndex = i + 1;
                }
                if (breakString.IsSubstring(switchBody, i))
                {
                    yield return switchBody.Substring(caseBodyStartIndex.Value, i - caseBodyStartIndex.Value).Trim();
                    yield return "break;";
                    caseBodyStartIndex = null;
                }
            }
            if (caseBodyStartIndex.HasValue)
                yield return switchBody.SafeSubstring(caseBodyStartIndex.Value, switchBody.Length).Trim();
            if (caseStartIndex.HasValue)
                yield return switchBody.SafeSubstring(caseStartIndex.Value, switchBody.Length).Trim();
        }

        private static string ComposeSwitchBody(IEnumerable<string> elements)
        {
            var stringBuilder = new StringBuilder();

            foreach (var element in elements)
            {
                if ("case".IsSubstring(element, 0) || "default".IsSubstring(element, 0))
                    stringBuilder.Append("\t\t\t");
                else
                    stringBuilder.Append("\t\t\t\t");
                stringBuilder.AppendLine(element);
            }

            return stringBuilder.ToString();
        }
    }
}