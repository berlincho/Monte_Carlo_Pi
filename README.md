# Monte Carlo Pi Computation

## About

Suppose we toss darts randomly at a square dartboard, whose bullseye is at the origin, and
whose sides are two feet in length. Suppose also that there is a circle inscribed in the square
dartboard. The radius of the circle is one foot, and its area is π square feet. If the points that
are hit by the darts are uniformly distributed (and we always hit the square), then the number
of darts that hit inside the circle should approximately satisfy the equation

number in circle/ total number of tosses = π/4

## Execute

Tool Chain: gcc -pthread -std=gnu99 -O2 -s pi.c -o pi

Execute: taskset -c 1,2,3,4 ./pi <# of cores> <# of tosses>
 
 
## Generate pseudo random numbers

```c
rndf = (double) rand_r() / (RAND_MAX + 1.0); // 產生 [0, 1) 浮點亂數
result = (up - low) * rndf + low;  // 產生 [low, up) 浮點亂數
```
