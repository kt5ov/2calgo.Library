using System.CodeDom.Compiler;
using System.Linq;
using System.Text.RegularExpressions;
using _2calgo.Model;

namespace _2calgo.Library.CodeBaseDetectors
{
    public static class MqCodeBaseDetector
    {
        private static readonly Regex NameRegex = new Regex(@"\'(?<name>[^\']*)\'", RegexOptions.Compiled);

        public static CodeBase GetCodeBaseFromErrors(CompilerError[] compilerErrors)
        {
            var groups = compilerErrors
                .Where(e => !e.IsWarning)
                .GroupBy(e => e.ErrorNumber)
                .ToArray();
            if (groups.Length == 1 && groups[0].Key == "CS0103")
            {
                var names = groups[0].Select(e => NameRegex.Match(e.ErrorText).Groups["name"].Value).Distinct().ToArray();
                var mq5Name = names.FirstOrDefault(n => NotSupportedNames.Mq5.Contains(n));
                if (mq5Name != null)
                {
                    return CodeBase.Mq5;
                }
                var invalidName = names.FirstOrDefault(n => !NotSupportedNames.Mq4.Contains(n));
                if (invalidName != null)
                {
                    return CodeBase.Invalid;
                }
            }
            return CodeBase.Mq4;
        }
    }
}