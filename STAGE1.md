## Stage 1

The most obvious first step to me in this project is to get a basic order book working. For now, this will include reading market orders from a .csv file and then completing it in a single-threaded approach, aiming for 100% correctness.

Pretty soon I'll need to develop some good way to test for correctness and performance, but for now the focus is getting something running.

I'm learning that real-world systems consume binary data from a stream through things like NASDAQ-ITCH. I'm going to try parsing from NASDAQ ITCH binary data for my order book simulator, since I found sample data [here](https://data.nasdaq.com/databases/NTV#anchor-totalview-itch-frequently-asked-questions). I do wonder if there's an advantage to keeping this data in its binary form and using bit manipulation, I'll have to explore. 

First step is to parse the NASDAQ ITCH data. This is just a stream of bits (coming from a file right now, can come from a socket connection later). Regardless the way the data is processed follows the same format.

I have all the message parsing done. With this, I can get my first initial benchmark: Just parsing 268.7 million messages took 21 seconds. This is obviously gonna take longer when I actually process the messages. But I'm sure the IO operations are taking up a lot of the time here.

## Stage 2

Now that the parsing is all done, I need to do something with the parsed messages.
Since I'm not interested in accounting for edge cases and more intricate market behavior, I can ignore all other types of messages except:
* Add order (A) 
* Add order w/ MPID (F)
* Execute order (E)
* Execute w/ Price (C)
* Delete order (D)
* Replace order (U)
* Cancel order (X)
