# Hanoi tower game

Hanoi tower game solved automaticaly. It posseses the following characteristics:

- Developed with C99;
- The [Hanoi Tower](https://en.wikipedia.org/wiki/Tower_of_Hanoi) is a puzzle with the following rules:
    - Only one disk may be moved at a time;
    - Each move consists of taking the upper disk from one of the towers and placing it on top of another tower;
    - No disk may be placed on top of a disk that is smaller than it;
- This code is platform dependent and will work only on Linux;
- It produces an terminal animation just like this:

![Alt Text](./hanoi.gif)

## Usage

Download this project and compile it by typing the command `make` in its folder. Next, just run the executable `./hanoiTower`. You may pass as an argument the number of discs of the game (the minimum is 4 discs). Here is an example:

```
./hanoiTower 5
```
