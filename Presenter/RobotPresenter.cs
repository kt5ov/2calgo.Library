using _2calgo.Model;

namespace _2calgo.Presenter
{
    public class RobotPresenter : AlgoPresenter
    {
        public override string GenerateCodeFrom(Algo algo)
        {
            var builder = new RobotBuilder();
            InitializeAlgoBuilder(algo, builder);

            return builder.Build();
        }
    }
}