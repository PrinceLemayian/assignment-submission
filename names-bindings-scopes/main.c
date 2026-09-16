#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE_LENGTH 80
#define MAX_NAME_LENGTH 60
#define MAX_BOOKS 12
#define MAX_BORROWERS 10
#define MAX_TRANSACTIONS 20
#define MAX_LOAN_DAYS 14
#define CHARGE_PER_DAY 20

typedef enum { AVAILABLE, LOW_STOCK, OUT_OF_STOCK } BookStatus;

typedef struct {
  int accessionNo;
  char title[MAX_TITLE_LENGTH];
  int availableCopies;
  BookStatus status;
} Book;

typedef struct {
  int studentId;
  char name[MAX_NAME_LENGTH];
  int borrowedAccessionNo;
} Borrower;

typedef struct {
  char type[12];
  int studentId;
  int accessionNo;
  int dayBorrowed;
  int dayReturned;
  int success;
  int fine;
  char message[120];
} Transaction;

static Transaction transactionLog[MAX_TRANSACTIONS];
static int transactionCount = 0;
static int successfulBorrowAttempts = 0;
static int failedBorrowAttempts = 0;
static int successfulReturnAttempts = 0;
static int failedReturnAttempts = 0;

const char *statusName(BookStatus status) {
  switch (status) {
  case AVAILABLE:
    return "Available";
  case LOW_STOCK:
    return "Low stock";
  case OUT_OF_STOCK:
    return "Out of stock";
  default:
    return "Unknown";
  }
}

void updateStatus(Book *book) {
  if (book == NULL) {
    return;
  }

  if (book->availableCopies <= 0) {
    book->availableCopies = 0;
    book->status = OUT_OF_STOCK;
  } else if (book->availableCopies == 1) {
    book->status = LOW_STOCK;
  } else {
    book->status = AVAILABLE;
  }
}

void addTransaction(const char *type, int studentId, int accessionNo,
                    int dayBorrowed, int dayReturned, int success, int fine,
                    const char *message) {
  if (transactionCount >= MAX_TRANSACTIONS) {
    return;
  }

  Transaction *entry = &transactionLog[transactionCount++];
  snprintf(entry->type, sizeof(entry->type), "%s", type);
  entry->studentId = studentId;
  entry->accessionNo = accessionNo;
  entry->dayBorrowed = dayBorrowed;
  entry->dayReturned = dayReturned;
  entry->success = success;
  entry->fine = fine;
  snprintf(entry->message, sizeof(entry->message), "%s", message);
}

int searchBookIndex(Book books[], int bookCount, int accessionNo) {
  int searchIndex;

  for (searchIndex = 0; searchIndex < bookCount; searchIndex++) {
    if (books[searchIndex].accessionNo == accessionNo) {
      printf("Search: local variable searchIndex found accession %d at index "
             "%d.\n",
             accessionNo, searchIndex);
      return searchIndex;
    }
  }

  printf("Search: local variable searchIndex did not find accession %d.\n",
         accessionNo);
  return -1;
}

Book *searchBook(Book books[], int bookCount, int accessionNo) {
  int index = searchBookIndex(books, bookCount, accessionNo);
  if (index == -1) {
    return NULL;
  }

  return &books[index];
}

Borrower *searchBorrower(Borrower borrowers[], int borrowerCount,
                         int studentId) {
  int i;

  for (i = 0; i < borrowerCount; i++) {
    if (borrowers[i].studentId == studentId) {
      return &borrowers[i];
    }
  }

  return NULL;
}

int calculateFine(int dayBorrowed, int dayReturned) {
  int daysKept = dayReturned - dayBorrowed;
  int overdueDays = daysKept - MAX_LOAN_DAYS;

  if (overdueDays <= 0) {
    return 0;
  }

  return overdueDays * CHARGE_PER_DAY;
}

int borrowBook(Book books[], int bookCount, Borrower borrowers[],
               int borrowerCount, int studentId, int accessionNo,
               int borrowDay) {
  static int totalLoans = 0;
  Book *book = searchBook(books, bookCount, accessionNo);
  Borrower *borrower = searchBorrower(borrowers, borrowerCount, studentId);

  if (borrower == NULL) {
    failedBorrowAttempts++;
    addTransaction("BORROW", studentId, accessionNo, borrowDay, 0, 0, 0,
                   "Failed: borrower was not registered.");
    return 0;
  }

  if (book == NULL) {
    failedBorrowAttempts++;
    addTransaction("BORROW", studentId, accessionNo, borrowDay, 0, 0, 0,
                   "Failed: book accession number was not found.");
    return 0;
  }

  if (borrower->borrowedAccessionNo != 0) {
    failedBorrowAttempts++;
    addTransaction("BORROW", studentId, accessionNo, borrowDay, 0, 0, 0,
                   "Failed: borrower already has a book.");
    return 0;
  }

  if (book->availableCopies <= 0) {
    failedBorrowAttempts++;
    addTransaction("BORROW", studentId, accessionNo, borrowDay, 0, 0, 0,
                   "Failed: no copy was available.");
    return 0;
  }

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

int returnBook(Book books[], int bookCount, Borrower borrowers[],
               int borrowerCount, int studentId, int returnDay, int borrowDay) {
  Borrower *borrower = searchBorrower(borrowers, borrowerCount, studentId);
  Book *book;
  int accessionNo;
  int fine;

  if (borrower == NULL) {
    failedReturnAttempts++;
    addTransaction("RETURN", studentId, 0, borrowDay, returnDay, 0, 0,
                   "Failed: borrower was not registered.");
    return 0;
  }

  accessionNo = borrower->borrowedAccessionNo;
  if (accessionNo == 0) {
    failedReturnAttempts++;
    addTransaction("RETURN", studentId, 0, borrowDay, returnDay, 0, 0,
                   "Failed: borrower has no active loan.");
    return 0;
  }

  book = searchBook(books, bookCount, accessionNo);
  if (book == NULL) {
    failedReturnAttempts++;
    addTransaction("RETURN", studentId, accessionNo, borrowDay, returnDay, 0, 0,
                   "Failed: borrowed accession number no longer exists.");
    return 0;
  }

  fine = calculateFine(borrowDay, returnDay);
  book->availableCopies++;
  borrower->borrowedAccessionNo = 0;
  updateStatus(book);
  successfulReturnAttempts++;

  addTransaction("RETURN", studentId, accessionNo, borrowDay, returnDay, 1,
                 fine, "Successful return.");
  return 1;
}

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

void inspectDynamicBook(Book *book, const char *label) {
  if (book == NULL) {
    return;
  }

  printf("%s: heap Book at %p -> %d, %s, copies=%d, status=%s\n", label,
         (void *)book, book->accessionNo, book->title, book->availableCopies,
         statusName(book->status));
}

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
      selectedBook->availableCopies, secondReferenceName->availableCopies);

  selectedBook->availableCopies += 2;
  updateStatus(secondReferenceName);

  printf("After change through selectedBook: selectedBook copies=%d, "
         "secondReferenceName copies=%d.\n",
         selectedBook->availableCopies, secondReferenceName->availableCopies);
  printf("Explanation: both names store the same address, so they share one "
         "Book object in memory.\n");
}

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

void printStockTable(Book books[], int bookCount) {
  int i;

  printf("\nCURRENT STOCK TABLE\n");
  printf("%-12s %-42s %-8s %-14s\n", "Accession", "Title", "Copies", "Status");
  printf("---------------------------------------------------------------------"
         "----------\n");
  for (i = 0; i < bookCount; i++) {
    printf("%-12d %-42s %-8d %-14s\n", books[i].accessionNo, books[i].title,
           books[i].availableCopies, statusName(books[i].status));
  }
}

void printTransactionLog(void) {
  int i;

  printf("\nTRANSACTION LOG\n");
  printf("%-3s %-8s %-10s %-10s %-10s %-8s %-6s %s\n", "No", "Type", "Student",
         "Book", "BorrowDay", "Return", "Fine", "Message");
  printf("---------------------------------------------------------------------"
         "---------------------------\n");
  for (i = 0; i < transactionCount; i++) {
    Transaction t = transactionLog[i];
    printf("%-3d %-8s %-10d %-10d %-10d %-8d %-6d %s\n", i + 1, t.type,
           t.studentId, t.accessionNo, t.dayBorrowed, t.dayReturned, t.fine,
           t.message);
  }
}

void printOverdueCharges(void) {
  int i;

  printf("\nOVERDUE CHARGES\n");
  for (i = 0; i < transactionCount; i++) {
    if (strcmp(transactionLog[i].type, "RETURN") == 0 &&
        transactionLog[i].success) {
      printf("Student %d returned accession %d: fine = %d\n",
             transactionLog[i].studentId, transactionLog[i].accessionNo,
             transactionLog[i].fine);
    }
  }
}

void initializeStatuses(Book books[], int bookCount) {
  int i;

  for (i = 0; i < bookCount; i++) {
    updateStatus(&books[i]);
  }
}

int main(void) {
  Book books[MAX_BOOKS] = {
      {1001, "C Programming: A Modern Approach", 3, AVAILABLE},
      {1002, "Data Structures in C", 2, AVAILABLE},
      {1003, "Operating Systems Concepts", 1, AVAILABLE},
      {1004, "Database System Principles", 4, AVAILABLE},
      {1005, "Computer Networks", 2, AVAILABLE},
      {1006, "Discrete Mathematics", 1, AVAILABLE},
      {1007, "Software Engineering Practice", 3, AVAILABLE},
      {1008, "Algorithms Unlocked", 2, AVAILABLE}};
  int bookCount = 8;

  Borrower borrowers[MAX_BORROWERS] = {{501, "Amina Otieno", 0},
                                       {502, "Brian Mwangi", 0},
                                       {503, "Carol Njeri", 0},
                                       {504, "David Ouma", 0},
                                       {505, "Eunice Wanjiku", 0}};
  int borrowerCount = 5;

  initializeStatuses(books, bookCount);

  printf("GROUP 26 - UNIVERSITY LIBRARY BORROWING, FINES AND BOOK STATE "
         "TRACKER\n");
  printf("Named constants: MAX_LOAN_DAYS=%d, CHARGE_PER_DAY=%d\n\n",
         MAX_LOAN_DAYS, CHARGE_PER_DAY);

  printf("A. BORROW/RETURN WORKFLOW\n");
  borrowBook(books, bookCount, borrowers, borrowerCount, 501, 1001, 1);
  borrowBook(books, bookCount, borrowers, borrowerCount, 502, 1003, 2);
  borrowBook(books, bookCount, borrowers, borrowerCount, 503, 1003, 3);
  returnBook(books, bookCount, borrowers, borrowerCount, 501, 10, 1);
  returnBook(books, bookCount, borrowers, borrowerCount, 502, 25, 2);
  borrowBook(books, bookCount, borrowers, borrowerCount, 503, 1003, 26);
  borrowBook(books, bookCount, borrowers, borrowerCount, 504, 9999, 27);
  returnBook(books, bookCount, borrowers, borrowerCount, 505, 28, 10);

  demonstrateAliasing(books, bookCount);
  demonstrateLifetime();
  printTransactionLog();
  printStockTable(books, bookCount);
  printOverdueCharges();

  printf("\nTOTAL ATTEMPTS\n");
  printf("Successful borrowing attempts: %d\n", successfulBorrowAttempts);
  printf("Failed borrowing attempts: %d\n", failedBorrowAttempts);
  printf("Successful return attempts: %d\n", successfulReturnAttempts);
  printf("Failed return attempts: %d\n", failedReturnAttempts);

  return 0;
}
