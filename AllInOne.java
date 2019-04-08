import java.io.*;
import java.util.*;
import java.util.concurrent.TimeUnit;
//import java.nio.file.Files;
//import java.nio.file.StandardCopyOption;

public class AllInOne {

	public static void main(String[] args) {
		
		//construct a list of all paths	to students submissions
		String currentDirPath = System.getProperty("user.dir");
		File submDir = new File(currentDirPath + "/subms");
		System.out.println("{Current Directory}: " + currentDirPath);
		File[] allDirs = submDir.listFiles();
		
		String CSV_HEADER = "id,firstName,lastName,extraction,compilation,testsRun,testsCorrect,definitelyLost(bytes),definitelyLost(blocks),errorSummary(errors),errorSummary(contexts)";
		
		FileWriter csvWriter = null;
		
		try{
			csvWriter = new FileWriter("allCombined.csv");
			csvWriter.append(CSV_HEADER + "\n");
		} catch (IOException e) {
			System.out.println("{Initialization}: FAILURE\n" + e.getMessage());	
		}
				
		int id = 0;
				
		//loop to go through all tudent folders
		for(File dir: allDirs){
			System.out.println("\n\n~~~~~~~~~~~~~~~~~~~>>");
			System.out.println("{Folder name}: " + dir.getName());
			System.out.println("~~~~~~~~~~~~~~~~~~~>>");
			
			id++;
			
			boolean canProceed = false;
			
			String path = dir.getAbsolutePath();
			
			System.out.println("{Path}: " + path);
			
			String[] nameTokens = dir.getName().split("[\\s _]+");
			//System.out.println("~~~~~~" + nameTokens[0] + " " + nameTokens[1]);
			
			BufferedWriter errorBufWriter = null;
			//BufferedWriter outBufWriter = null;
			try{
				errorBufWriter = new BufferedWriter(new FileWriter(path + "/errorLog.txt"));
				//outBufWriter = new BufferedWriter(new FileWriter(path + "/outputLog.txt"));
				csvWriter.append(id + "," + nameTokens[0] + "," + nameTokens[1] + ",");
			} catch (IOException e){
				System.out.println("{Initialization}: FAILURE\n" + e.getMessage());			
				try {
					errorBufWriter.write("{Initialization}: FAILURE\n" + e.getMessage() + "\n");
				} catch (IOException ex){
					System.out.println("{Initialization}: FAILURE\n" + ex.getMessage());
				}
			}
			
			String[] files = dir.list();
			
			System.out.println("\n{Extracting}: ..." );		
			//System.out.println("{Extracting}: " + filename);
			try {
				errorBufWriter.write("\n{Extracting}: ..." + "\n");
				//errorBufWriter.write("{Extracting}: " + filename + "\n");
			} catch (IOException e){
				System.out.println("{Extracting}: " + e.getMessage());
			}
			
			ProcessBuilder extrProcBuilder = null;
			
			for(String filename: files){
				//extract files	
				
				if(filename.contains(".txt")){
					/*System.out.println("{Extracting}: ignoring");
					try {
						errorBufWriter.write("{Extracting}: ignoring\n");
					} catch (IOException e){
						System.out.println("{Extracting}: " + e.getMessage());
					}*/		
					continue;
				} else if(filename.contains(".zip")){
					extrProcBuilder = new ProcessBuilder("unzip", "-o", path + "/" + filename, "-d", path);
					canProceed = true;
					break;
				} else if(filename.contains(".tar.gz")){
					extrProcBuilder = new ProcessBuilder("sudo", "tar", "--absolute-names", "-C", path, "-xzf", path + "/" + filename);				
					canProceed = true;
					break;
				} else if(filename.contains(".tar.xz")){
					extrProcBuilder = new ProcessBuilder("tar", "-C", path, "-xJf", path + "/" + filename);		
					canProceed = true;
					break;		
				} else if(filename.contains(".tar.bz2")){
					extrProcBuilder = new ProcessBuilder("tar", "-C", path, "-xjf", path + "/" + filename);		
					canProceed = true;
					break;		
				} else if(filename.contains(".tar.gz")){
					extrProcBuilder = new ProcessBuilder("tar", "-C", path, "-xzf", path + "/" + filename);		
					canProceed = true;
					break;		
				} else if(filename.contains(".tar")){
					extrProcBuilder = new ProcessBuilder("tar", "-C", path, "-xvf", path + "/" + filename);		
					canProceed = true;
					break;		
				} else if(filename.contains(".gz")){
					extrProcBuilder = new ProcessBuilder("gunzip", "-c", path + "/" + filename, ">", path);			
					canProceed = true;
					break;	
				} else if(filename.contains(".7z")){
					extrProcBuilder = new ProcessBuilder("7za", "x", "-o" + path, path + "/" + filename, "-y");				
					canProceed = true;
					break;
				} else if(filename.contains(".rar")){
					extrProcBuilder = new ProcessBuilder("unrar", "x", "-o-",  path + "/" + filename, path);		
					canProceed = true;
					break;		
				} else {
					/*System.out.println("{Extracting}: FAILURE (no archive was found)");
					try{
						errorBufWriter.write("{Extracting}: FAILURE (no archive was found)\n");
					} catch (IOException e) {
						System.out.println("{Extracting}: FAILURE\n" + e.getMessage());
					}*/
					continue;
				}
			}
				
			if(!canProceed) {
				System.out.println("{Extracting}: FAILURE (no archive was found)");
				try{
					errorBufWriter.write("{Extracting}: FAILURE (no archive was found)\n");
					csvWriter.append("no,-,-,-,-,-,-,-\n");
				} catch (IOException e) {
					System.out.println("{Extracting}: FAILURE\n" + e.getMessage());
				}
				try {
					errorBufWriter.close();
					//outBufWriter.close();
				} catch (IOException e){
					System.out.println("{Closing}: " + e.getMessage());
				}
				continue;
			}
			canProceed = false;
			
			
			try {
				Process extrProc = extrProcBuilder.start();
				BufferedReader errorReader = new BufferedReader(new InputStreamReader(extrProc.getErrorStream()));
				extrProc.waitFor();
				String errorLine = errorReader.readLine();
				if(errorLine != null){
					System.out.println("{Extracting}: FAILURE");
					errorBufWriter.write("{Extracting}: FAILURE\n");
					System.out.println("==> " + errorLine);
					errorBufWriter.write("==> " + errorLine + "\n"); 
					while((errorLine = errorReader.readLine()) != null){
						System.out.println("==> " + errorLine);
						errorBufWriter.write("==> " + errorLine + "\n");
					}
				} else {
					System.out.println("{Extracting}: SUCCESS");
					errorBufWriter.write("{Extracting}: SUCCESS\n");
					csvWriter.append("yes,");
					canProceed = true;
				}
				extrProc.destroy();
				if(extrProc.isAlive())
					extrProc.destroyForcibly();
				errorReader.close();
			} catch (IOException | InterruptedException e){
				System.out.println("{Extracting}: FAILURE\n" + e.getMessage());
				try {
					errorBufWriter.write("{Extracting}: FAILURE\n" + e.getMessage() + "\n");
				} catch (IOException ex){
					System.out.println("{Extracting}: FAILURE\n" + ex.getMessage());
				}
			}
		
			if(!canProceed) {
				try {
					csvWriter.append("no,-,-,-,-,-,-,-\n");
					errorBufWriter.close();
					//outBufWriter.close();
				} catch (IOException e){
					System.out.println("{Closing}: " + e.getMessage());
				}
				continue;
			}	
			canProceed = false;
			
			ArrayList<File> stackFiles = findFilesByName(dir.listFiles(), "set");
			
			//plant testing files
			System.out.println("\n{Copying}: ..." );
			try	{
				errorBufWriter.write("\n{Copying}: ...\n" );
			} catch (IOException e) {
				System.out.println("\n{Copying}: FAILURE\n" + e.getMessage());
			}
			File testFileFrom = new File(currentDirPath + "/main.cpp");
			File makeFileFrom = new File(currentDirPath + "/Makefile");
			File msetFileFrom1 = new File(currentDirPath + "/mset.cpp");
			File msetFileFrom2 = new File(currentDirPath + "/mset.h");
			
			File testFileTo = new File(path + "/main.cpp");
			File makeFileTo = new File(path + "/Makefile");
			File msetFileTo1 = new File(path + "/mset.cpp");
			File msetFileTo2 = new File(path + "/mset.h");
						
			try{
				//Files.copy(testFileFrom.toPath(), testFileFrom.toPath(), StandardCopyOption.REPLACE_EXISTING);
				//Files.copy(makeFileFrom.toPath(), makeFileFrom.toPath(), StandardCopyOption.REPLACE_EXISTING);
				FileInputStream fileInputStream = new FileInputStream(testFileFrom);
				FileOutputStream fileOutputStream = new FileOutputStream(testFileTo);
				byte[] buffer = new byte[1024];
				int fileLength;
				while((fileLength = fileInputStream.read(buffer)) > 0)
					fileOutputStream.write(buffer, 0, fileLength);
				fileInputStream.close();
				fileOutputStream.close();
				
				fileInputStream = new FileInputStream(makeFileFrom);
				fileOutputStream = new FileOutputStream(makeFileTo);
				
				while((fileLength = fileInputStream.read(buffer)) > 0)
					fileOutputStream.write(buffer, 0, fileLength);
				fileInputStream.close();
				fileOutputStream.close();
				
				fileInputStream = new FileInputStream(msetFileFrom1);
				fileOutputStream = new FileOutputStream(msetFileTo1);
				
				while((fileLength = fileInputStream.read(buffer)) > 0)
					fileOutputStream.write(buffer, 0, fileLength);
				fileInputStream.close();
				fileOutputStream.close();
				
				fileInputStream = new FileInputStream(msetFileFrom2);
				fileOutputStream = new FileOutputStream(msetFileTo2);
				
				while((fileLength = fileInputStream.read(buffer)) > 0)
					fileOutputStream.write(buffer, 0, fileLength);
				fileInputStream.close();
				fileOutputStream.close();
				
				for(File stackFile: stackFiles){
					if(stackFile.getAbsolutePath().equals(path + "/" + stackFile.getName()))
						continue;
					fileInputStream = new FileInputStream(stackFile);
					fileOutputStream = new FileOutputStream(path + "/" + stackFile.getName());
					while((fileLength = fileInputStream.read(buffer)) > 0)
						fileOutputStream.write(buffer, 0, fileLength);
					fileInputStream.close();
					fileOutputStream.close();
				}
				
				canProceed = true;
			} catch (IOException e){
				System.out.println("{Copying}: FAILURE\n");
				try {
						errorBufWriter.write("{Copying}: FAILURE\n" + e.getMessage() + "\n");
				} catch (IOException ex){
					System.out.println("{Copying}: FAILURE\n" + ex.getMessage());
				}
			}
			
			if(!canProceed) {
				try {
					errorBufWriter.close();
					//outBufWriter.close();
					csvWriter.append("no,-,-,-,-,-,-,-\n");
				} catch (IOException e){
					System.out.println("{Closing}: " + e.getMessage());
				}
				continue;
			}
			canProceed = false;
			
			//delete double extensions if exists
			for(File toRenameFile: dir.listFiles()){
				if(toRenameFile.getName().contains("set.cpp.cpp")){
					File newNameFile = new File(dir + "/set.cpp");
					if(toRenameFile.renameTo(newNameFile)){
						System.out.println("\n{Renaming}: SUCCESS");
						try {
							errorBufWriter.write("\n{Renaming}: SUCCESS\n");
						} catch(IOException e) {
							System.out.println("\n{Renaming}: " + e.getMessage());
						}
					} else {
						System.out.println("\n{Renaming}: FAILURE");
						try{
							errorBufWriter.write("\n{Renaming}: FAILURE\n");
						} catch(IOException e) {
							System.out.println("\n{Renaming}: " + e.getMessage());
						}
					}
				}
				
				if(toRenameFile.getName().contains("set.h.h")){
					File newNameFile = new File(dir + "/set.h");
					if(toRenameFile.renameTo(newNameFile)){
						System.out.println("\n{Renaming}: SUCCESS");
						try {
							errorBufWriter.write("\n{Renaming}: SUCCESS\n");
						} catch(IOException e) {
							System.out.println("\n{Renaming}: " + e.getMessage());
						}
					} else {
						System.out.println("\n{Renaming}: FAILURE");
						try{
							errorBufWriter.write("\n{Renaming}: FAILURE\n");
						} catch(IOException e) {
							System.out.println("\n{Renaming}: " + e.getMessage());
						}
					}
				}
			} 
			
			//compile
			System.out.println("\n{Compiler}: ...");
			try {
				errorBufWriter.write("\n{Compiler}: ...\n");
			} catch (IOException e){
				System.out.println("\n{Compiler}: FAILURE\n" + e.getMessage());
			}
			ProcessBuilder compileProcBuilder = new ProcessBuilder("make", "-C", path);
			try {
				Process compileProc = compileProcBuilder.start();
				BufferedReader errorReader = new BufferedReader(new InputStreamReader(compileProc.getErrorStream()));
				//BufferedReader outputReader = new BufferedReader(new InputStreamReader(compileProc.getInputStream()));
				compileProc.waitFor();
				String errorLine = "";
				//String outLine = outputReader.readLine();
				
				boolean errorFlag = false; 
				while((errorLine = errorReader.readLine()) != null){
					if(errorLine.contains("error") || errorLine.contains("Error") || errorLine.contains("Stop"))
						errorFlag = true;
					System.out.println("==> " + errorLine);
					errorBufWriter.write("==> " + errorLine + "\n");
				}
				
				if(errorFlag){
					System.out.println("{Compiler}: FAILURE");
					errorBufWriter.write("{Compiler}: FAILURE\n");
				} else {
					/*if(outLine != null){
						//System.out.println("{Compiler}: WARNING");
						//errorBufWriter.write("{Compiler}: WARNING\n");
						System.out.println("==> " + outLine);
						errorBufWriter.write("==> " + outLine + "\n");
						while((outLine = outputReader.readLine()) != null){
							System.out.println("==> " + outLine);
							errorBufWriter.write("==> " + outLine + "\n");
						}
					}*/
					System.out.println("{Compiler}: SUCCESS");
					errorBufWriter.write("{Compiler}: SUCCESS\n");
					csvWriter.append("yes,");
					canProceed = true;
				}
				errorReader.close();
				//outputReader.close();
				compileProc.destroy();
				if(compileProc.isAlive())
					compileProc.destroyForcibly();
			} catch (IOException | InterruptedException e){
				System.out.println("{Compiler}: FAILURE\n" + e.getMessage());
				try {
						errorBufWriter.write("{Compiler}: FAILURE\n" + e.getMessage() + "\n");
				} catch (IOException ex){
					System.out.println("{Compiler}: FAILURE\n" + ex.getMessage());
				}
			}
			
			if(!canProceed) {
				try {
					errorBufWriter.close();
					//outBufWriter.close();
					csvWriter.append("no,-,-,-,-,-,-\n");
				} catch (IOException e){
					System.out.println("{Closing}: " + e.getMessage());
				}
				continue;
			}
			canProceed = false;
			
			//test
			System.out.println("\n{Test}: ...");
			try {
				errorBufWriter.write("\n{Test}: ...\n");
			} catch (IOException e){
				System.out.println("\n{Test}: FAILURE\n" + e.getMessage());
			}
			ProcessBuilder testProcBuilder = new ProcessBuilder(path + "/main");
			try {
				Process testProc = testProcBuilder.start();
				BufferedReader errorReader = new BufferedReader(new InputStreamReader(testProc.getErrorStream()));
				BufferedReader outputReader = new BufferedReader(new InputStreamReader(testProc.getInputStream()));
				if(!testProc.waitFor(7, TimeUnit.SECONDS)){
					System.out.println("{Test}: FAILURE");
					errorBufWriter.write("{Test}: FAILURE\n");
				
					csvWriter.append("\n");
					errorReader.close();
					outputReader.close();
					testProc.destroy();
					if(testProc.isAlive())
						testProc.destroyForcibly();
						
					File mainFile = new File(path + "/main");
					try {
						if(mainFile.exists())
							mainFile.delete();
						else {
							System.out.println("\n{Cleaning}: FAILURE (no executable file found)");
							errorBufWriter.write("\n{Cleaning}: FAILURE (no executable file found)\n");
						}
					} catch (IOException e){
						System.out.println("\n{Cleaning}: FAILURE (deletion failed)");
						try {
							errorBufWriter.write("\n{Cleaning}: FAILURE (deletion failed)\n");
						} catch (IOException ex){
							System.out.println("\n{Cleaning}: FAILURE");
						}
					}
		
					try {
						errorBufWriter.close();
						//outBufWriter.close();
					} catch (IOException e){
						System.out.println("\n{Closing}: " + e.getMessage());
					}
					System.out.println("\n\n");	
					continue;
				}
				
				//String testsRun = "", testsCorrect = "";
				String errorLine = errorReader.readLine();
				if(errorLine != null){
					System.out.println("{Test}: FAILURE");
					errorBufWriter.write("{Test}: FAILURE\n");
					System.out.println("==> " + errorLine);
					errorBufWriter.write("==> " + errorLine + "\n");
					while((errorLine = errorReader.readLine()) != null){
						System.out.println("==> " + errorLine);
						errorBufWriter.write("==> " + errorLine + "\n");
					}
				}
				
				int testsIncorrect = 0, testsCorrect = 0;
				String outLine;
				while((outLine = outputReader.readLine()) != null){
					/*if(outLine.contains("TESTS RUN:")){
						testsRun = outLine.replaceAll("\\D+", "");
					}
					if(outLine.contains("TESTS CORRECT:")){
						testsCorrect = outLine.replaceAll("\\D+", "");
					}*/
					if(outLine.contains("~CORRECT")){
						testsCorrect++;
					} else if(outLine.contains("~INCORRECT")){
						testsIncorrect++;						
					}
					System.out.println("==> " + outLine);
					//outBufWriter.write(outLine + "\n");
					errorBufWriter.write("==> " + outLine + "\n");
				}
				
				csvWriter.append((testsCorrect + testsIncorrect) + "," + testsCorrect + ",");
				
				errorReader.close();
				outputReader.close();
				testProc.destroy();
				if(testProc.isAlive())
					testProc.destroyForcibly();
			} catch (IOException | InterruptedException e){
				System.out.println("{Test}: FAILURE\n" + e.getMessage());
				try {
						errorBufWriter.write("{Test}: FAILURE\n" + e.getMessage() + "\n");
				} catch (IOException ex){
					System.out.println("{Test}: FAILURE\n" + ex.getMessage());
				}
			}
			
			//valgrind checking
			System.out.println("\n{Valgrind}: ...");
			try {
				errorBufWriter.write("\n{Valgrind}: ...\n");
			} catch (IOException e){
				System.out.println("\n{Valgrind}: FAILURE\n" + e.getMessage());
			}
			ProcessBuilder valProcBuilder = new ProcessBuilder("valgrind", path + "/main");
			try {
				Process valProc = valProcBuilder.start();
				
				BufferedReader errorReader = new BufferedReader(new InputStreamReader(valProc.getErrorStream()));
				BufferedReader outputReader = new BufferedReader(new InputStreamReader(valProc.getInputStream()));
				if(!valProc.waitFor(7, TimeUnit.SECONDS)){
					System.out.println("{Valgrind}: FAILURE");
					errorBufWriter.write("{Valgrind}: FAILURE\n");
				
					csvWriter.append("\n");
					errorReader.close();
					outputReader.close();
					valProc.destroy();
					if(valProc.isAlive())
						valProc.destroyForcibly();
						
					File mainFile = new File(path + "/main");
					try {
						if(mainFile.exists())
							mainFile.delete();
						else {
							System.out.println("\n{Cleaning}: FAILURE (no executable file found)");
							errorBufWriter.write("\n{Cleaning}: FAILURE (no executable file found)\n");
						}
					} catch (IOException e){
						System.out.println("\n{Cleaning}: FAILURE (deletion failed)");
						try {
							errorBufWriter.write("\n{Cleaning}: FAILURE (deletion failed)\n");
						} catch (IOException ex){
							System.out.println("\n{Cleaning}: FAILURE");
						}
					}
		
					try {
						errorBufWriter.close();
						//outBufWriter.close();
					} catch (IOException e){
						System.out.println("\n{Closing}: " + e.getMessage());
					}
					System.out.println("\n\n");	
					continue;
				}
				boolean killed = false;
				String defLost = "", defLostTokens[] = null, errorSum = "", errorSumTokens[] = null;
				String errorLine = errorReader.readLine();
				if(errorLine != null){
					//System.out.println("{Valgrind}: FAILURE");
					//errorBufWriter.write("{Valgrind}: FAILURE\n");
					if(errorLine.contains("definitely lost:")){
						defLost = errorLine.replaceAll("\\D+", " ");
						defLostTokens = defLost.split("[ ]+");
					}
					if(errorLine.contains("ERROR SUMMARY:")){
						errorSum = errorLine.replaceAll("\\D+", " ");
						errorSumTokens = errorSum.split("[ ]+");
					}
					System.out.println("==> " + errorLine);
					errorBufWriter.write("==> " + errorLine + "\n");
					while((errorLine = errorReader.readLine()) != null){
						if(errorLine.contains("Killed by fatal signal")){
							killed = true;
							break;
						}
						if(errorLine.contains("definitely lost:")){
							defLost = errorLine.replaceAll("\\D+", " ");
							defLostTokens = defLost.split("[ ]+");
						}
						if(errorLine.contains("ERROR SUMMARY:")){
							errorSum = errorLine.replaceAll("\\D+", " ");
							errorSumTokens = errorSum.split("[ ]+");
						}
						System.out.println("==> " + errorLine);
						errorBufWriter.write("==> " + errorLine + "\n");
					}
				}
				
				//this block outputs 
				/*
				String outLine;
				while((outLine = outputReader.readLine()) != null){
					System.out.println("==> " + outLine);
					errorBufWriter.write("==> " + outLine);
				}*/
				
				if(killed) {
					csvWriter.append("killed" + "," + "killed" + "," + "killed" + "," + "killed\n");
				} else {
					if(defLostTokens != null && errorSumTokens != null){
						csvWriter.append(defLostTokens[2] + "," + defLostTokens[3] + "," + errorSumTokens[2] + "," + errorSumTokens[3] + "\n");
					} else {
						csvWriter.append("-,-\n");
					}
				}
				errorReader.close();
				outputReader.close();
				valProc.destroy();
				if(valProc.isAlive())
					valProc.destroyForcibly();
			} catch (IOException | InterruptedException e){
				System.out.println("{Valgrind}: FAILURE\n" + e.getMessage());
				try {
						errorBufWriter.write("{Valgrind}: FAILURE\n" + e.getMessage() + "\n");
				} catch (IOException ex){
					System.out.println("{Valgrind}: FAILURE\n" + ex.getMessage());
				}
			}
			
			File mainFile = new File(path + "/main");
			try {
				if(mainFile.exists())
					mainFile.delete();
				else {
					System.out.println("\n{Cleaning}: FAILURE (no executable file found)");
					errorBufWriter.write("\n{Cleaning}: FAILURE (no executable file found)\n");
				}
			} catch (IOException e){
				System.out.println("\n{Cleaning}: FAILURE (deletion failed)");
				try {
					errorBufWriter.write("\n{Cleaning}: FAILURE (deletion failed)\n");
				} catch (IOException ex){
					System.out.println("\n{Cleaning}: FAILURE");
				}
			}
			
			try {
				errorBufWriter.close();
				//outBufWriter.close();
			} catch (IOException e){
				System.out.println("\n{Closing}: " + e.getMessage());
			}
			System.out.println("\n\n");			
		}
		
		try {
			csvWriter.close();
		} catch (IOException e){
			System.out.println("{Closing}: " + e.getMessage());
		}			
	}
	
	static ArrayList<File> findFilesByName(File[] files, String name){
		ArrayList<File> toReturn = new ArrayList<>();
		for(File file: files){
			if(file.isFile() && file.getName().contains(name)){
				toReturn.add(file);
			} else if(file.isDirectory()) {
				toReturn.addAll(findFilesByName(file.listFiles(), name));
			}
		}
		return toReturn;
	} 
}
