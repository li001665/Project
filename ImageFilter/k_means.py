#Jianhua Li
#li001665
import random
from image_utils import *

def initial_guess(k):
    means = []
    for i in range(k):
        means += [random_color()]
    return means
    
def update_assignments(image, means):
    max_val = 510
    min_index = 0 # the largest distance is 255*(3**0.5), which is smaller than 510
    assignments_1 = []
    assignments = []
    num = len(means)
    width, height = get_width_height(image)
    for i in range(width):
        for j in range(height):
            for n in range(num):
                distance = ((image[i][j][0] - means[n][0]) ** 2 + (image[i][j][1] - means[n][1]) **2 + (image[i][j][2] - means[n][2]) ** 2) ** 0.5
                if distance < max_val:
                    max_val = distance
                    min_index = n
            assignments_1 += [min_index]
            max_val = 510
            min_index = 0
        assignments += [assignments_1]
        assignments_1 = []
    return assignments

def update_means(image, assignments, k):
    width, height = get_width_height(image)
    extract_lst = []
    means = []
    color1 = 0
    color2 = 0
    color3 = 0
    for i in range(k):
        means += [0]
    for i in range(k):
        for j in range(width):
            for h in range(height):
                if assignments[j][h] == i:
                    extract_lst += [image[j][h]]
        n = 0
        for pixel in extract_lst:
             color1 += pixel[0]
             color2 += pixel[1]
             color3 += pixel[2]
             n += 1
        if n == 0:
            means[i] = (0, 0, 0)
        else:
            means[i] = (round(color1/n), round(color2/n), round(color3/n))
        color1 = 0
        color2 = 0
        color3 = 0
        extract_lst = []
    return means
             
def K_means(image, k):
    means = initial_guess(k)
    assignments = update_assignments(image, means)
    means = update_means(image, assignments, k)
    assignments_2 = update_assignments(image, means)
    while assignments_2 != assignments:
        means = update_means(image, assignments_2, k)
        assignments = assignments_2
        assignments_2 = update_assignments(image, means) 
    return (means, assignments_2)
       
        
        


                    
                    
                
                
                
    

