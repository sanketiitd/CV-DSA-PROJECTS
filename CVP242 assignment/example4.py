import pandas as pd
import numpy as np
from collections import Counter
import operator
import math
import matplotlib.pyplot as plt


def extract_start_end_points(adjacency_matrix):
    n = len(adjacency_matrix)
    start_points = []
    end_points = []
    for i in range(n):
        for j in range(i+1, n):  # Start j from i
            if adjacency_matrix[i][j] == 1:
                start_points.append(chr(ord('A') + i))
                end_points.append(chr(ord('A') + j))
    return start_points, end_points 

n = 8
node_matrix = [(0, 0), (17, 18), (17, 0), (34, 18), (34, 0), (51, 18), (51, 0), (68, 10)]
constraints = [(1, 0), (0, 1), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 1)]
nodal_loads = [(0, 0), (10, 0), (0, -10), (0, 50), (100, -10), (0, 0), (0, -10), (0, 0)]
adjacency_matrix = [
    [1, 1, 1, 0, 0, 0, 0, 0],
    [1, 1, 1, 1, 1, 0, 0, 0],
    [1, 1, 1, 0, 1, 0, 0, 0],
    [0, 1, 0, 0, 1, 1, 0, 0],
    [0, 1, 1, 1, 0, 1, 1, 0],
    [0, 0, 0, 1, 1, 0, 1, 1],
    [0, 0, 0, 0, 1, 1, 0, 1],
    [0, 0, 0, 0, 0, 1, 1, 1]
]

bars_list = np.array([chr(ord('A') + i) for i in range(n)])
X_coordinates_list = [coord[0] for coord in node_matrix]
Y_coordinates_list = [coord[1] for coord in node_matrix]
X_reactions_list = [constraint[0] for constraint in constraints]
Y_reactions_list = [constraint[1] for constraint in constraints]
FX_forces_list = [load[0] for load in nodal_loads]
FY_forces_list = [load[1] for load in nodal_loads]



start_points_list, end_points_list = extract_start_end_points(adjacency_matrix)

# print("Bars List:", bars_list)
# print("X Coordinates List:", X_coordinates_list)
# print("Y Coordinates List:", Y_coordinates_list)
# print("X Reactions List:", X_reactions_list)
# print("Y Reactions List:", Y_reactions_list)
# print("FX Forces List:", FX_forces_list)
# print("FY Forces List:", FY_forces_list)
# print("Start Points List:", start_points_list)
# print("End Points List:", end_points_list)


X_index_list = []
for i in range(len(X_reactions_list)):
  if X_reactions_list[i] == 1:
    X_index_list.append(i)

Y_index_list = []
for i in range(len(Y_reactions_list)):
  if Y_reactions_list[i] == 1:
    Y_index_list.append(i)

# Equations list
if (len(X_index_list)) == 1:  # 2 Rx and 1 Ry
  x_eq_list = [1, 0, 0]
  y_eq_list = [0, 1, 1]
else:  # 1 Rx and 1 Ry
  x_eq_list = [1, 1, 0]
  y_eq_list = [0, 0, 1]

X_values_list = [-force for force in FX_forces_list]  # Forces in the x-direction on the other side of the equation
Y_values_list = [-force for force in FY_forces_list]  # Forces in the y-direction on the other side of the equation

momentum_eq_list = []  # List to save Momentum equation

for i in X_index_list:  # Reaction in X times Y distance to joint A: (0,0)
  momentum_eq_list.append(-X_reactions_list[i] * Y_coordinates_list[i])

for j in Y_index_list:  # Reaction in Y times X distance to joint A: (0,0)
  momentum_eq_list.append(Y_reactions_list[j] * X_coordinates_list[j])

M_F_X_list = []
for i in range(len(FX_forces_list)):
  M_F_X_list.append(FX_forces_list[i] * Y_coordinates_list[i])

M_F_Y_list = []
for i in range(len(FY_forces_list)):
  M_F_Y_list.append(-FY_forces_list[i] * X_coordinates_list[i])

a_eq_list = np.array([x_eq_list, y_eq_list, momentum_eq_list])  # Left side of the equation system
b_eq_list = np.array([sum(X_values_list), sum(Y_values_list), sum(M_F_Y_list) + sum(M_F_X_list)])  # Right side of the equation system

R_eq_list = np.linalg.solve(a_eq_list, b_eq_list)
R_eq_list = R_eq_list.tolist()

# Reaction solutions R1, R2 and R3
#print(a_eq_list, b_eq_list, R_eq_list)

# Replace the reaction values
for i in range(len(X_reactions_list)):
  if X_reactions_list[i] == 1:
    X_reactions_list[i] = round(R_eq_list[0], 2)
    R_eq_list.pop(0)

for j in range(len(Y_reactions_list)):
  if Y_reactions_list[j] == 1:
    Y_reactions_list[j] = round(R_eq_list[0], 2)
    R_eq_list.pop(0)

element_names_list = []
element_values_list = []

for i in range(len(start_points_list)):
    element_name = start_points_list[i] + end_points_list[i]
    element_names_list.append(element_name)
    element_values_list.append(-1)

letters_count_dict = {}

combined_points_list = start_points_list + end_points_list

for point in combined_points_list:
    if point in letters_count_dict:
        letters_count_dict[point] += 1
    else:
        letters_count_dict[point] = 1

sorted_letters_list = []

sorted_letters_iter_list = sorted(letters_count_dict.items(), key=operator.itemgetter(1))

for element in sorted_letters_iter_list:
    sorted_letters_list.append(list(element))

while (-1 in element_values_list):
    joint_point = sorted_letters_list[0][0]
    element_forces_list = []
 
    for i in range(len(start_points_list)):
        if (joint_point in element_names_list[i]) and element_values_list[i] == -1:
            element_forces_list.append(element_names_list[i])

    if len(element_forces_list) == 2:
        angles_list = []

        for point in element_forces_list:
            temp_point = point.replace(joint_point, "")
            y_coordinate_point = Y_coordinates_list[ord(temp_point) - ord('A')]
            y_coordinate_joint = Y_coordinates_list[ord(joint_point) - ord('A')]
            y_value = y_coordinate_point - y_coordinate_joint

            temp_point = point.replace(joint_point, "")
            x_coordinate_point = X_coordinates_list[ord(temp_point) - ord('A')]
            x_coordinate_joint = X_coordinates_list[ord(joint_point) - ord('A')]
            x_value = x_coordinate_point - x_coordinate_joint

            if x_value < 0:
                angles_list.append(math.atan(y_value / x_value) + math.pi)
            elif x_value > 0:
                angles_list.append(math.atan(y_value / x_value))
            else:
                if y_value > 0:
                    angles_list.append(math.pi / 2)
                else:
                    angles_list.append(-math.pi / 2)

            for i in range(len(sorted_letters_list)):
                if point.replace(joint_point, "") == sorted_letters_list[i][0]:
                    sorted_letters_list[i][1] -= 1

        left_x_list = [math.cos(angles_list[0]), math.cos(angles_list[1])]
        left_y_list = [math.sin(angles_list[0]), math.sin(angles_list[1])]

        right_x_value = -(X_reactions_list[ord(joint_point) - ord('A')] + FX_forces_list[ord(joint_point) - ord('A')])
        right_y_value = -(Y_reactions_list[ord(joint_point) - ord('A')] + FY_forces_list[ord(joint_point) - ord('A')])

        a_eq_list = np.array([left_x_list, left_y_list])  # Left side of the equation system
        b_eq_list = np.array([right_x_value, right_y_value])  # Right side of the equation system

        R_eq_list = np.linalg.solve(a_eq_list, b_eq_list).tolist()  # Solution of forces
        result_list = R_eq_list.copy()

        elements_forces_list = [tuple(x) for x in list(zip(element_forces_list, R_eq_list))]

        for i in element_forces_list:
            for j, valuesi in enumerate(element_names_list):
                if i == valuesi:
                    element_values_list[j] = round(result_list[0], 2)
                    result_list.pop(0)
    else:
        angles_list = []
        temp_point_y = element_forces_list[0].replace(joint_point, "")
        y_coordinate_point = Y_coordinates_list[ord(temp_point_y) - ord('A')]
        y_coordinate_joint = Y_coordinates_list[ord(joint_point) - ord('A')]
        y_value = y_coordinate_point - y_coordinate_joint

        temp_point_x = element_forces_list[0].replace(joint_point, "")
        xx_coordinate_point = X_coordinates_list[ord(temp_point_x) - ord('A')]
        x_coordinate_joint = X_coordinates_list[ord(joint_point) - ord('A')]
        x_value = xx_coordinate_point - x_coordinate_joint

        if x_value < 0:
            angles_list.append(math.atan(y_value / x_value) + math.pi)
        elif x_value > 0:
            angles_list.append(math.atan(y_value / x_value))
        else:
            if y_value > 0:
                angles_list.append(math.pi / 2)
            else:
                angles_list.append(-math.pi / 2)

        for i in range(len(sorted_letters_list)):
            if element_forces_list[0].replace(joint_point, "") == sorted_letters_list[i][0]:
                sorted_letters_list[i][1] -= 1

        if angles_list[0] == 0:
            a_eq_list = np.array([[math.cos(angles_list[0])]])
            b_eq_list = np.array([-(X_reactions_list[ord(joint_point) - ord('A')] + FX_forces_list[ord(joint_point) - ord('A')])])

            result_list = np.linalg.solve(a_eq_list, b_eq_list)[0]  # Reaction solutions R1, R2 and R3
        else:
            a_eq_list = np.array([[math.sin(angles_list[0])]])  # Left side of the equation system
            b_eq_list = np.array([-(Y_reactions_list[ord(joint_point) - ord('A')] + FY_forces_list[ord(joint_point) - ord('A')])])  # Right side of the equation system

            R_eq_list = np.linalg.solve(a_eq_list, b_eq_list)[0]  # Reaction solutions R1, R2 and R3
            result_list = R_eq_list

            elements_forces_list = (element_forces_list[0], R_eq_list)

        for j, valuesi in enumerate(element_names_list):
            if element_forces_list[0] == valuesi:
                element_values_list[j] = round(result_list, 2)

    for point in element_forces_list:
        if len(element_forces_list) == 2:
            for i, j in enumerate(elements_forces_list):
                if point == j[0]:
                    for k in range(len(FX_forces_list)):
                        ind = chr(ord('A') + k)
                        if point.replace(joint_point, "") == ind:
                            FX_forces_list[k] -= R_eq_list[i] * math.cos(angles_list[i])
                            FY_forces_list[k] -= R_eq_list[i] * math.sin(angles_list[i])
        else:
            for k, ind in enumerate(range(len(FX_forces_list))):
                if point.replace(joint_point, "") == chr(ord('A') + ind):
                    FX_forces_list[ind] -= R_eq_list * math.cos(angles_list[0])
                    FY_forces_list[ind] -= R_eq_list * math.sin(angles_list[0])

    sorted_letters_list.pop(0)
    sorted_letters_list = sorted(sorted_letters_list, key=operator.itemgetter(1))

print(bars_list)
print(X_reactions_list)
print(Y_reactions_list)
print(element_names_list)
print(element_values_list)
displacement_list = []  # List to store displacement values for each element

for i in range(len(start_points_list)):
    start_node_index = ord(start_points_list[i]) - ord('A')
    end_node_index = ord(end_points_list[i]) - ord('A')
    
    start_x, start_y = node_matrix[start_node_index]
    end_x, end_y = node_matrix[end_node_index]

    displacement_x = end_x - start_x
    displacement_y = end_y - start_y
    displacement = math.sqrt(displacement_x**2 + displacement_y**2)
    
    displacement_list.append(displacement)

print("Displacement of nodal points:")
for i in range(len(displacement_list)):
    print(f"Element {start_points_list[i]} to {end_points_list[i]}: {round(displacement_list[i], 2)}")

for i, name in enumerate(element_names_list):

    x_coord = [X_coordinates_list[ord(name[0]) - ord('A')], X_coordinates_list[ord(name[1]) - ord('A')]]
    y_coord = [Y_coordinates_list[ord(name[0]) - ord('A')], Y_coordinates_list[ord(name[1]) - ord('A')]]


    plt.plot(x_coord, y_coord, "ro-")

    
    plt.text(np.mean(x_coord), np.mean(y_coord), str(round(element_values_list[i], 2)), fontsize=12)
    plt.text(x_coord[0], y_coord[0], name[0], fontsize=12, color="b", fontweight="bold")
    plt.text(x_coord[1], y_coord[1], name[1], fontsize=12, color="b", fontweight="bold")


plt.show()
