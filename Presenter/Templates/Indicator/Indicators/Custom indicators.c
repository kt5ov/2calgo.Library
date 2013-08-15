    [Conditional("IndicatorBuffers")]
	void IndicatorBuffers(int count) {}

	[Conditional("SetIndexStyle")]
    void SetIndexStyle(int index, int type, int style=EMPTY, int width=EMPTY, int clr=CLR_NONE) { }

	[Conditional("IndicatorDigits")]
    void IndicatorDigits(int digits) { }

	[Conditional("IndicatorDigits")]
    void IndicatorDigits(double digits) { }
	
	[Conditional("SetIndexDrawBegin")]
    void SetIndexDrawBegin(int index, int begin) { }

	[Conditional("SetLevelValue")]
	void SetLevelValue(int level, double value) { }

	[Conditional("SetIndexBuffer")]
    bool SetIndexBuffer(int index, Mq4OutputDataSeries dataSeries) 
    {
      return true;
    }

	[Conditional("IndicatorShortName")]
    void IndicatorShortName(string name) { }

	[Conditional("SetIndexLabel")]
    void SetIndexLabel(int index, string text) { }

    const string xArrow = "\u2716";
    public Dictionary<int, string> ArrowByIndex = new Dictionary<int, string>{ {0, xArrow},  {1, xArrow},  {2, xArrow},  {3, xArrow},  {4, xArrow},  {5, xArrow},  {6, xArrow},  {7, xArrow}};
    void SetIndexArrow(int index, int code) 
    {
        ArrowByIndex[index] = GetArrowByCode(code);
    }

    public static string GetArrowByCode(int code)
    {
        switch(code)
        {
            case 32: return "\u0020"; case 33: return "\u270F"; case 34: return "\u2702"; case 35: return "\u2701"; case 36: return "\u1F453"; case 37: return "\u1F514"; case 38: return "\u1F4D6"; case 40: return "\u260E"; case 41: return "\u2706"; case 42: return "\u2709"; case 44: return "\u1F4EA"; case 45: return "\u1F4EB"; case 46: return "\u1F4EC"; case 47: return "\u1F4ED"; case 48: return "\u1F4C1"; case 49: return "\u1F4C2"; case 50: return "\u1F4C4"; case 54: return "\u231B"; case 55: return "\u2328"; case 58: return "\u1F4BB"; case 60: return "\u1F4BE"; case 62: return "\u2707"; case 63: return "\u270D"; case 65: return "\u270C"; case 66: return "\u1F44C"; case 67: return "\u1F44D"; case 68: return "\u1F44E"; case 69: return "\u261C"; case 70: return "\u261E"; case 71: return "\u261D"; case 72: return "\u261F"; case 73: return "\u270B"; case 74: return "\u263A"; case 75: return "\u1F610"; case 76: return "\u2639"; case 77: return "\u1F4A3"; case 78: return "\u2620"; case 79: return "\u2690"; case 80: return "\u1F6A9"; case 81: return "\u2708"; case 82: return "\u263C"; case 83: return "\u1F4A7"; case 84: return "\u2744"; case 86: return "\u271E"; case 88: return "\u2720"; case 89: return "\u2721"; case 90: return "\u262A"; case 91: return "\u262F"; case 92: return "\u0950"; case 93: return "\u2638"; case 94: return "\u2648"; case 95: return "\u2649"; case 96: return "\u264A"; case 97: return "\u264B"; case 98: return "\u264C"; case 99: return "\u264D"; case 100: return "\u264E"; case 101: return "\u264F"; case 102: return "\u2650"; case 103: return "\u2651"; case 104: return "\u2652"; case 105: return "\u2653"; case 106: return "\u0026"; case 107: return "\u0026"; case 108: return "\u25CF"; case 109: return "\u274D"; case 110: return "\u25A0"; case 111: case 112: return "\u25A1"; case 113: return "\u2751"; case 114: return "\u2752"; case 115: return "\u2B27"; case 116: return "\u29EB"; case 117: return "\u25C6"; case 118: return "\u2756"; case 119: return "\u2B25"; case 120: return "\u2327"; case 121: return "\u2353"; case 122: return "\u2318"; case 123: return "\u2740"; case 124: return "\u273F"; case 125: return "\u275D"; case 126: return "\u275E"; case 127: return "\u25AF"; case 128: return "\u24EA"; case 129: return "\u2460"; case 130: return "\u2461"; case 131: return "\u2462"; case 132: return "\u2463"; case 133: return "\u2464"; case 134: return "\u2465"; case 135: return "\u2466"; case 136: return "\u2467"; case 137: return "\u2468"; case 138: return "\u2469"; case 139: return "\u24FF"; case 140: return "\u2776"; case 141: return "\u2777"; case 142: return "\u2778"; case 143: return "\u2779"; case 144: return "\u277A"; case 145: return "\u277B"; case 146: return "\u277C"; case 147: return "\u277D"; case 148: return "\u277E"; case 149: return "\u277F"; case 158: return "\u00B7"; case 159: return "\u2022"; case 160: case 166: return "\u25AA"; case 161: return "\u25CB"; case 162: case 164: return "\u2B55"; case 165: return "\u25CE"; case 167: return "\u2716"; case 168: return "\u25FB"; case 170: return "\u2726"; case 171: return "\u2605"; case 172: return "\u2736"; case 173: return "\u2734"; case 174: return "\u2739"; case 175: return "\u2735"; case 177: return "\u2316"; case 178: return "\u27E1"; case 179: return "\u2311"; case 181: return "\u272A"; case 182: return "\u2730"; case 183: return "\u1F550"; case 184: return "\u1F551"; case 185: return "\u1F552"; case 186: return "\u1F553"; case 187: return "\u1F554"; case 188: return "\u1F555"; case 189: return "\u1F556"; case 190: return "\u1F557"; case 191: return "\u1F558"; case 192: return "\u1F559"; case 193: return "\u1F55A"; case 194: return "\u1F55B"; case 195: case 197: case 215: case 219: case 223: case 231: return "\u25C0"; case 196: case 198: case 224: return "\u25B6"; case 213: return "\u232B"; case 214: return "\u2326"; case 216: return "\u27A2"; case 220: return "\u27B2"; case 232: return "\u2794"; case 233: case 199: case 200: case 217: case 221: case 225: return "\u25B2"; case 234: case 201: case 202: case 218: case 222: case 226: return "\u25BC"; case 239: return "\u21E6"; case 240: return "\u21E8"; case 241: return "\u21E7"; case 242: return "\u21E9"; case 243: return "\u2B04"; case 244: return "\u21F3"; case 245: case 227: case 235: return "\u2B00"; case 246: case 228: case 236: return "\u2B01"; case 247: case 229: case 237: return "\u2B03"; case 248: case 230: case 238: return "\u2B02"; case 249: return "\u25AD"; case 250: return "\u25AB"; case 251: return "\u2717"; case 252: return "\u2713"; case 253: return "\u2612"; case 254: return "\u2611";
            default:
                return xArrow;
        }
    }

	[Conditional("SetIndexShift")]
    void SetIndexShift(int index, int shift) 
    {
      _allBuffers[index].SetShift(shift);
    }
        
	[Conditional("SetIndexEmptyValue")]
    void SetIndexEmptyValue(int index, double value)
    {
      _allBuffers[index].SetEmptyValue(value);
    }
	    
    private int _indicatorCounted;
	[Conditional("IndicatorCounted")]
    private int IndicatorCounted()
    {
        return _indicatorCounted;
    }