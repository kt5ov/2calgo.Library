using _2calgo.Presenter.PartsProviders;

namespace _2calgo.Presenter
{
    class IndicatorTemplateProvider : TemplateProvider
    {
        public virtual string GetTemplate()
        {
            var mainTemplate = StringResourceReader.Read(FolderPath + "Indicator/IndicatorMainTemplate.c");
            return GetTemplate(mainTemplate)
                       .Replace("#IndicatorInnerParts_PLACE_HOLDER#", IndicatorInnerPartsProvider.Get());
        }
    }
}