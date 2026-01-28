### Midterm Quiz Practice Questions

Q1) Declare a variable called `size` with a value of 15. Increase the value of `size` by 10 each frame. 

```javascript

function setup() {
    createCanvas(600, 400);

}

function draw() {
    background(220);

}
```

Q2)  Declare a variable called `x` and give it a random value between 0 and the width of the sketch each frame.<br>
Use a conditional to draw a square of size 30 at the value of `x`, colored RED if on the left of the screen, WHITE if on the right

```javascript


function setup() {
    createCanvas(600, 400);


}


function draw() {
    background(220);




}
```
<div style="page-break-after: always;"></div>
Q3) Write a `while` loop that prints out the odd numbers exactly once, in decreasing order, from 97 down to 3

```javascript


function setup() {
    createCanvas(600, 400);




}


function draw() {
    background(220);


}
```

Q4) Add one line of code to draw a circle with diameter `diam` each frame, centered at the mouse position

```javascript

let diam = 20;

function setup() {
    createCanvas(600, 400);

}

function draw() {
    background(220);

}
```

Q5) Add code to turn the circle GREEN when the mouse is within it
```javascript

let x = 300;
let y = 200;

function setup() {
    createCanvas(600, 400);

}

function draw() {
    background(220);
    fill(0);
    x = x + random(-3, 3);
    y = y + random(-3, 3);
    let d = dist(x, y, mouseX, mouseY);
     

   
    circle(x, y, 20);
}
```
<div style="page-break-after: always;"></div>
Q6) Add code to draw a grid of rectangles, with 8 columns and 7 rows, using the entire canvas

```javascript


function setup() {
    createCanvas(800, 700);






}
```
Q7) What does the following program print to the console

```js
let i = 5;

function setup() {
  createCanvas(600, 400);
  let num = map(i, 0, 10, 200, 0);
  console.log(num);
}
```
Answer: 

<br>&nbsp;<br>
<br>&nbsp;<br>



<div style="page-break-after: always;"></div>


Q8) Fill in the arguments to `map()` in the sketch, mapping `i` to the circle size, with the following output:

<img width="223" alt="image" src="images/3e045b7c-dc28-4b67-b313-599a731e9512.png" />

```js

function setup() {
  createCanvas(200, 200);
  for (let i = 0; i < 4; i++) {
    fill(i * 50);

    let sz = map(                      );

    circle(100, 100, sz);
  }
}
```

Q9) Use `map()` with one or more of the `r`,`g`,`b` variables below so that the background color changes gradually as the mouse moves, from `red` when the mouse is nearest to the left edge of the canvas, to `white` when the mouse is near the right edge.

Hint: consider the separate R,G,B values for the two colors.

```js

let r,b,g;

function setup() {
    createCanvas(600, 400);



}

function draw() {
    background(r,g,b);








}
```
Q10) Create a global array called `arr` containing 5 numbers of your choice. 
Then replace the 2nd item in the array with a random number from 0 to 100.

```javascript


function setup() {
    createCanvas(600, 400);





}
```

Q11) Create a global array called `arr`, then use a loop to fill it with 100 random numbers
```javascript

function setup() {
    createCanvas(600, 400);





}

```

Q12) Add code below to make the circle move along the y-axis with a "random walk".
```javascript

let y = 300;

function draw() {
    background(200);






    circle(y, 50, 20);
}

```



Q13) Create an array called `arr` with 4 different letters or characters inside. 
Use `text()` to display a random element from the array, centered on the screen, exactly once.

```javascript



function setup() {
    createCanvas(600, 400);



}

function draw() {
    background(220);


}
```


<!-- Q14) Write the min/max possible outputs for the functions below:<br>

sin() : <br>
cos() : <br>
noise() : <br> -->

<!-- <br>

Q15) Fill in the parameters to the `lerp()` function to create the image below:

<img width="442" alt="image" src="images/d4871f98-b8ae-433d-ab03-346de4dc76a2.png" />

```javascript
function setup() {
  createCanvas(400, 100);
  for (let i = 0; i < 10; i++) {
    let col = lerp(             )
    fill(col);
    rect(i*40, 0, 40, 400)  
  }
}
``` -->

<!-- <br>

Q16) Complete lines 8 and 9 below, setting `x` and `y` so that the 2nd circle is 300 pixels away from the first, at an angle of 10 degrees

<img width="442" alt="image" src="images/ded28d74-4898-471b-909d-4136c44736b1.png" />

```javascript
1.   let x = 50, y = 25;
2. 
3.   function setup() {
4.     createCanvas(400, 100);
5.     background(220);
6.     angleMode(DEGREES);
7.     circle(x, y, 20)  
8.     x = 
9.     y = 
10.    circle(x, y, 20);  
11.  }
```
<br> -->

<!-- 
Q17) Fill in the parameters to the `lerp()` function to create the image below:

<img width="442" alt="image" src="https://git.arts.ac.uk/user-attachments/assets/08810af7-504b-4f11-b16a-7c7ee49c9505"/>

```javascript
function setup() {
  createCanvas(400, 100);
  for (let i = 0; i < 10; i++) {
    let col = lerp(             )
    fill(col);
    rect(i*40, 0, 40, 400)  
  }
}
``` -->

<br>

Q14) Create a JavaScript object called `circ` in `setup()`, giving it `x`, `y` and `radius` properties, with values of your choice 
```javascript
function setup() {





}
```
<br>

Q15) 
Change the `x` property of the 3rd element in the `positions` array to be 50
```javascript
let positions = [
  { x: 20, y: 30 },
  { x: 100, y: 30 },
  { x: 40, y: 80 },
  { x: 120, y: 100 },
];

function setup() {
  createCanvas(400, 100);


}
```

Q16) Add the `dividedRect()` function, using the same parameters as `rect()`, to complete the sketch below:

<img width="800" alt="image" src="images/4a127c71-853b-4db3-90b2-24ff5b758c0a.png" />

```js
// your code here








```

<br>&nbsp;<br>


Q17) Fill in the function below so that it computes and returns the square of any number n (the number multiplied by itself).

_No need to worry about edge cases like non-numbers or undefined inputs._

```js
 function setup() {

    createCanvas(200,200)
    background(255);

    // tests for your function
    let a = squared(1); // 1
    print(a);

    a = squared(3);   // 9
    print(a);

    a = squared(-5); // 25
    print(a);
  }

  function squared(n) {
     // your code here




  }
```

<br>


Q18) Fill in the function below so that it returns the smallest of the numbers in the input array, without using the built-in `max()` function. 

_No need to worry about edge cases like empty arrays, non-numbers, or undefined inputs._

```js
 function setup() {

    createCanvas(200,200)
    background(255);

    let a = smallest([ 5, 9, 6 ]); 
    print(a);  //5

    a = smallest([ 7, 4, 0 ]); 
    print(a);  // 0

    a = smallest([ -3, -5, -100 ]); 
    print(a); // -100
  }

  function smallest(arr) {
     // your code here




  }
```


Q19) Fill in the function below so that it it computes and returns the sum of _all numbers_ from _a_ to _b_. 

```js
  function setup() {

    createCanvas(200,200)
    background(255);

      // tests for your function
     let a = sumFromAToB(1, 3);
     print(a);  // 6

     a = sumFromAToB(-5, 6); 
     print(a);  // 6

     a = sumFromAToB(2, 18);
     print(a);  // 170
  }

  function sumFromAToB(a, b) {
     // your code here




  }
```

Q20) Fill in the function below so that it computes and returns the sum of _all odd positive numbers_ between _a_ and _b_
 
```js
 function setup() {

    createCanvas(200,200)
    background(255);

    // tests for your function
    let a = oddsFromAToB(1, 3);
    print(a);  // 4

    a = oddsFromAToB(-7, 4);   
    print(a);  // 4

    a = oddsFromAToB(2, 19); 
    print(a); // 99
  }

  function oddsFromAToB(a,  b) {
     // your code here




  }
```

<br>


Q21) Complete the sketch below by adding a mouseClicked() event handler.

When the mouse is clicked, update `xPos` and `yPos` so the circle moves to the mouse position.

```javascript
let xPos;
let yPos;

function setup() {
  createCanvas(500, 400);

  // initialize circle at center
  xPos = width / 2;
  yPos = height / 2;
}

function draw() {
  background(220);
  circle(xPos, yPos, 40);
}

// add mouseClicked() event handler below
```