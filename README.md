# BimaruSo

This program solves Bimaru riddles. You only need to enter the Bimaru initial state, then it checks if it is solvable and if so, it returns a solution. It only runs in the console.

##  Reqiurements

You only need `gcc` and `make`.

## Installation

```bash
git clone https://github.com/Benjaminreinhard/dotfiles.git
cd bimaruso
make MODE=release
```

## Usage

Enter your Bimaru riddle into the `input.md` file according to the rules that are explained in the next section and then run `./bimaruso`.

## Input file

The `input.md` file has to look as as the following example:

	# Board
	........,
	........,
	...r....,
	.......b,
	........,
	t.w.....,
	........,
	......l.

	# Row numbers
	4, 2, 5, 2, 1, 3, 1, 2

	# Column numbers
	5, 0, 3, 2, 1, 3, 1, 5

	# Ship counts
	4: 1, 3: 2, 2: 3, 1: 4

You can add as many whitespaces and newlines as you want. They are ignored by the program.

It needs all four sections and they have to be in the order as above.

### Board section

The board section starts with the title `# Board`. Every row of the board must be separated by a comma.

It only allows the following characters and they are understood as follows:

* `.` = empty cell
* `w` = water
* `s` = single boat
* `r` = right end of a boat
* `l` = left end of a boat
* `t` = top end of a boat
* `b` = bottom end of a boat
* `m` = middle piece of a boat

### Row numbers section

The row numbers section starts with the title `# Row numbers`. The numbers are separated by a comma.

The first number says how many cells in the first row are occupied by boats, the second number says how many cells in the second row are occupied by boats and so on.

### Column numbers section

The column numbers section starts with the title `# Column numbers`. The numbers are separated by a comma.

The first number says how many cells in the first column are occupied by boats, the second number says how many cells in the second column are occupied by boats and so on.

### Ship counts section

The ship counts section starts with the title `# Ship counts`. Each ship count is of the form `x: y` with and each ship count is separated by a comma.

The number to the left of the colon describes the size of the boat and the number to the right of the colon describes the amount. E.g. `3: 2` means that there are two boats of size three on the board.

# License

See `LICENSE`.
