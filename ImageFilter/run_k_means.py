#Jianhua Li
#li001665
from image_utils import *
from k_means import *

filename = input("Please input the filename you want to load: ")
k = int(input("Please input the number of colors: "))
new_filename = input("Please input the new filename you want to output: ")

image = read_ppm(filename)
means, assignment = K_means(image, k)
width, height = get_width_height(image)
for i in range(width):
    for j in range(height):
       assignment[i][j] = means[assignment[i][j]]
save_ppm(new_filename, assignment)


