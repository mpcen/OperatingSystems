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
	private boolean queued;
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
				if(scenario.onTrip == false && scenario.atDoor.size() < 3 && queued == false )
				{
					if(scenario.atDoor.size() == 2)
					{
						scenario.atDoor.add(this);
						queued = true;
						scenario.onTrip = true;
						//lets go to santas door guys!
						for(Elf elf: scenario.atDoor)
						{
							elf.gotoSanta();
							elf.setState(ElfState.AT_SANTAS_DOOR);
						}
					}
					else
					{
						scenario.atDoor.add(this);
						queued = true;
					}
				}
				else
				{}
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
	
	//clear status of waiting to go to santa
	public void gotoSanta()
	{
		queued = false;
	}
	
}
