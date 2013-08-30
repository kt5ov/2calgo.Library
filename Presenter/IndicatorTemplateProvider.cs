using System;
using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Presenter
{
    static class IndicatorTemplateProvider
    {
        private const string FolderPath = @"pack://application:,,,/2calgo.Presenter;component/Templates/Indicator/";
        private const string ChartObjects = "ChartObjects";
        private const string DataSeries = "DataSeries";
        private const string Indicators = "Indicators";
        private const string StandardConstants = "Standard constants";

        private static readonly IEnumerable<string> OuterParts = new[]
            {
                "Mq4Double",
                 DataSeries + "/DataSeriesExtensions",
                 DataSeries + "/TimeSeriesExtensions",
            };

        private static readonly IEnumerable<string> InnerParts = new[]
            {
                "Debug",
                "Predefined variables",
                "Math and Trig",
                "Conversion functions",
                "Window functions",
                "String functions",
                "Date and Time functions",
                "Messages",
                "Timeseries access",
                "Common functions",
                "Array functions",
                "Alert",
                "Checkup",
                "stdlib",

                Indicators + "/iIndicators",
                Indicators + "/CashedStandardIndicators",
                Indicators + "/Custom indicators",

                StandardConstants + "/Constants",
                StandardConstants + "/Trade operations",
                
                ChartObjects + "/Mq4ChartObjects",
                ChartObjects + "/Mq4Object",
                ChartObjects + "/Mq4VerticalLine",
                ChartObjects + "/Mq4HorizontalLine",
                ChartObjects + "/Mq4Text",
                ChartObjects + "/Mq4Label",
                ChartObjects + "/Mq4TrendLine",
                ChartObjects + "/Mq4Rectangle",
                ChartObjects + "/Mq4Arrow",
                ChartObjects + "/Object functions",
                
                DataSeries + "/DataSeriesExtremums",
                DataSeries + "/Mq4MarketDataSeries",
                DataSeries + "/Mq4OutputDataSeries",
                DataSeries + "/Mq4TimeSeries",
                DataSeries + "/IMq4Array",
                DataSeries + "/Mq4Array",
                DataSeries + "/DefaultValues",
                DataSeries + "/Mq4ArrayToDataSeriesConverter",
                DataSeries + "/Mq4ArrayToDataSeriesConverterFactory",
            };

        public static string GetTemplate()
        {
            var mainTemplate = StringResourceReader.Read(FolderPath + "MainTemplate.c");
            var innerParts = JoinParts(InnerParts);
            var outerParts = JoinParts(OuterParts);

            return mainTemplate
                .Replace("#InnerParts_PLACE_HOLDER#", innerParts)
                .Replace("#OuterParts_PLACE_HOLDER#", outerParts);
        }

        private static string JoinParts(IEnumerable<string> parts)
        {
            var partsContents = parts.Select(part => StringResourceReader.Read(FolderPath + part + ".c"));
            var allParts = string.Join(Environment.NewLine, partsContents);
            return allParts;
        }
    }
}
