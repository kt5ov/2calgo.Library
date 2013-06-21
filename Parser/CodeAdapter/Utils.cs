using _2calgo.Parser.Extensions;

namespace _2calgo.Parser.CodeAdapter
{
    public static class Utils
    {
        public static string GetBodyWithAroundingBrackets(this string code, int startIndexToSearch)
        {
            var openBracketIndex = code.IndexOf('{', startIndexToSearch);
            var codeLength = 0;
            string bodyWithAroundingBrackets;
            do
            {
                codeLength++;
                bodyWithAroundingBrackets = code.Substring(openBracketIndex, codeLength);
            } while (!bodyWithAroundingBrackets.ContainsBracketsAndTheyAreSimmetric('{', '}'));
            return bodyWithAroundingBrackets;
        }
    }
}