#References_PLACE_HOLDER##Usings_PLACE_HOLDER#

namespace cAlgo.Robots
{
    [Robot()]
    public class #AlgoName_PLACE_HOLDER# : Robot
    {
#Mq4Functions_PLACE_HOLDER#

[Conditional("SendMail")]
//{
		[Parameter("Email from", DefaultValue = "your_email@gmail.com")]
        public string EmailAddressFrom { get; set; }

        [Parameter("Email to", DefaultValue = "your_email@gmail.com")]
        public string EmailAddressTo { get; set; }
//}

#Parameters_PLACE_HOLDER#

#Lines_declarations_PLACE_HOLDER#
     
#Mq4Fields_PLACE_HOLDER#
        
#ColorParameters_PLACE_HOLDER#
                
#WidthParameters_PLACE_HOLDER#

#LevelParameters_PLACE_HOLDER#

#Inverted_buffers_declarations_PLACE_HOLDER#     

List<Mq4OutputDataSeries> AllBuffers = new List<Mq4OutputDataSeries>();     
public List<DataSeries> AllOutputDataSeries = new List<DataSeries>();
        
	protected override void OnStart()
    {
#Initialize_buffers_PLACE_HOLDER#
#Initialize_AllOutputDataSeries_PLACE_HOLDER#        
		CommonInitialize();			            
		#DebugActions_PLACE_HOLDER#           

		try
        {
			Mq4Init();
		}
        catch(Exception e)
        {
            #HandleException_PLACE_HOLDER#
        }  
    }

    protected override void OnTick()
    {        
        var index = MarketSeries.Close.Count - 1;
        #Buffers_SetCurrentIndex_PLACE_HOLDER#        

        try
        {
            Mq4Start();
        }
        catch(Exception e)
        {
            #HandleException_PLACE_HOLDER#
        }    
    }

    private bool IsLastBar
    {
        get { return true; }
    }

    private bool IsTradeAllowed()
    {
        return true;
    }

#InnerParts_PLACE_HOLDER#
#RobotInnerParts_PLACE_HOLDER#

	}

	//Custom Indicators Place Holder

#OuterParts_PLACE_HOLDER#

}