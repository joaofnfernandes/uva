import java.io.*;
import java.util.regex.*;


public class WordCount {
	
	private String filename;
	private boolean hasPrinted = false;
	
	public WordCount(String filename) {
		this.filename = filename;
	}
	
	public static void PrintFileNotFound() {
		System.out.print("WordCount <filename>");
	}
	
	public void Count(){
		BufferedReader reader;
		Pattern regexPattern = Pattern.compile("[a-zA-Z]+");
		int wordCount;
		Matcher regexMatcher;
		String line;
		
		try {
			reader = new BufferedReader(new FileReader(filename));
			while((line = reader.readLine()) != null) {
				wordCount = 0;
				regexMatcher = regexPattern.matcher(line);
				while(regexMatcher.find()) {
					wordCount++;
				}
				if (hasPrinted) {
					System.out.print("\n" + wordCount);
				} else {
					hasPrinted = !hasPrinted;
					System.out.print(wordCount);
				}
				
			}
			
		} catch(FileNotFoundException e) {
			PrintFileNotFound();
		} catch (IOException e) {
			PrintFileNotFound();			
		}
	}
	
	
	
	public static void main(String args[]) {
		if(args.length > 0) {
			(new WordCount(args[0])).Count();
		} else {
			WordCount.PrintFileNotFound();
			System.exit(0);
		}
	}

}
