## Objectives summary — “The World Around Us” (PHP + WAMP)

You need to build a **dynamic kids quiz website** called **“The world around us”** where users can play **multiple quizzes in one game session**, earn points, and view a cumulative leaderboard.

### Core flow

* **Start of game:** user enters a **nickname** and chooses a quiz topic:

  * **Animals**
  * **Environment**
* The user starts each game with **0 overall points**.
* A single game session can include **unlimited quizzes**, across the same or different topics.

### Quiz rules

* Each quiz displays **4 randomly chosen questions** from the selected topic.
* Each topic must have **at least 8 questions** available in storage.

**Animals topic**

* Each question is an **animal description**.
* User types the animal name in a **text field**.
* If the answer is **wrong or empty**, it counts as **incorrect**.

**Environment topic**

* Each question is an **interesting environmental fact**.
* User answers **True / False**.

### Scoring + results

After each quiz, show:

* Number of **correct** and **incorrect** answers
* **Points earned in the current quiz**, computed as:
  **(correct × 2) − (incorrect × 1)**
* The user’s **overall points in the current game** (sum across quizzes in that session)

### After-quiz options (navigation)

After showing results, user can choose to:

* Start a **new quiz** (same or different topic)
* View **Leaderboard**
* **Exit**

### Leaderboard requirements

* Leaderboard shows **all players’ cumulative points across all games** (not just current session).
* Must allow sorting players by:

  * **nickname**, or
  * **greatest score**

### Exit screen

When the user chooses **Exit**:

* Display the user’s **nickname** and their **overall points across all games** (previous + current).
* Provide an option to **start a new game** (enter a nickname again).

### Data storage constraint

* Use **text files** to store:

  * question banks (Animals/Environment)
  * user scores / leaderboard data

### UI/interfaces you must implement

* Nickname entry + main menu (topics, leaderboard, exit)
* Quiz screen (4 questions)
* Results screen (quiz + session total)
* Leaderboard screen with sorting options
* Exit/summary screen

If you want, next we can turn this into a clean **page map + file structure** for a PHP/WAMP implementation (e.g., `index.php`, `quiz.php`, `result.php`, `leaderboard.php`, `data/*.txt`).
