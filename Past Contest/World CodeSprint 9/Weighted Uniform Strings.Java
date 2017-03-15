import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;

class weightedUniformString {
    static int weight(char ch){
        return ch - 'a' + 1;
    }
    static Set<Integer> getSet(String s) {
        Set<Integer> set = new HashSet<>();
        int cumWeight = 0;
        for (int i = 0; i < s.length(); i++) {
            char ch = s.charAt(i);
            int chWeight = weight(ch);
            
            if(i != 0 && ch == s.charAt(i-1)){
                cumWeight += chWeight;
            }
            else{
                cumWeight = chWeight;
            }
            set.add(cumWeight);
            set.add(chWeight);
        }
        return set;
    }
    
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        String s = in.next();
        int n = in.nextInt();
        Set<Integer> sol = getSet(s);
        
        for(int a0 = 0; a0 < n; a0++){
            int x = in.nextInt();
            if (sol.contains(x)){
                System.out.println("Yes");
            }
            else{
                System.out.println("No");
            }
        }
    }
}
