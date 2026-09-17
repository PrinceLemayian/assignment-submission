# Programming Languages Assignment Submission

**Institution:** Dedan Kimathi University of Technology  
**School:** School of Computer Science and IT  
**Department:** Department of Computer Science  
**Unit:** Programming Languages  
**Unit Code:** CCS 2105  
**Group:** Group 26

## Group Members

| Name                    | Registration Number |
| ----------------------- | ------------------- |
| Daniel Kinyanjui Njenga | C026-01-0911/2025   |
| Prince Lemayian Riamet  | C026-01-0972/2025   |
| Carren Cherotich Ngeno  | C026-01-2560/2025   |

## Repository Overview

This repository contains two Programming Languages assignments:

| Folder                  | Assignment                                                 | Language |
| ----------------------- | ---------------------------------------------------------- | -------- |
| `names-bindings-scopes` | University Library Borrowing, Fines and Book-State Tracker | C        |
| `lua-coroutines`        | Adaptive Nairobi Traffic Control Using Lua Coroutines      | Lua      |

The individual assignment folders contain detailed README files. This root README summarizes the complete submission after reviewing each README and each source file.

## Assignment 1: Names, Bindings and Scopes

**Folder:** `names-bindings-scopes`  
**Lab Topic:** Names, Bindings and Scopes  
**Case Study:** University Library Borrowing, Fines and Book-State Tracker  
**Main Source File:** `main.c`

This assignment implements a C-based university library tracker. It manages books, borrowers, borrowing operations, returns, overdue fines, transaction logging and book-stock states. It also demonstrates the required Programming Languages concepts using the running program.

### Program Data Model

The program defines these main record types:

| Structure     | Purpose                                                                       |
| ------------- | ----------------------------------------------------------------------------- |
| `Book`        | Stores accession number, title, available copies and book status.             |
| `Borrower`    | Stores student ID, borrower name and the currently borrowed accession number. |
| `Transaction` | Stores borrow/return history, dates, success status, fines and messages.      |

Book status is represented using the `BookStatus` enum:

```c
typedef enum { AVAILABLE, LOW_STOCK, OUT_OF_STOCK } BookStatus;
```

### Named Constants

The source file uses named constants instead of repeated magic numbers:

| Constant           | Value | Purpose                                          |
| ------------------ | ----: | ------------------------------------------------ |
| `MAX_TITLE_LENGTH` |    80 | Maximum book title length.                       |
| `MAX_NAME_LENGTH`  |    60 | Maximum borrower name length.                    |
| `MAX_BOOKS`        |    12 | Book array capacity.                             |
| `MAX_BORROWERS`    |    10 | Borrower array capacity.                         |
| `MAX_TRANSACTIONS` |    20 | Transaction log capacity.                        |
| `MAX_LOAN_DAYS`    |    14 | Maximum borrowing period before overdue charges. |
| `CHARGE_PER_DAY`   |    20 | Fine charged per overdue day.                    |

### Main Workflow

The program initializes:

- 8 books.
- 5 borrowers.
- A transaction log.
- Borrowing and return attempt counters.

The run in `main()` performs eight borrow/return attempts:

| Operation                                                   | Result                                 |
| ----------------------------------------------------------- | -------------------------------------- |
| Borrower `501` borrows book `1001` on day `1`               | Successful borrow                      |
| Borrower `502` borrows book `1003` on day `2`               | Successful borrow                      |
| Borrower `503` tries to borrow book `1003` on day `3`       | Failed because no copy is available    |
| Borrower `501` returns on day `10`                          | Successful return, fine `0`            |
| Borrower `502` returns on day `25`                          | Successful return, fine `180`          |
| Borrower `503` borrows book `1003` on day `26`              | Successful borrow                      |
| Borrower `504` tries to borrow accession `9999` on day `27` | Failed because the book is not found   |
| Borrower `505` tries to return on day `28`                  | Failed because there is no active loan |

The final counters printed by the program are expected to be:

| Counter                       | Value |
| ----------------------------- | ----: |
| Successful borrowing attempts |     3 |
| Failed borrowing attempts     |     2 |
| Successful return attempts    |     2 |
| Failed return attempts        |     1 |

### Concepts Demonstrated

| Concept                          | Evidence in the Program                                                                       |
| -------------------------------- | --------------------------------------------------------------------------------------------- |
| Aliasing                         | `selectedBook` and `secondReferenceName` point to the same `Book`.                            |
| Pointer/reference behavior       | `Book *book` and `Borrower *borrower` update real array records.                              |
| Explicit heap-dynamic storage    | `createDynamicBook()` allocates a `Book` using `malloc(sizeof(Book))`.                        |
| Releasing heap storage           | `demonstrateLifetime()` releases the dynamic book using `free(dynamicBook)`.                  |
| Automatic lifetime               | `searchIndex` exists only during a call to `searchBookIndex()`.                               |
| Static lifetime with local scope | `static int totalLoans` exists across calls but is visible only inside `borrowBook()`.        |
| Scope vs lifetime                | The README compares `searchIndex`, `totalLoans`, `books` and the heap `Book`.                 |
| L-values and r-values            | Examples include `book->availableCopies--` and `borrower->borrowedAccessionNo = accessionNo`. |

### Important Functions

| Function                | Responsibility                                                    |
| ----------------------- | ----------------------------------------------------------------- |
| `statusName()`          | Converts a `BookStatus` value to display text.                    |
| `updateStatus()`        | Updates book status from available copy count.                    |
| `addTransaction()`      | Adds a borrow/return entry to the transaction log.                |
| `searchBookIndex()`     | Searches for a book accession number and returns its array index. |
| `searchBook()`          | Returns a pointer to the matching `Book`.                         |
| `searchBorrower()`      | Returns a pointer to the matching `Borrower`.                     |
| `calculateFine()`       | Calculates overdue fine from borrow and return days.              |
| `borrowBook()`          | Validates and records borrowing.                                  |
| `returnBook()`          | Validates and records returns.                                    |
| `createDynamicBook()`   | Allocates and initializes a heap `Book`.                          |
| `demonstrateAliasing()` | Shows two pointer names sharing one object.                       |
| `demonstrateLifetime()` | Shows heap object lifetime and correct release.                   |
| `printTransactionLog()` | Prints the transaction history.                                   |
| `printStockTable()`     | Prints the current book stock table.                              |
| `printOverdueCharges()` | Prints fines for successful returns.                              |

### Output Produced

The program prints:

- A labelled borrow/return workflow.
- Search messages showing whether accession numbers were found.
- The aliasing experiment.
- The lifetime experiment.
- A transaction log.
- A current stock table.
- Overdue charges.
- Successful and failed borrowing attempts.
- Successful and failed return attempts.

### Files in This Assignment

| File         | Description                                                                                                |
| ------------ | ---------------------------------------------------------------------------------------------------------- |
| `main.c`     | Complete C implementation for the library tracker and concept demonstrations.                              |
| `README.md`  | Detailed assignment explanation, code excerpts, output analysis, concept note, viva answers and checklist. |
| `.gitignore` | Ignore rules for the assignment folder.                                                                    |

### How to Run

Open a terminal in the `names-bindings-scopes` folder, compile the program and run it:

```bash
gcc main.c -o library_tracker
./library_tracker
```

On Windows PowerShell, run the executable as:

```powershell
.\library_tracker.exe
```

## Assignment 2: Lua Coroutines

**Folder:** `lua-coroutines`  
**Assignment Question:** Question 6 - Adaptive Nairobi Traffic Control  
**Main Source File:** `traffic_control.lua`

This assignment implements an adaptive traffic-light simulation for four Nairobi roads using Lua coroutines. Each road is modeled as an independent coroutine that maintains its own queue and waiting time, then yields its current state to the scheduler.

### Roads Simulated

| Road         | Initial Queue | Flow When Green | Arrival Base |
| ------------ | ------------: | --------------: | -----------: |
| Thika Road   |            12 |               5 |            3 |
| Mombasa Road |             7 |               4 |            4 |
| Ngong Road   |             2 |               2 |            1 |
| Waiyaki Way  |             9 |               4 |            2 |

The simulation uses `math.randomseed(42)`, so the random vehicle arrivals are reproducible.

### Coroutine Design

The function `make_road(def)` creates one coroutine for each road. Each coroutine stores its own:

- `queue`
- `waiting`
- road definition values such as `flow` and `arrival`

On each cycle:

- If the road received green light, vehicles are removed from its queue and waiting time resets to `0`.
- If it did not receive green light, waiting time increases.
- New vehicles are added using `math.random(0, def.arrival * 2)`.
- The coroutine yields a table containing `name`, `queue` and `waiting`.

### Scheduling Design

The scheduler runs for:

```lua
local SIM_CYCLES = 20
```

It uses these scheduling constants:

| Constant           | Value | Meaning                                |
| ------------------ | ----: | -------------------------------------- |
| `CONGESTION_W`     |   1.0 | Weight applied to queue length.        |
| `FAIRNESS_W`       |   2.5 | Weight applied to waiting time.        |
| `STARVATION_LIMIT` |     3 | Waiting threshold that forces service. |

The normal priority score is:

```text
score = queue * CONGESTION_W + waiting * FAIRNESS_W
```

Before using the score, the scheduler checks for starvation. If any road has waited at least three cycles, the road with the highest waiting time is selected and the output marks the decision with `[starvation guard]`.

### Output Produced

The Lua program prints:

- A simulation title.
- A cycle-by-cycle road table.
- Queue length for each road.
- Waiting time for each road.
- A visual queue bar.
- The road selected for green light.
- Whether starvation prevention was used.
- A final green-light allocation summary with counts and percentages.

### Files in This Assignment

| File                  | Description                                                                                                 |
| --------------------- | ----------------------------------------------------------------------------------------------------------- |
| `traffic_control.lua` | Complete Lua coroutine simulation and scheduling program.                                                   |
| `README.md`           | Detailed explanation of the coroutine design, assignment scenario, scheduling approach and expected output. |

Note: the assignment README mentions `traffic_scheduler.lua` as a separate scheduling module, but the actual folder currently contains only `traffic_control.lua` and `README.md`; the scheduling logic is implemented directly inside `traffic_control.lua`.

### How to Run

Open a terminal in the `lua-coroutines` folder and run:

```bash
lua traffic_control.lua
```

## Submission Notes

The `names-bindings-scopes` assignment focuses on C concepts such as structures, pointers, aliasing, scope, lifetime, static local variables, heap allocation and transaction-style program output.

The `lua-coroutines` assignment focuses on coroutine-based cooperative multitasking and scheduling, using congestion and fairness to decide which Nairobi road receives green light in each simulation cycle.
