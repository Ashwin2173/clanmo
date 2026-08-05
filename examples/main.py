import time

def fib(n):
    if n < 2:
        return n
    return fib(n-1) + fib(n-2)

start_time = time.time()
result = fib(40)
end_time = time.time()
print(result, " ", end_time - start_time)
