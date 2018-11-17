import socket
from threading import Thread
from socketserver import ThreadingMixIn
import sys

processarray=[]

def converttoarray(string):
        stripped=string.replace(".,",".")
        newarray=[x.split(',') for x in stripped.split('.')]
        for x in range(len(newarray)):
                for y in range(len(newarray[x])):
                            newarray[x][y]=int(newarray[x][y])
        return newarray

def event(processarray, n):
        print("I have received request for" +str(n))
        print("I have this data")
        print(processarray)
        eventarray=processarray[int(n)]
        for i in eventarray:
                time=i[0]
                eventtype=i[1]
                pid=i[2]
                eid=[3]

                if(eventtype==0):
                        continue

                else:
                    if(eventtype==1):
                            ptr=processarray[pid][eid]
                            lenofarray=len(processarray[pid])

                            if((time+1)>ptr[0]):
                                ptr[0]=time+1
                                for j in range(eid+1,lenofarray):
                                        processarray[pid][j][0]=(processarray[pid][j-1][0])+1

        return processarray

class ClientThread(Thread):
        def __init__(self,ip,port):
            Thread.__init__(self)
            self.ip=ip
            self.port=port

        def run(self):
                while True:
                        global processarray
                        data=conn.recv(2048)
                        data=data.decode()
                        if(len(data)==1):
                            if(data=="x"):
                                    print("\n\nThe final time of process are")
                                    for z in range(3):
                                        print("\nProcess"+str(z),end=': ')
                                        for x in processarray[z]:
                                            print(x[0],end=' ')
                                        print("\n")
                            else:
                                n=int(data[0])
                                print("the data to be sent to event "+str(n))
                                processarray=event(processarray,n)

                        else:
                            p=data[0]
                            print("\nData received from Process "+p)
                            array=converttoarray(data[2:-1])
                            print("Process data received: ",array)
                            processarray.append(array)
                            print(processarray)

TCP_IP='0.0.0.0'
TCP_PORT=2004
BUFFER_SIZE=20
tcpServer=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpServer.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
tcpServer.bind((TCP_IP, TCP_PORT))
threads=[]

while True:
        tcpServer.listen(4)
        print("Waiting for the process to send data!")
        (conn,(ip,port))=tcpServer.accept()
        newthread=ClientThread(ip,port)
        newthread.start()
        threads.append(newthread)

for t in threads:
        t.join()
