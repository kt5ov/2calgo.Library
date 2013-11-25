using System.Text;
using System.Text.RegularExpressions;
using _2calgo.Parser.InnerStructures;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Names
    {
        private static readonly Regex IncludeRegex = new Regex(@"#include.*");

        public static string RemoveDotsFromNames(this string code)
        {
            var result = new StringBuilder();
            var stringStructure = new StringStructure();

            for (int i = 0; i < code.Length; i++)
            {
                stringStructure.Handle(code[i]);
                if (i > 0 && i < code.Length - 1 &&
                    code[i] == '.')
                {
                    if (!stringStructure.IsInsideString
                        && IsInsideWord(i, code))
                        continue;
                }

                result.Append(code[i]);
            }

            return result.ToString();
        }

        private static bool IsInsideWord(int index, string code)
        {
            int i = index - 1;
            while (i >= 0 && !IsSplitChar(code[i]))
            {
                if (char.IsLetter(code[i]) || code[i] == '_')
                    return true;
                i--;
            }
            i = index + 1;
            while (i < code.Length && !IsSplitChar(code[i]))
            {
                if (char.IsLetter(code[i]) || code[i] == '_')
                    return true;
                i++;
            }
            return false;
        }

        private static bool IsSplitChar(char c)
        {
            switch (c)
            {
                case ' ':
                case '\n':
                case '\r':
                case ';':
                case ':':
                case '?':
                case '=':
                case '*':
                case '+':
                case '/':
                case '-':
                case '(':
                case ')':
                    return true;
            }
            return false;
        }

        public static string RemoveIncludes(this string code)
        {
            return IncludeRegex.Replace(code, string.Empty);
        }
    }
}