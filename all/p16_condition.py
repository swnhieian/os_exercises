import threading
totnum = 10
whitenum = totnum
blacknum = totnum

def pick_black():
    global blacknum  
    global turn 
    while (blacknum >0):
        cond.acquire()
        if (turn == 1):
            cond.wait()
        blacknum -= 1
        print "pick one black,rest:%d" %blacknum
        turn = 1
        cond.notify()
        cond.release()

def pick_white():
    global whitenum
    global turn
    while (whitenum >0):
        cond.acquire()
        if (turn == 0):
            cond.wait()
        whitenum -= 1
        print "pick one white,rest:%d" %whitenum
        turn = 0
        cond.notify()
        cond.release()

if __name__ == '__main__':    
    cond = threading.Condition()
    turn = 0
    thread1 = threading.Thread(target=pick_black)
    thread2 = threading.Thread(target=pick_white)
    thread1.start()
    thread2.start()
    thread1.join()
    thread2.join()

