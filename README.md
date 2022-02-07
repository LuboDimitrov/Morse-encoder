# Morse-translator
Morse translator based on the client-server model

##Instructions
###1. Clone this repository
```
$ git clone https://github.com/LuboDimitrov/Morse-translator.git
```
###2. Make sure you have gcc 8.1.0 or higher installed. Do so by typing this in your console
```
$ gcc -v
```
###3. Open the terminal from the folder you have the files in

###4. Compile the files by typing the following commands.
```
$ gcc ClientMorse.c -o ClientMorse.exe -lwsock32
```
```
$ gcc ServerMorse.c -o ServerMorse.exe -lwsock32
```

Now you should see two executable files have been generated.

###5. To run them just type their names on the console and hit enter
> I would recommend you running each executable on a separate terminal
