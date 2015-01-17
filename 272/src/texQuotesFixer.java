import java.io.*;


public class texQuotesFixer {
	public final static String FILENOTFOUND = "textQuotesFixer <filename>"; 

	private state s = state.initial;
	private String filename;
	
	
	public texQuotesFixer(String filename) {
		this.filename = filename;
	}		
	
	public void FixQuotes() {
		BufferedReader reader;
		StringBuilder builder = new StringBuilder();
		
		try {
			reader = new BufferedReader(new FileReader(filename));
			int c = 0;
			while ((c = reader.read()) != -1) {
				switch (this.s) {
					case initial: {
						// opening quotes
						if((char)c == '"') {
							builder.append("``");
							this.s = state.quotes_started;
						} else {
							builder.append((char) c);
						}
						break;
					}
					
					case quotes_started:
						if((char)c == '"') {
							builder.append("''");
							this.s = state.initial;
						} else {
							builder.append((char) c);
						}
						break;
	
					default:
						throw new IllegalStateException("Invalid case switch");
					}
				}
		} catch (FileNotFoundException e) {
			System.out.print(FILENOTFOUND);
		} catch (IOException e) {
			System.out.print(FILENOTFOUND);
		}
		System.out.print(builder.toString());
		
	}
	
	
	private enum state {
		initial,
		quotes_started
	}
	
	public static void main (String args[]) {
		if(args.length > 0) {
			(new texQuotesFixer(args[0])).FixQuotes();
		} else {
			System.out.print(texQuotesFixer.FILENOTFOUND);
			System.exit(0);
		}
	}

}
