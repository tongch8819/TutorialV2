# Answer Key: Database Systems Entry-Level Diagnostic Assessment

### Part A: Core Data Concepts (60%)

1. **Transforming Data to Information**
   * **Answer**: Data is transformed into information by applying context, organization, and analysis. For a business, raw numbers (data) become information when they are aggregated to show trends, such as "Monthly Sales Growth."

2. **Importance of Unique Identifiers**
   * **Answer**: A unique identifier ensures that each record is distinct. In a university, it prevents confusion between two students with the same name, ensuring grades and fees are assigned to the correct individual.

3. **Data Redundancy Problems**
   * **Answer**: Redundancy leads to wasted storage and data inconsistency. If a phone number is stored in five places and only one is updated, the system will contain conflicting information (Data Inconsistency).

4. **Category Combinations (Subsets)**
   * **Answer**: For a set of 4 categories, there are $2^4 = 16$ possible subsets. This represents every possible combination of those categories, including the empty set and the full set.

5. **Concept of a Relationship**
   * **Answer**: A relationship is a logical link between entities. The link between "Author" and "Book" is "Writes"—an author writes a book, and a book is written by an author.

6. **Linear Search Efficiency**
   * **Answer**: 500 attempts. In the worst-case scenario (the name is last or missing), the computer must check every single record in an unsorted list.

7. **Set Intersection**
   * **Answer**: The intersection is $\{30\}$. In data terms, this represents the specific IDs or records that are common to both groups A and B.

### Part B: Course-Wide Foundational Knowledge (40%)

8. **Structured vs. Unstructured Data**
   * **Answer**: Structured data (tables) follows a strict format; unstructured data (photos) does not. Structured data is much easier for a database to manage because it can be queried using mathematical logic (SQL).

9. **Maintaining Data Integrity**
   * **Answer**: To maintain integrity, the system should either prevent the deletion of the student record while grades exist or use a "cascade" effect to archive or remove related grades so no "orphaned" data remains.

10. **Concurrency Errors**
   * **Answer**: This is a "Race Condition" or "Concurrency Conflict." Both agents might see the room as available, leading to a "Double Booking" if the system doesn't lock the record during the update.

11. **Database Indexing**
   * **Answer**: An index is a pointer system that allows the database to find data without searching every row. Just like a textbook index tells you exactly which page a word is on, a database index points directly to the data's location.

12. **Filtering Data**
   * **Answer**: Filtering is the process of hiding records that do not meet specific criteria. 
   * **Example**: A manager applies a filter "Years_Worked > 5" to the employee list; the system then suppresses all records where the value is 5 or less.