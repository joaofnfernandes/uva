import java.awt.font.NumericShaper;
import java.io.*;

public class EcologicalPremium {

	private String filename;
	
	public EcologicalPremium(String filename) {
		this.filename = filename;
	}
	
	
	public void Compute() {
		BufferedReader reader;
		int numberOfTests = 0;
		int numberOfFarmers = 0;
		int[] testsSubsidie;
		int[] farmersSubsidie;
		
		try {
			reader = new BufferedReader(new FileReader(filename));
			// How many simulations?
			numberOfTests = Integer.parseInt(reader.readLine());
			if(numberOfTests <= 0) {
				fail("Input file needs to have at least one test");
			}
			testsSubsidie = new int[numberOfTests];
			
			// How many farmers?
			for(int simulation = 0; simulation < numberOfTests; simulation++) {
				numberOfFarmers = Integer.parseInt(reader.readLine());
				if(numberOfFarmers <= 0) {
					fail("Simulation #" + simulation + " needs at least one farmer");
				}
				
				farmersSubsidie = new int[numberOfFarmers];
				for(int farmer = 0; farmer < numberOfFarmers; farmer++) {
					int[] farmerData = parseFarmerData(reader.readLine());
					float animalPerM = (float)farmerData[0] / (float)farmerData[1];
					farmersSubsidie[farmer] = Math.round(farmerData[1] * farmerData[2] * (float)animalPerM);
				}
				testsSubsidie[simulation] = sum(farmersSubsidie);
			}
			printResult(testsSubsidie);
			
		} catch(FileNotFoundException e) {
			PrintFileNotFound();
		} catch(IOException e) {
			PrintFileNotFound();
		}
		
	}
	
	private int sum(int[] arr) {
		int sum = 0;
		for (int i = 0; i < arr.length; i++) {
			sum+= arr[i];
		}
		return sum;
	}
	
	private int[] parseFarmerData(String FarmerData) {
		int[] parsedData = new int[3];
		String[] data = FarmerData.split("\\s");
		
		if (data.length != 3) {
			fail("Farmer's data invalid in simmulation ");
		} else {
			for (int i = 0; i < data.length; i++) {
				parsedData[i] = Integer.parseInt(data[i]);
			}
		}
		return parsedData;
	}
	
	private void printResult(int[] subsidies) {
		StringBuilder builder = new StringBuilder();
		boolean hasPrinted = false;
		
		for (int i = 0; i < subsidies.length; i++) {
			if(!hasPrinted) {
				hasPrinted = true;
			} else {
				builder.append("\n");
			}
			builder.append(subsidies[i]);
		}
		System.out.print(builder.toString());
	}
	
	private void fail(String reason) {
		System.out.println("Input file needs to have at least one test");
		System.exit(0);
	}
	
	
	public static void PrintFileNotFound() {
		System.out.print("EcologicalPremium <filename>");
	}
	
	public static void main(String[] args) {
		if(args.length > 0) {
			(new EcologicalPremium(args[0])).Compute();
		} else {
			EcologicalPremium.PrintFileNotFound();
			System.exit(0);
		}

	}

}
