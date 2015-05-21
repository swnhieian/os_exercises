import threading
import time
import random as rd
readerNum = 0
pno = 0;
xno = 0;
wait = []

def reader():
    global readerNum
    global pno, xno

    reqcon.acquire()
    myno = xno
    xno += 1
    while myno != pno:
        reqcon.wait()

    modify.acquire()
    if readerNum == 0:
        rwsem.acquire()
    readerNum += 1
    modify.release()
    pno += 1
    reqcon.notify()
    reqcon.release()

    printlock.acquire()
    print myno, "reader reading"
    printlock.release()

    time.sleep(1)
    
    printlock.acquire()
    print myno, "reader done"
    printlock.release()

    modify.acquire()
    readerNum -= 1
    if readerNum == 0:
        rwsem.release()
    modify.release()

def writer():

    global readerNum
    global pno, xno
    reqcon.acquire()
    myno = xno
    xno += 1
    while myno != pno:
        reqcon.wait()
    rwsem.acquire()
    pno += 1
    reqcon.notify()
    reqcon.release()

    printlock.acquire()
    print myno, "writer writing"
    printlock.release()
    time.sleep(1)
    printlock.acquire()
    print myno, "writer done"
    printlock.release()

    rwsem.release()
    #reqcon.release()


if __name__ == '__main__':
    queue = []
    rwsem = threading.Semaphore(1)
    reqcon = threading.Condition()
    modify = threading.Semaphore(1)
    printlock = threading.Semaphore(1)
    '''
    seq = []
    for i in range(10):
        if (rd.randint(0,1) == 0):
            seq.append("r")
        else:
            seq.append("w")
    '''
    seq = "rrwrrwrrwrwrwrwwwrrrrwwr"
    print seq

    for i in seq:
        if i == "r":
            queue.append(threading.Thread(target=reader))
        else:
            queue.append(threading.Thread(target=writer))

    for i in queue:
        i.start()
        time.sleep(0.1)
    for i in queue:
        i.join()


