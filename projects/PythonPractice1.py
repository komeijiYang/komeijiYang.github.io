# -*- coding: utf-8 -*-
import math

L = ["Bart", "Lisa", "Adam"]
for name in L:
    print("hello, %s" % (name))

d = {"Michael": 95, "Bob": 75, "Tracy": 85}
print(d["Michael"])
d.pop("Bob")

s = {1, 2, 3}
s.add(4)
s.remove(4)

t1 = (1, 2, 3)
t2 = (1, [2, 3])

# s1 = {t1, 1, 2}
# print(s1)
# s2 = {t2}
# print(s2)

d1 = {t1: 1}
# d2 = {t2: 2}   # wrong
# print(d1, d2)

n1 = 255
n2 = 1000
print(hex(n1), hex(n2))


def my_abs(x):
    if not isinstance(x, (int, float)):
        raise TypeError("bad operand type")
    if x >= 0:
        return x
    else:
        return -x


print(my_abs(-99))
a = my_abs
a(-99)


def nop():
    pass


def quadratic(a, b, c):
    if (
        not isinstance(a, (int, float))
        or not isinstance(b, (int, float))
        or not isinstance(c, (int, float))
    ):
        raise TypeError("bad operand type")
    delta = b * b - 4 * a * c
    if delta < 0:
        raise ValueError("This equation has no real solution")
    sqrt_delta = math.sqrt(delta)
    return ((-b + sqrt_delta) / (2 * a), (-b - sqrt_delta) / (2 * a))


print("quadratic(2, 3, 1) =", quadratic(2, 3, 1))
print("quadratic(1, 3, -4) =", quadratic(1, 3, -4))

if quadratic(2, 3, 1) != (-0.5, -1.0):
    print("failed")
elif quadratic(1, 3, -4) != (1.0, -4.0):
    print("failed")
else:
    print("succeeded")


def add_end(L=None):
    if L is None:
        L = []
    L.append("END")
    return L
def calc(*numbers):
    sum=0
    for n in numbers:
        sum+=n*n
    return sum
nums=[1,2,3]
calc(*nums)