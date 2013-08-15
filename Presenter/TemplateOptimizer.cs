using System.Text;
using System.Text.RegularExpressions;
using System.Linq;
using _2calgo.Parser.InnerStructures;
using _2calgo.Parser.Extensions;

namespace _2calgo.Presenter
{
    public static class TemplateOptimizer
    {
        private static readonly Regex ConditionalRegex = new Regex(@"\[Conditional\((?<words>[^\)]+)\)\]", RegexOptions.Compiled);

        public static string RemoveUnusedCode(string template, Words words)
        {
            var result = new StringBuilder();

            var lastIndex = 0;
            foreach (var match in ConditionalRegex.Matches(template).OfType<Match>().ToArray())
            {
                if (match.Index < lastIndex)
                    continue;
                
                var innerStructures = new InnerStructures();
                var currentIndex = template.IndexOf('{', match.Index);
                innerStructures.Handle(template[currentIndex]);
                while (innerStructures.IsInsideSomeStructure)
                {
                    currentIndex++;
                    innerStructures.Handle(template[currentIndex]);
                }
                result.AppendLine(template.SubstringFromTo(lastIndex, match.Index - 1).Trim());
                var wordsGroup = match.Groups["words"].Value;
                var conditionalWords = wordsGroup
                    .SplitByComma()
                    .Select(s => s.Replace("\"", string.Empty))
                    .ToArray();
                
                if (conditionalWords.Any() && conditionalWords.First() == "OBJ_LABEL")
                {
                    
                }

                if (conditionalWords.Any(words.Contains))
                {
                    var partToCopy = template.SubstringFromTo(match.Index + match.Length, currentIndex);
                    partToCopy = RemoveUnusedCode(partToCopy, words);
                    result.AppendLine(partToCopy.Trim());
                }
                lastIndex = currentIndex + 1;
            }
            result.AppendLine(template.SubstringFromTo(lastIndex, template.Length - 1).Trim());
            return result.ToString();
        }
    }
}