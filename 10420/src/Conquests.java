import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.TreeMap;


public class Conquests {

	public static void exit(String message) { 
		System.out.println(message);
		System.exit(0);
	}
	
	public static void main(String[] args) {
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		TreeMap<String,Integer> countries = new TreeMap<String, Integer>();
		StringBuilder builder = new StringBuilder();
		boolean hasPrinted = false;
		String country;
		String[] conquests = new String[0];
		int linesToParse = 0;
		try {
			 linesToParse = Integer.parseInt(reader.readLine());
		} catch (NumberFormatException e) {
			Conquests.exit("First line of input must be a number");
		} catch(IOException e) {
			Conquests.exit("Problem reading first line");
		}
		if (linesToParse <= 0) {
			Conquests.exit("First line needs to be a number greater than zero");
		}
		while((linesToParse--) > 0) {
			try {
				conquests = reader.readLine().split("\\s", 2);
			} catch(IOException e) {
				Conquests.exit("Problem reading conquests");
			}
			if(conquests.length <= 1) {
				Conquests.exit("Each conquest must have a Country and a name");
			}
			country = conquests[0];
			int count = countries.containsKey(country) ? countries.get(country) + 1 : 1;
			countries.put(country, count);
		}
		for(String c : countries.keySet()) {
			if(!hasPrinted) {
				hasPrinted = true;
			} else {
				builder.append("\n");
			}
			builder.append(c + " " + countries.get(c));
		}
		System.out.print(builder.toString());

	}

}
