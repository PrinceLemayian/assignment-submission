# Adaptive Nairobi Traffic Control Using Lua Coroutines

**Institution:** Dedan Kimathi University of Technology<br>
**School:** School of Computer Science and IT<br>
**Department:** Department of Computer Science<br>
**Unit:** Programming Languages<br>
**Assignment Question:** Question 6 - Adaptive Nairobi Traffic Control<br>
**Programming Language:** Lua<br>
**Group:** Group 26<br>

## Members

| Name                    | Registration Number |
| ----------------------- | ------------------- |
| Daniel Kinyanjui Njenga | C026-01-0911/2025   |
| Prince Lemayian Riamet  | C026-01-0972/2025   |
| Carren Cherotich Ngeno  | C026-01-2560/2025   |

---

## Table of Contents

1. [Introduction](#1-introduction)
2. [Assignment Scenario](#2-assignment-scenario)
3. [Development Environment](#3-development-environment)
4. [Project Structure](#4-project-structure)
5. [Understanding Coroutines](#5-understanding-coroutines)
6. [Program Implementation](#6-program-implementation)
7. [Scheduling Design](#7-scheduling-design)
8. [How to Run the Program](#8-how-to-run-the-program)
9. [Expected Output](#9-expected-output)
10. [Fairness and Throughput Trade-off](#10-fairness-and-throughput-trade-off)
11. [Challenges, Lessons Learnt and Conclusion](#11-challenges-lessons-learnt-and-conclusion)

---

## 1. Introduction

This project implements an adaptive traffic-light scheduler for four major Nairobi roads:

- Thika Road
- Mombasa Road
- Ngong Road
- Waiyaki Way

The main objective is to decide which road should receive a green light at each simulation cycle. The decision is based on two important factors:

- **Congestion**, represented by the number of vehicles waiting in a road queue.
- **Fairness**, represented by how many cycles a road has waited without receiving a green light.

The program uses Lua coroutines to model each road independently. Each road coroutine keeps track of its own queue, waiting time and traffic state, then yields that information to the main scheduler.

---

## 2. Assignment Scenario

Traffic sensors report vehicle queues on Thika Road, Mombasa Road, Ngong Road and Waiyaki Way. A road with a long queue should receive a green light more frequently, but no road should be ignored indefinitely.

The assignment required the program to:

| Requirement | Description                                                                     |
| ----------- | ------------------------------------------------------------------------------- |
| a           | Maintain queue information separately for every road.                           |
| b           | Allow each road coroutine to yield its current traffic state.                   |
| c           | Select the next road using both congestion and fairness as scheduling criteria. |
| d           | Prevent starvation of roads with lower traffic volumes.                         |
| e           | Discuss the trade-off between fairness and traffic throughput.                  |

---

## 3. Development Environment

The project was developed using the following tools:

| Tool                    | Purpose                                  |
| ----------------------- | ---------------------------------------- |
| Visual Studio Code      | Writing and editing the Lua source files |
| Lua                     | Running the traffic-control simulation   |
| Git                     | Version control                          |
| Command Line / Terminal | Executing the program                    |

Lua was selected because it provides built-in coroutine support through functions such as `coroutine.create`, `coroutine.resume`, `coroutine.yield` and `coroutine.status`.

---

## 4. Project Structure

The repository contains the following files:

| File                    | Description                                                                                                                                                    |
| ----------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `traffic_control.lua`   | Main simulation program. It creates road coroutines, collects traffic states, selects the road to receive green light and prints the simulation output.        |
| `traffic_scheduler.lua` | Separate scheduling module containing reusable scoring and green-light selection logic. It has no printing or random behavior, making it suitable for testing. |
| `README.md`             | Project documentation explaining the assignment, coroutine design and scheduling approach.                                                                     |

---

## 5. Understanding Coroutines

A coroutine is a function that can pause its execution and later continue from where it stopped. Unlike a normal function, which starts and finishes in one continuous flow, a coroutine can cooperate with other parts of the program by yielding control.

In Lua, coroutines are useful for cooperative multitasking. This means different tasks take turns running, but they do not run at the exact same time. Instead, each coroutine voluntarily pauses using `coroutine.yield`, allowing the scheduler to decide what happens next.

In this project, each road is represented as a coroutine. This is a good fit because every road has its own changing state:

- Current vehicle queue.
- Number of vehicles that can pass when the road gets green light.
- New vehicles arriving each cycle.
- Number of cycles spent waiting.

Each road coroutine updates its local traffic information and yields a table containing its current state:

```lua
{
    name = def.name,
    queue = queue,
    waiting = waiting,
}
```

The scheduler resumes each coroutine once per cycle, receives the yielded state, compares all roads and chooses the next road to receive green light.

---

## 6. Program Implementation

### 6.1 Road Queue Information

The roads are stored separately in the `ROADS` table inside `traffic_control.lua`.

Each road has:

- `name` - the road name.
- `queue` - the starting number of vehicles.
- `flow` - the number of vehicles cleared when the road gets green light.
- `arrival` - the base rate used to simulate new vehicle arrivals.

Example:

```lua
{ name = "Thika Road", queue = 12, flow = 5, arrival = 3 }
```

This satisfies the requirement to maintain queue information separately for every road.

### 6.2 Road Coroutines

The function `make_road(def)` creates one coroutine for each road. Inside the coroutine, the road maintains its own `queue` and `waiting` values.

When the road receives green light, vehicles are removed from its queue:

```lua
queue = math.max(0, queue - def.flow)
waiting = 0
```

When the road does not receive green light, its waiting time increases:

```lua
waiting = waiting + 1
```

After updating its values, the road yields its current state back to the scheduler. This satisfies the requirement that each road coroutine should yield its current traffic state.

### 6.3 Main Scheduling Loop

The main loop runs for a fixed number of simulation cycles:

```lua
local SIM_CYCLES = 20
```

During each cycle, the program:

1. Resumes every road coroutine.
2. Collects the yielded traffic states.
3. Computes which road should receive green light.
4. Marks that road as green for the next cycle.
5. Prints the current traffic state.
6. Updates the final green-light allocation count.

This creates a cooperative simulation where the roads and scheduler work together step by step.

---

## 7. Scheduling Design

The scheduler uses both congestion and fairness.

### 7.1 Congestion Score

Congestion is measured using the current queue length. A larger queue means more vehicles are waiting, so the road becomes more urgent.

```lua
local CONGESTION_W = 1.0
```

### 7.2 Fairness Score

Fairness is measured using the number of cycles a road has waited without green light. The longer a road waits, the more priority it gains.

```lua
local FAIRNESS_W = 2.5
```

### 7.3 Weighted Scheduling Formula

The scheduler computes a score for each road:

```lua
score = queue * CONGESTION_W + waiting * FAIRNESS_W
```

This means a road can be selected because it has a very long queue, because it has waited for several cycles, or because both conditions are true.

### 7.4 Starvation Prevention

Starvation occurs when a road is repeatedly ignored because other roads always have heavier traffic.

To prevent this, the program uses a starvation limit:

```lua
local STARVATION_LIMIT = 3
```

If a road has waited for at least three cycles, the scheduler forces it to receive green light. In the output, this is shown using:

```text
[starvation guard]
```

This ensures that even roads with lower traffic volumes, such as Ngong Road, are not ignored indefinitely.

---

## 8. How to Run the Program

Open a terminal in the project folder and run:

```bash
lua traffic_control.lua
```

If Lua is not recognized, install Lua first or ensure it is added to the system path.

---

## 9. Expected Output

The program prints a cycle-by-cycle simulation. Each cycle shows:

- Road name.
- Current queue length.
- Waiting time.
- A visual queue bar.
- The road selected for green light.
- Whether starvation prevention was used.

Example output format:

```text
Cycle 01
Thika Road      q=15   w=1
Mombasa Road    q=10   w=1
Ngong Road      q=3    w=1
Waiyaki Way     q=11   w=1  < GREEN
```

At the end, the program prints a final allocation summary showing how many green lights each road received across the simulation.

---

## 10. Fairness and Throughput Trade-off

The scheduling design must balance two goals:

- **Throughput:** clearing as many vehicles as possible by giving green light to the most congested roads.
- **Fairness:** ensuring every road eventually receives green light, even if its queue is smaller.

If the program focuses only on throughput, roads like Thika Road or Waiyaki Way may receive green light most of the time because they often have larger queues. This can reduce total congestion quickly, but smaller roads may wait too long.

If the program focuses only on fairness, every road may receive green light equally often. This prevents starvation, but it can reduce efficiency because a lightly congested road may receive green light while a heavily congested road continues to build up traffic.

This project uses a balanced approach. The weighted score favors roads with longer queues, while the waiting-time component and starvation guard protect roads that have waited too long. As a result, the system remains adaptive while still respecting fairness.

---

## 11. Challenges, Lessons Learnt and Conclusion

### 11.1 Challenges Encountered

Some of the challenges in the project included:

- Understanding how Lua coroutines pause and resume execution.
- Deciding what state each road coroutine should yield.
- Balancing congestion priority with fairness.
- Preventing low-volume roads from being ignored.
- Designing output that clearly shows each scheduling decision.

### 11.2 Lessons Learnt

This project helped demonstrate:

- How coroutines support cooperative multitasking.
- How independent tasks can maintain their own state.
- How scheduling algorithms can use weighted decision-making.
- Why fairness is important in resource allocation.
- How starvation prevention improves long-running systems.

### 11.3 Conclusion

The adaptive Nairobi traffic-control simulation successfully models each road as an independent Lua coroutine. The scheduler collects traffic states, calculates urgency using congestion and fairness, and applies a starvation guard to make sure no road is ignored indefinitely.

The project shows how coroutine-based programming can be used to solve scheduling problems in a clear and modular way. It also demonstrates the practical trade-off between maximizing traffic throughput and maintaining fairness across all roads.
