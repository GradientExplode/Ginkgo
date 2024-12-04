import networkx as nx
import numpy as np
import pyvista as pv
import argparse


def read_SWC(path):
    """ Reads SWC file and return list of lists
        Each item is a list, and the items respects the SWC Format
        Format details: https://neuromorpho.org/myfaq.jsp
    """

    lines = []
    with open(path, "r") as f:
        for i, line in enumerate(f):
            if line.startswith('#') or line == "\n":
                continue

            # Processing data to have the right format
            line = line.split()

            line[0] = int(line[0])
            line[1] = int(line[1])
            line[2] = float(line[2])
            line[3] = float(line[3])
            line[4] = float(line[4])
            line[5] = float(line[5])
            line[6] = int(line[6])

            lines.append(line)

    return lines


def calculate_N_sphere_atoms(dist, radius, Overlap_ratio=0.97):
    """ Calculates the number of sphere atoms required
        for a fiber of a given length and radius
        dist: length of a fiber
        radius: radius of the fiber
                     ((d / 2R) + (1-o_r²)**0.5 - 1)  
        formula: N = -----------------------------
                           (1-o_r²)**0.5
    """

    #p = (1.- Overlap_ratio**2.) **0.5
    #f = dist / (2.*radius)

    #n_spheres = int((f+p-1)/p)

    f = dist / (2. * radius)

    p = (1.- Overlap_ratio**2.) **0.5

    n_spheres = max(int(1. + ((f - 1.) / p)), 6) 

    return n_spheres


def reconstruct_swc_data_graph(swc_data_list):
    """ Takes a list of lists having SWC data in the neuromorpho format """

    G = nx.Graph()

    # Construct cell graph from SWC Data
    for i, elem in enumerate(swc_data_list):

        sampleID = elem[0]
        typeID = elem[1]
        atom_params = elem[2:6]
        parentID = elem[6]    

        G.add_node(sampleID,
                   type_id=typeID,
                   atom_params=atom_params,
                   pos=elem[2:5]
                )

        if parentID != -1:

            G.add_node(parentID)
            G.add_edge(sampleID, parentID)


    # Check if the network is connected
    # if nx.is_connected(G):

    #     print("The graph is connected")
    # else:
    #     print("No connections")

    return G


def get_interpolation_func(sphere_1, sphere_2):

    # Get interpolation function 
    sphere_rad_1 = sphere_1[-1]
    sphere_rad_2 = sphere_2[-1]

    rad_difference = sphere_rad_1 - sphere_rad_2

    atom1_pos = np.asarray(sphere_1[:3])
    atom2_pos = np.asarray(sphere_2[:3])

    starting_point = atom1_pos if rad_difference <= 0 else atom2_pos
    ending_point = atom1_pos if rad_difference > 0 else atom2_pos

    starting_radius = min(sphere_rad_1, sphere_rad_2)
    ending_radius = max(sphere_rad_1, sphere_rad_2)

    radius_function = lambda t: (1-t)*starting_radius + t*ending_radius

    position_function = lambda t: (1-t)*starting_point + t*ending_point

    return position_function, radius_function


def get_plane_orthornomal_vecs(normal):
    """ Return two vectors perpendicular to each others
        representing the plane described by normal and origin
    """

    threshold = 1e-2 # Check if vec component is not too small

    if abs(normal[2]) > threshold :

        z_comp = - normal[0] / normal[2]

        e_x = np.array([1,0,z_comp])

        e_x /= np.linalg.norm(e_x)

        e_y = np.cross(normal, e_x)

    elif abs(normal[0]) > threshold :

        x_comp = - normal[1] / normal[0]

        e_y = np.array([x_comp, 1, 0])

        e_y /= np.linalg.norm(e_y)

        e_x = np.cross(e_y, normal)
    
    elif abs(normal[1]) > threshold :

        y_comp = - normal[2] / normal[1]

        e_y = np.array([0, y_comp, 1])

        e_y /= np.linalg.norm(e_y)

        e_x = np.cross(e_y, normal)


    else :
        print(f"the normal vector causing issues {normal}")
        raise NotImplementedError

    return (e_x, e_y)

def interpolate_given_overlap_ratio(
        center_func,
        radii_func,
        overlap_ratio=0.99
    ):

    """ Return intermediate interpolating values for a segment """

    #end_condition = True
    t_values = [0.]

    distance = np.linalg.norm(center_func(0) - center_func(1))

    N_atoms = calculate_N_sphere_atoms(distance, min(radii_func(0.),radii_func(1.)), overlap_ratio) + 2

    t_values = np.linspace(0,1,N_atoms)

    return t_values


def reconstruct_cell_from_graph(cell_graph, ovrlap_ratio=0.995, data={}):

    atom_params = nx.get_node_attributes(cell_graph, "atom_params")
    cell_type = nx.get_node_attributes(cell_graph, "type_id")

    cell_atoms = {}

    cell_atoms["soma_data"] = []

    for i, (u,v) in enumerate(cell_graph.edges):

        soma_interpolation = False

        type_u, type_v = cell_type[u], cell_type[v]
        atom_u, atom_v = atom_params[u], atom_params[v]

        if type_u !=1 and type_v != 1:

            center_interp, radii_interp = get_interpolation_func(atom_u, atom_v)

        else:

            soma_interpolation = (type_u == 1 and type_v == 1)

            if soma_interpolation:
                cell_atoms["soma_data"].append(i)

            center_interp, radii_interp = get_interpolation_func(atom_u, atom_v)
            del(radii_interp)

            radii_interp = lambda t : min(atom_u[-1], atom_v[-1])

        t_values = interpolate_given_overlap_ratio( center_interp, radii_interp, overlap_ratio=ovrlap_ratio )
        
        cell_atoms[i] = [ list((*center_interp(t_val), radii_interp(t_val))) for t_val in t_values ]
        cell_atoms[i].insert(0, atom_u)
        cell_atoms[i].append(atom_v)

    return cell_atoms
 

def aggregate_branched_atoms(branch_atoms, cell_data=None):

    aggregated_atoms = []

    for k,v in branch_atoms.items():

        if isinstance(v, list):
            for el in v:

                aggregated_atoms.append(el)

    return aggregated_atoms


def branch_aligned(branch_1, branch_2):

    sphere_1, sphere_2 = branch_1
    sphere_3, sphere_4 = branch_2

    center_1 = np.asarray(sphere_1[:3])
    center_2 = np.asarray(sphere_2[:3])
    center_3 = np.asarray(sphere_3[:3])
    center_4 = np.asarray(sphere_4[:3])

    direction_1 = center_2 - center_1
    direction_2 = center_4 - center_3

    direction_1_norm = np.linalg.norm(direction_1)
    direction_2_norm = np.linalg.norm(direction_2)

    if direction_1_norm < 1e-4 or direction_2_norm < 1e-4:
        raise ValueError("Branch aligned branch distance is zeros")
    else:
        direction_1 /= direction_1_norm
        direction_2 /= direction_2_norm

        dot_prod = np.dot(direction_1, direction_2)

        angle = np.arccos( dot_prod )

        return angle


def get_capsule_volume(cap_radius, cap_distance):

    return np.pi * (4./3. * cap_radius + cap_distance) * cap_radius ** 2.


def extract_soma_data(branch_atoms):

    """ Assumes that the soma body has capsule shape
        Assumes also that capsule endpoints are 
        the endpoints of the branch atoms list
    """

    soma_data = {}

    alignement_threshold = 1e-4

    soma_branches = branch_atoms.get("soma_data", False)

    # Check if soma branches exists
    if soma_branches:

        n_branches = len(soma_branches)

        if n_branches == 1:

            # Assumption that 
            capsule_dist = np.linalg.norm(
                np.asarray(branch_atoms[soma_branches[0]][0][:3]) - np.asarray(branch_atoms[soma_branches[0]][-1][:3])
            )

            capsule_radius = branch_atoms[soma_branches[0]][0][-1]

            capsule_vol = get_capsule_volume(capsule_radius, capsule_dist)

            #atom_center = 0.5* np.asarray(branch_atoms[soma_branches[0]][0][:3]) + 0.5*np.asarray(branch_atoms[soma_branches[0]][-1][:3])

            capsule_direction = (np.asarray(branch_atoms[soma_branches[0]][0][:3]) - np.asarray(branch_atoms[soma_branches[0]][-1][:3]))/capsule_dist

            # Equivalent axis of ellipsoid with same volume as the capsule
            a = capsule_radius + 3./4. * capsule_dist
            b = capsule_radius
            c = capsule_radius

            soma_center = 0.5* np.asarray(branch_atoms[soma_branches[0]][0][:3]) + 0.5*np.asarray(branch_atoms[soma_branches[0]][-1][:3])

            ax1 = capsule_direction
            ax2, ax3 = get_plane_orthornomal_vecs(ax1)

        elif n_branches == 2:

            branch_1_center_A = np.asarray(branch_atoms[soma_branches[0]][0][:3])
            branch_1_center_B = np.asarray(branch_atoms[soma_branches[0]][-1][:3])


            branch_2_center_A = np.asarray(branch_atoms[soma_branches[1]][0][:3])
            branch_2_center_B = np.asarray(branch_atoms[soma_branches[1]][-1][:3])


            branch_1_dist = np.linalg.norm(
                branch_1_center_A  - branch_1_center_B
            )

            branch_2_dist = np.linalg.norm(
                branch_2_center_A - branch_2_center_B
            )

            # branches_aligned = abs(branch_aligned(branch_1, branch_2) % 3.14159) > alignement_threshold
            difference_arrays = [
                branch_1_center_A -
                branch_2_center_A,
                branch_1_center_A -
                branch_2_center_B,
                branch_1_center_B -
                branch_2_center_A,
                branch_1_center_B -
                branch_2_center_B
            ]

            branch_indices_pairs = [(0,0), (0,-1), (-1,0), (-1,-1)]
            pair_distances_list = [ np.linalg.norm(difference_arrays[i]) for i in range(len(difference_arrays)) ]

            min_dist = min(pair_distances_list)
            max_dist = max(pair_distances_list)

            max_idx = np.argmax(pair_distances_list)

            if min_dist < 1e-3 and abs(max_dist-branch_1_dist-branch_2_dist) < 1e-3:

                capsule_length = max_dist
                capsule_radius = branch_atoms[soma_branches[0]][0][-1]

                capsule_vol = get_capsule_volume(capsule_radius, capsule_length)

                # This is false
                soma_center = 0.5* (difference_arrays[max_idx] -
                    2. * np.asarray(branch_atoms[soma_branches[0]][branch_indices_pairs[max_idx][0]][:3]))

                capsule_direction = difference_arrays[max_idx]
                capsule_direction /= np.linalg.norm(capsule_direction)

                # Equivalent axis of ellipsoid with same volume as the capsule
                a = capsule_radius + 3./4. * capsule_length
                b = capsule_radius
                c = capsule_radius

                ax1 = capsule_direction
                ax2, ax3 = get_plane_orthornomal_vecs(ax1)

            else:
                raise ValueError("Branches not aligned !")

        else:
            raise NotImplementedError("Soma with more than 2 branches not implemented")

        soma_data["neuron_model_center"] = list(soma_center)
        soma_data["radii"] = [a, b, c]
        soma_data["neuron_model_ax1"] = list(ax1)
        soma_data["neuron_model_ax2"] = list(ax2)
        soma_data["neuron_model_ax3"] = list(ax3)
        soma_data["neuron_model_volume"] = capsule_vol
        soma_data["neuron_model_diameter"] = 2.*(a*b*c)**(1./3.)

    return soma_data


def plot_graph_with_cell(cell_graph, all_atoms=None):

    atoms_params = nx.get_node_attributes(cell_graph, "atom_params")

    pl = pv.Plotter()

    # Adding nodes 
    for node in cell_graph.nodes:

        sphere = pv.Sphere(center=atoms_params[node][:3], radius=atoms_params[node][-1]/5.)

        pl.add_mesh(sphere, color="red", opacity= 0.82)

    # Adding edges to make skeleton
    cells = []

    for ( e, v ) in cell_graph.edges:

        cells += [ 2, e-1, v-1 ]

    points = np.zeros((len(atoms_params), 3))

    for i in range(len(atoms_params)):

        points[i] = np.asarray(atoms_params[i+1][:3])

    # plot edges

    skel = pv.PolyData()

    skel.lines = cells
    skel.points = points

    pl.add_mesh(skel, color="white", opacity=0.82)

    if all_atoms is not None:

        N_atoms = len(all_atoms)

        for i, atom in enumerate(all_atoms):

            sphere = pv.Sphere(center=atom[:3], radius=atom[-1])
            pl.add_mesh(sphere, color="green", opacity= 0.99)
            if (i+1)%1000 == 0:
                print(f"i : {i} / {N_atoms}")

    pl.show()


def test_plot_random_sphere_interp():

    # Generate pair of spheres
    center_1 = 60*np.random.random(3)

    direction = np.random.random(3)

    direction /= np.linalg.norm(direction)

    translation = np.random.uniform(0.2, 25)

    center_2 = center_1 + translation * direction

    radius_1 = 5.

    radius_2 = 0.8

    sphere_1 = (*center_1, radius_1)
    sphere_2 = (*center_2, radius_2)

    plot_sphere_interp(sphere_1, sphere_2)


def plot_sphere_interp(sphere_1, sphere_2):

    center_1 = np.array([sphere_1[0], sphere_1[1], sphere_1[2]])

    radius_1 = sphere_1[-1]

    center_2 = np.array([sphere_2[0], sphere_2[1], sphere_2[2]])

    radius_2 = sphere_2[-1]

    position_function, radius_function = get_interpolation_func(sphere_1, sphere_2)

    t_vals = interpolate_given_overlap_ratio( position_function, radius_function,
                                              overlap_ratio=0.92 )

    cell_atoms = []
    cell_atoms = [ list( (*position_function(t_val), radius_function(t_val)) ) for t_val in t_vals ]

    pl = pv.Plotter()

    N_atoms = len(cell_atoms)

    for i, atom in enumerate(cell_atoms):

        sphere = pv.Sphere(center=atom[:3], radius=atom[-1])
        pl.add_mesh(sphere, color="green", opacity= 0.09)

        if (i+1) % 1000 == 0:
            print(f"i : {i} / {N_atoms}")

    sphere = pv.Sphere(center=center_1, radius=radius_1/2.)
    pl.add_mesh(sphere, color="red", opacity= 0.57)

    sphere = pv.Sphere(center=center_2, radius=radius_2/2.)
    pl.add_mesh(sphere, color="red", opacity= 0.57)

    pl.show()


def get_single_neuron(swc_fname):

    swc_data = read_SWC(swc_fname)

    neuron_graph = reconstruct_swc_data_graph(swc_data)

    cell_atoms = reconstruct_cell_from_graph(neuron_graph, ovrlap_ratio=0.98)

    soma_data_dict = extract_soma_data(cell_atoms)

    new_cell_atoms = {}

    new_cell_atoms = {k:v for k,v in cell_atoms.items() if k not in ["soma_data"]}

    aggregated_atoms = aggregate_branched_atoms(new_cell_atoms)

    return (soma_data_dict, aggregated_atoms)


def plot_neuron(aggregated_atoms):

    pl = pv.Plotter()

    N_atoms = len(aggregated_atoms)

    for i, atom in enumerate(aggregated_atoms):

        sphere = pv.Sphere(center=atom[:3], radius=atom[-1])
        pl.add_mesh(sphere, color="green", opacity= 0.9)

        if (i+1) % 1000 == 0:
            print(f"Plot progress : {i} / {N_atoms}")

    pl.show()

 
def main():

    parser = argparse.ArgumentParser(description='Process some data and optionally plot it.')
    parser.add_argument('--file_path', type=str, default='test_data/03b_pyramidal2aACC.CNG.swc', help='Path to the input file')
    parser.add_argument('--plot', action='store_true', help='Flag to enable plotting (default: False)')
    
    args = parser.parse_args()

    file_path = args.file_path
    plot = args.plot

    # Now you can use file_path and plot in your code
    print(f'File path: {file_path}')
    soma_data_dict, neuron_spheres_list = get_single_neuron(file_path)

    print(soma_data_dict)
    print("Neuron sphere count :", len(neuron_spheres_list))

    if plot:
        print(f'Plot')
        plot_neuron(neuron_spheres_list)


if __name__ == "__main__":

    main()

