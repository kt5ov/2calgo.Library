static class Comparers
{
    public static IComparer<T> GetComparer<T>()
    {
        if (typeof(T) == typeof(Mq4Double))
            return (IComparer<T>)new Mq4DoubleComparer();

        return Comparer<T>.Default;
    }
}