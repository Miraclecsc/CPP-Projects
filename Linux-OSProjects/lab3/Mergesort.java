import java.util.Arrays;
import java.util.Scanner;
import java.util.concurrent.*;

public class Mergesort extends RecursiveAction
{
    private static final int rounds = 100;
    private int[] array;
    private int begin;
    private int end;

    public Mergesort(int[] array, int begin, int end)
    {
        this.array = array;
        this.begin = begin;
        this.end = end;
    }  
    private static void insertionSort(int[] array, int begin, int end)
    {
        for (int i = begin + 1; i <= end; i++) {
            int current = array[i];
            int j = i - 1;

            while (j >= begin && array[j] > current) {
                array[j + 1] = array[j];
                j--;
            }

            array[j + 1] = current;
        }
    }
    protected void compute()
    {
        if (end - begin < rounds) {
            insertionSort(array, begin, end);
        } else 
        {
            int middle = (begin + end) / 2;
            Mergesort leftSort = new Mergesort(array, begin, middle);
            leftSort.fork();
            leftSort.join();
            Mergesort rightSort = new Mergesort(array, middle + 1, end);
            rightSort.fork();
            rightSort.join();
            merge(array, begin, middle, end);
        }
    }  
    private static void merge(int[] array, int begin, int middle, int end)
    {
        int[] left = Arrays.copyOfRange(array, begin, middle + 1);
        int[] right = Arrays.copyOfRange(array, middle + 1, end + 1);

        int i = 0;
        int j = 0;
        int k = begin;

        while (i < left.length && j < right.length) {
            if (left[i] <= right[j]) 
                array[k++] = left[i++];
            else 
                array[k++] = right[j++];
        }

        while (i < left.length) 
            array[k++] = left[i++];

        while (j < right.length) 
            array[k++] = right[j++];
    }
    public static void main(String[] args)
    {
        System.out.println("Enter the number of elements:");
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        int[] array = new int[n];
        System.out.println("Enter the elements:");
        for (int i = 0; i < n; i++) 
            array[i] = scanner.nextInt();

        ForkJoinPool pool = new ForkJoinPool();
        pool.invoke(new Mergesort(array, 0, n - 1));

        for (int i = 0; i < n; i++) {
            System.out.print(array[i] + " ");
        }
    }
}