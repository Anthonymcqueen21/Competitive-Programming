import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;

public class BoatTrips {

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        int c = in.nextInt();
        int m = in.nextInt();
        int[] p = new int[n];
        int count = 0;
        for(int p_i=0; p_i < n; p_i++){
            p[p_i] = in.nextInt();
        }
        for(int i = 0; i < n; i++){
            if(p[i] <= (m*c) || p[i] <= c){
                count = count+ 1;
            }
            else
                break;
        }
        if(count == n){
            System.out.println("Yes");
        }
        else
            System.out.println("No");
    }
}
