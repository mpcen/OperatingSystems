import java.util.Random;

public class Elf implements Runnable {

	enum ElfState {
		WORKING, TROUBLE, AT_SANTAS_DOOR
	};
	private boolean engine = true;
	private ElfState state;
	/**
	 * The number associated with the Elf
	 */
	private int number;
	private Random rand = new Random();
	private SantaScenario scenario;
	
	public Elf(int number, SantaScenario scenario) {
		this.number = number;
		this.scenario = scenario;
		this.state = ElfState.WORKING;
	}
	
	
	public ElfState getState() {
		return state;
	}

	/**
	 * Santa might call this function to fix the trouble
	 * @param state
	 */
	public void setState(ElfState state) {
		this.state = state;
	}


	@Override
	public void run() {
		// wait a day
		while (engine) {
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			switch (state) {
			case WORKING: {
				// at each day, there is a 1% chance that an elf runs into
				// trouble.
				if (rand.nextDouble() < 0.01) {
					state = ElfState.TROUBLE;
				}
				break;
			}
			case TROUBLE:
				// FIXME: if possible, move to Santa's door
				synchronized(scenario.lock)
				{
					try
					{
						//get onboard -- first three elves
						scenario.trouble.acquire();
						scenario.wait.add(this);
						
						//wait for the others 2 elves and then head to santas door
						scenario.lock.wait();
						setState(ElfState.AT_SANTAS_DOOR);
					}
					catch(InterruptedException e)
					{
						e.printStackTrace();
					}
				}
				break;
			case AT_SANTAS_DOOR:
				// FIXME: if feasible, wake up Santa
				scenario.santa.wakeUpElves();
				break;
			}
		}
	}
	
	/**
	 * Report about my state
	 */
	public void report() {
		System.out.println("Elf " + number + " : " + state);
	}
	
	//after day 370 finish up
	public void timeToDie()
	{
		engine = false;
	}
	
	//return back and give up spot for another elf that may be in trouble
	public void goBack()
	{
		scenario.trouble.release();
	}
	
}
