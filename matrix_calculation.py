#!/usr/bin/python3

import random


def my_rand():
    return (random.random() - 0.5)*10.0


class Vec:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return '({}, {}, {})'.format(self.x, self.y, self.z)

    def scaled(self, s):
        return Vec(self.x * s, self.y * s, self.z * s)

    def __add__(self, b):
        return Vec(self.x + b.x, self.y + b.y, self.z + b.z)

    @staticmethod
    def random():
        return Vec(my_rand(), my_rand(), my_rand())


def calc_r(a, b, c, u, t, s):
    return (t*u.x - s*c.x - a.x) / b.x


def calc_t(a, b, c, u, s):
    return ((-(s*c.x*b.y + a.x*b.y)/b.x + s*c.y+a.y)/(u.y - (u.x*b.y)/b.x))


def calc_s(a, b, c, u):
    A = u.y*b.x - u.x*b.y
    return (a.x*b.y*u.x*b.z/(b.x*A) - a.y*u.x*b.z/A + a.x*b.z/b.x - a.x*b.y*u.z/A + a.y*u.z*b.x/A - a.z)/(-c.x*b.y*u.x*b.z/(b.x*A) + c.y*u.x*b.z/A - c.x*b.z/b.x + c.z + c.x*b.y*u.z/A - c.y*u.z*b.x/A)

def calc_s_simple_b(a, c, u):
    A = u.x
    return (a.x*b.z/b.x - a.x*b.y*u.z/A + a.y*u.z*b.x/A - a.z)/(-c.x*b.y*u.x*b.z/(b.x*A) + c.y*u.x*b.z/A - c.x*b.z/b.x + c.z + c.x*b.y*u.z/A - c.y*u.z*b.x/A)


def main():
    u = Vec.random()  # (1, 2, 3)
    b = Vec.random()  # (2, 2, 1)
    c = Vec.random()  # (-1, 1, 2)
    t = my_rand()  # 2
    r = my_rand()  # 1
    s = my_rand()  # -2

    a = u.scaled(t) + b.scaled(-r) + c.scaled(-s)
    # print(a)
    # print('r = ', r)
    # print('t = ', t)
    # print('s = ', s)

    calculated_s = calc_s(a, b, c, u)
    diff_s = abs(calculated_s - s)

    print('calc s: ', calculated_s)
    print('real s: ', s)
    print('diff: ', diff_s)

    calculated_t = calc_t(a, b, c, u, calculated_s)
    diff_t = abs(calculated_t - t)

    print('calc t: ', calculated_t)
    print('real t: ', t)
    print('diff: ', diff_t)

    calculated_r = calc_r(a, b, c, u, calculated_t, calculated_s)
    diff_r = abs(calculated_r - r)

    print('calc r: ', calculated_r)
    print('real r: ', r)
    print('diff: ', diff_r)

    if sum((diff_s, diff_t, diff_r)) < 0.000001:
        print('success')
    else:
        print('error')

    # print('calc -a.z: ',
    #     (((-(s*c.x*b.y + a.x*b.y)/b.x + s*c.y+a.y)/(u.y - (u.x*b.y)/b.x))*u.x*b.z)/b.x - (s*c.x*b.z)/b.x - (a.x*b.z)/b.x + s*c.z - ((-(s*c.x*b.y + a.x*b.y)/b.x + s*c.y+a.y)/(u.y - (u.x*b.y)/b.x))*u.z
    # )
    # print('calc -a.z: ',
    #     (-s*c.x*b.y*u.x*b.z/b.x - a.x*b.y*u.x*b.z/b.x + s*c.y*u.x*b.z + a.y*u.x*b.z)/(A*b.x) - (s*c.x*b.z)/b.x - (a.x*b.z)/b.x + s*c.z - ((-(s*c.x*b.y + a.x*b.y)/b.x + s*c.y+a.y)/(u.y - (u.x*b.y)/b.x))*u.z
    # )
    # print('calc -a.z: ',
    #     -s*c.x*b.y*u.x*b.z/(b.x*b.x*A) - a.x*b.y*u.x*b.z/(b.x*b.x*A) + s*c.y*u.x*b.z/(b.x*A) + a.y*u.x*b.z/(b.x*A) - s*c.x*b.z/b.x - a.x*b.z/b.x + s*c.z - ((-(s*c.x*b.y + a.x*b.y)/b.x + s*c.y+a.y)/(u.y - (u.x*b.y)/b.x))*u.z
    # )
    # print('calc -a.z: ',
    #     -s*c.x*b.y*u.x*b.z/(b.x*b.x*A) - a.x*b.y*u.x*b.z/(b.x*b.x*A) + s*c.y*u.x*b.z/(b.x*A) + a.y*u.x*b.z/(b.x*A) - s*c.x*b.z/b.x - a.x*b.z/b.x + s*c.z - ((-(s*c.x*b.y + a.x*b.y)/b.x + s*c.y + a.y)*u.z/(u.y - (u.x*b.y)/b.x))
    # )
    # print('calc -a.z: ',
    #     -s*c.x*b.y*u.x*b.z/(b.x*b.x*A) - a.x*b.y*u.x*b.z/(b.x*b.x*A) + s*c.y*u.x*b.z/(b.x*A) + a.y*u.x*b.z/(b.x*A) - s*c.x*b.z/b.x - a.x*b.z/b.x + s*c.z - ((-(s*c.x*b.y + a.x*b.y)/b.x + s*c.y + a.y)*u.z/(u.y - (u.x*b.y)/b.x))
    # )
    # print('calc -a.z: ',
    #     -s*c.x*b.y*u.x*b.z/(b.x*b.x*A) - a.x*b.y*u.x*b.z/(b.x*b.x*A) + s*c.y*u.x*b.z/(b.x*A) + a.y*u.x*b.z/(b.x*A) - s*c.x*b.z/b.x - a.x*b.z/b.x + s*c.z - (-s*c.x*b.y/b.x - a.x*b.y/b.x + s*c.y + a.y)*u.z/(u.y - (u.x*b.y)/b.x)
    # )
    # print('calc -a.z: ',
    #     -s*c.x*b.y*u.x*b.z/(b.x*b.x*A) - a.x*b.y*u.x*b.z/(b.x*b.x*A) + s*c.y*u.x*b.z/(b.x*A) + a.y*u.x*b.z/(b.x*A) - s*c.x*b.z/b.x - a.x*b.z/b.x + s*c.z + (s*c.x*b.y*u.z/b.x + a.x*b.y*u.z/b.x - s*c.y*u.z - a.y*u.z)/(u.y - (u.x*b.y)/b.x)
    # )
    # print('calc -a.z: ',
    #     -s*c.x*b.y*u.x*b.z/(b.x*b.x*A) - a.x*b.y*u.x*b.z/(b.x*b.x*A) + s*c.y*u.x*b.z/(b.x*A) + a.y*u.x*b.z/(b.x*A) - s*c.x*b.z/b.x - a.x*b.z/b.x + s*c.z + s*c.x*b.y*u.z/(b.x*A) + a.x*b.y*u.z/(b.x*A) - s*c.y*u.z/A - a.y*u.z/A
    # )
    # print('calc -a.z: ',
    #     s*(-c.x*b.y*u.x*b.z/(b.x*b.x*A) + c.y*u.x*b.z/(b.x*A) - c.x*b.z/b.x + c.z + c.x*b.y*u.z/(b.x*A) - c.y*u.z/A) - a.x*b.y*u.x*b.z/(b.x*b.x*A) + a.y*u.x*b.z/(b.x*A) - a.x*b.z/b.x + a.x*b.y*u.z/(b.x*A) - a.y*u.z/A
    # )
    # print('0: ',
    #     s*(-c.x*b.y*u.x*b.z/(b.x*b.x*A) + c.y*u.x*b.z/(b.x*A) - c.x*b.z/b.x + c.z + c.x*b.y*u.z/(b.x*A) - c.y*u.z/A) - a.x*b.y*u.x*b.z/(b.x*b.x*A) + a.y*u.x*b.z/(b.x*A) - a.x*b.z/b.x + a.x*b.y*u.z/(b.x*A) - a.y*u.z/A + a.z
    # )
    # print(
    #     s*(-c.x*b.y*u.x*b.z/(b.x*b.x*A) + c.y*u.x*b.z/(b.x*A) - c.x*b.z/b.x + c.z + c.x*b.y*u.z/(b.x*A) - c.y*u.z/A) == a.x*b.y*u.x*b.z/(b.x*b.x*A) - a.y*u.x*b.z/(b.x*A) + a.x*b.z/b.x - a.x*b.y*u.z/(b.x*A) + a.y*u.z/A - a.z
    # )
    # print(
    #     s == (a.x*b.y*u.x*b.z/(b.x*b.x*A) - a.y*u.x*b.z/(b.x*A) + a.x*b.z/b.x - a.x*b.y*u.z/(b.x*A) + a.y*u.z/A - a.z)/(-c.x*b.y*u.x*b.z/(b.x*b.x*A) + c.y*u.x*b.z/(b.x*A) - c.x*b.z/b.x + c.z + c.x*b.y*u.z/(b.x*A) - c.y*u.z/A)
    # )




if __name__ == '__main__':
    main()
