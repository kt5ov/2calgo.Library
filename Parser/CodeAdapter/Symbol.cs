using System.Text.RegularExpressions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Symbol
    {
        private static readonly Regex SymbolCallRegex = new Regex(@"[^\w](?<symbol>Symbol\s*\(\s*\))", RegexOptions.Compiled);

         public static string ReplaceSymbolToEmptyString(this string code)
         {
             do
             {
                 var match = SymbolCallRegex.Match(code);
                 if (!match.Success)
                     break;

                 var symbolCall = match.Groups["symbol"];
                 code = code.Remove(symbolCall.Index, symbolCall.Length);
                 code = code.Insert(symbolCall.Index, "Symbol.Code");
             } while (true);
             return code;
         }
    }
}