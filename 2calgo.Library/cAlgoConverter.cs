using System.CodeDom.Compiler;
using System.IO;
using _2calgo.Parser;
using _2calgo.Presenter;

namespace _2calgo.Library
{
    public static class CAlgoConverter
    {
        private const string CustomIndicatorTemplate = @"
            public class CustomIndicatorName : Indicator
            {
                public override void Calculate(int index) {}
            }
        ";

        public static ConvertionResult Convert(string code)
        {
            var parser = new IndicatorParser();
            
            var indicatorParsingResult = parser.Parse(code);
            var indicator = indicatorParsingResult.Indicator;

            var presenter = new IndicatorPresenter();
            var calgoCode = presenter.GenerateCodeFrom(indicator);

            var compiler = new CSharpCompiler();
            var fileName = Path.GetTempFileName();
            CompilerError[] compilerErrors;
            try
            {
                var codeToCompile = calgoCode;
                var indexToInsert = codeToCompile.IndexOf("//Custom Indicators Place Holder");
                foreach (var customIndicatorName in indicator.CustomIndicators)
                {
                    codeToCompile = codeToCompile.Insert(indexToInsert, CustomIndicatorTemplate.Replace("CustomIndicatorName", customIndicatorName));
                }
                compilerErrors = compiler.Compile(codeToCompile, fileName);
            }
            finally 
            {
                File.Delete(fileName);
            }

            return new ConvertionResult(calgoCode, indicatorParsingResult.ParsingErrors, compilerErrors);
        }
    }
}
