using System.Text.RegularExpressions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Numbers
    {
        private static readonly Regex Regex = new Regex(@"(?<decimal>\d+\.)(?<notDigit>[^\d])", RegexOptions.Compiled);

        public static string AddZeroToDecimalNumbers(this string code)
        {
            while (true)
            {
                
                var match = Regex.Match(code);
                if (!match.Success)
                    return code;

                code = code.Replace(match.Value, match.Groups["decimal"].Value + "0" + match.Groups["notDigit"]);
            }
        }
    }
}