using System;
using _2calgo.Model;

namespace _2calgo.Presenter
{
    static class TemplateProvider
    {
        private const string FolderPath = @"pack://application:,,,/2calgo.Presenter;component/Templates/";

        public static string GetTemplate(AlgoType algoType)
        {
            var mainTemplate = GetMainTemplate(algoType);

            return mainTemplate
                .Replace("#Usings_PLACE_HOLDER#", StringResourceReader.Read(FolderPath + "Common/Usings.c"))
                .Replace("#InnerParts_PLACE_HOLDER#", InnerPartsProvider.Get())
                .Replace("#OuterParts_PLACE_HOLDER#", OuterPartsProvider.Get());
        }

        private static string GetMainTemplate(AlgoType algoType)
        {
            switch (algoType)
            {
                case AlgoType.Indicator:
                    return StringResourceReader.Read(FolderPath + "Indicator/IndicatorMainTemplate.c");
                case AlgoType.Robot:
                    return StringResourceReader.Read(FolderPath + "Robot/RobotMainTemplate.c");
                default:
                    throw new ArgumentOutOfRangeException("algoType");
            }
        }
    }
}
