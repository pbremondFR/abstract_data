# Monkey tester

This tester use the fuzzing technique, it will randomly call random function with random values and randomly print their random contents.
It will then compare the output of your version with the one from the std.

## Usage

Set the path of your headers directory in the Makefile.

`./monkey.sh <container> [<seed>] [<iterations>]`

Setting iterations to 0 will cause the tester to run indefinitely. Launch it and go get a coffee with your friends, if it still runs when you're back this is a good sign!

If it prints OK then your container is ok, otherwise it will print the first diff in the output.

Here is a beautiful drawing of a monkey testing your containers:

![monke](https://upload.wikimedia.org/wikipedia/commons/f/f1/Monkey-typing.jpg)