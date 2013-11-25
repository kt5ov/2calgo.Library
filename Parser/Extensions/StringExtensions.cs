using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace _2calgo.Parser.Extensions
{
    public static class StringExtensions
    {
        public static bool AreBracketsSimmetric(this string code, char bracketOpen, char bracketClose)
        {
            var opened = 0;
            foreach (var c in code)
            {
                if (c == bracketOpen)
                    opened++;
                if (c == bracketClose)
                    opened--;
                if (opened < 0)
                    return false;
            }
            return opened == 0;
        }

        public static string SafeSubstring(this string str, int startIndex, int length)
        {
            var maxLength = str.Length - startIndex;
            return str.Substring(startIndex, Math.Min(length, maxLength));
        }

        public static string SubstringFromTo(this string str, int fromIndex, int toIndex)
        {
            return str.SafeSubstring(fromIndex, toIndex - fromIndex + 1);
        }

        private static readonly Regex WordsRegex = new Regex(@"\w{1,}", RegexOptions.Compiled);
        public static string FirstWord(this string str)
        {
            return WordsRegex.Match(str).Value;
        }

        public static string[] SplitByComma(this string code)
        {
            var result = new List<string>();

            var currentVariable = new StringBuilder();
            var innerStructures = new InnerStructures.InnerStructures();
            foreach (var @char in code)
            {
                innerStructures.Handle(@char);
                if (@char != ',' || innerStructures.IsInsideSomeStructure)
                    currentVariable.Append(@char);
                else
                {
                    result.Add(currentVariable.ToString());
                    currentVariable.Clear();
                }
            }
            result.Add(currentVariable.ToString());
            
            return result.Select(s => s.Trim()).ToArray();
        }

        public static string RemoveSurroundingBrackets(this string fragment)
        {
            var openBracketIndex = fragment.IndexOf('{');
            var closeBracketIndex = fragment.LastIndexOf('}');
            return fragment.Substring(openBracketIndex + 1, closeBracketIndex - openBracketIndex - 1);
        }

        public static string[] SplitByLines(this string code)
        {
            return Regex.Split(code, @"\r\n|\r|\n", RegexOptions.Compiled);
        }

        public static bool IsSubstring(this string substring, string text, int index)
        {
            return text.SafeSubstring(index, substring.Length) == substring;
        }
    }
}