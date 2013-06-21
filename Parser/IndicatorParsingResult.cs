using System.Collections.Generic;
using _2calgo.Model;
using _2calgo.Parser.Errors;

namespace _2calgo.Parser
{
    public class IndicatorParsingResult
    {
        public Indicator Indicator { get; private set; }
        public IEnumerable<ParsingError> ParsingErrors { get; private set; }

        public IndicatorParsingResult(Indicator indicator, IEnumerable<ParsingError> parsingErrors)
        {
            Indicator = indicator;
            ParsingErrors = parsingErrors;
        }
    }
}