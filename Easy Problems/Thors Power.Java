import java.util.*;
import java.io.*;
import java.math.*;
class Player {
    
    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        int LX = in.nextInt();
        int LY = in.nextInt();
        int TX = in.nextInt();
        int TY = in.nextInt();
        in.nextLine();
        
        // The Game Loop
        while(true) {
            int E = in.nextInt();
            in.nextLine();
            
            // map size 40 X 18
            String direction = "";
            if(LY > TY && TY < 17) {
                TY++;
                direction+= "S";
            }
            if(LY < TY && TY > 0) {
                TY--;
                direction+= "N";
            }
            if(LX > TX && TX < 39){
                TX++;
                direction+= "E";
            }
            if(LX < TX && TX > 0){
                TX--;
                direction+= "W";
            }
            System.out.println(direction);
        }
    }
}
