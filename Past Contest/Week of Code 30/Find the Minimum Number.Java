import java.io.PrintWriter;
import java.util.Scanner;
public class Solution {
	public static void main(String[] args) {
		try(
	        Scanner in = new Scanner(System.in);
	        PrintWriter out = new PrintWriter(System.out);
		) {
			// get a count of min function called
			int n = in.nextInt();
			// init
			String repeatedPart = "min(int, ";
			String closedPart = "int";
			StringBuilder result = new StringBuilder();
			StringBuilder parentheses = new StringBuilder();
			// constraints
			if(n < 2 || n > 50) {
				throw new Exception("n is out of range.");
			}
			for(int i = 0; i < n; i++) {
				if(i != n - 1) {
					// append repeated part and parentheses
					result.append(repeatedPart);
					parentheses.append(")");
				} else {
			        // append closed part and parentheses
				result.append(closedPart).append(parentheses.toString());
				}
			}
		// print
			out.println(result.toString());
		} catch(Exception e) {

		}
	}
}
