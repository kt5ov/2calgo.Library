using System.Text.RegularExpressions;
using _2calgo.Model;

namespace _2calgo.Parser
{
    public static class AlgoNameProvider
    {
        public static string GetSimplifiedName(string fullNameWithExtraCharacters, AlgoType algoType)
        {
            var regex = new Regex(@"[^\w0-9]");
            var simplifiedName = regex.Replace(fullNameWithExtraCharacters, string.Empty);
            
            if (char.IsDigit(simplifiedName[0]))
                simplifiedName = "_" + simplifiedName;

            return simplifiedName + algoType.ToString();
        }
    }
}