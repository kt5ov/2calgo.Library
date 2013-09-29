using _2calgo.Presenter.PartsProviders;

namespace _2calgo.Presenter
{
    abstract class TemplateProvider
    {
        protected const string FolderPath = @"pack://application:,,,/2calgo.Presenter;component/Templates/";

        protected string GetTemplate(string mainTemplate)
        {
            return mainTemplate
                .Replace("#Usings_PLACE_HOLDER#", StringResourceReader.Read(FolderPath + "Common/Usings.c"))
                .Replace("#InnerParts_PLACE_HOLDER#", InnerPartsProvider.Get())
                .Replace("#OuterParts_PLACE_HOLDER#", OuterPartsProvider.Get());
        }
    }
}
