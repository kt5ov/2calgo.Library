using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using _2calgo.Model;
using _2calgo.Parser;

namespace _2calgo.Presenter
{
    public abstract class AlgoPresenter
    {
        private static readonly Regex Mq4NameRegex = new Regex(@"(?<name>.*)\.mq4");

        protected static void InitializeAlgoBuilder(Algo algo, AlgoBuilder builder)
        {
            builder.Mq4Code = algo.Mq4Code;
            builder.AlgoName = GetMq4Name(algo.Mq4Code);
            builder.Fields = algo.Code.FieldsDeclarations;

            builder.Mq4Functions = GetFunctions(algo.Code.Functions);
            foreach (var customIndicator in algo.CustomIndicators)
            {
                builder.References.AppendLine(string.Format("//#reference: {0}.algo", customIndicator));
            }
#if DEBUG
            builder.DebugActions.AppendLine("Debug.Activate();");
            builder.DebugActions.AppendLine("Debug.Initialize(m => Print(m));");
            builder.HandleException.AppendLine(@"

			var exceptionReportDir = @""D:\2calgo"";
			Directory.CreateDirectory(exceptionReportDir);
			var fileName = Path.Combine(exceptionReportDir, ""last exception.txt"");
			File.WriteAllText(fileName, e.ToString());
            ");
#endif
        }

        private static string GetMq4Name(string mq4Code)
        {
            var match = Mq4NameRegex.Match(mq4Code);
            if (!match.Success)
                return "ConvertedIndicator";

            return AlgoNameProvider.GetSimplifiedName(match.Groups["name"].Value);
        }

        private static string GetFunctions(IEnumerable<Function> functions)
        {
            var result = new StringBuilder();
            foreach (var function in functions)
            {
                var parameters = String.Join((string) ", ", (IEnumerable<string>) function.Parameters.Select(PresentParameter));
                result.AppendFormat("{0} {1}({2}){3}", function.ReturnType, function.Name, parameters, Environment.NewLine);
                result.AppendLine("{");
                result.Append(function.Body);
                result.AppendLine("}");
            }
            return result.ToString();
        }

        private static string PresentParameter(FunctionParameter functionParameter)
        {
            var parameter = string.Format("{0} {1}", functionParameter.Type, functionParameter.Name);
            if (functionParameter.ByReference)
                parameter = "ref " + parameter;
            if (functionParameter.DefaultValue != string.Empty)
                parameter = string.Format("{0} = {1}", parameter, functionParameter.DefaultValue);

            return parameter;
        }

        public abstract string GenerateCodeFrom(Algo algo);
    }
}