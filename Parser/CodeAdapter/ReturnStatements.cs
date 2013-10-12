using System;
using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Parser.Extensions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class ReturnStatements
    {
        private static string FixReturnStatement(string returnStatement)
        {
            var openBracketPosition = returnStatement.IndexOf('(');
            var closeBracketPosition = returnStatement.LastIndexOf(')');
            var expression = returnStatement.Substring(openBracketPosition + 1,
                                                       closeBracketPosition - openBracketPosition - 1);
            return String.Format("return {0};", expression);
        }

        private static readonly Regex ReturnRegex = new Regex(@"return\s*\([^;]*\;");
        public static string FixReturnStatements(this string code)
        {
            bool changed;
            do
            {
                changed = false;
                foreach (var match in ReturnRegex.Matches(code).OfType<Match>())
                {
                    var returnStatement = match.Value;
                    var fixedReturnStatement = FixReturnStatement(returnStatement);

                    if (!fixedReturnStatement.AreBracketsSimmetric('(', ')'))
                        continue;

                    code = code
                        .Remove(match.Index, match.Length)
                        .Insert(match.Index, fixedReturnStatement);

                    changed = true;
                    break;
                }
            } while (changed);

            return code;
        }

        private static readonly Regex CSharpReturnRegex = new Regex(@"return(?!\w)[^;]*;", RegexOptions.Compiled);

        public static string RemoveReturnStatements(this string code)
        {
            return CSharpReturnRegex.Replace(code, string.Empty);
        }
    }
}