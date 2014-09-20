import java.util.ArrayList;
import java.util.Queue;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.List;
import java.util.concurrent.Semaphore;

public class SantaScenario {

	public Santa santa;
	public Object lock = new Object();
	public Object lockReindeer = new Object();
	public List<Elf> elves;
	public List<Elf> atDoor;
	public Queue<Elf> wait;
	public List<Reindeer> reindeers;
	public Semaphore trouble;
	public Semaphore reindeer;
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
		
		// The reindeer: in this case: 9
		scenario.reindeers = new ArrayList<>();
		for(int i=0; i != 9; i++) {
			Reindeer reindeer = new Reindeer(i+1, scenario);
			scenario.reindeers.add(reindeer);
			th = new Thread(reindeer);
			th.start();
		}
		//semaphore for elves
		scenario.trouble = new Semaphore(3, true);
		//semaphore for reindeer
		scenario.reindeer = new Semaphore(8, true);
		//list of elves waiting to leave & at the door
		scenario.wait = new ArrayBlockingQueue<Elf>(3);
		scenario.atDoor = new ArrayList<>();
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
				
				for(Reindeer reindeer: scenario.reindeers)
				{
					reindeer.timeToDie();
				}
			}
			
			//check for elves
			if(scenario.wait.size() == 3)
				scenario.notifyElves();
			
			
			// after day 370 the last state will get printed out until day 500
			// print out the state:
			System.out.println("***********  Day " + day + " *************************");
			scenario.santa.report();
			for(Elf elf: scenario.elves) {
				elf.report();
			}
			
			for(Reindeer reindeer: scenario.reindeers) {
				reindeer.report();
			}
				
		}
	}
	
	public void notifyElves()
	{
		//check for three elves and send them on their way
		synchronized(lock)
		{
			this.atDoor.addAll(this.wait); //move the list of elves waiting into a door list
			this.wait.clear(); //clear the waiting list
			this.lock.notifyAll(); //signal the elves to actually move
		}
	}
	
	public void notifyReindeer()
	{
		synchronized(lockReindeer)
		{
			this.lockReindeer.notifyAll();
		}
	}
	
	
}

