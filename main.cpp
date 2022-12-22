#include <iostream>
#include <string>
#include <sqlite3.h>

using namespace std;

// Structure to represent a comic book
struct ComicBook {
  string title;
  string author;
  string publisher;
  int year;
};

// Function to add a new comic book to the database
void addComicBook(sqlite3* db, const ComicBook& book) {
  string sql = "INSERT INTO comics (title, author, publisher, year) VALUES (?, ?, ?, ?)";
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    sqlite3_bind_text(stmt, 1, book.title.c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 2, book.author.c_str(), -1, nullptr);
    sqlite3_bind_text(stmt, 3, book.publisher.c_str(), -1, nullptr);
    sqlite3_bind_int(stmt, 4, book.year);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
      cerr << "Error inserting data into the database." << endl;
    }
  }
  sqlite3_finalize(stmt);
}

// Function to retrieve all comic books from the database
vector<ComicBook> getComicBooks(sqlite3* db) {
  vector<ComicBook> comics;
  string sql = "SELECT * FROM comics";
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      ComicBook book;
      book.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
      book.author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
      book.publisher = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
      book.year = sqlite3_column_int(stmt, 3);
      comics.push_back(book);
    }
  }
  sqlite3_finalize(stmt);
  return comics;
}

int main() {
  sqlite3* db;
  if (sqlite3_open("comics.db", &db) != SQLITE_OK) {
    cerr << "Error opening database." << endl;
    return 1;
  }
  
  char choice;
  do {
    cout << "Welcome to the comic book inventory system!" << endl;
    cout << "Enter 'a' to add a new comic book to the inventory," << endl;
    cout << "Enter 'd
