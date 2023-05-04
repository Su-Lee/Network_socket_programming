# port number : 9090 

## 1. makefile
In the terminal, type "make" to run the code. (you can type "make clean" after done using.) 

## 2. communication between server and client 
When server starts its connection by typing "./server",
the client can connect to the server by typing "./client".
If connection is established, you can start communicating to each other.
Just like tcp connection, you can start communicating from the client side. The conversation goes by client entering message, and server repling back. The conversation is repeated until someone ends the connection. 
You can end the connection properly by typing "exit" in the server side. (ctrl + c also works for both client and server side.)

## 3. connecting to the website
While the server connection is still open, you can connect to the website by typing "http://127.0.0.1:9090" or "localhost:9090" in the web browser. 

## 4. end
To end, you have to type (ctrl + c) in the server side terminal, and make clean. 



