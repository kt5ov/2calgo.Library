using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using _2calgo.Library.CodeBaseDetectors;
using _2calgo.Model;
using _2calgo.Parser;
using _2calgo.Parser.Errors;
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
            string calgoCode = null;
            IEnumerable<ParsingError> parsingErrors = new ParsingError[0];
            var compilerErrors = new CompilerError[0];

            var codeBase = CodeBase.Mq4;
            if (CSharpCodeDetector.IsCSharpCode(code))
            {
                codeBase = CodeBase.CSharp;
            }
            else
            {
                var parser = new Mq4Parser();

                var indicatorParsingResult = parser.Parse(code, algotype, includeFiles);
                var algo = indicatorParsingResult.Algo;
                parsingErrors = indicatorParsingResult.ParsingErrors;
                if (parsingErrors.All(e => e.ErrorType < ErrorType.Error))
                {
                    var presenter = CreatePresenter(algotype);
                    calgoCode = presenter.GenerateCodeFrom(algo);

                    var compiler = new CSharpCompiler();
                    var fileName = Path.GetTempFileName();
                    try
                    {
                        var codeToCompile = calgoCode;
                        var indexToInsert = codeToCompile.IndexOf("//Custom Indicators Place Holder");
                        foreach (var customIndicatorName in algo.CustomIndicators)
                        {
                            codeToCompile = codeToCompile.Insert(indexToInsert,
                                                                 CustomIndicatorTemplate.Replace("CustomIndicatorName",
                                                                                                 customIndicatorName));
                        }
                        compilerErrors = compiler.Compile(codeToCompile, fileName);

                        codeBase = MqCodeBaseDetector.GetCodeBaseFromErrors(compilerErrors);
                    }
                    finally
                    {
                        File.Delete(fileName);
                    }
                }
            }

            return new ConvertionResult(calgoCode, parsingErrors, compilerErrors, codeBase);
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
