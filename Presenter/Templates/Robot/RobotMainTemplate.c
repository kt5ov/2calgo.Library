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

#Mq4Fields_PLACE_HOLDER#
        
	protected override void OnStart()
    {
		CommonInitialize();			            
		#DebugActions_PLACE_HOLDER#           

		Mq4Init();

		var mq4Thread = new Thread(Mq4ThreadStart);
		mq4Thread.Start();
    }

    AutoResetEvent _mq4Start = new AutoResetEvent(false);
    AutoResetEvent _mq4Finished = new AutoResetEvent(false);
    DesiredTrade _desiredTrade;
    Position _positionToProtect;
    PendingOrder _pendingOrderToModify;
    Position _lastOpenedPosition;
    PendingOrder _lastPlacedOrder;

    protected override void OnTick()
    {
        var openedPositionTrade = _desiredTrade as OpenPositionTrade;
        var protectPositionTrade = _desiredTrade as ProtectPositionTrade;
        var modifyPendingOrderTrade = _desiredTrade as ModifyPendingOrderTrade;
        var cancelPendingOrderTrade = _desiredTrade as CancelPendingOrderTrade;


        if (openedPositionTrade != null
            && _positionToProtect != null 
            && (!openedPositionTrade.StopLoss.HasValue || _positionToProtect.StopLoss.HasValue) 
            && (!openedPositionTrade.TakeProfit.HasValue || _positionToProtect.TakeProfit.HasValue) )
        {
            _positionToProtect = null;
            _desiredTrade = null;
            ExecuteMq4Code();
        } else if (protectPositionTrade != null
            && (!protectPositionTrade.StopLoss.HasValue || _positionToProtect.StopLoss.HasValue) 
            && (!protectPositionTrade.TakeProfit.HasValue || _positionToProtect.TakeProfit.HasValue) )
            {
                _positionToProtect = null;
                _desiredTrade = null;
                ExecuteMq4Code();
            } 
        else if (modifyPendingOrderTrade != null
            && (!modifyPendingOrderTrade.StopLoss.HasValue || _pendingOrderToModify.StopLoss.HasValue) 
            && (!modifyPendingOrderTrade.TakeProfit.HasValue || _pendingOrderToModify.TakeProfit.HasValue) )
        {
                _pendingOrderToModify = null;
                _desiredTrade = null;
                ExecuteMq4Code();
        }
        else if (cancelPendingOrderTrade != null && Account.PendingOrders.All(o => o != cancelPendingOrderTrade.PendingOrder))
        {
            _desiredTrade = null;
            ExecuteMq4Code();
        }
        else if (_desiredTrade == null)
            ExecuteMq4Code();
    }

    private void ExecuteMq4Code()
    {
        _mq4Start.Set();
        _mq4Finished.WaitOne();             
    }

    protected override void OnPositionOpened(Position openedPosition)
    {
        _lastOpenedPosition = openedPosition;
        var openedPositionTrade = _desiredTrade as OpenPositionTrade;
        if (openedPositionTrade != null)
        {
            if (openedPositionTrade.StopLoss == null && openedPositionTrade.TakeProfit == null)
            {
                _desiredTrade = null;
                ExecuteMq4Code();
            }
            else
            {
                _positionToProtect = openedPosition;
                Trade.ModifyPosition(openedPosition, openedPositionTrade.StopLoss, openedPositionTrade.TakeProfit);
            }
        }
    }

    protected override void OnPendingOrderCreated(PendingOrder newOrder)
    {
        _lastPlacedOrder = newOrder;
        _desiredTrade = null;
        ExecuteMq4Code();
    }

    protected override void OnPositionClosed(Position position)
    {
        _desiredTrade = null;
        ExecuteMq4Code();        
    }

    private void Mq4ThreadStart()
    {
    	while (_mq4Start.WaitOne())
    	{
			try
			{
				Mq4Start();
			}
			catch(Exception e)
			{
				#HandleException_PLACE_HOLDER#
			}
    		_mq4Finished.Set();
    	}
    }

	private IndicatorDataSeries CreateDataSeries()
	{
		return null;
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

    static class ConvertExtensions
    {
        public static double? ToNullableDouble(this double protection)
        {
            if (protection == 0)
                return null;
            return protection;
        }
    }

}