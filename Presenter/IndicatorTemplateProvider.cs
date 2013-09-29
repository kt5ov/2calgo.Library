namespace _2calgo.Presenter
{
    static class IndicatorTemplateProvider
    {
        private const string FolderPath = @"pack://application:,,,/2calgo.Presenter;component/Templates/";

        public static string GetTemplate()
        {
            var mainTemplate = StringResourceReader.Read(FolderPath + "Indicator/MainTemplate.c");

            return mainTemplate
                .Replace("#Usings_PLACE_HOLDER#", StringResourceReader.Read(FolderPath + "Common/Usings.c"))
                .Replace("#InnerParts_PLACE_HOLDER#", InnerPartsProvider.Get())
                .Replace("#OuterParts_PLACE_HOLDER#", OuterPartsProvider.Get());
        }
    }
}
