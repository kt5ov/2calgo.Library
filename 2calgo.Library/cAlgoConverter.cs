using System.CodeDom.Compiler;
using System.IO;
using _2calgo.Parser;
using _2calgo.Presenter;

namespace _2calgo.Library
{
    public static class CAlgoConverter
    {
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
                compilerErrors = compiler.Compile(calgoCode, fileName);
            }
            finally 
            {
                File.Delete(fileName);
            }

            return new ConvertionResult(calgoCode, indicatorParsingResult.ParsingErrors, compilerErrors);
        }
    }
}
