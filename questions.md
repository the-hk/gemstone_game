# Gemstone Mining Game 
<img src="https://www.marengocave.com/assets/gemstoneB_800x534-30.jpeg" />
A mother has a gemstone mining business. She needs help with sifting for gemstones from her children. She gives a bucket of dirt to each of her children. 

Price list:
- diamond, 3500$ <img src="https://i.dlpng.com/static/png/5199274-diamond-png-download-10241024-free-transparent-minecraft-png-minecraft-diamond-png-900_900_preview.png" width="50" />
- ruby, 50$ <img src="https://vignette.wikia.nocookie.net/minecraftfanfictions/images/a/a9/Ruby.png/revision/latest?cb=20120803041131" width="50" />
- sapphire, 1200$ <img src="https://www.tynker.com/minecraft/api/item?id=5a501d3e76f29325548b468d&w=400&h=400&width=400&height=400&mode=contain&format=jpg&quality=75&cache=max&v=1515199806" width="50" />
- emerald 800$ <img src="https://p7.hiclipart.com/preview/867/644/1013/minecraft-roblox-diamond-video-game-emerald-stone-png-transparent-images.jpg" width="50" />

In this assignment, 
- a bucket of dirt is represented by a text file which is filled with random characters, the dirt<img src="https://emojipedia-us.s3.dualstack.us-west-1.amazonaws.com/thumbs/160/google/263/bucket_1faa3.png" width="30" />. 
- The gemstones are represented by their first letter (lowercase). An example file content is given below:


**s**uff**r**kly7acxjvg8**er**06q5wxk6pgqqywq4q**e**y0eafdwc9**e**4w9**e**

In this sample bucket there are **1 sapphire, 2 rubies, 4 emeralds** which makes it worth of 1200+50x2+800x4=4500$  

- The mother is represented by the main process, children are the child processes. 
## Constraints
- The children get tired after checking out 24 characters in the text file and go to sleep for a random amount between 500-800msec.
- All of the displayed messages in this assingment must start with the process id of the message owner such as:

**[PID:4256] A new child has been created with PID: 5738.** 

- Every action carried out by either children or the parent will be displayed such as:

**[PID:5738] Tired. Going to sleep for 712msec.**

- You are not allowed to use any highlevel libraries or shell commands in your codes such as `grep`. You need to implement all of the answers using low level C functions such as open, read, fprintf, sprintf, etc. But ofcourse, for some tasks we need to use system libraries such as creating a pipe, getting the process id etc. If you are really in doubt about a specific function, please share your question in Canvas discussions.

- Before exiting the program, clean up your resources shm-unlink, munmap, close, free, etc.

- Do not use the word *result* in your output messages until the end. This is a special keyword I checkon it in the Makefile to evaluate your final result. 
## Notes
- We haven't yet learned syncronization, that is, locks, semaphores, etc. You do not need to worry about race conditions in this assignment.

# Part 1
You are given N text files (buckets) filled with random characters (dirt). 
- The main process (the mother) will create N children.
- She will printout the process IDs of the created child.
- Each child will take its text file and start reading the text file **one character at a time**. 
- The bucket worths 0$ in the beginning. 
- When the child finds a gem, she/he increases the bucket value by the amount of the found gemstone.
- The child also prints out the found gem and her/his process id.
- As soon as the end of the file is reached, they return their bucket values to the mother.
- When all the children are done, **Mother adds up the values of all buckets and displays the result** in the following format:

**[PID:4256] Result 7250**


# Part 2
This time, the children will not calculate the bucket values. **Only the mother will do the bucket value calculation**. Children will only **send the gemstone to the mother as soon as it is found**.

## 2.1) Message passing
Communication from child to parent will be done via message passing.

## 2.2) Shared memory
Communication from child to parent will be done via a shared memory.

# Part 3
There will be only one child. The communication between parent and child will be full-duplex (double way).
- If the bucket value exceeds 200, The parent will send a message to child asking her/him to exit.

# Help
- You can run ```make``` to build,
- You can run ```make clean``` to clean executables,
- You can run ```make test1``` to test your solution1 (test2 for sol2 ..., or ```make tests``` for all),
- You can run ```make generate``` to generate text files. This one always generates the same text file. You can use it for debug purposes.,
- You can run ```make generate_with_random_seeds``` to generate **random** text files at each run,

## Shell commands
In linux command line, we can checkout the content of a bucket:

```Shell
grep -o [dser] 0.txt | sort | uniq -c 
```  

Output:
      8 r
      1 s

With total worth:

```Shell
grep -o [dser] 1.txt | sort | uniq -c | awk '{if ($2 == "d") print $2, $1, value=$1*3500;  else if ($2 == "s") print $2, $1, value=$1*1200 ; else if ($2 == "e") print $2, $1, value=$1*800; else if ($2 == "r") print $2, $1, value=$1*50; total+=value} END {print "total", total}' 
```   

Output:  
e 1 800  
r 3 150  
total 950  

Regex with PCRE (Matches the number after result)

```Shell
grep -ioP "result\s*\K\d+"
```

