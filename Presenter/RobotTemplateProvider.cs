namespace _2calgo.Presenter
{
    class RobotTemplateProvider : TemplateProvider
    {
        public virtual string GetTemplate()
        {
            var mainTemplate = StringResourceReader.Read(FolderPath + "Robot/RobotMainTemplate.c");
            return GetTemplate(mainTemplate);
        }
    }
}