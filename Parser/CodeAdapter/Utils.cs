using _2calgo.Parser.Extensions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Utils
    {
        public static string GetBodyWithAroundingBrackets(this string code, int startIndexToSearch)
        {
            var openBracketIndex = code.IndexOf('{', startIndexToSearch);
            var codeLength = 0;

            var openBrackets = 0;
            while (true)
            {
                codeLength++;
                var index = openBracketIndex + codeLength - 1;
                if (code[index] == '{') 
                    openBrackets++;
                if (code[index] == '}')
                    openBrackets--;
                if (openBrackets == 0)
                    break;
            }

            return code.Substring(openBracketIndex, codeLength);
        }
    }
}