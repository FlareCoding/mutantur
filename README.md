# mutantur

Metamorphism is a process of an application rewriting itself, modifying its own code. It is one of the things most malware developers aspire to implement, and it's really difficult to pull off pure metamorphism. Even though metamorphism is a hot topic in malware development, the ability of programs to modify themselves can be applied in a lot of useful ways as well such as a neural network evolving or a system protecting itself by rewriting vulnerable parts of itself.

**mutantur** is by no means a framework that will magically rewrite your programs whose code will look entirely different from the original, while still
preserving the functionality, but it can help mutate the program enough to modify the signature and the hash of applications. It currently only works on Windows, 
but it will most likely be expanded to linux and OSX in the near future.

The way **mutantur** achieves code mutation is by manually mutating each function that the user passes the address of.<br/>
It makes use of the 16 byte alignment, and if there are padding bytes available, mutantur erases them, moves the actual function content to the end of the
function's 16 byte region, and fills the preceding area with harmless instructions that are selected at random at each iteration.

## **Important Remarks:**

In order for this to work, all optimizations must be turned off, especially the optimization for file size since it often removes the padding<br/>
which makes it possible to mutate the function's body.<br/><br/>

## Example (demo.cpp)

Let's say our program's main code is in the function ```add```:
```
int add(int x, int y)
{
    int z = x + y;
    return z;
}
```

Let's make our program modify the function ```add``` only if it was launched with command line arguments,<br/>
otherwise we will print out the contents of the function and test the function with some sample input:
```
int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        mutantur::mutant mutant("demo.exe");
        mutant.mutate((unsigned char*)&add, mutantur::utils::calculate_function_size((unsigned char*)&add));
        mutant.finalize_mutation("demo_clone.exe");

        auto current_dir = std::filesystem::current_path();
        auto clone = current_dir.string() + "\\demo_clone.exe";
        auto target = current_dir.string() + "\\demo.exe";
        mutantur::utils::merge_clone_file(0, clone.c_str(), target.c_str());
    }

    std::cout << "Function Code:\n";
    std::cout << mutantur::utils::hex_dump((unsigned char*)&add, mutantur::utils::calculate_function_size((unsigned char*)&add)) << "\n";
        
    std::cout << "Proof of Work:\n4 + 10 = " << add(4, 10) << "\n\n";
    system("pause");

    return 0;
}
```

Alright, there is a lot going on here, let's break it down.<br/>
First let's discuss the scenario below the ```if``` statement that gets executed when the program is launched with no command line arguments.
```
std::cout << "Function Code:\n";
std::cout << mutantur::utils::hex_dump((unsigned char*)&add, mutantur::utils::calculate_function_size((unsigned char*)&add)) << "\n";
```

The ```hex_dump``` function takes in the address of the function and the known length of the function in bytes.<br/>
We already know the address of our ```add``` function, so we have to calculate the length of it using ```calculate_function_size``` function from the utils namespace.<br/>
This will give the following output:
```
Function Code:
0x000000: 89 54 24 10 89 4c 24 08 48 83 ec 18 8b 44 24 28  .T$..L$.H....D$(
0x000010: 8b 4c 24 20 03 c8 8b c1 89 04 24 8b 04 24 48 83  .L$ ......$..$H.
0x000020: c4 18 c3 cc cc                                   .....
```

Great, we can see all the contents of our ```add``` function and we can see that it's 37 bytes long.<br/>
The last thing we do here is just printing the output of a sample usage of ```add``` with parameters **4** and **10**.<br/> 

Code:

```
std::cout << "Proof of Work:\n4 + 10 = " << add(4, 10) << "\n\n";
```
Result:

```
Proof of Work:
4 + 10 = 14
```

Now that we are done with the standard code here, let's talk about what happens when we launch the demo with a command line argument, like this: ```.\demo.exe 1```.<br/><br/>

First we create a ***mutant*** object and pass in the name of the executable that we want to mutate.
```
mutantur::mutant mutant("demo.exe");
```

This will invoke the ```mutant```'s constructor and read the contents of the executable into an array.


Next we call call the ```mutate()``` function to actually mutate it, and once again the function takes in the address of the function and the known length of the function in bytes.

```
mutant.mutate((unsigned char*)&add, mutantur::utils::calculate_function_size((unsigned char*)&add));
```

At this point, the function's content will be mutated only in the byte array that the executable was read into.<br/>
In order to actually do something with the modification, we must call:

```
mutant.finalize_mutation("demo_clone.exe");
```
We pass in the name of the mutated executable that is created. ```demo_clone.exe``` is basically a mutated clone of our ```demo.exe```.

The next lines of code are optional, but are good if you want your program to be self-modifying.
```
auto current_dir = std::filesystem::current_path();
auto clone = current_dir.string() + "\\demo_clone.exe";
auto target = current_dir.string() + "\\demo.exe";
mutantur::utils::merge_clone_file(0, clone.c_str(), target.c_str());
```

We set ***clone** to the path of the mutated clone, and ***target*** to the path of the current executable.<br/>
Next we call ```merge_clone_file``` passing in the exit code, mutated clone's path, and the existing executable's path.<br/>
This will temporarily create a batch file that will delete the current executable, and rename the clone to the current executable's name.<br/>
**Import Note:** this function will call ```exit(exit_code)```, so be sure that no other code is planned on being executed after this.<br/>
This is the reason why there is no ```else``` block after the ```if```.<br/>


## Proof of Work:

**Iteration #1**<br/>
**Command: ```.\demo.exe```**<br/>
**Output:**<br/>
```
Function Code:
0x000000: 89 54 24 10 89 4c 24 08 48 83 ec 18 8b 44 24 28  .T$..L$.H....D$(
0x000010: 8b 4c 24 20 03 c8 8b c1 89 04 24 8b 04 24 48 83  .L$ ......$..$H.
0x000020: c4 18 c3 cc cc                                   .....

Proof of Work:
4 + 10 = 14
```
**Mutation: ***None*****<br/>
**File Hash: ***626DB9B0FFB1B35CAC4E8EE09FD6ABA6B171E1E753769CFCA761FE8B6A1B17D5*****<br/><br/>

***Mutating with ```.\demo.exe 1```***<br/><br/>

**Iteration #2**<br/>
**Command: ```.\demo.exe```**<br/>
**Output:**<br/>
```
Function Code:
0x000000: b4 09 48 89 e4 48 89 ed 48 89 c0 89 54 24 10 89  ..H..H..H...T$..
0x000010: 4c 24 08 48 83 ec 18 8b 44 24 28 8b 4c 24 20 03  L$.H....D$(.L$ .
0x000020: c8 8b c1 89 04 24 8b 04 24 48 83 c4 18 c3 cc cc  .....$..$H......

Proof of Work:
4 + 10 = 14
```
**Mutation: ***b4 09 48 89 e4 48 89 ed 48 89 c0*****<br/>
**File Hash: ***C6D8E8EA819B150C1E964B9D00A2EBBD246EE555AEEF8614E97760700898AF01*****<br/>

***Mutating with ```.\demo.exe 1```***<br/><br/>

**Iteration #3**<br/>
**Command: ```.\demo.exe```**<br/>
**Output:**<br/>
```
Function Code:
0x000000: b4 09 48 89 c0 48 89 db 4d 89 ff 89 54 24 10 89  ..H..H..M...T$..
0x000010: 4c 24 08 48 83 ec 18 8b 44 24 28 8b 4c 24 20 03  L$.H....D$(.L$ .
0x000020: c8 8b c1 89 04 24 8b 04 24 48 83 c4 18 c3 cc cc  .....$..$H......

Proof of Work:
4 + 10 = 14
```
**Mutation: ***b4 09 48 89 c0 48 89 db 4d 89 ff*****<br/>
**File Hash: ***59E657F9A6A6E726F86E01D56D68F2847DE9DED3C93668D5D6C3EE6699E8D336*****<br/>

***Mutating with ```.\demo.exe 1```***<br/><br/>

**Iteration #4**<br/>
**Command: ```.\demo.exe```**<br/>
**Output:**<br/>
```
Function Code:
0x000000: b4 09 4d 89 ff 4d 89 f6 4d 89 f6 89 54 24 10 89  ..M..M..M...T$..
0x000010: 4c 24 08 48 83 ec 18 8b 44 24 28 8b 4c 24 20 03  L$.H....D$(.L$ .
0x000020: c8 8b c1 89 04 24 8b 04 24 48 83 c4 18 c3 cc cc  .....$..$H......

Proof of Work:
4 + 10 = 14
```
**Mutation: ***b4 09 4d 89 ff 4d 89 f6 4d 89 f6*****<br/>
**File Hash: ***A997E69A3D3404B90EB1AA86658F08AC0F1CBA7542431C3E96341BC2C8DE09F7*****<br/><br/>


As you can clearly see, on every iteration after mutating the mutation body was different each time and the file hash was changed.

