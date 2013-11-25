using System;
using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Presenter.PartsProviders
{
    public static class CommonInnerPartsProvider
    {
        private const string FolderPath = @"pack://application:,,,/2calgo.Presenter;component/Templates/Common/InnerParts/";
        
        private const string ChartObjects = "ChartObjects";
        private const string DataSeries = "DataSeries";
        private const string Indicators = "Indicators";
        private const string StandardConstants = "Standard constants";
        private const string Account = "Account";

        private static readonly IEnumerable<string> InnerParts = new[]
            {
#if DEBUG
                "Debug.c",
#endif
                "Initialize.c",
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
                "Series.c",
                "Arrows.c",
                "Mq4OutputDataSeries.c",
                "Custom indicators.c",
                "File functions.c",
                "FolderPaths.c",
  
                Account + "/Orders.c",
                Account + "/Account information.c",
                
                Indicators + "/iIndicators.c",
                Indicators + "/CachedStandardIndicators.c",

                StandardConstants + "/Constants.c",
                StandardConstants + "/Trade operations.c",
                StandardConstants + "/ObjectVisibility.c",
                StandardConstants + "/Uninitialize reason codes.c",
                StandardConstants + "/stderror.c",
                
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
                DataSeries + "/IMq4Array.c",
                DataSeries + "/Mq4Array.c",
                DataSeries + "/DefaultValues.c",
                DataSeries + "/Mq4ArrayToDataSeriesConverter.c",
                DataSeries + "/Mq4ArrayToDataSeriesConverterFactory.c",
            };

        public static string Get()
        {
            var partsContents = InnerParts.Select(part => StringResourceReader.Read(FolderPath + part));
            var allParts = string.Join(Environment.NewLine, partsContents);
            return allParts;
        }
    }
}