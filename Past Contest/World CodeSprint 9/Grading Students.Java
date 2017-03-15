import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;

class grading {
    static int convert(int score){
        if (score < 38) return score;
        int toNext5 = 5 - score % 5;
        if (toNext5 < 3){
            score += toNext5;
        }
        return score;
    }
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        for(int a0 = 0; a0 < n; a0++) {
            int grade = in.nextInt();
                System.out.println(convert(grade));
        }
    }
}
