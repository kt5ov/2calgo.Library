using System;
using System.Text;
using System.Text.RegularExpressions;
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

        public static string FixReturnStatements(this string code)
        {
            const string returnString = "return";

            var result = new StringBuilder();
            var i = 0;
            while (i < code.Length)
            {
                if (code.SafeSubstring(i, returnString.Length) == returnString)
                {
                    var semicolonPosition = code.IndexOf(';', i);
                    var returnStatement = code.Substring(i, semicolonPosition - i + 1);
                    if (returnStatement.Contains("("))
                        result.Append(FixReturnStatement(returnStatement));
                    else
                        result.Append(returnStatement);
                    i += returnStatement.Length;
                }
                else
                {
                    result.Append(code[i]);
                    i++;
                }
            }
            return result.ToString();
        }

        private static readonly Regex CSharpReturnRegex = new Regex(@"return[^;]*;", RegexOptions.Compiled);

        public static string RemoveReturnStatements(this string code)
        {
            return CSharpReturnRegex.Replace(code, string.Empty);
        }
    }
}