# Answer Key — Assessment Task (CS3230 DAA)

## 1) Rank functions by asymptotic growth (slowest → fastest) with justification

### Final ranking
$$
n\log n \;\;<\;\; n^{1.5} \;\;<\;\; n^2 \;\;<\;\; 2^n \;\;<\;\; n! 
$$
and also
$$
\log(n!) \text{ fits between } n \text{ and } n\log n,\ \text{specifically } \log(n!) = \Theta(n\log n).
$$

$$
n! \sim \sqrt{2\pi n}\left(\frac{n}{e}\right)^n \qquad (n\to\infty).$$

So a consistent total order is:
$$
\log(n!) \;\sim\; n\log n \;\;<\;\; n^{1.5} \;\;<\;\; n^2 \;\;<\;\; 2^n \;\;<\;\; n!
$$

### Pairwise justifications (key comparisons)
- **$\log(n!)$ vs $n\log n$**: By Stirling’s approximation,
  $$
  n! \approx \sqrt{2\pi n}\left(\frac{n}{e}\right)^n
  \Rightarrow \log(n!) = \Theta(n\log n).
  $$
  Hence $\log(n!)$ and $n\log n$ have the same asymptotic growth (up to constant factors).

- **$n\log n$ vs $n^{1.5}$**:
  $$
  \frac{n\log n}{n^{1.5}}=\frac{\log n}{n^{0.5}}\to 0 \quad (n\to\infty),
  $$
  so $n\log n = o(n^{1.5})$.

- **$n^{1.5}$ vs $n^2$**:
  $$
  \frac{n^{1.5}}{n^2}=n^{-0.5}\to 0,
  $$
  so $n^{1.5}=o(n^2)$.

- **$n^2$ vs $2^n$**:
  $$
  \frac{n^2}{2^n}\to 0,
  $$
  exponential dominates any polynomial.

- **$2^n$ vs $n!$**: Use $n! \ge (n/2)^{n/2}$ for $n\ge2$.
  Then
  $$
  \frac{2^n}{n!} \le \frac{2^n}{(n/2)^{n/2}}
  = \left(\frac{4}{n}\right)^{n/2}\to 0,
  $$
  so $2^n = o(n!)$.

---

## 2) Prove $f(n)=5n^3+2n^2+20$ is $O(n^3)$ (find $c, n_0$)

**Definition:** $f(n)=O(g(n))$ if there exist constants $c>0$ and $n_0$ such that
$$
0 \le f(n) \le c\cdot g(n)\quad \forall n\ge n_0.
$$

Let $g(n)=n^3$. For $n\ge 1$,
$$
2n^2 \le 2n^3 \quad\text{(since } n^2 \le n^3\text{)}
$$
and
$$
20 \le 20n^3.
$$
So for all $n\ge 1$,
$$
f(n) = 5n^3 + 2n^2 + 20 \le 5n^3 + 2n^3 + 20n^3 = 27n^3.
$$

Thus choose:
- $c = 27$
- $n_0 = 1$

Then for all $n\ge n_0$, $0\le f(n)\le c n^3$. Hence $f(n)=O(n^3)$.

---

## 3) Linear search: loop invariant + correctness proof

### Linear Search (typical)
Given array $A$$1..n$$$ and target $x$, scan $i=1$ to $n$; if $A$$i$$=x$ return $i$, else after loop return NOT_FOUND.

### Loop invariant
At the start of each iteration with index $i$ (i.e., before checking $A$$i$$$):
> **Invariant:** $x$ does not appear in the prefix $A$$1..i-1$$$.

### Proof
- **Initialization:** Before the first iteration, $i=1$. The prefix $A$$1..0$$$ is empty, so $x$ does not appear there. Invariant holds.

- **Maintenance:** Assume invariant holds at start of iteration $i$.
  - If $A$$i$$=x$, algorithm returns $i$ and stops, so correctness is immediate.
  - Otherwise $A$$i$$\ne x$. Then $x$ does not appear in $A$$1..i-1$$$ (by invariant) and also not at $A$$i$$$. Hence $x$ does not appear in $A$$1..i$$$.
  At next iteration, index is $i+1$, and the invariant statement becomes “$x$ does not appear in $A$$1..(i+1)-1$$=A$$1..i$$$” which we just established. So invariant is maintained.

- **Termination:** If the loop terminates without returning, then $i = n+1$. By the invariant at that point, $x$ does not appear in $A$$1..n$$$. Therefore returning NOT_FOUND is correct.

---

## 4) Master Method for $T(n)=4T(n/2)+n^2$

Identify:
- $a=4$
- $b=2$
- $f(n)=n^2$

Compute:
$$
n^{\log_b a} = n^{\log_2 4} = n^2.
$$

Compare $f(n)$ with $n^{\log_b a}$:
$$
f(n) = n^2 = \Theta(n^{\log_b a}).
$$

This is **Master Theorem Case 2** (equal up to $\Theta$).
So:
$$
T(n) = \Theta\left(n^{\log_b a}\log n\right)
= \Theta(n^2 \log n).
$$

---

## 5) Time complexity of the pseudo-code (summation shown)

Code:
```text
for i = 1 to n:
    for j = 1 to i * i:
        print(i, j)
```

For a fixed (i), inner loop runs (i^2) times.

Total operations (prints):
$$
\sum_{i=1}^{n} i^2.
$$

We know:
$$
\sum_{i=1}^{n} i^2 = \frac{n(n+1)(2n+1)}{6} = \Theta(n^3).
$$

Therefore the runtime is:
$$
O(n^3) \quad \text{(and in fact } \Theta(n^3)\text{)}.
$$

---

## 6) Coin system where greedy fails (show greedy vs optimal)

Let coin denominations be:
$$
{1,\ 3,\ 4}
$$
and value (V=6).

### Greedy (take largest coin first)

* Take 4 (remaining 2)
* Take 1 (remaining 1)
* Take 1 (remaining 0)

Greedy uses **3 coins**: (4+1+1).

### Optimal solution

* Take 3 (remaining 3)
* Take 3 (remaining 0)

Optimal uses **2 coins**: (3+3).

So greedy fails to find the minimum number of coins for this coin system.

---

## 7) Why comparison sorting needs (\Omega(n\log n)) comparisons (decision tree)

In any comparison-based sorting algorithm, the sequence of comparisons depends on outcomes, so the algorithm corresponds to a **decision tree**:

* Each internal node: a comparison (two possible outcomes).
* Each leaf: a possible final ordering/output.

To correctly sort (n) *distinct* keys, there are (n!) possible input permutations, and the algorithm must be able to output a different sorted result for each permutation. Thus the decision tree must have at least (n!) leaves.

A binary decision tree of height (h) has at most (2^h) leaves, so:
$$
2^h \ge n! \Rightarrow h \ge \log_2(n!).
$$

Using Stirling’s approximation (or standard bound) (\log(n!) = \Theta(n\log n)), hence:
$$
h = \Omega(n\log n).
$$

Since each comparison gives at most 1 bit of information, the worst-case number of comparisons is at least the height:
$$
\Omega(n\log n).
$$

---

## 8) Stable sort definition + trace of instability (Quicksort example)

### Definition (stable)

A sorting algorithm is **stable** if, whenever two elements have equal keys, their **relative order in the output is the same** as in the input.

### Demonstration of instability (Quicksort-style partition)

Use labeled equal keys to track order:
Input array (key, label):
$$
$$(2,a),\ (1,x),\ (2,b),\ (1,y)$$
$$

Suppose Quicksort picks pivot ((1,x)) and uses an *in-place partition* scheme that swaps elements to group “< pivot” and “> pivot” (common implementations are not stable).

One possible partition process:

* Pivot is ((1,x)). We want elements (\le 1) on the left, others on the right.
* When scanning, we may swap ((1,y)) leftward past ((2,b)) and ((2,a)) using swaps.

A possible sequence (not unique, but valid for typical in-place partition):

1. Start: ($$(2,a),(1,x),(2,b),(1,y)$$)
2. Swap ((1,y)) left with ((2,b)): ($$(2,a),(1,x),(1,y),(2,b)$$)
3. Swap ((1,y)) left with ((2,a)): ($$(1,y),(1,x),(2,a),(2,b)$$)

Now the two equal keys “1” appear as ((1,y)) before ((1,x)), **reversing their original order** (original had ((1,x)) before ((1,y))).

Therefore this Quicksort partition is **unstable**.

---

## 9) Define **P** and **NP** + why NP not necessarily “hard”

* **P**: Class of decision problems solvable in **polynomial time** by a deterministic algorithm (e.g., (O(n^k)) for some constant (k)).

* **NP**: Class of decision problems for which a “yes” instance has a **polynomial-size certificate** that can be **verified in polynomial time** by a deterministic algorithm.
  Equivalently: solvable in polynomial time by a nondeterministic machine.

### Why NP is not necessarily “hard”

A problem being in NP means **solutions can be verified quickly**, not that they are hard to *find*. Some NP problems are also in P (e.g., shortest path decision variants), meaning they are efficiently solvable.

### Role of polynomial-time verifiability

Polynomial-time verifiability is the core of NP: even if finding the solution seems difficult, **checking** a proposed solution can be done efficiently.

---

## 10) Counting Sort: process + when preferred (range (k))

### Process (high-level)

Counting Sort assumes keys are integers in a bounded range ($$0..k$$) (or ($$1..k$$)):

1. Create count array (C$$0..k$$) initialized to 0.
2. Scan input array (A), increment (C$$A$$i$$$$).
3. (Optional for stable version) Compute prefix sums in (C) so (C$$v$$) tells the ending position of value (v).
4. Place each element into the correct output position (stable version scans from right to left).

### Complexity and when preferred

* Time: (\Theta(n + k))
* Space: (\Theta(n + k)) (or (\Theta(k)) extra plus output)

Counting Sort is preferred over (O(n\log n)) comparison sorts when:

* (k) is **not too large**, typically (k = O(n)) or (k = O(n^c)) for a small constant (c),
  so that (n+k) is competitive with (or better than) (n\log n).
  In practice: when the range of keys is reasonably small relative to (n), counting sort can be faster.

---

## 11) Potential method of amortized analysis + difference from average-case (binary counter example)

### Potential method (definition)

Amortized analysis bounds the cost of a **sequence** of operations, not a probability-based expectation.

Pick a potential function (\Phi(\text{state}) \ge 0), representing “stored energy/credits”.
Define amortized cost:
$$
\hat{c_i} = c_i + \Phi(D_i) - \Phi(D_{i-1}),
$$
where:

* (c_i) is actual cost of operation (i),
* (D_i) is data structure state after operation (i).

Summing over operations telescopes:
$$
\sum_{i=1}^{m} \hat{c_i}
= \sum_{i=1}^{m} c_i + \Phi(D_m) - \Phi(D_0)
\ge \sum_{i=1}^{m} c_i - \Phi(D_0),
$$
so amortized costs upper-bound total actual cost (up to a constant).

### Difference vs average-case

* **Average-case** depends on a probability distribution over inputs.
* **Amortized** makes **no probability assumptions**; it guarantees that *over any sequence* of operations, the average cost is bounded.

### Binary counter example (conceptual)

Operation: increment a binary counter.

* Worst-case single increment: flips many trailing 1s to 0 and one 0 to 1 → can cost (\Theta(\log n)).
* But over many increments, each bit flips from 0→1 infrequently.

Use potential:
$$
\Phi = \text{number of 1 bits in the counter}.
$$

When increment happens:

* Actual cost (c): number of flipped bits.
* If (t) trailing 1s flip to 0 and one 0 flips to 1, then (c = t + 1).
* Potential change:

  * (t) ones become zeros → potential decreases by (t),
  * one zero becomes one → potential increases by 1,
    so (\Delta\Phi = 1 - t).

Amortized cost:
$$
\hat{c} = (t+1) + (1 - t) = 2 = O(1).
$$

Thus, although a single increment can be expensive, a sequence of increments has **O(1) amortized** cost per operation.


