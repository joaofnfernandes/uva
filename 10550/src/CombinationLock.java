import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;


public class CombinationLock {

	
	public class Combination{
		int initialState, one, two, three;
		
		public Combination(int initialState, int one, int two, int three) {
			this.initialState = initialState;
			this.one = one;
			this.two = two;
			this.three = three;
		}
		
		public boolean isAllZero() {
			return initialState == 0 && one == 0 && two == 0 && three == 0;
		}
	}
		
	
	
	public void Solve() {
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		StringBuilder builder = new StringBuilder();
		boolean hasPrinted = false;
		String[] stringValues;
		int[] intValues;
		Combination combination;
		while(true) {
			try {
				stringValues = reader.readLine().split("\\s");
				if(stringValues.length != 4) {
					CombinationLock.exit("<initial state> <first number> <second number> <third number>");
				}
				intValues = new int[4];
				for(int i = 0; i < 4; i++) {
					intValues[i] = Integer.parseInt(stringValues[i]);
				}
				combination = new Combination(intValues[0], intValues[1], intValues[2], intValues[3]);
				if (combination.isAllZero()) {
					break;
				}
				if(!hasPrinted) {
					hasPrinted = true;
				} else {
					builder.append("\n");
				}
				builder.append(computeDegrees(combination));
			} catch(IOException e) {
				CombinationLock.exit("Error reading from standard input");
			}
		}
		System.out.println(builder.toString());
		
	}
	
	private int computeDegrees(Combination combination) {
		int degrees = 0;
		degrees += 360* 2;	//turn twice cw
		degrees += Math.abs(40 - (combination.one - combination.initialState)) * 360 / 40; // turn cw until 1st number
		degrees += 360;	//turn twice ccw
		degrees += Math.abs(40 - (combination.one - combination.two)) * 360 / 40; //turn ccw until 2n number
		degrees += Math.abs(40 - (combination.three - combination.two)) * 360 / 40; //turn cw until 3rd number
		return degrees;
	}
	
	
	public static void exit(String message) { 
		System.out.println(message);
		System.exit(0);
	}
	
	public static void main(String[] args) {
		CombinationLock combinationLock = new CombinationLock();
		combinationLock.Solve();
	}

}
