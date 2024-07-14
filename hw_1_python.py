import math
import copy
import sys
import time

def euclidian_distance(point1, point2):
    return math.sqrt(sum((float(p1) - float(p2)) ** 2 for p1, p2 in zip(point1, point2)))

def update(center_sum, center_size, dim):
    for x in range(dim):
        center_sum[x] = (center_sum[x] / center_size) 
    return center_sum    

def add_vec(x, y):
    for i in range(len(x)):
        x[i] += y[i]

def cast_to_float(point):
    newpoint = []
    for x in point:
        try:
            newpoint.append( float(x)) 
        except ValueError:
            print("An error has occured")
            sys.exit(1)  # Exit the program with a non-zero status code
    return newpoint

def round_vec(vecs, dim):
    for vec in vecs:
        for i in range(dim):
            vec[i] = f"{vec[i]:.4f}"

    return vecs

    


def kmeans(K, iter, input_data):
    centeroids = []
    for i in range(K):
       line = input_data.readline()
       point = line.split("/")[0].split(",")
       point = cast_to_float(point)

       centeroids.append(point)
    dim = len(point)

    for i in range(iter):
        input_data.seek(0)  # go back to the start of the file
        centeroids_sums = [[0] * dim for _ in range(K)]
        centeroids_sizes = [0] * K
        line = input_data.readline()
        while line:
            point = line.split("/")[0].split(",")
            if len(point) < 3:
                print("problem")
            point = cast_to_float(point)
            min_dis = -1
            min_index = -1
            for i in range(len(centeroids)):
                dis = euclidian_distance(point, centeroids[i])
                if min_dis == -1:
                    min_dis = dis
                    min_index = i
                elif dis < min_dis:
                    min_dis = dis
                    min_index = i
            add_vec(centeroids_sums[min_index], point)
            centeroids_sizes[min_index]+= 1
            line = input_data.readline()
        check = True
        for  i in range (K):
            temp = copy.deepcopy(centeroids[i])
            centeroids[i] = update(centeroids_sums[i], centeroids_sizes[i], dim)
            if euclidian_distance(temp,centeroids[i])>= 0.001:
                check = False

        if check:
            
            print(round_vec(centeroids, dim))
            break

file = open('input_2.txt', 'r')

kmeans(7,100, file)
time.sleep(5)

            

        
        
        
        #line = input_data.readline()

