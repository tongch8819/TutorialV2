# Assessment Task 

1. Rank the following functions by their asymptotic growth rate from slowest to fastest. For each pair in your ranking, provide a brief mathematical justification: $n^2$, $n \log n$, $2^n$, $n!$, $\log(n!)$, $n^{1.5}$.

2. Using the formal definition of $O$-notation, prove that $f(n) = 5n^3 + 2n^2 + 20$ is $O(n^3)$. You must explicitly identify the constants $c$ and $n_0$ that satisfy the condition.

3. Consider a standard linear search algorithm. State a formal loop invariant for the search process and use it to prove the correctness of the algorithm (Initialization, Maintenance, and Termination).

4. Use the Master Method to solve the following recurrence: $T(n) = 4T(n/2) + n^2$. Show your step-by-step comparison of $f(n)$ with $n^{\log_b a}$.

5. Analyze the asymptotic time complexity of the following pseudo-code. Provide the result in Big-O notation and show the summation used to derive it:
```
for i = 1 to n: 
    for j = 1 to i * i: 
        print(i, j)
```


6. The "Change-Making Problem" seeks to use the minimum number of coins for a value $V$. Provide a set of coin denominations where the greedy approach (always taking the largest coin first) fails to give the optimal solution. Show the step-by-step process of the greedy failure versus the optimal result.



7.  Explain why any comparison-based sorting algorithm requires $\Omega(n \log n)$ comparisons in the worst case. Focus on the relationship between the height of a decision tree and the number of reachable leaves.

8. Define what it means for a sorting algorithm to be "stable." Provide a trace (a step-by-step sequence) of an unstable sort (like Heapsort or Quicksort) on a small array with duplicate keys to demonstrate how the relative order of equal elements can change.

9. Briefly define the classes **P** and **NP**. Why is a problem in **NP** not necessarily "hard," and what role does "Polynomial-time Verifiability" play in this definition? 

10. Research the Counting Sort algorithm. Explain the process and describe the specific scenario (regarding the range of input values $k$) where this algorithm is preferred over $O(n \log n)$ sorting algorithms.

11. Explain the "Potential Method" of amortized analysis. How does this differ from simple "average-case" analysis? Use the process of incrementing a binary counter as a conceptual example.
