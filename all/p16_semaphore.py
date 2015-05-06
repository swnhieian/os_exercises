import threading
totnum = 10
whitenum = totnum
blacknum = totnum

def pick_black():
    global blacknum    
    while (blacknum >0):
        black.acquire()
        blacknum -= 1
        print "pick one black,rest:%d" %blacknum
        white.release()

def pick_white():
    global whitenum    
    while (whitenum >0):
        white.acquire()
        whitenum -= 1
        print "pick one white,rest:%d" %whitenum
        black.release()

if __name__ == '__main__':    
    black = threading.Semaphore(1)
    white = threading.Semaphore(0)
    thread1 = threading.Thread(target=pick_black)
    thread2 = threading.Thread(target=pick_white)
    thread1.start()
    thread2.start()
    thread1.join()
    thread2.join()

