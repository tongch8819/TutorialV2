# Answer Key: Database Systems Pre-Course Diagnostic Test

### 1. Data vs. Information
* **Answer**: Data is a raw, unorganized collection of facts or symbols (e.g., a list of numbers: 85, 90, 72). Information is data that has been processed, organized, or structured to provide context and meaning (e.g., the average test score for the class is 82.3).

### 2. Relationships
* **Answer**: A relationship represents an association between entities. In the student/course scenario, the relationship is "Enrolls." A student enrolls in a course, and a course has students enrolled in it.

### 3. Subsets Calculation
* **Answer**: For a set with $n$ elements, the number of possible subsets is $2^n$. 
* **Process**: Since $n=5$, $2^5 = 32$. This includes the empty set and the set itself.

### 4. Unique Identifiers
* **Answer**: A unique identifier is a specific value used to distinguish one record from every other record in a system. 
* **Example**: A National ID number, a Passport number, or a University Student ID (e.g., your NTU/NUS Matriculation number).

### 5. Linear Search Comparison
* **Answer**: 1000. In an unsorted list, the item you are looking for could be at the very last position, or not in the list at all, requiring you to check every single record.

### 6. Data Redundancy
* **Answer**: This refers to the unnecessary duplication of data within a system (e.g., storing a customer's address in ten different files). 
* **Problem**: It wastes storage space and, more importantly, leads to "Data Inconsistency" where one copy of the data is updated but others are not.

### 7. Set Intersection
* **Answer**: $\{3\}$. The intersection includes only the elements that are present in both Set A and Set B.

### 8. Structured vs. Unstructured Data
* **Answer**: Structured data is highly organized and fits into a predefined schema or table format (rows and columns). Unstructured data does not have a pre-defined model (e.g., emails, videos, or PDFs) and is harder to search/query mathematically.

### 9. Handling Related Records
* **Answer**: This is the concept of "Referential Integrity." If a student is deleted, the system should either prevent the deletion if grades exist, or perform a "Cascade Delete" to remove all related grades to prevent "orphaned" records.

### 10. Concurrency Scenario
* **Answer**: Scenario: Two people try to book the last seat on a flight at the exact same millisecond. 
* **Problem**: Without proper control, both might be told the seat is available, resulting in an overbooked flight (the "Lost Update" problem).

### 11. Purpose of Sorting
* **Answer**: Sorting organizes data into a predictable order (alphabetical or numerical). 
* **Benefit**: For computers, it allows for much faster retrieval (e.g., using Binary Search instead of Linear Search). For humans, it makes data readable and scannable.

### 12. Filtering Concept
* **Answer**: Filtering is the process of extracting only the records that meet a specific criterion.
* **Example**: From an employee list, "Filter all employees where salary > $5000." The result excludes everyone earning less than that amount.