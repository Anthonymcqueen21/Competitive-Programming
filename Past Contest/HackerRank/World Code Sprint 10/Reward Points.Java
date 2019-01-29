import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;
public class Solution {
      // Setting points
      static int getPoints(int month1, int month2, int month3){
             if(month1<=10){
                int first;
                first = month1*10;
                month1 = first;
            }else if(month1 > 10) { 
                month1 = 100;
            }
            if(month2 <= 10){
                int second;
                second = month2*10;
                month2 = second;
            }else if(month2 > 10){
                month2 = 100;               
            }
            if(month3 <= 10){
                int third;
                third = month3*10;
                month3 = third;
            }else if(month3 > 10){
                month3 = 100;
            }
        
         return month1 + month2 + month3;
        
      }
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int month1 = sc.nextInt();
        int month2 = sc.nextInt();
        int month3 = sc.nextInt();
        int pointsEarned = getPoints(month1, month2, month3);
        System.out.println(pointsEarned);
    }
}
