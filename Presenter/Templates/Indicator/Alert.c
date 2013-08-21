	[Conditional("Alert")]
    //{
	
	Lazy<AlertWindowWrapper> _alertWindowWrapper = new Lazy<AlertWindowWrapper>(() => 
	{
		var appDomain = AppDomain.CreateDomain("Alert domain");
        var alertWindowWrapper = (AlertWindowWrapper)appDomain.CreateInstanceAndUnwrap(typeof(AlertWindowWrapper).Assembly.FullName, typeof (AlertWindowWrapper).FullName);
		return alertWindowWrapper;
	});

	void Alert(params object[] objects)
    {
        var text = string.Join("", objects.Select(o => o.ToString()));      
        _alertWindowWrapper.Value.ShowAlert(text);
    }

	internal static class FolderService
    {
        public static string SystemAppData
        {
            get { return Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData); }
        }

        public static string AlertFolder
        {
            get { return EnsureDirectoryExists(Path.Combine(_2calgoFolder, "Alert")); }
        }

        public static string _2calgoFolder
        {
            get { return EnsureDirectoryExists(Path.Combine(SystemAppData, "2calgo")); }
        }

        public static string EnsureDirectoryExists(string directory)
        {
            Directory.CreateDirectory(directory);
            return directory;
        }
    }

    public class AlertWindowWrapper : MarshalByRefObject
    {
        private Thread _windowThread;
        private readonly AutoResetEvent _loadedEvent = new AutoResetEvent(false);

        const string AlertSynchPrefix = "2calgo_alert_";
        private readonly Mutex _accessFileMutex = new Mutex(false, AlertSynchPrefix + "AccessFileMutex");
        private readonly Mutex _checkForWindowMutex = new Mutex(false, AlertSynchPrefix + "CheckForWindowMutex");
        private readonly EventWaitHandle _fileUpdatedEvent = new EventWaitHandle(false, EventResetMode.AutoReset, AlertSynchPrefix + "FileUpdatedEvent");
        private readonly EventWaitHandle _windowExistsEvent = new EventWaitHandle(false, EventResetMode.ManualReset, AlertSynchPrefix + "WindowExistsEvent");
        private readonly AutoResetEvent _windowClosedEvent = new AutoResetEvent(false);
        private object _window;

        public void ShowAlert(string message)
        {
            var messagesFilePath = Path.Combine(FolderService.AlertFolder, "messages.txt");

            _accessFileMutex.WaitOne();
            try
            {
                if (!File.Exists(messagesFilePath))
                    File.WriteAllText(messagesFilePath, string.Empty);
                var formattedMessage = string.Format("{0}|{1}{2}", DateTime.Now, message, Environment.NewLine);
                File.AppendAllText(messagesFilePath, formattedMessage);
            }
            finally
            {
                _accessFileMutex.ReleaseMutex();
            }
            _fileUpdatedEvent.Set();

            CreateWindowIfNeeded(messagesFilePath);
        }

        private void CreateWindowIfNeeded(string messagesFilePath)
        {
            _checkForWindowMutex.WaitOne();
            try
            {
                if (!_windowExistsEvent.WaitOne(0))
                {
                    _windowExistsEvent.Set();

                    var alertWindowModel = CreateWindow();
                    var readingThread = new Thread(() =>
                        {
                            while (true)
                            {
                                var signaledEvent = WaitAny(_fileUpdatedEvent, _windowClosedEvent);
                                if (signaledEvent == _windowClosedEvent)
                                {
                                    _windowExistsEvent.Reset();
                                    if (ReadAlertItemsFromFile(messagesFilePath, false).Any())
                                        CreateWindowIfNeeded(messagesFilePath);
                                    break;
                                }

                                var items = alertWindowModel.Items.ToList();
                                var newItems = ReadAlertItemsFromFile(messagesFilePath, true);
                                items.InsertRange(0, newItems);
                                if (items.Any())
                                    alertWindowModel.Message = items[0].Message;
                                alertWindowModel.Items = items;
                            }
                        });
                    readingThread.Start();
                }
            }
            finally
            {
                _checkForWindowMutex.ReleaseMutex();
            }
        }
        
        private IEnumerable<AlertItem> ReadAlertItemsFromFile(string messagesFilePath, bool cleanFileAfterRead)
        {
            var newItems = new List<AlertItem>();
                                
            _accessFileMutex.WaitOne();
            try
            {
                var contentLines = File.ReadAllLines(messagesFilePath);
                foreach (var contentLine in contentLines)
                {
                    var parts = contentLine.Split('|');
                    var time = DateTime.Parse(parts[0]);
                    var textMessage = contentLine.Remove(0, parts[0].Length + 1);
                    newItems.Insert(0, new AlertItem(time, textMessage));
                }
                if (cleanFileAfterRead)
                    File.WriteAllText(messagesFilePath, string.Empty);
            }
            finally
            {
                _accessFileMutex.ReleaseMutex();
            }

            return newItems;
        }

        private WaitHandle WaitAny(params WaitHandle[] waitHandles)
        {
            var signaledEventIndex = WaitHandle.WaitAny(waitHandles);
            return waitHandles[signaledEventIndex];
        }

        private AlertWindowModel CreateWindow()
        {
            AlertWindowModel windowModel = null;
            _windowThread = new Thread(() =>
            {
                _window = ReflectionHelper.CreateInstance(WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.Window");
                SubscribeToEvent(_window, "Loaded", "Window_Loaded");
                SubscribeToEvent(_window, "Closing", "Window_Closing");
                ReflectionHelper.SetProperty(_window, "ShowInTaskbar", true);
                ReflectionHelper.SetProperty(_window, "ShowActivated", true);
                ReflectionHelper.SetProperty(_window, "Width", 525);
                ReflectionHelper.SetProperty(_window, "Height", 400);
                ReflectionHelper.SetProperty(_window, "WindowStartupLocation", ReflectionHelper.GetStaticValue(WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.WindowStartupLocation", "CenterScreen"));
                ReflectionHelper.SetProperty(_window, "WindowStyle", ReflectionHelper.GetStaticValue(WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.WindowStyle", "ToolWindow"));
                ReflectionHelper.SetProperty(_window, "Title", "Alert");
                var rootGrid = WpfReflectionHelper.CreateGrid();
                WpfReflectionHelper.SetMargin(rootGrid, 5);
                WpfReflectionHelper.AddAutoRowDefinition(rootGrid);
                WpfReflectionHelper.AddStarRowDefinition(rootGrid);

                var messageGrid = WpfReflectionHelper.CreateGrid();
                WpfReflectionHelper.AddAutoColumnDefinition(messageGrid);
                WpfReflectionHelper.AddStarColumnDefinition(messageGrid);

                var image = ReflectionHelper.CreateInstance(WpfReflectionHelper.PresentationFrameworkAssembly,
                                                            "System.Windows.Controls.Image");
                WpfReflectionHelper.SetMargin(image, 20);
                var bitmapImage = Base64ToImage("iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAS/0lEQVR42u2ae6xlV33fP+uxn2efc+49996ZOw/PeLDnYWzjUY2NQTxK3cQlGLVBlKKmIqVBTZES2qZBNEFJIaFt1KZRUJRWqBUNtFJRTAKBPkJJIhVDoDEkwY9gbDxjz/vOfZ999tnPtVb/2Gfm3jtzZwDb+E5FtrS079065+y1vuv7+/5+6/f7KXboyr75T1/7E2+U73/gjvLN9+xn+PlH61M7MQ+1Ey+tV/7TjwX7HvycN/rGvcvnn73n/LnsXZHT337yonnspZ6L3AkAmvN/8ivmzGdFtnSeleURdx0fiK89W/3KTsxlRwBYOn1m/9P/57/x+GNPcOb0mHv+5j+hN+vt34m56J146Vrq89sP/QXSOJ55DtZ+41OcWjD8wDAgnjvI1x93nFvWEAd85tNf54Fjlh8YBnT7fV55BF7/hj6dJOa/f+40/R78zp/9gDDAuZ3Z7e0u8VK85GM/Fd401Q+Oah3e7Af+rpfddvu/fObRpzh6bEAYR5w8sYKzFXESfsCY8mKe588O0/GTD/7i8pn/LwH4wI+o2W5s39pNeFOau9c2Rs32e5r9eyPmdyf4nkcQBGjPYy1rqGrDeJxTlhV1VWNMgzMWgV2yTfWlNG3+1/qQ333fJ1m6oQH49Xd0Do/z4oNxh7c9/KjxswLe/qaEO26fpdefJogitA6Ikz5BZwoVxkgvxNmGOhtSjlPG6Qqry0ssXFjg5MlVzp6vMRYij8pXfGo05oO/9D94+oYD4Jff7L86z6sv3HV8V2f/LfO8472P8vP/sM/9b7iF/uw8nd40fhgjpYeMpxFhDJ4PygdnoS7BWOx4jfHqOdYunuHsqWc4+fQZFpcc5xchy6Ebk5UNP/SrX+ArN4wI/sIPS5mOqo/fdCDpPPCm4/h+hJLQiQK8IMbzO3hBByEkpsyw42VckUJdgKna4RpwFQKD0hrPD4njhE43pN+DA/OwewBfeorOKOfjb73rxZn7i+IGrXH3KcXhV933MpRSxJFACVBa4UcJXtRHyBBTZ2SrCwhrCKMYP4pQYQTW4oTEyQBrwDYWIT08P8T3NFKCENAJQfvw6FkOH5rhPuCPbwgGSMlRz4O9+6ZaVLVASlBKtTvvJQgvQQUJIDBNTVNV2KqEqoCmAieQXoyKpgg6A6LugCDqoL2tezQuYGEISnL0hmGA59MTAsIwuiwsUoAQEuVHCB23Q1ikklwVBQiB0D74XQQa4TQ6NPhRgva8LR8ta7CAdfRuGAB8X0RKua2sECCEQMgAoTqgYoSrEEJur8XKAx2B9RBKIlSNF8QotfXEXjbtcI7ohgFASdeLog2HYoxDidZuhfLa3VcxmOE1fNEEAOWDCBBWInSN1AFCSMQmX2Vdy4LYv4EYICRTccffsNPcIeWltSmc0Aj5HV4lNUgfZAROI1SBlBopt3pqa6GsIPaYupE0YBDH/hZGX5q3m1DeIRDO4py7BooKhNcCYD2EzGAbc9FA1YCAwY2jAZ6YieNNYuVaALRy4MA52mDHmWsDICVIBdJDSBDS2/azgWxdV1Ezc8MA4LB7NgMwyloAWmFsh3OuBeG6tqQmnlm0d3s1YIFqJx0q9twwkaDnsTuOww0RtBYlQEuLc7aFwNkJGNeLysUWWJ0zV30l1C0AWcXuGwKA//LTMvQ0s1EcXH6WFyVCtgxwzoJz16b+JZu5YjrOWZy1uCsQiALwgKxg9i1HCHccAKXYrz2Io3CTFyiRAnzPgW3axTgHSIQQ1wFh02KdaRlwBQCxD55oD0aBZv8NAIA46HmCKNqIS/LctiKoBdbUOFthmvI6i7+0Trf1wTasSSLQCkYFBB4Hd1wElRKHpFREmxgwylqqOuewTYWrM6yrcdptf/5uRQK2BMli2xN7EoKnICsg0BzacQCkFEfDMMDzNuKAYZoTR2CNwdQ5rslwrsLSXEcGTeslxIYeCCGvAqwbgy9bADoBR3YcAO3pI0m3s+XZ2npOHAmsMTTVGNdkWKdpnMHZa+T/J3EC2A3qi8k5+AoAtIK8BHjhJ8IXDICAO5Ik2fJsedWQxAJjDGWeEpcpaE1pK6wx29tAmwgAKpytgbo9OImrXC7dCGoDznHHjorg7/9yr6s9fagTb2XA0nJKN4GmMRRZSp4ukafLZOtLWNNcI6vSgK3B5GDGOFNdM2E36LU6sD7m0Dtup7tjACglj2utRRzHW4Kgi0uGXiIxxjDOhozWlxivL5ENV6ircrtCAZgGbNUC0IxxpoBrnB0G3dYMhhki8Di+YyYgpbxXKsVmAM4vrGMMTPXAWst4NEaplYlIOhQW3+tvo4ENmGISApQ4k2NMjbPuKm846IEnYS2D+WnuBR7emThAiNckna1Ji2eey1ACZqbbjc3znNEwJUtTxqOMsii3IYDDmRrqHEyOM61wNuUYs41mDHqTWCCH0OM1O2ICD//avFRSvK6bbDXBJ55cI4mhE4tJcsQyHhdk45yqqinLawFQ4coUV48wVUo1XqPMM0xztWZ4ujUDKSCveN3bjz3/dTz/Lwpxl6e9uc4VHuCRbyxxYO9kUduMsiy3tWvb1JhyRJ2tUIxWyEdrlHlKXdfbvn/XNGgJKylzgeaul94EhHiL73vE0Yb9nzi1ytMncg6/TEwi2Yn9Xl5wC0K1DQtMXVMXGWW2TpWtUYzXKfKMum6uC8ByCnHAW15yAITgbUmSbLH/j3zsKW6ah4N7NnZ8I9JtwbDOURT5NhpYUxdjqnFKMU4ps5RiPMY02wdOcQhTSRsQCcHbXlIA/uQ3Dxz3tL4z6W7kJX/r09/mkUfXePCvXk6BbGLBxuJxjnGebxMIWpqyoCwyyvGIYpxR5CXXO0XvmQEtYHHInT/28ufnDp8fA5z7Sd/3uRQA/eFXz/FvP3aSv/sA9JKNk+0lELiUDXIO6xxlUWC3CYkbU1MVOUWeU+QFjbl+28zcFIQ+XFyDTsBPviQAfO3fH5wXQvx4r99HSsnnv3yWn/k3T/D3H4CDeybrZGPnrbPt+p27/Nxay3icbR8QWoe1pmXLd4xDYN8cGAvDnB//O0eZ/74DYK39kO/7Ua/b5+O/d4L3/+pf8I/eDEf2b2idm+z+hga4DXc3eZaN0hcjo8X8NEQ+nF4mijw+9H0F4I8/sv9+IcS7pwcDlFKcu1iiVZukcG4T9S8z3l2O5C7fncNaxygb4ewLb5WRAg7OgTGwsM6733oL939P4fx3+8E//Nezxz0tPzs1PdUZTM8ghOB1d89x/32z6HiGfftn2Lt3jufO1/zBlwsGPUg6AinaNJiSEqUVUkqUUigpCOOYOElwztFUFXVdUV2+11R1TV3VVJWhrqBp2sLIlSPQbX7gwjoi8viRA12+cHLIhReNAZ/5+d6DJ0+uPry+Ppqta8PyyjLpKGWc53QTwfysj6dj1tKa1WGOp8TGbk9E4UqTcM6xeHFh21D3KpcrIQgkSddjaipgZsZnbk4yNQUqgLwBrWG5hC+fZfbPF3j4r0zz4Hd5nL/+9b8/PPWPpZb/bv++fSrpJJRlSVlV1E1FVdY88s0xv/7JRY4MLHcfkiRJOzqJTxj6hKEgCBRh5BFHAX7Q9gd5gY/nefQP3E00c5i6SCmLdUy+QpEtkQ+XyYYZ2XrNcHXE4mLK8lLF6lrJ8lrFegpF3dYKmVSLLgxbJjQGpmPMzbP8s4ee5iPPG4D/+cHuh5QWv3j41sMMBoNreETH3/7Zr5CuZ7z7h7fmMS8ldJQS+L7CDxS+L/F9TRj4+IHPrt272bNnL6M0JctGjNIRo9GIUTYmy0qyrCbPDOMxVHWrLdZCXbejmdyryf3ZRXh2GSoDMx04sodf+q9P8C++Zw343Z8LPiwVv3DbsaN89ournDqzinZDhsOUNM24cHGVx568yCc/9y2qIuPBV0IQtBkb34cgBD+AMIIwVMSxRxT5hFFAGASEYUAUhtx08yHipNvWg2RbHLmcCRMOQZsen4QROLbXAWNbcDpB+/+4bBlSNrzhjUfwvnGBP/qu8wH/+T3ez6ytVh+4/Y6b0J7HnlnFhz96glcfNbzq2NbPHt4Ft+7a5P4mk5RCsHt+QBSGaK1QWiNlK4Ke5xHHHWZmZ+j0+tgX6A2EhMBvT4mqgX0DWEhhWMIzq7BU8oG/cYsc//4z9l99RwY89D7/7UqZjx47douYnZ2mLAtme3DP7T6/9lDKwgL0NJRlm8NwgFYCz1dEkSKOfcJQ44c+vW6HTidGa40X+MRRRJJ06fV6JEmPIAxRWl/lBUC0QPkBQRST9HoMpnvMzfXp9z2SxMMPfZSGRjjGlePiOpxehFOLcHoF0rqdW6Da/qs9fffX/vrL+eYjp3jimhrw0Pv8O5qm+uqhQwc6R48eQQjB6nrJf/zUCb76p+d45WHLrXsmtte0vtc0E/o5Jn1BE+qHijD0SZKAuX272L13lmS6j477eOEUvp/g+QlCRRhrKYqMMh9SZmvkwwXSlQVWl9cYLuWsLaUsL+esrzkaM3mvbe92I8oG24phUYOSbS/RYtqKYmPhtXeSac19v/klHr8KgN/+Wd+ryuprg9nuK1517z0Y6/itz5zkdz5/itfeZrntwNYqnttUyXKu/SUp2xdLuVHtFpPnfuCxa27Avv37iOOYIIgIwxAdBG1onKZk4xHZaMQ4y8jGY7JxzjjLGaUVoxGUBdTNhvBdKYJNvQGAVm0Z7VsXYJi33xMSfvQNPHZ2kbs/+hXqLXFAWbv3KC1e8fKX38ZTz6X86Hu/wiNfe5Z3/VC7eMfWc82WxXNFFLi50iUu9Tk41taHnDt/AaV0O7QGBNrTeJ6Hpz209i4HTJtLac+no1MI6PhtBtnT7bz/7FvceXC/954tIvgb73u1ovi/P7V77yynFwx/75//OW+513D8lisqV5N/lALPVygNOIGQAmcdUgmUFO2uK9FW+ZVAa0UUBSSdhMFg+qqSQJYW35e29bSAcdUCcAnA0wvwGmne+/53vfE/dGb2Gf3Bn/tpvao9PzFPPdvrdm/99MMXwBlx7MD2dAfwfcXUwEfKtnAh2hIZytdoLVFSIWjvSkq01vhhQBRGBMHWirbyFOuLBcLVeHL7nZaqbZD0Bcim3dnvFEAOx5BdWrzYoPp0D/TUzV/Xs69JGlfmunSRcrX0nvTe/RMXTz19/yvuWvlb/yAMX/mNM0u7QkZ6rmtFL3YE3kaVajw2VHVBJ9F0pzw6/Yi43yFKenhBiABMWVKNxzTjum19mHxZTBohNle8HHD24jp7Z8IJzq1ZJFMJ3V0DpB8idICQkqZpyEdDVpdWWDw7ZOG0o17b+K2qgZVRGwj5qtWC0MfF04NzgwM3/+nUgdt+b6lz81ettUFtZKM9PzBBEDQgitIc//LjWf1Us7u46aY96W2hW70zTy++bDFdnFf1sCubUeDqXFpTCl9ZkRQ1rgNTg3lmDtzN1Owx/LCLwGGqdYr0WUYXv022vLpR+J20z20+7Zd1QzoqGAaQhJLO9BT9To9wah/h4DBB7yAyGCCUD87R5MtcPPFHfPuxPwBxkmwEdkL3vJZWdbpVtzOVdgazi52Z3c+FM/uf8KLeCd/3z0jPu+BrPZZSVs65RmvtGa29Ympqqul2uxWQVVW1VlXVubIsH5dlsVfl+b6yGO0tytEeU6zuGq0vTjX5WmJW0+BEnnmPX6jl/J5nxIEDQszNzZMkiQg8gWsSqjqidENE7RDCIaRFaodUlrqxGGFYXc8pKoNSGhlFeHGC150nmLoV0TtKIQJMVpANz7G0cNqdPfE433r8CXfmVGXXhoebJuxWoj81Vp1BOtObWep0kqUoii5EUXQ+DMNzYRhe8H1/IQiCZa310DmXra2tVWmaNltM7hOf+IScnp5W/X7fi6IocM7FdV13y7LsV1U1U5blTFmWc0VRzJVFMZcXxUyRZ9N5nvXKIuvWVR41dRlYU3rYUps6V6bOpW1qITAi8oXQWhLHsZCuaXsAhWQ9LcjymjBQziGonHTa80H6zuJZ47RtjDYIr5EqLIMgLjudzjiKojSO4/UwDFejKFoOgmAxDMPFIAgWfd9fDoJgxff9dd/3R8aYfDwelysrK02WZfad73ynva53+eIXvyiCIJD9fl9prXUcx55zLvA8L6yqKjbGJMaYpGma3uZR13XXGNOt67rTNE3cNE1kjAmapvGNMZ4xRltrlXNWOndZm5wQwgkprJKqUUo1Sqlaa10qpUqtda61zrTWmed5qdZ6eMVIlVIjpVTm+/64LMtCKVWlaVqXZdmkaWqbprGvf/3r3fd8HN58nTx5UjjnRBiGMggCqbWWzjnl+752zmmllBZCeLR9TL619tLfnnNOW2v1xPVKNvrhxKYGISuEMM65RkrZCCEaoAZqKWUF1EKIummaevLcFEXRKKVMWZa2LEtb17UTQrhDhw65FyUf8D353TQVm6pAwvM8IYTAOdee7dpgQlyvY2ziLZwQwk3uGGMwxrQsEQIhBN1u1/GX119eL/j6f13TfTYXJjiZAAAAAElFTkSuQmCC");
                ReflectionHelper.SetProperty(image, "Source", bitmapImage);
                WpfReflectionHelper.AddChild(messageGrid, image);

                var messageTextBlock =
                    ReflectionHelper.CreateInstance(WpfReflectionHelper.PresentationFrameworkAssembly,
                                                    "System.Windows.Controls.TextBlock");

                WpfReflectionHelper.SetGridColumn(messageTextBlock, 1);
                ReflectionHelper.SetProperty(messageTextBlock, "HorizontalAlignment", ReflectionHelper.GetEnumValue(WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.HorizontalAlignment", "Left"));
                ReflectionHelper.SetProperty(messageTextBlock, "TextWrapping", ReflectionHelper.GetEnumValue(WpfReflectionHelper.PresentationCoreAssembly, "System.Windows.TextWrapping", "Wrap"));
                ReflectionHelper.SetProperty(messageTextBlock, "VerticalAlignment", ReflectionHelper.GetEnumValue(WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.VerticalAlignment", "Center"));
                ReflectionHelper.SetProperty(messageTextBlock, "FontSize", 18);
                WpfReflectionHelper.SetBinding(messageTextBlock, "Message", "System.Windows.Controls.TextBlock", "TextProperty");
                WpfReflectionHelper.AddChild(messageGrid, messageTextBlock);

                var allAlertsGrid = WpfReflectionHelper.CreateGrid();
                WpfReflectionHelper.SetGridRow(allAlertsGrid, 1);
                WpfReflectionHelper.AddStarRowDefinition(allAlertsGrid);
                WpfReflectionHelper.AddAutoRowDefinition(allAlertsGrid);
                var dataGrid = ReflectionHelper.CreateInstance(WpfReflectionHelper.PresentationFrameworkAssembly,
                                                               "System.Windows.Controls.DataGrid");
                WpfReflectionHelper.SetBinding(dataGrid, "Items", "System.Windows.Controls.ItemsControl", "ItemsSourceProperty");
                WpfReflectionHelper.AddChild(allAlertsGrid, dataGrid);
                var closeButton = ReflectionHelper.CreateInstance(
                    WpfReflectionHelper.PresentationFrameworkAssembly, "System.Windows.Controls.Button");
                SubscribeToEvent(closeButton, "Click", "ButtonOnClick");
                WpfReflectionHelper.SetGridRow(closeButton, 1);
                ReflectionHelper.SetProperty(closeButton, "Width", 100);
                WpfReflectionHelper.SetMargin(closeButton, 0, 5, 0, 0);
                ReflectionHelper.SetProperty(closeButton, "Content", "Close");
                WpfReflectionHelper.AddChild(allAlertsGrid, closeButton);
                
                WpfReflectionHelper.AddChild(rootGrid, messageGrid);
                WpfReflectionHelper.AddChild(rootGrid, allAlertsGrid);
                
                ReflectionHelper.SetProperty(_window, "Content", rootGrid);
                windowModel = new AlertWindowModel();
                ReflectionHelper.SetProperty(_window, "DataContext", windowModel);
                
                ReflectionHelper.InvokeMethod(_window, "ShowDialog");
            }
                )
            {
                IsBackground = false
            };

            _windowThread.TrySetApartmentState(ApartmentState.STA);
            _windowThread.Start();
            _loadedEvent.WaitOne();

            return windowModel;
        }

        private static object Base64ToImage(string stringValue)
        {

            var binaryData = Convert.FromBase64String(stringValue);

            var bitmapImage = ReflectionHelper.CreateInstance(WpfReflectionHelper.PresentationCoreAssembly,
                                                              "System.Windows.Media.Imaging.BitmapImage");
            
            ReflectionHelper.InvokeMethod(bitmapImage, "BeginInit");
            ReflectionHelper.SetProperty(bitmapImage, "StreamSource", new MemoryStream(binaryData));
            ReflectionHelper.InvokeMethod(bitmapImage, "EndInit");

            return bitmapImage;
        }

        public void ButtonOnClick(object sender, object routedEventArgs)
        {
            ReflectionHelper.InvokeMethod(_window, "Close");
        }

        public void Window_Loaded(object s, object e)
        {
            _loadedEvent.Set();
        }

        public void Window_Closing(object sender, CancelEventArgs e)
        {
            _windowClosedEvent.Set();
        }

        private void SubscribeToEvent(object @object, string eventName, string handlerName)
        {
            var eventInfo = @object.GetType().GetEvent(eventName);
            var methodInfo = GetType().GetMethod(handlerName);
            var handler = Delegate.CreateDelegate(eventInfo.EventHandlerType, this, methodInfo);
            eventInfo.AddEventHandler(@object, handler);
        }
    }

    public class AlertWindowModel : INotifyPropertyChanged
    {
        private string _message;
        private IEnumerable<AlertItem> _items = new List<AlertItem>();

        public string Message
        {
            get { return _message; }
            set
            {
                if (_message == value)
                    return;

                _message = value;
                OnPropertyChanged("Message");
            }
        }

        public IEnumerable<AlertItem> Items
        {
            get { return _items; }
            set
            {
                if (_items == value)
                    return;

                _items = value;
                OnPropertyChanged("Items");
            }
        }


        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            var handler = PropertyChanged;
            if (handler != null)
                handler(this, new PropertyChangedEventArgs(propertyName));
        }
    }

    public class AlertItem
    {
        public DateTime Time { get; private set; }
        public string Message { get; private set; }

        public AlertItem(DateTime time, string message)
        {
            Time = time;
            Message = message;
        }
    }

    public static class WpfReflectionHelper
    {
        public static Assembly PresentationFrameworkAssembly = Assembly.Load("PresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35");
        public static Assembly PresentationCoreAssembly = Assembly.Load("PresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35");
        
        public static void SetMargin(object rootGrid, params object[] parameters)
        {
            var thickness = ReflectionHelper.CreateInstance(PresentationFrameworkAssembly, "System.Windows.Thickness", parameters);
            ReflectionHelper.SetProperty(rootGrid, "Margin", thickness);
        }

        public static object CreateGrid()
        {
            return ReflectionHelper.CreateInstance(PresentationFrameworkAssembly, "System.Windows.Controls.Grid");
        }

        public static void AddChild(object grid, object child)
        {
            var rootGridChildren = ReflectionHelper.GetPropertyValue(grid, "Children");
            ReflectionHelper.InvokeMethod(rootGridChildren, "Add", child);
        }

        private static readonly object AutoGridLegth = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly,
                                                                                       "System.Windows.GridLength", "Auto");

        public static void AddAutoRowDefinition(object grid)
        {
            var rowDefinition = CreateRowDefinition();
            ReflectionHelper.SetProperty(rowDefinition, "Height", AutoGridLegth);
            AddRowDefinition(grid, rowDefinition);
        }

        public static void AddStarRowDefinition(object grid)
        {
            var rowDefinition = CreateRowDefinition();
            var starUnitType = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly,
                                                               "System.Windows.GridUnitType", "Star");
            var gridLength = ReflectionHelper.CreateInstance(PresentationFrameworkAssembly, "System.Windows.GridLength", new[]{ 1, starUnitType});
            ReflectionHelper.SetProperty(rowDefinition, "Height", gridLength);
            AddRowDefinition(grid, rowDefinition);
        }

        private static object CreateRowDefinition()
        {
            var rowDefinition = ReflectionHelper.CreateInstance(PresentationFrameworkAssembly,
                                                                "System.Windows.Controls.RowDefinition");
            return rowDefinition;
        }

        public static void AddRowDefinition(object grid, object rowDefinition)
        {
            var rowDefinitions = ReflectionHelper.GetPropertyValue(grid, "RowDefinitions");
            ReflectionHelper.InvokeMethod(rowDefinitions, "Add", rowDefinition);
        }

        public static void AddAutoColumnDefinition(object grid)
        {
            var ColumnDefinition = CreateColumnDefinition();
            ReflectionHelper.SetProperty(ColumnDefinition, "Width", AutoGridLegth);
            AddColumnDefinition(grid, ColumnDefinition);
        }

        public static void AddStarColumnDefinition(object grid)
        {
            var ColumnDefinition = CreateColumnDefinition();
            var starUnitType = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly,
                                                               "System.Windows.GridUnitType", "Star");
            var gridLength = ReflectionHelper.CreateInstance(PresentationFrameworkAssembly, "System.Windows.GridLength", new[]{ 1, starUnitType});
            ReflectionHelper.SetProperty(ColumnDefinition, "Width", gridLength);
            AddColumnDefinition(grid, ColumnDefinition);
        }

        private static object CreateColumnDefinition()
        {
            var ColumnDefinition = ReflectionHelper.CreateInstance(PresentationFrameworkAssembly,
                                                                "System.Windows.Controls.ColumnDefinition");
            return ColumnDefinition;
        }

        public static void AddColumnDefinition(object grid, object ColumnDefinition)
        {
            var ColumnDefinitions = ReflectionHelper.GetPropertyValue(grid, "ColumnDefinitions");
            ReflectionHelper.InvokeMethod(ColumnDefinitions, "Add", ColumnDefinition);
        }

        public static void SetGridColumn(object element, int column)
        {
            var columnProperty = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly, "System.Windows.Controls.Grid", "ColumnProperty");
            ReflectionHelper.InvokeMethod(element, "SetValue", columnProperty, column);
        }

        public static void SetGridRow(object element, int row)
        {
            var columnProperty = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly, "System.Windows.Controls.Grid", "RowProperty");
            ReflectionHelper.InvokeMethod(element, "SetValue", columnProperty, row);
        }

        public static void SetBinding(object element, string path, string propertyTypeName, string propertyName)
        {
            var property = ReflectionHelper.GetStaticValue(PresentationFrameworkAssembly, propertyTypeName, propertyName);
            ReflectionHelper.InvokeMethod(element, "SetBinding", property, path);
        }
    }

    public static class ReflectionHelper
    {
        public static object GetEnumValue(Assembly assembly, string typeName, string value)
        {
            var type = assembly.GetType(typeName);
            return Enum.Parse(type, value);
        }

        public static void InvokeMethod(object instance, string methodName, params object[] parameters)
        {
            instance.GetType().InvokeMember(methodName, BindingFlags.InvokeMethod, null, instance, parameters);
        }

        public static object InvokeStaticMethod(Assembly assembly, string typeName, string methodName, params object[] parameters)
        {
            return assembly.GetType(typeName).InvokeMember(methodName, BindingFlags.InvokeMethod, null, null, parameters);
        }

        public static object GetStaticValue(Assembly assembly, string typeName, string propertyName)
        {
            var type = assembly.GetType(typeName);
            var propertyInfo = type.GetProperty(propertyName);
            if (propertyInfo != null)
                return propertyInfo.GetValue(null, null);

            var fieldInfo = type.GetField(propertyName);
            return fieldInfo.GetValue(null);
        }

        public static void SetProperty(object instance, string propertyName, Assembly assembly, string typeName, params object[] parameters)
        {
            var propertyInfo = instance.GetType().GetProperty(propertyName);
            var propertyValue = CreateInstance(assembly, typeName, parameters);
            propertyInfo.SetValue(instance, propertyValue, null);
        }

        public static void SetProperty(object instance, string propertyName, object value)
        {
            var propertyInfo = instance.GetType().GetProperty(propertyName);
            propertyInfo.SetValue(instance, value, null);
        }

        public static object GetPropertyValue(object instance, string propertyName)
        {
            var propertyInfo = instance.GetType().GetProperty(propertyName);
            return propertyInfo.GetValue(instance, null);
        }

        public static object CreateInstance(Assembly assembly, string typeName)
        {
            return CreateInstance(assembly, typeName, new object[0]);
        }

        public static object CreateInstance(Assembly assembly, string typeName, object[] parameters)
        {
            var pointType = assembly.GetType(typeName);
            var propertyValue = Activator.CreateInstance(pointType, parameters);
            return propertyValue;
        }
    }
	//}