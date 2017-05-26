import java.util.*;
import java.io.*;
import java.math.*;
class Solution {
    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        int N = in.nextInt();
        in.nextLine();
        String TEMPS = in.nextLine();
        
        int result = 0;
        if (TEMPS.isEmpty()) {
            System.out.println(result);
        }
        else {
            List<Integer> list = new ArrayList<Integer>();
            List<Integer> originalList = new ArrayList<Integer>();
            for (int i = 0; i < N; i++) {
                list.add(Math.abs(Integer.valueOf(TEMPS.split(" ")[i])));
                originalList.add(Integer.valueOf(TEMPS.split(" ")[i]));
            }
            if (originalList.size() == 1) {
                result = originalList.get(0);
            }
            else {
                Collections.sort(list);
                result = list.get(N-1);
                for (int i = 0; i < list.size(); i++) {
                    int tmp = originalList.get(i);
                    System.err.println(result);
                    if (Math.abs(tmp) <= Math.abs(result)) {
                        result = tmp;
                    }
                }
                if (Math.signum(result) == -1.0 && originalList.contains(Math.abs(result))) {
                    result = Math.abs(result);
                    System.err.println(result);
                }
            }
            System.out.println(result);
        }
    }
}
