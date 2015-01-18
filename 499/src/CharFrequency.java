import java.io.*;
import java.util.*;

/**
 * 
 * TODO: we don't need to allocate int[256]. 57 are enough.
 *
 */
public class CharFrequency {

	private String filename;
	private boolean hasPrinted = false;
	
	public CharFrequency(String filename) {
		this.filename = filename;
	}
	
	public void Count() {
		BufferedReader reader;
		StringBuilder builder = new StringBuilder();
		int[] map;
		String line;
		char c;
		int maxValue;
		
		try {
			reader = new BufferedReader(new FileReader(filename));
			// for each line in input
			while((line = reader.readLine()) != null) {
				map = new int[256];
				maxValue = 0;
				
				for(int i = 0; i < line.length(); i++) {
					c = line.charAt(i);
					// A-Za-z
					if ((int) c >= 65 && (int) c <= 122) {
						map[(int)c]++;						
					}
				}
			
				maxValue = max(map);
				
				
				if(!hasPrinted) {
					hasPrinted = true;
				} else {
					builder.append("\n");
				}
				builder.append(maxFrequencyToString(map, maxValue));
			}
			System.out.print(builder.toString());
			
		} catch(FileNotFoundException e) {
			PrintFileNotFound();
		} catch(IOException e) {
			PrintFileNotFound();
		}
	}
	
	private int max(int[] arr) {
		int m = arr[0];
		for(int i = 1; i < arr.length; i++) {
			if (arr[i] > m) {
				m = arr[i];
			}
		}
		return m;
	}
	
	private String maxFrequencyToString(int[] arr, int max) {
		StringBuilder builder = new StringBuilder();
		// output all chars that have max frequency
		for(int i = 0; i < arr.length; i++) {
			if(arr[i] == max) {
				builder.append((char) i);
			}
		}
		builder.append(" " + max);
		return builder.toString();
	}
	
	public static void PrintFileNotFound() {
		System.out.print("WordCount <filename>");
	}
	
	
	public static void main(String[] args) {
		if(args.length > 0) {
			(new CharFrequency(args[0])).Count();
		} else {
			CharFrequency.PrintFileNotFound();
			System.exit(0);
		}
	}

}
