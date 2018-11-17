#Python Process0
import socket
host=socket.gethostname()
port=2004
BUFFER_SIZE=2000
array=[]
processarray=[]

n=int(input("Enter the number of events in Process 0: "))
for j in range(n):
    array=[int(x) for x in input().split(" ")]
    array.insert(0,j+1)
    processarray.append(array)

string="0"
for x in processarray:
    for y in x:
        string+=","+str(y)
    string+="."

MESSAGE=string.encode()

print("The given process-event array is")
for i in range(n):
    print(processarray[i])
process0=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
process0.connect((host,port))

while(True):
    process0.send(MESSAGE)
    MESSAGE=input("Enter 1 to continue/Enter exit: ")

    if(MESSAGE=="exit"):
        MESSAGE="x"
        MESSAGE=MESSAGE.encode()
        process0.send(MESSAGE)
        break
    else:
        MESSAGE=str(0)
        MESSAGE=MESSAGE.encode()

process0.close()


