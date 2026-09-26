# Axys Language

![Axys Logo](images/axys_logo.png)

 ## What is it?

 Axys is an instruction- and memory-based programming language that brings together the experience of **Assembly** and **Brainfuck**.

 Its goal is to make low-level programming much easier to learn, bringing back the enjoyment of studying low-level concepts without feeling lost.

 ## How does it work?

 Axys runs on a large memory array containing **256 positions**. Each position has its own address and value.

 The programmer's role is to manipulate this memory through different instructions in order to create various types of programs.

 > **Why would I use Axys instead of another language to learn low-level programming?**

 One of the main reasons many people avoid learning low-level programming is the difficulty that comes with it.

 Axys was designed to be the **intermediate Axis** between the transparency of low-level programming and the simplicity of high-level programming, allowing beginners to understand low-level concepts without feeling overwhelmed.

 ## How to install?

 ### 1\. Clone the repository

 Open your terminal and run:

```
git clone https://github.com/hatsunemikutoro-hash/axyslang.git
```

 ### 2\. Open the project folder

```
cd axyslang
```

 ### 3\. Build the project

```
make
```

 ### 4\. Create an `.axs` file

 For example:

```
printc "Hello, Axys!"
printc "Two Sum program!"

add 2
add 2
print

@ new line
printc 10
```

 ### 5\. Run your program

```
./axys filename.axs
```