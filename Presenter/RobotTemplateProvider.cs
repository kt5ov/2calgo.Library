using _2calgo.Presenter.PartsProviders;

namespace _2calgo.Presenter
{
    class RobotTemplateProvider : TemplateProvider
    {
        public string GetTemplate()
        {
            var mainTemplate = StringResourceReader.Read(FolderPath + "Robot/RobotMainTemplate.c");
            return GetTemplate(mainTemplate)
                       .Replace("#RobotInnerParts_PLACE_HOLDER#", RobotInnerPartsProvider.Get());
        }
    }
}