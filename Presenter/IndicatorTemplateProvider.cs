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

        private static readonly IEnumerable<string> PartNames = new[]
            {
                "Debug",
                "Predefined variables",
                "Constants",
                "Math and Trig",
                "Conversion functions",
                "Window functions",
                "String functions",
                "Date and Time functions",
                "Messages",
                "Validation",
                "Timeseries access",
                "Common functions",
                "Array functions",
                "Mq4Double",

                Indicators + "/iIndicators",
                Indicators + "/CashedStandardIndicators",
                Indicators + "/Custom indicators",
                
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
                DataSeries + "/IndicatorDataSeries",
                DataSeries + "/IMq4Array",
                DataSeries + "/Mq4Array",
                DataSeries + "/DefaultValues",
            };

        public static string GetTemplate()
        {
            var mainTemplate = StringResourceReader.Read(FolderPath + "MainTemplate.c");
            var parts = PartNames.Select(part => StringResourceReader.Read(FolderPath + part + ".c"));
            var allParts = string.Join(Environment.NewLine, parts);

            return mainTemplate.Replace("#Conditional_Part_PLACE_HOLDER#", allParts);
        }
    }
}
