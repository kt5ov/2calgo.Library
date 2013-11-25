using _2calgo.Presenter.PartsProviders;

namespace _2calgo.Presenter
{
    public abstract class TemplateProvider
    {
        protected const string FolderPath = @"pack://application:,,,/2calgo.Presenter;component/Templates/";

        protected string GetTemplate(string mainTemplate)
        {
            return mainTemplate
                .Replace("#Usings_PLACE_HOLDER#", StringResourceReader.Read(FolderPath + "Common/Usings.c"))
                .Replace("#InnerParts_PLACE_HOLDER#", CommonInnerPartsProvider.Get())
                .Replace("#OuterParts_PLACE_HOLDER#", CommonOuterPartsProvider.Get());
        }
    }
}
