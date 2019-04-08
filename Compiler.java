import java.io.*;
import java.util.*;
public class Compiler {

	private ProcessBuilder processBuilder;
	private Process process;
	ArrayList<String> listCompiled;
	ArrayList<String> listNotCompiled;
	File[] dirs;
	
	public Compiler(){
		init();	
	}
	
	private void init(){
		System.out.println("{Compiler}: initialization");
		String currentDirPath = System.getProperty("user.dir");
		File submDir = new File(currentDirPath + "/subms");
		
		dirs = submDir.listFiles();
		listCompiled = new ArrayList<>();
		listNotCompiled = new ArrayList<>();
	}
	
	public void compile(){
		System.out.println("{Compiler}: compiling ...");
		for(File file: dirs){			
			System.out.println(" ==> Folder name: { " + file.getName() + " }");
			
			String path = file.getAbsolutePath();
			
			processBuilder = new ProcessBuilder("make", "-C", path);
			
			try {
				process = processBuilder.start();
				BufferedReader errorReader = new BufferedReader(new InputStreamReader(process.getErrorStream()));
				process.waitFor();
				String errorLine = errorReader.readLine();
				process.destroy();
				if(process.isAlive())
					process.destroyForcibly();
				System.out.println(errorLine);
				if(errorLine == null){
					listCompiled.add(file.getName());
				} else {
					listNotCompiled.add(file.getName());
				}
			} catch (IOException | InterruptedException e){
				System.out.println("{Compiler}: compiling error");
			}
		}
	}
	
	public void createCompiledTxt(String filename){
		System.out.println("{Compiler}: writing compiled txt file");
		try{
			BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
			for(String s: listCompiled){
				writer.write(s + "\n");
			}
			writer.close();
		} catch (IOException e){
			System.out.println("{Compiler}: createCompiledTxt error");
		}
	}
	
	public void createNotCompiledTxt(String filename){
		try{
			System.out.println("{Compiler}: writing not compiled txt file");
			BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
			for(String s: listNotCompiled){
				writer.write(s + "\n");
			}
			writer.close();
		} catch (IOException e){
			System.out.println("{Compiler}: createNotCompiledTxt error");
		}
	}
}
