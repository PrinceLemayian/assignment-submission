# University Library Borrowing, Fines and Book-State Tracker

**Institution:** Dedan Kimathi University of Technology<br>
**School:** School of Computer Science and IT<br>
**Department:** Department of Computer Science<br>
**Unit Code:** CCS 2105<br>
**Unit Name:** Programming Languages<br>
**Lab Topic:** Names, Bindings and Scopes<br>
**Case Study:** University Library Borrowing, Fines and Book-State Tracker<br>
**Group:** Group 26<br>

## Members

| Name                    | Registration Numbers |
| ----------------------- | -------------------- |
| Daniel Kinyanjui Njenga | C026-01-0911/2025    |
| Prince Lemayian Riamet  | C026-01-0972/2025    |
| Carren Cherotich Ngeno  | C026-01-2560/2025    |

---

## B. Aliasing Experiment

### Objective

The purpose of this experiment is to demonstrate **aliasing**, where two different pointer names refer to the same `Book` object in memory.

The experiment requires a pointer/reference to a selected `Book` and a second reference name for the same object. A modification made through one name should then be observable through the other.

### Relevant Code

```c
void demonstrateAliasing(Book books[], int bookCount) {
    Book *selectedBook = searchBook(books, bookCount, 1003);
    Book *secondReferenceName = selectedBook;

    printf("\nB. ALIASING EXPERIMENT\n");

    if (selectedBook == NULL) {
        printf("Aliasing experiment skipped: selected book was not found.\n");
        return;
    }

    printf("selectedBook points to %p and secondReferenceName points to %p.\n",
           (void *)selectedBook, (void *)secondReferenceName);

    printf(
        "Before change: selectedBook copies=%d, secondReferenceName copies=%d.\n",
        selectedBook->availableCopies,
        secondReferenceName->availableCopies);

    selectedBook->availableCopies += 2;
    updateStatus(secondReferenceName);

    printf("After change through selectedBook: selectedBook copies=%d, "
           "secondReferenceName copies=%d.\n",
           selectedBook->availableCopies,
           secondReferenceName->availableCopies);

    printf("Explanation: both names store the same address, so they share one "
           "Book object in memory.\n");
}
```

### How the Experiment Works

First, the program searches for Book `1003`:

```c
Book *selectedBook = searchBook(books, bookCount, 1003);
```

The function returns a pointer to the actual `Book` object in the `books` array.

Next:

```c
Book *secondReferenceName = selectedBook;
```

copies the address stored in `selectedBook` into `secondReferenceName`.

Therefore, both pointers refer to the same `Book`.

Conceptually:

```text
selectedBook
     |
     | same address
     v
+-----------------------------+
| Book 1003                   |
| Operating Systems Concepts  |
| availableCopies = 1        |
+-----------------------------+
     ^
     | same address
     |
secondReferenceName
```

The program then modifies the object through `selectedBook`:

```c
selectedBook->availableCopies += 2;
```

If the book initially has one available copy:

```text
Before:
availableCopies = 1
```

the value becomes:

```text
After:
availableCopies = 3
```

Because `secondReferenceName` refers to the same object, it also observes:

```text
secondReferenceName->availableCopies = 3
```

### Expected/Observed Result

The program prints the two pointer addresses. They should be identical:

```text
selectedBook points to 0x... and secondReferenceName points to 0x...
```

It then prints the values before and after the modification.

Example:

```text
Before change: selectedBook copies=1, secondReferenceName copies=1.
After change through selectedBook: selectedBook copies=3, secondReferenceName copies=3.
```

The exact memory address will vary between executions. The important observation is that both pointers contain the same address and both observe the same change.

### Explanation

This demonstrates **aliasing**.

`selectedBook` and `secondReferenceName` are two different pointer variables, but they refer to the same `Book` object.

Therefore:

```c
selectedBook == secondReferenceName
```

is true.

Changing the object through one pointer changes the same object seen through the other pointer.

### Conclusion for B

The experiment proves that **different names can refer to the same memory location/object**. This relationship is called aliasing. In this program, aliasing allows the same `Book` object to be accessed and modified through more than one pointer.

---

# C. Lifetime Experiment

## Objective

The purpose of this experiment is to demonstrate **object lifetime** using explicit heap-dynamic storage.

The experiment requires a dynamically created `Book` that remains alive across multiple function calls and is later released correctly. It also requires comparison with the local `searchIndex` variable, whose lifetime ends when the search function returns.

## Relevant Code

The dynamically allocated book is created using the following function:

```c
Book *createDynamicBook(int accessionNo, const char *title, int copies) {
    Book *book = malloc(sizeof(Book));

    if (book == NULL) {
        printf("Could not allocate dynamic Book.\n");
        exit(EXIT_FAILURE);
    }

    book->accessionNo = accessionNo;
    snprintf(book->title, sizeof(book->title), "%s", title);
    book->availableCopies = copies;
    updateStatus(book);

    return book;
}
```

The lifetime experiment is performed in:

```c
void demonstrateLifetime(void) {
    Book *dynamicBook =
        createDynamicBook(9001, "Training Heap Allocation Manual", 1);

    printf("\nC. LIFETIME EXPERIMENT\n");

    inspectDynamicBook(dynamicBook, "First function call");

    dynamicBook->availableCopies--;
    updateStatus(dynamicBook);

    inspectDynamicBook(dynamicBook, "Second function call after update");

    printf("The heap object is still alive after createDynamicBook returned "
           "because it was allocated with malloc.\n");

    printf("By contrast, searchIndex inside searchBookIndex is a local automatic "
           "variable and disappears when that function returns.\n");

    free(dynamicBook);
    dynamicBook = NULL;

    printf("Dynamic book released with free at the correct point.\n");
}
```

## How the Experiment Works

### Step 1 - Allocate the Book

The program calls:

```c
createDynamicBook(9001, "Training Heap Allocation Manual", 1);
```

Inside the function:

```c
Book *book = malloc(sizeof(Book));
```

`malloc()` allocates memory for a `Book` in heap storage.

The pointer `book` contains the address of this dynamically allocated object.

### Step 2 - Return the Address

The function executes:

```c
return book;
```

The local pointer `book` belongs to `createDynamicBook()`.

When `createDynamicBook()` returns, that local pointer is no longer accessible from the calling context.

However, the dynamically allocated `Book` object itself is still alive.

The caller stores its address in:

```c
Book *dynamicBook;
```

### Step 3 - Use the Object After the Function Returns

The program then calls:

```c
inspectDynamicBook(dynamicBook, "First function call");
```

The object can still be accessed because it was allocated using `malloc()`.

The program then modifies it:

```c
dynamicBook->availableCopies--;
updateStatus(dynamicBook);
```

and accesses it again:

```c
inspectDynamicBook(dynamicBook, "Second function call after update");
```

This demonstrates that the heap object survived the return of `createDynamicBook()`.

### Step 4 - Release the Object

The object is eventually released using:

```c
free(dynamicBook);
```

The pointer is then set to:

```c
dynamicBook = NULL;
```

Setting it to `NULL` prevents the pointer from continuing to contain the old address.

## Expected/Observed Result

The program should produce output similar to:

```text
C. LIFETIME EXPERIMENT

First function call: heap Book at 0x... -> 9001, Training Heap Allocation Manual, copies=1, status=Low stock

Second function call after update: heap Book at 0x... -> 9001, Training Heap Allocation Manual, copies=0, status=Out of stock

The heap object is still alive after createDynamicBook returned because it was allocated with malloc.

By contrast, searchIndex inside searchBookIndex is a local automatic variable and disappears when that function returns.

Dynamic book released with free at the correct point.
```

The actual memory address will vary between executions.

---

# Comparison with `searchIndex`

The program contains:

```c
int searchIndex;
```

inside:

```c
int searchBookIndex(Book books[], int bookCount, int accessionNo)
```

`searchIndex` is an **automatic local variable**.

Its lifetime follows the execution of the function:

```text
searchBookIndex() starts
        ↓
searchIndex is created
        ↓
searchIndex is used
        ↓
searchBookIndex() returns
        ↓
searchIndex lifetime ends
```

The dynamically allocated `Book` behaves differently:

```text
malloc()
   ↓
Book object created
   ↓
createDynamicBook() returns
   ↓
Book still exists
   ↓
Book used again
   ↓
free()
   ↓
Book lifetime ends
```

## Scope vs Lifetime

This experiment demonstrates that **scope and lifetime are different concepts**.

### `searchIndex`

```text
Scope:
Inside searchBookIndex()

Lifetime:
During execution of searchBookIndex()
```

### `dynamicBook`

```text
Pointer scope:
Inside demonstrateLifetime()

Pointer lifetime:
During execution of demonstrateLifetime()

Book object lifetime:
From malloc() until free()
```

The important point is that the lifetime of the heap-allocated `Book` is not tied to the lifetime of the local pointer that initially pointed to it.

## Conclusion for C

The experiment demonstrates **explicit heap-dynamic storage and lifetime**.

The dynamically allocated `Book` begins its lifetime when `malloc()` successfully allocates its storage and remains alive until `free()` releases that storage.

In contrast, `searchIndex` is an automatic local variable whose lifetime ends when `searchBookIndex()` returns.

Therefore:

> **Scope tells us where a name can be accessed, while lifetime tells us how long the associated variable or object exists.**

## D. Scope vs Lifetime

### Objective

The purpose of this experiment is to demonstrate that **scope** and **lifetime** are related but different concepts.

The question specifically requires:

1. A `static` local variable named `totalLoans` inside the borrow function.
2. An explanation of why its scope is local to the function.
3. An explanation of why its lifetime extends across repeated function calls.
4. One more example where a variable or object is alive but not visible from the current function.

### Relevant Code

The static local variable appears inside `borrowBook()`:

```c
int borrowBook(Book books[], int bookCount, Borrower borrowers[],
               int borrowerCount, int studentId, int accessionNo,
               int borrowDay) {
  static int totalLoans = 0;
  Book *book = searchBook(books, bookCount, accessionNo);
  Borrower *borrower = searchBorrower(borrowers, borrowerCount, studentId);

  ...

  book->availableCopies--;
  borrower->borrowedAccessionNo = accessionNo;
  updateStatus(book);
  successfulBorrowAttempts++;
  totalLoans++;

  printf("Borrow function: static local totalLoans is now %d.\n", totalLoans);
  addTransaction("BORROW", studentId, accessionNo, borrowDay, 0, 1, 0,
                 "Successful borrow.");
  return 1;
}
```

Another useful example is the `books` array declared inside `main()`:

```c
int main(void) {
  Book books[MAX_BOOKS] = {
      {1001, "C Programming: A Modern Approach", 3, AVAILABLE},
      ...
      {1008, "Algorithms Unlocked", 2, AVAILABLE}};

  ...

  borrowBook(books, bookCount, borrowers, borrowerCount, 501, 1001, 1);
}
```

The array is alive while `main()` is running, but the name `books` is not visible inside `borrowBook()` unless it is passed as a parameter.

### How the Experiment Works

#### Step 1 - Declare `totalLoans` inside `borrowBook()`

Inside `borrowBook()` the program declares:

```c
static int totalLoans = 0;
```

This declaration gives `totalLoans` two important properties:

| Property | Meaning for `totalLoans` |
| -------- | ------------------------ |
| Scope    | Local to `borrowBook()`  |
| Lifetime | Whole program execution  |

#### Step 2 - Observe local scope

`totalLoans` is declared inside `borrowBook()`, so only statements inside `borrowBook()` can use the name `totalLoans`.

Code in `main()`, `returnBook()`, `printTransactionLog()`, or any other function cannot directly write:

```c
totalLoans
```

because the name is outside their scope. If a programmer tried to print `totalLoans` directly from `main()`, the compiler would reject it as an undeclared identifier in that scope.

#### Step 3 - Observe static lifetime

Although the name is local, the variable does not disappear when `borrowBook()` returns. Because it is declared with `static`, the storage for `totalLoans` is created once and remains alive until the whole program terminates.

Each successful borrow executes:

```c
totalLoans++;
printf("Borrow function: static local totalLoans is now %d.\n", totalLoans);
```

Therefore, each successful call remembers the value left by the previous successful call.

### Expected/Observed Result

During the transaction sequence, successful borrow calls produce output similar to:

```text
Borrow function: static local totalLoans is now 1.
Borrow function: static local totalLoans is now 2.
Borrow function: static local totalLoans is now 3.
```

This proves that `totalLoans` is not recreated as `0` on every call. If it were an ordinary automatic local variable, it would be created again each time `borrowBook()` was called and would not preserve the previous count.

### Additional Example: Alive but Not Visible

The `books` array in `main()` is an example of an object that is alive but not directly visible from another function.

```c
Book books[MAX_BOOKS] = { ... };
```

The array is alive while `main()` is executing. However, inside `updateStatus()` there is no name `books`. The function can only work with the `Book *book` parameter that was passed to it:

```c
void updateStatus(Book *book) {
  if (book == NULL) {
    return;
  }

  ...
}
```

So `books[0]` may still exist in memory, but the name `books` is not visible inside `updateStatus()`. Access is possible only through a pointer or parameter supplied by the caller.

Another example is the heap `Book` created by `malloc()` in `createDynamicBook()`. The object remains alive after the local pointer name `book` inside `createDynamicBook()` disappears, because the object's lifetime is controlled by `malloc()` and `free()`, not by the local pointer name.

### Explanation

Scope and lifetime answer different questions:

| Concept  | Question answered                         | Example from program                                 |
| -------- | ----------------------------------------- | ---------------------------------------------------- |
| Scope    | Where can this name be used?              | `totalLoans` can be named only inside `borrowBook()` |
| Lifetime | How long does this variable/object exist? | `totalLoans` exists for the whole program            |

For `totalLoans`:

```text
Scope:
Only inside borrowBook()

Lifetime:
From program start until program termination
```

For `searchIndex`:

```text
Scope:
Only inside searchBookIndex()

Lifetime:
Only during one call of searchBookIndex()
```

For the heap `Book` object:

```text
Scope:
No direct variable name belongs to the heap object itself.
It is accessed through pointer variables such as book or dynamicBook.

Lifetime:
From malloc(sizeof(Book)) until free(dynamicBook)
```

### Conclusion for D

The experiment proves that **scope is about name visibility**, while **lifetime is about how long storage exists**. The static local variable `totalLoans` is the clearest proof: its name is local to `borrowBook()`, but its value survives repeated calls for the entire program.

---

## E. Output and Analysis

### Objective

The purpose of this section is to show that the program prints enough output to verify the whole library tracker and to answer the final analysis questions about aliasing.

The question requires the program to print:

1. A transaction log.
2. A current stock table.
3. Overdue charges.
4. Total successful and failed borrowing attempts.
5. Two places where aliasing helps the program.
6. One way careless aliasing could make debugging harder.

### Relevant Code

The transaction log is stored using:

```c
static Transaction transactionLog[MAX_TRANSACTIONS];
static int transactionCount = 0;
```

The transaction entries are added by:

```c
void addTransaction(const char *type, int studentId, int accessionNo,
                    int dayBorrowed, int dayReturned, int success, int fine,
                    const char *message)
```

The output functions are:

```c
void printTransactionLog(void)
void printStockTable(Book books[], int bookCount)
void printOverdueCharges(void)
```

The borrowing and return counters are:

```c
static int successfulBorrowAttempts = 0;
static int failedBorrowAttempts = 0;
static int successfulReturnAttempts = 0;
static int failedReturnAttempts = 0;
```

They are printed at the end of `main()`:

```c
printf("Successful borrowing attempts: %d\n", successfulBorrowAttempts);
printf("Failed borrowing attempts: %d\n", failedBorrowAttempts);
printf("Successful return attempts: %d\n", successfulReturnAttempts);
printf("Failed return attempts: %d\n", failedReturnAttempts);
```

### How the Output Works

#### 1. Transaction log

Each entry records:

```c
type
studentId
accessionNo
dayBorrowed
dayReturned
success
fine
message
```

The function `printTransactionLog()` prints those fields in table form. This lets the marker see which operations succeeded and which failed.

Example transaction meanings:

| Transaction                                                         | Meaning                                   |
| ------------------------------------------------------------------- | ----------------------------------------- |
| Borrower `501` borrows accession `1001`                             | Successful borrow                         |
| Borrower `503` borrows accession `1003` when copies are unavailable | Failed borrow                             |
| Borrower `502` returns accession `1003` late                        | Successful return with fine               |
| Borrower `504` borrows accession `9999`                             | Failed borrow because book does not exist |
| Borrower `505` returns with no active loan                          | Failed return                             |

#### 2. Current stock table

The stock table is printed by:

```c
void printStockTable(Book books[], int bookCount)
```

It prints:

```text
Accession
Title
Copies
Status
```

This proves that borrowing and returning have changed the actual book records.

#### 3. Overdue charges

Overdue charges are printed by:

```c
void printOverdueCharges(void)
```

The fine is calculated in:

```c
int calculateFine(int dayBorrowed, int dayReturned)
```

For the return where day borrowed is `2` and day returned is `25`:

```text
daysKept = 25 - 2 = 23
overdueDays = 23 - 14 = 9
fine = 9 * 20 = 180
```

So the program correctly reports a fine of `180`.

#### 4. Successful and failed attempts

Borrowing attempts are counted in `borrowBook()`:

```c
successfulBorrowAttempts++;
failedBorrowAttempts++;
```

Return attempts are counted in `returnBook()`:

```c
successfulReturnAttempts++;
failedReturnAttempts++;
```

The question specifically asks for total successful/failed borrowing attempts. The program also prints return attempts, which adds useful evidence without weakening the required borrowing summary.

### Expected/Observed Result

The program output includes sections like:

```text
TRANSACTION LOG
No  Type     Student    Book       BorrowDay  Return   Fine   Message
...

CURRENT STOCK TABLE
Accession    Title                                      Copies   Status
...

OVERDUE CHARGES
Student 501 returned accession 1001: fine = 0
Student 502 returned accession 1003: fine = 180

TOTAL ATTEMPTS
Successful borrowing attempts: 3
Failed borrowing attempts: 2
Successful return attempts: 2
Failed return attempts: 1
```

The exact pointer addresses in the aliasing section may change every time the program runs, but the two alias addresses should match.

### Aliasing Analysis

#### Place 1 - `borrowBook()` updates the real book object

Aliasing helps in `borrowBook()`:

```c
Book *book = searchBook(books, bookCount, accessionNo);
```

The pointer `book` refers to the real object inside the `books` array. Therefore:

```c
book->availableCopies--;
```

updates the actual library stock rather than a temporary copy.

#### Place 2 - `returnBook()` updates the real borrower and book objects

Aliasing also helps in `returnBook()`:

```c
Borrower *borrower = searchBorrower(borrowers, borrowerCount, studentId);
Book *book;
```

The `borrower` pointer refers to the real borrower record, so:

```c
borrower->borrowedAccessionNo = 0;
```

clears the real borrower's active loan. The `book` pointer refers to the real book record, so:

```c
book->availableCopies++;
```

returns the copy to the actual stock.

#### Place 3 - the deliberate aliasing experiment

The deliberate aliasing experiment uses:

```c
Book *selectedBook = searchBook(books, bookCount, 1003);
Book *secondReferenceName = selectedBook;
```

Both pointer names refer to the same `Book`.

### Careless Aliasing Risk

Careless aliasing can make debugging harder because two or more pointer names can modify the same object. If `selectedBook` changes `availableCopies`, then `secondReferenceName` observes the changed value later. If the programmer forgets that the two names share the same address, it may look as if `secondReferenceName` changed unexpectedly.

Another risk is using a pointer after its object has been freed. For example, after:

```c
free(dynamicBook);
dynamicBook = NULL;
```

the old heap object no longer exists. Any other pointer that still held the old address would become a dangling pointer. Dereferencing it would be undefined behavior in C.

### Conclusion for E

The output section proves that the program is not only performing calculations but also showing the required programming-language concepts. The transaction log, stock table, overdue charges, and attempt counters verify the library workflow. The aliasing analysis explains why pointers are useful for updating shared `Book` and `Borrower` objects, while also warning that shared access must be handled carefully in C.

---

# Final Summary

| Task | Concept              | Evidence in Code                              |
| ---- | -------------------- | --------------------------------------------- |
| B    | Aliasing             | `selectedBook` and `secondReferenceName`      |
| B    | Shared storage       | Both pointers contain the same address        |
| B    | Object modification  | `selectedBook->availableCopies += 2`          |
| C    | Heap-dynamic storage | `malloc(sizeof(Book))`                        |
| C    | Object lifetime      | Heap `Book` survives function return          |
| C    | Lifetime termination | `free(dynamicBook)`                           |
| C    | Automatic lifetime   | `searchIndex`                                 |
| C    | Scope vs lifetime    | `searchIndex` vs dynamically allocated `Book` |

## Key Viva Answers

### What is aliasing?

Aliasing occurs when two different names or pointers refer to the same object or memory location.

### Which variables demonstrate aliasing?

```c
selectedBook
secondReferenceName
```

Both point to the same `Book` object.

### What proves that they are aliases?

They contain the same address, and a modification through one pointer is visible through the other.

### What is lifetime?

Lifetime is the period during which a variable or object exists in memory.

### What demonstrates heap-dynamic lifetime?

```c
malloc(sizeof(Book))
```

creates the dynamically allocated `Book`, and:

```c
free(dynamicBook)
```

ends its lifetime.

### What happens to `searchIndex`?

`searchIndex` is an automatic local variable. Its lifetime ends when `searchBookIndex()` returns.

### What is the difference between scope and lifetime?

Scope describes where a name can be used in the program, while lifetime describes how long the associated variable or object exists.

### What is the main lesson from Task B?

Two different pointer names can refer to one object.

### What is the main lesson from Task C?

An object's lifetime does not necessarily equal the scope of the name used to access it.

---

# Additional Answers for the Group 26 Question

The original practical question is for **Group 6 - University Library Borrowing, Fines and Book-State Tracker**. The program in `main.c` is written in C and uses structures, arrays, pointers, named constants, `malloc()`, `free()`, and `static` variables to demonstrate aliases, pointers/references, explicit heap-dynamic storage, lifetime, and scope vs lifetime.

## A. Borrow/Return Workflow

### What data model is used for books?

Book records are represented by the `Book` structure:

```c
typedef struct {
  int accessionNo;
  char title[MAX_TITLE_LENGTH];
  int availableCopies;
  BookStatus status;
} Book;
```

This satisfies the required book data:

| Required field   | Program field     |
| ---------------- | ----------------- |
| Accession number | `accessionNo`     |
| Title            | `title`           |
| Available copies | `availableCopies` |
| Status           | `status`          |

The status is not stored as plain text. It is stored as the `BookStatus` enum:

```c
typedef enum { AVAILABLE, LOW_STOCK, OUT_OF_STOCK } BookStatus;
```

This is better than using unexplained numbers because the names describe the possible book states.

### What data model is used for borrowers?

Borrower records are represented by the `Borrower` structure:

```c
typedef struct {
  int studentId;
  char name[MAX_NAME_LENGTH];
  int borrowedAccessionNo;
} Borrower;
```

This satisfies the required borrower data:

| Required field            | Program field         |
| ------------------------- | --------------------- |
| Student ID                | `studentId`           |
| Name                      | `name`                |
| Borrowed accession number | `borrowedAccessionNo` |

The value `0` in `borrowedAccessionNo` means the borrower currently has no active loan.

### Which named constants are used?

The program uses named constants at the top of `main.c`:

```c
#define MAX_LOAN_DAYS 14
#define CHARGE_PER_DAY 20
```

`MAX_LOAN_DAYS` is used in `calculateFine()` to decide when a book is overdue. `CHARGE_PER_DAY` is used to calculate the training overdue fine. This answers the requirement to avoid magic numbers for loan days and overdue charges.

Other constants are also used:

```c
#define MAX_TITLE_LENGTH 80
#define MAX_NAME_LENGTH 60
#define MAX_BOOKS 12
#define MAX_BORROWERS 10
#define MAX_TRANSACTIONS 20
```

These constants control array sizes and string lengths.

### How is book searching implemented?

Book searching is split into two functions:

```c
int searchBookIndex(Book books[], int bookCount, int accessionNo)
Book *searchBook(Book books[], int bookCount, int accessionNo)
```

`searchBookIndex()` searches the array and returns the array index of the matching accession number. If the book is not found, it returns `-1`.

`searchBook()` calls `searchBookIndex()`. If the index is valid, it returns the address of the actual book:

```c
return &books[index];
```

This means the caller receives a pointer to the real `Book` object, not a copy.

### How is borrowing implemented?

Borrowing is handled by:

```c
int borrowBook(Book books[], int bookCount, Borrower borrowers[],
               int borrowerCount, int studentId, int accessionNo,
               int borrowDay)
```

The function checks:

1. Whether the borrower exists.
2. Whether the book exists.
3. Whether the borrower already has a book.
4. Whether at least one copy is available.

If any check fails, the function records a failed transaction and returns `0`.

If all checks pass, the function performs the borrow operation:

```c
book->availableCopies--;
borrower->borrowedAccessionNo = accessionNo;
updateStatus(book);
successfulBorrowAttempts++;
totalLoans++;
```

This prevents borrowing when no copy is available because the code checks:

```c
if (book->availableCopies <= 0)
```

before decreasing `availableCopies`.

### How is returning implemented?

Returning is handled by:

```c
int returnBook(Book books[], int bookCount, Borrower borrowers[],
               int borrowerCount, int studentId, int returnDay, int borrowDay)
```

The function checks:

1. Whether the borrower exists.
2. Whether the borrower has an active loan.
3. Whether the borrowed accession number still exists in the book array.

If the return is valid, the function calculates the fine, increases the available copies, clears the borrower's loan, updates the book status, and logs the return:

```c
fine = calculateFine(borrowDay, returnDay);
book->availableCopies++;
borrower->borrowedAccessionNo = 0;
updateStatus(book);
successfulReturnAttempts++;
```

### How is the fine calculated?

The fine is calculated by:

```c
int calculateFine(int dayBorrowed, int dayReturned) {
  int daysKept = dayReturned - dayBorrowed;
  int overdueDays = daysKept - MAX_LOAN_DAYS;

  if (overdueDays <= 0) {
    return 0;
  }

  return overdueDays * CHARGE_PER_DAY;
}
```

For example, if a book is borrowed on day `2` and returned on day `25`:

```text
daysKept = 25 - 2 = 23
overdueDays = 23 - 14 = 9
fine = 9 * 20 = 180
```

So the overdue charge is `180`.

### What invalid or exceptional cases are handled?

The program handles at least three invalid cases:

| Invalid case                    | Where handled                   | Result             |
| ------------------------------- | ------------------------------- | ------------------ |
| Borrower not registered         | `borrowBook()` / `returnBook()` | Failed transaction |
| Book accession number not found | `borrowBook()` / `returnBook()` | Failed transaction |
| Borrower already has a book     | `borrowBook()`                  | Failed transaction |
| No copy available               | `borrowBook()`                  | Failed transaction |
| Borrower has no active loan     | `returnBook()`                  | Failed transaction |

The run in `main()` includes boundary/error examples such as trying to borrow accession number `9999` and trying to return for borrower `505`, who has no active loan.

## D. Scope vs Lifetime

### What is the `static` local variable in the borrow function?

Inside `borrowBook()` the program declares:

```c
static int totalLoans = 0;
```

This variable has **local scope** but **static lifetime**.

### Why is the scope of `totalLoans` local?

`totalLoans` is declared inside `borrowBook()`, so its name is visible only inside that function. Code in `main()`, `returnBook()`, or `printTransactionLog()` cannot directly write:

```c
totalLoans
```

because the name is outside their scope.

### Why does the lifetime of `totalLoans` extend across repeated calls?

Because `totalLoans` is declared with the `static` storage-class specifier. It is created once and remains alive until the program ends. Each successful call to `borrowBook()` continues from the previous value:

```c
totalLoans++;
printf("Borrow function: static local totalLoans is now %d.\n", totalLoans);
```

That is why repeated successful borrowing calls print increasing values such as `1`, `2`, and `3`.

### What is an additional example where a variable is alive but not visible?

One clear example is the global array:

```c
static Transaction transactionLog[MAX_TRANSACTIONS];
```

`transactionLog` has static lifetime, so it is alive for the whole execution of the program. However, a local variable inside another function, such as `searchIndex` inside `searchBookIndex()`, cannot see or access local variables declared inside `main()` such as:

```c
Book books[MAX_BOOKS]
Borrower borrowers[MAX_BORROWERS]
```

Another example is `books[0]`. The `Book` object is alive while `main()` is running, but its name `books` is not visible inside `updateStatus()` unless `main()` or another caller passes a pointer to a book. This proves that an object can exist even when a particular name for it is not visible in the current function.

### What is the difference between scope and lifetime?

| Concept  | Meaning                                        | Example from the program                                                                     |
| -------- | ---------------------------------------------- | -------------------------------------------------------------------------------------------- |
| Scope    | Where a name can be used in the source code    | `searchIndex` can be named only inside `searchBookIndex()`                                   |
| Lifetime | How long a variable or object exists in memory | `totalLoans` exists for the whole program even though it is named only inside `borrowBook()` |

Scope is about **visibility of a name**. Lifetime is about **existence of storage/object**.

## E. Output and Analysis

### What output does the program produce?

The program prints:

1. A labelled borrow/return workflow.
2. Search messages showing whether accession numbers were found.
3. The aliasing experiment.
4. The lifetime experiment.
5. A transaction log.
6. A current stock table.
7. Overdue charges.
8. Successful and failed borrowing attempts.
9. Successful and failed return attempts.

### How does the transaction log meet the requirement?

Transactions are stored in:

```c
static Transaction transactionLog[MAX_TRANSACTIONS];
```

Each entry records:

```c
type
studentId
accessionNo
dayBorrowed
dayReturned
success
fine
message
```

The function `printTransactionLog()` prints these entries in table form. This makes both successful and failed operations visible.

### How does the stock table meet the requirement?

The function:

```c
void printStockTable(Book books[], int bookCount)
```

prints each book's accession number, title, available copies, and status. This directly satisfies the requirement to print the current stock table.

### How are overdue charges shown?

The function:

```c
void printOverdueCharges(void)
```

prints fines for successful return transactions. The fine value comes from `calculateFine()`, which uses `MAX_LOAN_DAYS` and `CHARGE_PER_DAY`.

### How are successful and failed attempts shown?

The program uses counters:

```c
static int successfulBorrowAttempts = 0;
static int failedBorrowAttempts = 0;
static int successfulReturnAttempts = 0;
static int failedReturnAttempts = 0;
```

At the end of `main()`, it prints:

```c
printf("Successful borrowing attempts: %d\n", successfulBorrowAttempts);
printf("Failed borrowing attempts: %d\n", failedBorrowAttempts);
printf("Successful return attempts: %d\n", successfulReturnAttempts);
printf("Failed return attempts: %d\n", failedReturnAttempts);
```

The question specifically asks for total successful/failed borrowing attempts. The program also prints return attempts, which adds useful evidence without weakening the required borrowing summary.

### Identify two places where aliasing helps the program

First, aliasing helps in `borrowBook()`:

```c
Book *book = searchBook(books, bookCount, accessionNo);
```

The pointer `book` refers to the real object inside the `books` array. Therefore:

```c
book->availableCopies--;
```

updates the actual library stock rather than a temporary copy.

Second, aliasing helps in `returnBook()`:

```c
Borrower *borrower = searchBorrower(borrowers, borrowerCount, studentId);
Book *book;
```

The `borrower` pointer refers to the real borrower record, so:

```c
borrower->borrowedAccessionNo = 0;
```

clears the real borrower's active loan. The `book` pointer refers to the real book record, so:

```c
book->availableCopies++;
```

returns the copy to the actual stock.

A third clear place is the aliasing experiment itself:

```c
Book *selectedBook = searchBook(books, bookCount, 1003);
Book *secondReferenceName = selectedBook;
```

Both pointer names refer to the same `Book`.

### How could careless aliasing make debugging harder?

Careless aliasing can make debugging harder because two or more pointer names can modify the same object. If `selectedBook` changes `availableCopies`, then `secondReferenceName` observes the changed value later. If the programmer forgets that the two names share the same address, it may look as if `secondReferenceName` changed unexpectedly.

Another risk is using a pointer after its object has been freed. For example, after:

```c
free(dynamicBook);
dynamicBook = NULL;
```

the old heap object no longer exists. Any other pointer that still held the old address would become a dangling pointer. Dereferencing it would be undefined behavior in C.

---

# General Submission Requirements Answered

## Does the program process at least five records or transactions?

Yes. The program contains at least eight books:

```c
Book books[MAX_BOOKS] = {
  {1001, "C Programming: A Modern Approach", 3, AVAILABLE},
  ...
  {1008, "Algorithms Unlocked", 2, AVAILABLE}
};
```

It also contains five borrowers:

```c
Borrower borrowers[MAX_BORROWERS] = {
  {501, "Amina Otieno", 0},
  ...
  {505, "Eunice Wanjiku", 0}
};
```

The transaction sequence in `main()` includes eight borrow/return attempts, which is more than the minimum of five.

## Are named constants used instead of magic numbers?

Yes. Important policy values use named constants:

```c
#define MAX_LOAN_DAYS 14
#define CHARGE_PER_DAY 20
```

Array and string limits also use named constants, such as `MAX_BOOKS`, `MAX_BORROWERS`, and `MAX_TRANSACTIONS`.

## Are functions used to separate responsibilities?

Yes. The program is modular. Examples:

| Function                | Responsibility                             |
| ----------------------- | ------------------------------------------ |
| `searchBookIndex()`     | Finds a book's array index                 |
| `searchBook()`          | Returns a pointer to a matching book       |
| `searchBorrower()`      | Returns a pointer to a matching borrower   |
| `borrowBook()`          | Validates and records borrowing            |
| `returnBook()`          | Validates and records returns              |
| `calculateFine()`       | Calculates overdue charge                  |
| `updateStatus()`        | Updates book state                         |
| `addTransaction()`      | Adds a transaction log entry               |
| `demonstrateAliasing()` | Shows two pointer names sharing one object |
| `demonstrateLifetime()` | Shows heap lifetime and `free()`           |
| `printTransactionLog()` | Prints the transaction log                 |
| `printStockTable()`     | Prints current stock                       |
| `printOverdueCharges()` | Prints fines                               |

## Are claimed concepts linked to specific program elements?

Yes:

| Concept                          | Program evidence                               |
| -------------------------------- | ---------------------------------------------- |
| Aliasing                         | `selectedBook` and `secondReferenceName`       |
| Pointer/reference behavior in C  | `Book *book`, `Borrower *borrower`             |
| Explicit heap-dynamic storage    | `malloc(sizeof(Book))`                         |
| Releasing heap storage           | `free(dynamicBook)`                            |
| Automatic lifetime               | `searchIndex` inside `searchBookIndex()`       |
| Static lifetime with local scope | `static int totalLoans` inside `borrowBook()`  |
| Named constants                  | `MAX_LOAN_DAYS`, `CHARGE_PER_DAY`              |
| Scope vs lifetime                | `totalLoans`, `searchIndex`, and `dynamicBook` |

## What are the normal and boundary/error test runs?

The sequence in `main()` gives normal and error cases:

| Test                                                   | Type             | Expected result               |
| ------------------------------------------------------ | ---------------- | ----------------------------- |
| Borrower `501` borrows book `1001`                     | Normal           | Successful borrow             |
| Borrower `502` borrows book `1003`                     | Normal           | Successful borrow             |
| Borrower `501` returns on day `10`                     | Normal           | Successful return, fine `0`   |
| Borrower `502` returns on day `25`                     | Boundary/overdue | Successful return, fine `180` |
| Borrower `503` borrows book `1003` when copies are `0` | Error            | Failed borrow                 |
| Borrower `504` borrows accession `9999`                | Error            | Failed borrow, book not found |
| Borrower `505` returns without active loan             | Error            | Failed return                 |

---

# One-Page Concept Note

The library tracker demonstrates the required programming-language concepts through the actual running program. The main records are `Book`, `Borrower`, and `Transaction`. Their names are bound to structure types at compile time in C. Objects such as `books` and `borrowers` are created as arrays in `main()`, while `transactionLog` and the attempt counters are static objects that exist for the whole program.

Named constants such as `MAX_LOAN_DAYS` and `CHARGE_PER_DAY` are defined using preprocessor macros. They make the rules easy to find and change. For example, `calculateFine()` does not repeat the literal `20`; it uses `CHARGE_PER_DAY`.

Aliasing is shown when `selectedBook` and `secondReferenceName` both store the same address. A change through `selectedBook->availableCopies` is visible through `secondReferenceName->availableCopies` because both pointers refer to the same `Book` object. Aliasing also appears in the normal workflow: `borrowBook()` and `returnBook()` receive pointers to real `Book` and `Borrower` records so they can update the original data.

Lifetime is shown with `createDynamicBook()`, which allocates a `Book` with `malloc()`. The local pointer named `book` inside `createDynamicBook()` disappears when the function returns, but the heap object continues to exist because its lifetime lasts until `free(dynamicBook)` is called. This is compared with `searchIndex`, a normal local variable whose lifetime ends when `searchBookIndex()` returns.

Scope vs lifetime is shown by `static int totalLoans` inside `borrowBook()`. The name `totalLoans` is visible only inside `borrowBook()`, but the variable remains alive for the whole program and remembers its value across calls. This proves that scope and lifetime are related but not the same.

---

# Variable, Scope, and Lifetime Table

| Name                         | Type                            | Where declared                       | Scope                                   | Lifetime                            | Current/typical value                  |
| ---------------------------- | ------------------------------- | ------------------------------------ | --------------------------------------- | ----------------------------------- | -------------------------------------- |
| `books`                      | `Book[MAX_BOOKS]`               | `main()`                             | Local to `main()`                       | While `main()` is executing         | Array of 8 initialized books           |
| `borrowers`                  | `Borrower[MAX_BORROWERS]`       | `main()`                             | Local to `main()`                       | While `main()` is executing         | Array of 5 borrowers                   |
| `book` in `borrowBook()`     | `Book *`                        | `borrowBook()`                       | Local to `borrowBook()`                 | One call of `borrowBook()`          | Address of selected book or `NULL`     |
| `borrower` in `borrowBook()` | `Borrower *`                    | `borrowBook()`                       | Local to `borrowBook()`                 | One call of `borrowBook()`          | Address of selected borrower or `NULL` |
| `searchIndex`                | `int`                           | `searchBookIndex()`                  | Local to `searchBookIndex()`            | One call of `searchBookIndex()`     | Current loop index                     |
| `totalLoans`                 | `static int`                    | `borrowBook()`                       | Local to `borrowBook()`                 | Whole program                       | Number of successful loans             |
| `dynamicBook`                | `Book *`                        | `demonstrateLifetime()`              | Local to `demonstrateLifetime()`        | One call of `demonstrateLifetime()` | Address returned by `malloc()`         |
| heap `Book` object           | `Book`                          | `createDynamicBook()` via `malloc()` | No direct name; accessed by pointer     | From `malloc()` until `free()`      | Accession `9001`                       |
| `transactionLog`             | `Transaction[MAX_TRANSACTIONS]` | File scope                           | Visible from declaration to end of file | Whole program                       | Transaction history                    |
| `fine`                       | `int`                           | `returnBook()`                       | Local to `returnBook()`                 | One call of `returnBook()`          | Overdue charge for a return            |

---

# Answers to Suggested Viva Questions at the End of the Document

## At what point is this variable bound to its type? To its storage? To its current value?

Example variable:

```c
int fine;
```

In C, `fine` is bound to type `int` at compile time because C is statically typed. Its storage is bound when execution enters the block in `returnBook()` where `fine` is declared. Its current value is bound at run time when this assignment executes:

```c
fine = calculateFine(borrowDay, returnDay);
```

For a heap object created by:

```c
Book *book = malloc(sizeof(Book));
```

the pointer variable `book` is bound to type `Book *` at compile time. The pointer variable's own storage is created when `createDynamicBook()` is called. The heap storage for the actual `Book` object is bound at run time when `malloc()` succeeds.

## Where is this name visible, and why is it not visible elsewhere?

Example:

```c
int searchIndex;
```

`searchIndex` is visible only inside `searchBookIndex()` because it is declared inside that function. It is not visible in `main()`, `borrowBook()`, or `returnBook()` because C uses lexical/block scope: a local declaration can be named only within its declaring block after the declaration point.

## When does this variable/object begin and end its lifetime?

For `searchIndex`, lifetime begins when `searchBookIndex()` is called and execution reaches the declaration. Its lifetime ends when `searchBookIndex()` returns.

For the heap `Book` object created in `createDynamicBook()`, lifetime begins when `malloc(sizeof(Book))` successfully returns storage. It ends when:

```c
free(dynamicBook);
```

is executed.

For `totalLoans`, lifetime begins before program execution starts and ends when the program terminates because it is a static local variable.

## Which two names in your program are aliases, and what evidence proves they share one object/location?

The clearest aliases are:

```c
selectedBook
secondReferenceName
```

Both are `Book *` variables in `demonstrateAliasing()`. The assignment:

```c
Book *secondReferenceName = selectedBook;
```

copies the address from `selectedBook` into `secondReferenceName`. The output prints both addresses and they match. The program also changes:

```c
selectedBook->availableCopies += 2;
```

and then shows that `secondReferenceName->availableCopies` has the same updated value.

## Identify an l-value and r-value from one assignment in your program

Example:

```c
book->availableCopies--;
```

This can be understood as:

```c
book->availableCopies = book->availableCopies - 1;
```

On the left side, `book->availableCopies` is an l-value because it identifies the storage location to be updated. On the right side, `book->availableCopies - 1` is an r-value expression because it produces the new value to store.

Another example:

```c
borrower->borrowedAccessionNo = accessionNo;
```

`borrower->borrowedAccessionNo` is the l-value. `accessionNo` is used as an r-value because its current value is copied into the borrower's record.

## If this local declaration were removed, which outer declaration would be found next?

In the current program there is no deliberate shadowing example with the same identifier in nested scopes. For ordinary local names such as:

```c
int i;
```

inside `printStockTable()`, removing the declaration would not find another valid `i` because there is no file-scope or enclosing block declaration named `i`. The compiler would report an undeclared identifier error.

For a name such as `transactionLog`, functions can find it because it is declared at file scope before the functions that use it. If a local variable named `transactionLog` were declared inside a function, that local declaration would hide the file-scope `transactionLog` within that function. Removing the local declaration would make references resolve to the file-scope array again.

## What would change if this program used dynamic scoping instead of static scoping?

C uses static, also called lexical, scoping. This means name resolution depends on where names are declared in the source code, not on which function called which function at run time.

If the program used dynamic scoping, a function could resolve a name by searching the active caller chain. For example, if `updateStatus()` referred to a variable named `bookCount` without receiving it as a parameter, a dynamically scoped language might search the calling function, such as `borrowBook()` or `main()`, to find an active `bookCount`. In C this is not allowed. `updateStatus()` can access only its parameter `book` and names visible from its lexical/file scope.

Static scoping is more predictable here because a reader can determine which variable a name refers to by reading the source code structure.

## Why is a named constant better than repeating the same literal throughout the code?

A named constant is better because it gives meaning to a value and centralizes changes. For example:

```c
#define CHARGE_PER_DAY 20
```

makes it clear that `20` is the training overdue charge per day. If the charge changes, only the constant definition needs to be updated. Repeating `20` throughout the program would make the code harder to read and easier to update incorrectly.

## Explain one variable from declaration through allocation, value changes, visibility, and end of lifetime

Example variable/object: the dynamically allocated `Book`.

1. Declaration of pointer:

```c
Book *book;
```

inside `createDynamicBook()` creates a local pointer name.

1. Allocation:

```c
book = malloc(sizeof(Book));
```

allocates storage for a `Book` object on the heap.

1. Value changes:

```c
book->accessionNo = accessionNo;
snprintf(book->title, sizeof(book->title), "%s", title);
book->availableCopies = copies;
updateStatus(book);
```

initialize the heap object's fields.

1. Visibility:

The local pointer name `book` is visible only inside `createDynamicBook()`. After the function returns, that name is gone, but the object is accessed through `dynamicBook` inside `demonstrateLifetime()`.

1. End of lifetime:

```c
free(dynamicBook);
dynamicBook = NULL;
```

releases the heap object and removes the stale address from the pointer variable.

---

# Final Checklist for Group 6

| Requirement from document                                 | Covered? | Evidence                                                |
| --------------------------------------------------------- | -------- | ------------------------------------------------------- |
| Book data: accession no., title, available copies, status | Yes      | `Book` struct                                           |
| Borrower data: student ID, name, borrowed accession no.   | Yes      | `Borrower` struct                                       |
| Named constant `MAX_LOAN_DAYS`                            | Yes      | `#define MAX_LOAN_DAYS 14`                              |
| Named constant `CHARGE_PER_DAY = 20`                      | Yes      | `#define CHARGE_PER_DAY 20`                             |
| At least 8 books                                          | Yes      | 8 initialized `Book` records                            |
| At least 5 transactions                                   | Yes      | 8 borrow/return attempts in `main()`                    |
| Search function                                           | Yes      | `searchBookIndex()` and `searchBook()`                  |
| Borrow function                                           | Yes      | `borrowBook()`                                          |
| Return function                                           | Yes      | `returnBook()`                                          |
| Fine calculation                                          | Yes      | `calculateFine()`                                       |
| Prevent borrowing when no copy is available               | Yes      | `if (book->availableCopies <= 0)`                       |
| Aliasing experiment                                       | Yes      | `selectedBook` and `secondReferenceName`                |
| Explain shared storage                                    | Yes      | Same pointer address and same observed copy count       |
| Heap-dynamic object                                       | Yes      | `malloc(sizeof(Book))`                                  |
| Release heap storage                                      | Yes      | `free(dynamicBook)`                                     |
| Compare with local `searchIndex`                          | Yes      | `searchIndex` in `searchBookIndex()`                    |
| Static local `totalLoans`                                 | Yes      | `static int totalLoans = 0;`                            |
| Variable alive but not visible example                    | Yes      | `totalLoans`, `books`, and heap `Book` examples         |
| Transaction log                                           | Yes      | `printTransactionLog()`                                 |
| Current stock table                                       | Yes      | `printStockTable()`                                     |
| Overdue charges                                           | Yes      | `printOverdueCharges()`                                 |
| Successful/failed borrowing attempts                      | Yes      | `successfulBorrowAttempts`, `failedBorrowAttempts`      |
| Places aliasing helps                                     | Yes      | `borrowBook()`, `returnBook()`, `demonstrateAliasing()` |
| Careless aliasing risk                                    | Yes      | Unexpected shared updates and dangling pointers         |
