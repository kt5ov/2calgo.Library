using System.CodeDom.Compiler;
using System.IO;
using _2calgo.Model;
using _2calgo.Parser;
using _2calgo.Presenter;
using File = System.IO.File;

namespace _2calgo.Library
{
    public static class CAlgoConverter
    {
        private const string CustomIndicatorTemplate = @"
            public class CustomIndicatorName : Indicator
            {
                public List<DataSeries> AllOutputDataSeries = new List<DataSeries>();

                public override void Calculate(int index) {}
            }
        ";

        public static ConvertionResult Convert(string code, AlgoType algotype, Model.File[] includeFiles)
        {
            var parser = new Mq4Parser();
            
            var indicatorParsingResult = parser.Parse(code, algotype, includeFiles);
            var algo = indicatorParsingResult.Algo;

            var presenter = CreatePresenter(algotype);
            var calgoCode = presenter.GenerateCodeFrom(algo);

            var compiler = new CSharpCompiler();
            var fileName = Path.GetTempFileName();
            CompilerError[] compilerErrors;
            try
            {
                var codeToCompile = calgoCode;
                var indexToInsert = codeToCompile.IndexOf("//Custom Indicators Place Holder");
                foreach (var customIndicatorName in algo.CustomIndicators)
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

        private static AlgoPresenter CreatePresenter(AlgoType algotype)
        {
            string template;
            switch (algotype)
            {
                case AlgoType.Indicator:
                    template = new IndicatorTemplateProvider().GetTemplate();
                    break;
                default:
                    template = new RobotTemplateProvider().GetTemplate();
                    break;
            }
            return new AlgoPresenter(new AlgoBuilder(template));
        }
    }
}
