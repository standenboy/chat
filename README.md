# string chat 
a very simple chat protocol writen in pure c

run the server and then any number of clients (less than the number MAXCLIENTS defined in server.c)

they call all message eachother and use the following commands

`/nick` --- will set a nick name that is append on to the start of each message

`/exit` --- will safely disconnect the user from the server (ctrl+c does this too)

# todo
message history

client config file

make the client act like sic, which takes input and output from a file (allows for larger clients to be writen in any language)
