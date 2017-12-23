import java.util.Arrays;
import java.util.Scanner;

public class TheSalesman {
    static int minimumTime(int[] x) {
        int diff = 0;
        Arrays.sort(x);
        for (int i = 0; i < x.length-1; i++) {
            diff += Math.abs(x[i]-x[i+1]);
        }
        return diff ;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int t = in.nextInt();
        for(int a0 = 0; a0 < t; a0++){
            int n = in.nextInt();
            int[] x = new int[n];
            for(int x_i = 0; x_i < n; x_i++){
                x[x_i] = in.nextInt();
            }
            int result = minimumTime(x);
            System.out.println(result);
        }
        in.close();
    }
}
