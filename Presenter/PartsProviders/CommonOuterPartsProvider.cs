using System;
using System.Collections.Generic;
using System.Linq;

namespace _2calgo.Presenter.PartsProviders
{
    public static class CommonOuterPartsProvider
    {
        private const string FolderPath = @"pack://application:,,,/2calgo.Presenter;component/Templates/Common/OuterParts/";
        private const string Arrays = @"Arrays";

        private static readonly IEnumerable<string> OuterParts = new[]
            {
                "Mq4DoubleComparer.cs",
                "Mq4String.c",
                "Mq4Char.c",
                "Mq4Null.cs",
                "Comparers.cs",
                "DataSeriesExtensions.c",
                "TimeSeriesExtensions.c",
                "Mq4Colors.c",
                "EventExtensions.c",
                "Mq4LineStyles.c",
                "Mq4TimeSeries.c",
                "ConvertExtensions.c",    
                "Mq4Double.cs",      

                Arrays + "/Mq4DoubleTwoDimensionalArray.c",      
                Arrays + "/Mq4DoubleArray.c",
                Arrays + "/Mq4MarketDataSeries.c",
                Arrays + "/Mq4StringArray.c",
                Arrays + "/IMq4DoubleArray.c",
                Arrays + "/Mq4ArrayToDataSeriesConverter.c",
                Arrays + "/Mq4ArrayToDataSeriesConverterFactory.c",
            };

        public static string Get()
        {
            var partsContents = OuterParts.Select(part => StringResourceReader.Read(FolderPath + part));
            var allParts = string.Join(Environment.NewLine, partsContents);
            return allParts;
        }
    }
}