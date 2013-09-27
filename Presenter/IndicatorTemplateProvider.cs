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
                "Mq4Double.cs",
                "Mq4DoubleComparer.cs",
                "Mq4String.c",
                "Mq4Null.cs",
                "Comparers.cs",
                 DataSeries + "/DataSeriesExtensions.c",
                 DataSeries + "/TimeSeriesExtensions.c",
            };

        private static readonly IEnumerable<string> InnerParts = new[]
            {
#if DEBUG
                "Debug.c",
#endif
                "Predefined variables.c",
                "Math and Trig.c",
                "Conversion functions.c",
                "Window functions.c",
                "String functions.c",
                "Date and Time functions.c",
                "Messages.c",
                "Timeseries access.c",
                "Common functions.c",
                "Array functions.c",
                "Alert.cs",
                "Checkup.c",
                "stdlib.c",
                "Global variables.c",

                Indicators + "/iIndicators.c",
                Indicators + "/CachedStandardIndicators.c",
                Indicators + "/Custom indicators.c",

                StandardConstants + "/Constants.c",
                StandardConstants + "/Trade operations.c",
                StandardConstants + "/ObjectVisibility.c",
                StandardConstants + "/Uninitialize reason codes.c",
                
                ChartObjects + "/Mq4ChartObjects.c",
                ChartObjects + "/Mq4Object.c",
                ChartObjects + "/Mq4VerticalLine.c",
                ChartObjects + "/Mq4HorizontalLine.c",
                ChartObjects + "/Mq4Text.c",
                ChartObjects + "/Mq4Label.c",
                ChartObjects + "/Mq4TrendLine.c",
                ChartObjects + "/Mq4Rectangle.c",
                ChartObjects + "/Mq4Arrow.c",
                ChartObjects + "/Mq4Fibo.c",
                ChartObjects + "/Object functions.c",
                
                DataSeries + "/Mq4MarketDataSeries.c",
                DataSeries + "/Mq4OutputDataSeries.c",
                DataSeries + "/Mq4TimeSeries.c",
                DataSeries + "/IMq4Array.c",
                DataSeries + "/Mq4Array.c",
                DataSeries + "/DefaultValues.c",
                DataSeries + "/Mq4ArrayToDataSeriesConverter.c",
                DataSeries + "/Mq4ArrayToDataSeriesConverterFactory.c",
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
            var partsContents = parts.Select(part => StringResourceReader.Read(FolderPath + part));
            var allParts = string.Join(Environment.NewLine, partsContents);
            return allParts;
        }
    }
}
