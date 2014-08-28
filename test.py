import urllib
import time
import thread
from threading import Thread
from multiprocessing import Pool, Process
import os

NUM_ITERATIONS_PER_THREAD = 5
NUM_THREADS = 2
NUM_ITERATIONS = NUM_ITERATIONS_PER_THREAD * NUM_THREADS

pool = Pool(processes=NUM_THREADS)


# class URL(object):
#     url = 'http://localhost:1337/toucan'
#     counter = 0

#     @classmethod
#     def next(self):
#         self.counter += 1
# return self.url  # + str(self.counter)


# def req():
#     url = URL.next()
#     thread_num = URL.counter
# command = 'curl --silent -XGET "' + url + '"'
# print "COMMAND -> %s" % command

#     def run():
#         for i in xrange(0, NUM_ITERATIONS_PER_THREAD):
#             if (i % 100) == 0:
#                 print "THREAD %d: %d" % (thread_num, i)
#             os.system('curl --silent -XGET "http://localhost:1337/toucan" > /dev/null')
# call('./test_curl.sh')
# urllib.urlopen(url)
#     return run


def go(i):
    os.system('curl --silent -XGET "http://localhost:1337/toucan" > /dev/null')

if __name__ == '__main__':
    # threads = []
    # for t in range(0, NUM_THREADS):
    #     threads.append(Process(target=req()))

    # MyThread.join()
    # requests.get
    # first_thread = threads[0]
    t1 = time.time()
    pool.map(go, range(NUM_ITERATIONS))
    # for t in threads:
    #     t.start()
    # first_thread.join()
    total_time = time.time() - t1

    avg_time_per_request = float(total_time) / NUM_ITERATIONS  # seconds
    requests_per_second = 1 / avg_time_per_request

    print "REQUESTS PER SECOND: %f" % requests_per_second
