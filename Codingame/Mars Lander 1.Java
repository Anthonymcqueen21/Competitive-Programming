import java.util.*;
import java.math.*;
class Player {
    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        int N = in.nextInt();
        int[] surfaceX = new int[N];
        int[] surfaceY = new int[N];
        for (int i = 0; i < N; i++) {
            surfaceX[i] = in.nextInt();
            surfaceY[i] = in.nextInt();
        }
        while (true) {
            int X = in.nextInt();
            int Y = in.nextInt();
            int dX = in.nextInt();
            int dY = in.nextInt();
            in.nextInt(); in.nextInt(); in.nextInt();
            // Finds landing altitude by Looking for 2 consecutive points w/ same Y
            int groundY = -1;
            for (int i = 0; (i < N && groundY == -1); i++)
                if (surfaceX[i] <= X && X <= surfaceX[i+1])
                    groundY = surfaceY[i];
            /* Finds out if it is safe to start braking only next turn, by
               simulating it and analyzing vertical speed at landing
               If it is safe, waits, else starts braking*/
            double vdY = dY - 8.555;
            double vY = Y - 36.665 + 5*dY;
            long t = Math.round((-40-vdY)/0.289);
            if (vY + t*(vdY+0.289*(1+t)/2) > groundY)
                System.out.println("0 0");
            else
                System.out.println("0 4");
        }
    }
}
