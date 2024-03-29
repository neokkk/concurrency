import java.util.concurrent.locks.Condition;

public class DiningPhilosophers extends Thread {
    private static final int NUM_PHILOSOPHERS = 5;
    private enum State { THINKING, EATING, HUNGRY };
    private State[] state = new State[NUM_PHILOSOPHERS];
    private Condition[] self = new Condition[NUM_PHILOSOPHERS];

    void pickup(int i) throws InterruptedException {
        state[i] = State.HUNGRY;
        test(i);
        if (state[i] != State.EATING) {
            self[i].wait();
        }
    }

    void putdown(int i) {
        state[i] = State.THINKING;
        test((i + 4) % NUM_PHILOSOPHERS);
        test((i + 1) % NUM_PHILOSOPHERS);
    }

    void test(int i) {
        if (
            (state[(i + 4) % NUM_PHILOSOPHERS] != State.EATING) &&
            (state[i] == State.HUNGRY) &&
            (state[(i + 1) % NUM_PHILOSOPHERS] != State.EATING)
        ) {
            state[i] = State.EATING;
            self[i].signal();
        }
    }

    void initialize() {
        for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
            state[i] = State.THINKING;
        }
    }
}