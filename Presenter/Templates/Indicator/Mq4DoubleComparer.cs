class Mq4DoubleComparer : IComparer<Mq4Double>
{
    public int Compare(Mq4Double x, Mq4Double y)
    {
        return x.CompareTo(y);
    }
}