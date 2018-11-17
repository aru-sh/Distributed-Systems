#Python Process1
import socket
host=socket.gethostname()
port=2004
BUFFER_SIZE=2000
array=[]
processarray=[]

n=int(input("Enter the number of events in Process 1: "))
for j in range(n):
    array=[int(x) for x in input().split(" ")]
    array.insert(0,j+1)
    processarray.append(array)

string="1"
for x in processarray:
    for y in x:
        string+=","+str(y)
    string+="."

MESSAGE=string.encode()

print("The given process-event array is")
for i in range(n):
    print(processarray[i])
process1=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
process1.connect((host,port))

while(True):
    process1.send(MESSAGE)
    MESSAGE=input("Enter 1 to continue/Enter exit: ")

    if(MESSAGE=="exit"):
        MESSAGE="x"
        MESSAGE=MESSAGE.encode()
        process1.send(MESSAGE)
        break
    else:
        MESSAGE=str(1)
        MESSAGE=MESSAGE.encode()

process1.close()
