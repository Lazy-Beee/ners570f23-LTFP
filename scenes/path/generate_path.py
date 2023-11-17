from operator import le
from pdb import post_mortem
from matplotlib import projections
import numpy as np
import matplotlib.pyplot as plt
import json
import os

def write_to_file(time_list, position_list, layer_info, case_name):
    data1 = {}
    data1['time'] = time_list
    data1['position'] = position_list
    
    file_name1 = os.path.dirname(os.path.realpath(__file__)) + '/' + case_name + "_path.json"
    with open(file_name1, 'w') as f:
        json.dump(data1, f)
    print(f'Result file written to {file_name1}')
    
    data2 = {}
    data2['layer scan count'] = layer_info[0]
    data2['layer duration'] = layer_info[1]
    data2['layer increment'] = layer_info[2]
    
    file_name2 = os.path.dirname(os.path.realpath(__file__)) + '/' + case_name + "_layer.json"
    with open(file_name2, 'w') as f:
        json.dump(data2, f)
    print(f'Result file written to {file_name2}')

def xy_line_scan(time_config, num_layers, scan_speed, domain, gap, increment_thickness):
    """Generate a regular bi-direction scan path on XZ plane with domain increment in Y direction

    Args:
        time_config (float list): start time, gap between scan, gap between layer
        num_layers (int): number of layers to scan
        scan_speed (float): laser scan speed
        domain (float list): x min, x mas, ymin, ymax, z min, z max
        gap (float): gap between scan 
        increment_thickness (float): thickness of domain increment

    Returns:
        time_list (float list): list of time periods
        position_list (vector3float list): list of laser center position sections
    """
    # Load parameters
    start_time = time_config[0]
    gap_time_scan = time_config[1]
    gap_time_layer = time_config[2]
    x_min = domain[0]
    x_max = domain[1]
    y_max = domain[3]
    z_min = domain[4]
    z_max = domain[5]
    half_gap = gap / 2
    
    # Generate scan on one layer
    layer_time_X = []
    layer_pos_X = []
    scan_on_time = (x_max - x_min - gap) / scan_speed
    scan_full_time = scan_on_time + gap_time_scan
    layer_scan_count_X = (int)((z_max - z_min) / gap - 1)
    layer_duration_X = layer_scan_count_X * scan_full_time
    for i in range(layer_scan_count_X) :
        layer_time_X.append([scan_full_time * i, scan_full_time * i + scan_on_time])
        layer_pos_X.append([[x_min + half_gap, y_max, z_min + half_gap + gap * i], [x_max - half_gap, y_max, z_min + half_gap + gap * i]])
        
    layer_time_Z = []
    layer_pos_Z = []
    scan_on_time = (z_max - z_min - gap) / scan_speed
    scan_full_time = scan_on_time + gap_time_scan
    layer_scan_count_Z = (int)((x_max - x_min) / gap - 1)
    layer_duration_Z = layer_scan_count_Z * scan_full_time
    for i in range(layer_scan_count_Z) :
        layer_time_Z.append([scan_full_time * i, scan_full_time * i + scan_on_time])
        layer_pos_Z.append([[x_min + half_gap + gap * i, y_max, z_min + half_gap], [x_min + half_gap + gap * i, y_max, z_max - half_gap]])

        
    # Assemble path    
    time_list = []
    position_list = []
    layer_duration = []
    layer_scan_count = []
    layer_increment = []
    
    current_time = start_time
    for layer_count in range(num_layers) :    # Layer loop
        layer_duration.append([current_time])
        y_increase = layer_count * increment_thickness
        if (layer_count % 2) == 0 :    # Scan from X- to X+
            for i in range(layer_scan_count_X):
                time = layer_time_X[i].copy()
                time[0] += current_time
                time[1] += current_time
                time_list.append(time)
                pos0 = layer_pos_X[i][0].copy()
                pos1 = layer_pos_X[i][1].copy()
                pos0[1] = pos0[1] + y_increase
                pos1[1] = pos1[1] + y_increase
                position_list.append([pos0, pos1])
            current_time += layer_duration_X
            layer_scan_count.append(layer_scan_count_X)
        elif (layer_count % 2) == 1 :    # Scan from Z- to Z+
            for i in range(layer_scan_count_Z):
                time = layer_time_Z[i].copy()
                time[0] += current_time
                time[1] += current_time
                time_list.append(time)
                pos0 = layer_pos_Z[i][0].copy()
                pos1 = layer_pos_Z[i][1].copy()
                pos0[1] = pos0[1] + y_increase
                pos1[1] = pos1[1] + y_increase
                position_list.append([pos0, pos1])
            current_time += layer_duration_Z
            layer_scan_count.append(layer_scan_count_Z)
        
        layer_duration[-1].append(current_time)
        current_time += gap_time_layer
        layer_increment.append(layer_count * increment_thickness)
    
    layer_info = [layer_scan_count, layer_duration, layer_increment]

    return time_list, position_list, layer_info

def plot_path(time_list, position_list, layer_info, case_name):
    layer_scan_count = layer_info[0]
    layer_duration = layer_info[1] 
    layer_increment = layer_info[2]
    
    output_dir = os.path.dirname(os.path.realpath(__file__)) + f'/{case_name}/'
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    j1 = 0
    j2 = 0
    for i in range(len(layer_scan_count)):
        j1 = j2
        j2 += layer_scan_count[i]
        
        plt.figure()
        
        for j in range(j1, j2) :
            line = position_list[j]
            plt.arrow(line[0][0], line[0][2], line[1][0] - line[0][0], line[1][2] - line[0][2], width=0.01, length_includes_head=True)
        
        plt.axis('equal')
        plt.title(f'Layer {i} with y increment of {layer_increment[i]} from time {layer_duration[i][0]} to {layer_duration[i][1]}')
        plt.savefig(output_dir + f'{i}.png')
        plt.close()
        
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    for pos in position_list:
        ax.plot([pos[0][0], pos[1][0]], [pos[0][1], pos[1][1]], [pos[0][2], pos[1][2]])
    plt.axis('equal')
    fig.savefig(output_dir + '3D.png')

if __name__ == '__main__':
    case_name = 'test'
    
    time_list, position_list, layer_info = xy_line_scan([0.0, 0.1, 2.0], 4, 2.0, [0.0, 1.1, 0.0, 1.0, 0.0, 2.1], 0.1, 0.2)
    write_to_file(time_list, position_list, layer_info, case_name)
    plot_path(time_list, position_list, layer_info, case_name)