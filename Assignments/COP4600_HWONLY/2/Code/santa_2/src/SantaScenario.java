import java.util.ArrayList;
import java.util.List;



public class SantaScenario {

	public Santa santa;
	public List<Elf> elves;
	public boolean isDecember;
	
	public static void main(String args[]) {
		SantaScenario scenario = new SantaScenario();
		scenario.isDecember = false;
		// create the participants
		// Santa
		scenario.santa = new Santa(scenario);
		Thread th = new Thread(scenario.santa);
		th.start();
		// The elves: in this case: 10
		scenario.elves = new ArrayList<>();
		for(int i = 0; i != 10; i++) {
			Elf elf = new Elf(i+1, scenario);
			scenario.elves.add(elf);
			th = new Thread(elf);
			th.start();
		}
		// now, start the passing of time
		for(int day = 1; day <= 500; day++) {
			// wait a day
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			// turn on December
			if (day > (365 - 31)) {
				scenario.isDecember = true;
			}
			
			//end operations at day 370 | main thread will die at 500
			if(day == 370)
			{
				scenario.santa.timeToDie();
				for(Elf elf: scenario.elves)
				{
					elf.timeToDie();
				}
				
			}
			
			// after day 370 the last state will get printed out until day 500
			// print out the state:
			System.out.println("***********  Day " + day + " *************************");
			scenario.santa.report();
			for(Elf elf: scenario.elves) {
				elf.report();
			}
				
		}
	}
	
	
	
}

