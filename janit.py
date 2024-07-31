import math

class Truss:
    def __init__(self):
        self.connectivity_matrix = []
        self.node_matrix = []
        self.nodal_loads = []
        self.support_reaction = []
        self.x_reaction_index = []
        self.y_reaction_index = []

    def is_determinate(self):
        degree_of_indeterminacy = 0
        number_of_joints = 0
        number_of_reaction_forces = 0
        number_of_nodes = len(self.connectivity_matrix)

        for temp in self.connectivity_matrix:
            number_of_joints += len(temp)

        number_of_joints = number_of_joints // 2

        for temp in self.support_reaction:
            for j in range(2):
                if temp[j] == 1:
                    number_of_reaction_forces += 1

        degree_of_indeterminacy = number_of_joints + number_of_reaction_forces - 2 * number_of_nodes
        if degree_of_indeterminacy <= 0:
            return True
        return False

    def reaction_force_calculator(self):
        value_of_n = len(self.nodal_loads)
        reaction_force_matrix = [[0.00, 0.00] for _ in range(value_of_n)]

        number_of_reaction_forces = 0
        x_reaction_forces = 0
        y_reaction_forces = 0
        for temp in self.support_reaction:
            for j in range(2):
                if temp[j] == 1:
                    number_of_reaction_forces += 1
                    if j == 0:
                        x_reaction_forces += 1
                        self.x_reaction_index.append(i)
                    else:
                        y_reaction_forces += 1
                        self.y_reaction_index.append(i)

        if number_of_reaction_forces == 3:
            if y_reaction_forces > x_reaction_forces:
                x_force = 0
                sum_of_all_x_forces = 0
                y1_force = 0
                y2_force = 0
                sum_of_all_y_forces = 0
                moment_sum = 0
                for i in range(value_of_n):
                    sum_of_all_y_forces += self.nodal_loads[i][1]
                    sum_of_all_x_forces += self.nodal_loads[i][0]
                x_force = (-1) * sum_of_all_x_forces
                reaction_force_matrix[self.x_reaction_index[0]][0] = x_force

                x_coordinate_of_y1 = self.node_matrix[self.y_reaction_index[0]][0]
                y_coordinate_of_y1 = self.node_matrix[self.y_reaction_index[0]][1]
                for i in range(value_of_n):
                    final_x_coordinate = self.node_matrix[i][0]
                    final_y_coordinate = self.node_matrix[i][1]
                    x_distance = final_x_coordinate - x_coordinate_of_y1
                    y_distance = final_y_coordinate - y_coordinate_of_y1
                    moment_sum += x_distance * self.nodal_loads[i][1] + (-1) * y_distance * self.nodal_loads[i][0]

                x_coordinate_of_y2 = self.node_matrix[self.y_reaction_index[1]][0]
                y_coordinate_of_y2 = self.node_matrix[self.y_reaction_index[1]][1]
                if self.x_reaction_index.__contains__(self.y_reaction_index[1]):
                    y2_force = ((-1) * moment_sum + x_force * (y_coordinate_of_y2 - y_coordinate_of_y1)) / (x_coordinate_of_y2 - x_coordinate_of_y1)
                else:
                    y2_force = (-1) * moment_sum / (x_coordinate_of_y2 - x_coordinate_of_y1)
                y1_force = (-1) * sum_of_all_y_forces - y2_force
                reaction_force_matrix[self.y_reaction_index[0]][1] = y1_force
                reaction_force_matrix[self.y_reaction_index[1]][1] = y2_force

            if y_reaction_forces < x_reaction_forces:
                y_force = 0
                sum_of_all_y_forces = 0
                x1_force = 0
                x2_force = 0
                sum_of_all_x_forces = 0
                moment_sum = 0
                for i in range(value_of_n):
                    sum_of_all_y_forces += self.nodal_loads[i][1]
                    sum_of_all_x_forces += self.nodal_loads[i][0]
                y_force = (-1) * sum_of_all_y_forces
                reaction_force_matrix[self.y_reaction_index[0]][1] = y_force

                x_coordinate_of_x1 = self.node_matrix[self.x_reaction_index[0]][0]
                y_coordinate_of_x1 = self.node_matrix[self.x_reaction_index[0]][1]
                for i in range(value_of_n):
                    final_x_coordinate = self.node_matrix[i][0]
                    final_y_coordinate = self.node_matrix[i][1]
                    x_distance = final_x_coordinate - x_coordinate_of_x1
                    y_distance = final_y_coordinate - y_coordinate_of_x1
                    moment_sum += x_distance * self.nodal_loads[i][1] + (-1) * y_distance * self.nodal_loads[i][0]

                x_coordinate_of_x2 = self.node_matrix[self.y_reaction_index[1]][0]
                y_coordinate_of_x2 = self.node_matrix[self.y_reaction_index[1]][1]
                if self.y_reaction_index.__contains__(self.x_reaction_index[1]):
                    x2_force = ((-1) * moment_sum + y_force * (y_coordinate_of_x2 - y_coordinate_of_x1)) / (x_coordinate_of_x2 - x_coordinate_of_x1)
                else:
                    x2_force = (-1) * moment_sum / (x_coordinate_of_x2 - x_coordinate_of_x1)
                x1_force = (-1) * sum_of_all_y_forces - x2_force
                reaction_force_matrix[self.x_reaction_index[0]][0] = x1_force
                reaction_force_matrix[self.x_reaction_index[1]][0] = x2_force

        return reaction_force_matrix

    def truss_solver_helper(self):
        member_force_matrix = []
        value_of_n = len(self.connectivity_matrix)
        for i in range(value_of_n):
            insertion = [0.00] * len(self.connectivity_matrix[i])
            member_force_matrix.append(insertion)

        if self.is_determinate():
            reaction_force_matrix = self.reaction_force_calculator()
            left_out_nodes_index = []
            for i in range(value_of_n):
                number_of_unknowns = 0
                for h in range(len(member_force_matrix[i])):
                    if member_force_matrix[i][h] == 0:
                        number_of_unknowns += 1
                if number_of_unknowns == 0:
                    continue
                else:
                    angle_array_list = []
                    x1 = self.node_matrix[i][0]
                    y1 = self.node_matrix[i][1]
                    for j in range(len(self.connectivity_matrix[i])):
                        x2 = self.node_matrix[self.connectivity_matrix[i][j]][0]
                        y2 = self.node_matrix[self.connectivity_matrix[i][j]][1]
                        slope = 0.00
                        angle = 0.00
                        if x2 - x1 == 0:
                            angle = 1.57079632679
                        else:
                            slope = (y2 - y1) / (x2 - x1)
                            angle = math.atan(slope)
                        angle_array_list.append(angle)

                    calculated_force_in_member_in_x = 0.00
                    calculated_force_in_member_in_y = 0.00
                    if len(member_force_matrix[i]) != 0:
                        for k in range(len(member_force_matrix[i])):
                            angle_of_known_force = angle_array_list[k]
                            if angle_of_known_force < 0:
                                angle_of_known_force = angle_of_known_force * (-1)
                            member_force_now = member_force_matrix[i][k]
                            x_cord_i = self.node_matrix[self.connectivity_matrix[i][k]][0]
                            y_cord_i = self.node_matrix[self.connectivity_matrix[i][k]][1]
                            x_sign = x_cord_i - x1
                            if x_sign >= 0:
                                x_sign = +1
                            else:
                                x_sign = -1
                            y_sign = y_cord_i - y1
                            if y_sign >= 0:
                                y_sign = +1
                            else:
                                y_sign = -1
                            calculated_force_in_member_in_x += (x_sign) * math.cos(angle_of_known_force) * member_force_matrix[i][k]
                            calculated_force_in_member_in_y += (y_sign) * math.sin(angle_of_known_force) * member_force_matrix[i][k]

                    if number_of_unknowns == 1:
                        force_in_unknown_member = 0.00
                        first_index = member_force_matrix[i].index(0.00)
                        index_of_1 = self.connectivity_matrix[self.connectivity_matrix[i][first_index]].index(i)
                        x_second_1 = self.node_matrix[index_of_1][0]
                        y_second_1 = self.node_matrix[index_of_1][1]
                        x_sign_1 = x_second_1 - x1
                        y_sign_1 = y_second_1 - y1
                        if x_sign_1 >= 0:
                            x_sign_1 = +1
                        else:
                            x_sign_1 = -1
                        if y_sign_1 >= 0:
                            y_sign_1 = +1
                        else:
                            y_sign_1 = -1
                        angle_of_unknown_force = angle_array_list[first_index]
                        if angle_of_unknown_force < 0:
                            angle_of_unknown_force = angle_of_unknown_force * (-1)

                        if angle_of_unknown_force != 1.57079632679:
                            a = x_sign_1 * math.cos(angle_of_unknown_force)
                            b = 0.00
                            b = self.nodal_loads[i][0]
                            if i in self.support_reaction:
                                b = b + self.support_reaction[i][0]
                            b = b + calculated_force_in_member_in_x
                            force_in_unknown_member = (-1) * (b) / (a)
                        else:
                            a = y_sign_1 * math.sin(angle_of_unknown_force)
                            b = 0.00
                            b = self.nodal_loads[i][1]
                            if i in self.support_reaction:
                                b = b + self.support_reaction[i][1]
                            b = b + calculated_force_in_member_in_y
                            force_in_unknown_member = (-1) * (b) / (a)
                        member_force_matrix[i][first_index] = force_in_unknown_member
                        member_force_matrix[self.connectivity_matrix[i][first_index]][index_of_1] = force_in_unknown_member

                    if number_of_unknowns == 2:
                        force_in_first_member = 0.00
                        force_in_second_member = 0.00
                        first_index = member_force_matrix[i].index(0.00)
                        second_index = member_force_matrix[i].lastIndexOf(0.00)
                        index_of_1 = self.connectivity_matrix[self.connectivity_matrix[i][first_index]].index(i)
                        index_of_2 = self.connectivity_matrix[self.connectivity_matrix[i][second_index]].index(i)
                        x_1_node_number = self.connectivity_matrix[i][first_index]
                        x_2_node_number = self.connectivity_matrix[i][second_index]
                        x_second_1 = self.node_matrix[x_1_node_number][0]
                        y_second_1 = self.node_matrix[x_1_node_number][1]
                        x_second_2 = self.node_matrix[x_2_node_number][0]
                        y_second_2 = self.node_matrix[x_2_node_number][1]
                        x_sign_1 = x_second_1 - x1
                        y_sign_1 = y_second_1 - y1
                        x_sign_2 = x_second_2 - x1
                        y_sign_2 = y_second_2 - y1
                        if x_sign_1 >= 0:
                            x_sign_1 = +1
                        else:
                            x_sign_1 = -1
                        if y_sign_1 >= 0:
                            y_sign_1 = +1
                        else:
                            y_sign_1 = -1
                        if x_sign_2 >= 0:
                            x_sign_2 = +1
                        else:
                            x_sign_2 = -1
                        if y_sign_2 >= 0:
                            y_sign_2 = +1
                        else:
                            y_sign_2 = -1
                        angle_one = angle_array_list[first_index]
                        angle_second = angle_array_list[second_index]
                        if angle_one < 0:
                            angle_one = angle_one * (-1)
                        if angle_second < 0:
                            angle_second = angle_second * (-1)
                        a = x_sign_1 * math.cos(angle_one)
                        b = x_sign_2 * math.cos(angle_second)
                        external_load_x = self.nodal_loads[i][0]
                        e = ((-1) * (external_load_x + calculated_force_in_member_in_x))
                        if i in self.support_reaction:
                            e = e - self.support_reaction[i][0]
                        c = y_sign_1 * math.sin(angle_one)
                        d = y_sign_2 * math.sin(angle_second)
                        external_load_y = self.nodal_loads[i][1]
                        f = ((-1) * (external_load_y + calculated_force_in_member_in_y))
                        if i in self.support_reaction:
                            f = f - self.support_reaction[i][1]
                        det = a * d - b * c
                        force_in_first_member = (d * e - b * f) / det
                        force_in_second_member = (a * f - c * e) / det
                        member_force_matrix[i][first_index] = force_in_first_member
                        member_force_matrix[i][second_index] = force_in_second_member
                        member_force_matrix[self.connectivity_matrix[i][first_index]][index_of_1] = force_in_first_member
                        member_force_matrix[self.connectivity_matrix[i][second_index]][index_of_2] = force_in_second_member

                    if number_of_unknowns > 2:
                        left_out_nodes_index.append(i)

        while len(left_out_nodes_index) != 0:
            for i in left_out_nodes_index:
                if 0.00 in member_force_matrix[i]:
                    number_of_unknowns = 0
                    for h in range(len(member_force_matrix[i])):
                        if member_force_matrix[i][h] == 0:
                            number_of_unknowns += 1

                    if number_of_unknowns == 0:
                        continue
                    else:
                        angle_array_list = []
                        x1 = self.node_matrix[i][0]
                        y1 = self.node_matrix[i][1]
                        for j in self.connectivity_matrix[i]:
                            x2 = self.node_matrix[j][0]
                            y2 = self.node_matrix[j][1]
                            slope = 0.00
                            angle = 0.00
                            if x2 - x1 == 0:
                                angle = 1.57079632679
                            else:
                                slope = (y2 - y1) / (x2 - x1)
                                angle = math.atan(slope)
                            angle_array_list.append(angle)

                        calculated_force_in_member_in_x = 0.00
                        calculated_force_in_member_in_y = 0.00
                        if len(member_force_matrix[i]) != 0:
                            for k in range(len(member_force_matrix[i])):
                                angle_of_known_force = angle_array_list[k]
                                if angle_of_known_force < 0:
                                    angle_of_known_force = angle_of_known_force * (-1)
                                member_force_now = member_force_matrix[i][k]
                                x_cord_i = self.node_matrix[self.connectivity_matrix[i][k]][0]
                                y_cord_i = self.node_matrix[self.connectivity_matrix[i][k]][1]
                                x_sign = x_cord_i - x1
                                if x_sign >= 0:
                                    x_sign = +1
                                else:
                                    x_sign = -1
                                y_sign = y_cord_i - y1
                                if y_sign >= 0:
                                    y_sign = +1
                                else:
                                    y_sign = -1
                                calculated_force_in_member_in_x = calculated_force_in_member_in_x + (x_sign) * math.cos(angle_of_known_force) * member_force_matrix[i][k]
                                calculated_force_in_member_in_y = calculated_force_in_member_in_y + (y_sign) * math.sin(angle_of_known_force) * member_force_matrix[i][k]

                        if number_of_unknowns == 1:
                            force_in_unknown_member = 0.00
                            first_index = member_force_matrix[i].index(0.00)
                            index_of_1 = self.connectivity_matrix[self.connectivity_matrix[i][first_index]].index(i)
                            x_second_1 = self.node_matrix[index_of_1][0]
                            y_second_1 = self.node_matrix[index_of_1][1]
                            x_sign_1 = x_second_1 - x1
                            y_sign_1 = y_second_1 - y1
                            if x_sign_1 >= 0:
                                x_sign_1 = +1
                            else:
                                x_sign_1 = -1
                            if y_sign_1 >= 0:
                                y_sign_1 = +1
                            else:
                                y_sign_1 = -1
                            angle_of_unknown_force = angle_array_list[first_index]
                            if angle_of_unknown_force < 0:
                                angle_of_unknown_force = angle_of_unknown_force * (-1)

                            if angle_of_unknown_force != 1.57079632679:
                                a = x_sign_1 * math.cos(angle_of_unknown_force)
                                b = 0.00
                                b = self.nodal_loads[i][0]
                                if i in self.support_reaction:
                                    b = b + self.support_reaction[i][0]
                                b = b + calculated_force_in_member_in_x
                                force_in_unknown_member = (-1) * (b) / (a)
                            else:
                                a = y_sign_1 * math.sin(angle_of_unknown_force)
                                b = 0.00
                                b = self.nodal_loads[i][1]
                                if i in self.support_reaction:
                                    b = b + self.support_reaction[i][1]
                                b = b + calculated_force_in_member_in_y
                                force_in_unknown_member = (-1) * (b) / (a)

                            member_force_matrix[i][first_index] = force_in_unknown_member
                            member_force_matrix[self.connectivity_matrix[i][first_index]][index_of_1] = force_in_unknown_member

                        if number_of_unknowns == 2:
                            force_in_first_member = 0.00
                            force_in_second_member = 0.00
                            first_index = member_force_matrix[i].index(0.00)
                            second_index = member_force_matrix[i].index(0.00, first_index + 1)
                            index_of_1 = self.connectivity_matrix[self.connectivity_matrix[i][first_index]].index(i)
                            index_of_2 = self.connectivity_matrix[self.connectivity_matrix[i][second_index]].index(i)
                            x_1_node_number = self.connectivity_matrix[i][first_index]
                            x_2_node_number = self.connectivity_matrix[i][second_index]
                            x_second_1 = self.node_matrix[x_1_node_number][0]
                            y_second_1 = self.node_matrix[x_1_node_number][1]
                            x_second_2 = self.node_matrix[x_2_node_number][0]
                            y_second_2 = self.node_matrix[x_2_node_number][1]
                            x_sign_1 = x_second_1 - x1
                            y_sign_1 = y_second_1 - y1
                            x_sign_2 = x_second_2 - x1
                            y_sign_2 = y_second_2 - y1
                            if x_sign_1 >= 0:
                                x_sign_1 = +1
                            else:
                                x_sign_1 = -1
                            if y_sign_1 >= 0:
                                y_sign_1 = +1
                            else:
                                y_sign_1 = -1
                            if x_sign_2 >= 0:
                                x_sign_2 = +1
                            else:
                                x_sign_2 = -1
                            if y_sign_2 >= 0:
                                y_sign_2 = +1
                            else:
                                y_sign_2 = -1
                            angle_one = angle_array_list[first_index]
                            angle_second = angle_array_list[second_index]
                            if angle_one < 0:
                                angle_one = angle_one * (-1)
                            if angle_second < 0:
                                angle_second = angle_second * (-1)
                            a = x_sign_1 * math.cos(angle_one)
                            b = x_sign_2 * math.cos(angle_second)
                            external_load_x = self.nodal_loads[i][0]
                            e = ((-1) * (external_load_x + calculated_force_in_member_in_x))
                            if i in self.support_reaction:
                                e = e - self.support_reaction[i][0]
                            c = y_sign_1 * math.sin(angle_one)
                            d = y_sign_2 * math.sin(angle_second)
                            external_load_y = self.nodal_loads[i][1]
                            f = ((-1) * (external_load_y + calculated_force_in_member_in_y))
                            if i in self.support_reaction:
                                f = f - self.support_reaction[i][1]
                            det = a * d - b * c
                            force_in_first_member = (d * e - b * f) / det
                            force_in_second_member = (a * f - c * e) / det
                            member_force_matrix[i][first_index] = force_in_first_member
                            member_force_matrix[i][second_index] = force_in_second_member
                            member_force_matrix[self.connectivity_matrix[i][first_index]][index_of_1] = force_in_first_member
                            member_force_matrix[self.connectivity_matrix[i][second_index]][index_of_2] = force_in_second_member

        return member_force_matrix

def main():
#     node_matrix = [(0, 0), (17, 8), (17, 0), (34, 8), (34, 0), (51, 8), (51, 0), (68, 0)]
# constraints = [(1, 1), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 1)]
# nodal_loads = [(0, 0), (0, 0), (0, -10), (0, 0), (0, -10), (0, 0), (0, -10), (0, 0)]
# adjacency_matrix = [
#     [1, 1, 1, 0, 0, 0, 0, 0],
#     [1, 1, 1, 1, 1, 0, 0, 0],
#     [1, 1, 1, 0, 1, 0, 0, 0],
#     [0, 1, 0, 0, 1, 1, 0, 0],
#     [0, 1, 1, 1, 0, 1, 1, 0],
#     [0, 0, 0, 1, 1, 0, 1, 1],
#     [0, 0, 0, 0, 1, 1, 0, 1],
#     [0, 0, 0, 0, 0, 1, 1, 1]
# ]
    my_truss = Truss()
    my_truss.connectivity_matrix =[
    [1, 1, 1, 0, 0, 0, 0, 0],
    [1, 1, 1, 1, 1, 0, 0, 0],
    [1, 1, 1, 0, 1, 0, 0, 0],
    [0, 1, 0, 0, 1, 1, 0, 0],
    [0, 1, 1, 1, 0, 1, 1, 0],
    [0, 0, 0, 1, 1, 0, 1, 1],
    [0, 0, 0, 0, 1, 1, 0, 1],
    [0, 0, 0, 0, 0, 1, 1, 1]
]
    my_truss.node_matrix =[[0, 0], [17, 8], [17, 0], [34, 8], [34, 0], [51, 8], [51, 0], (68, 0)]
    my_truss.nodal_loads = [(0, 0), (0, 0), (0, -10), (0, 0), (0, -10), (0, 0), (0, -10), (0, 0)]
    my_truss.support_reaction = [(1, 1), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 0), (0, 1)]

    if my_truss.is_determinate():
        print("The truss is determinate.")
        print("Reaction forces:", my_truss.reaction_force_calculator())
        print("Member forces:", my_truss.truss_solver_helper())
    else:
        print("The truss is indeterminate.")

if __name__ == "__main__":
    main()
