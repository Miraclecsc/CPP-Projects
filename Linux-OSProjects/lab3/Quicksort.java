import java.util.Arrays;
import java.util.Scanner;
import java.util.concurrent.*;

public class Quicksort extends RecursiveAction 
{
    private static final int rounds = 100;
    private int[] array;
    private int left;
    private int right;

    public Quicksort(int[] array, int left, int right) 
    {
        this.array = array;
        this.left = left;
        this.right = right;
    }



    private static void swap(int[] array, int i, int j) 
    {
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
    
    private static int partition(int[] array, int left, int right) 
    {
        int pivot = array[right];
        int i = left - 1;

        for (int j = left; j < right; j++) {
            if (array[j] <= pivot) {
                i++;
                swap(array, i, j);
            }
        }

        swap(array, i + 1, right);
        return i + 1;
    }

    private static void insertionSort(int[] array, int left, int right) 
    {
        for (int i = left + 1; i <= right; i++) {
            int current = array[i];
            int j = i - 1;

            while (j >= left && array[j] > current) {
                array[j + 1] = array[j];
                j--;
            }

            array[j + 1] = current;
        }
    }
    protected void compute() 
    {
        if (right - left < rounds)
            insertionSort(array, left, right);
        else 
        {
            int pivot = partition(array, left, right);
            Quicksort leftSort = new Quicksort(array, left, pivot - 1);
            leftSort.fork();
            leftSort.join();
            Quicksort rightSort = new Quicksort(array, pivot + 1, right);
            rightSort.fork();
            rightSort.join();
        }
    }

    public static void main(String[] args) 
    {
        System.out.println("Enter the number of elements: ");
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        int[] array = new int[n];
        
        System.out.println("Enter the elements: ");
        for (int i = 0; i < n; i++) 
            array[i] = scanner.nextInt();

        ForkJoinPool pool = new ForkJoinPool();
        pool.invoke(new Quicksort(array, 0, array.length - 1));

        for (int i = 0; i < n; i++) {
            System.out.print(array[i] + " ");
        }
    }
}