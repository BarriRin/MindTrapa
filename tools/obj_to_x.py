"""
OBJ -> DirectX .x converter for DxLib
Converts OBJ+MTL files (solid/vertex-color materials, no textures) to .x format.
Usage: python obj_to_x.py input.obj [output.x]
       python obj_to_x.py --batch input_dir output_dir
"""
import sys
import os
import re

def parse_mtl(mtl_path):
    """Parse .mtl file, return dict of material name -> (r, g, b, a)"""
    materials = {}
    current = None
    if not os.path.exists(mtl_path):
        return materials
    with open(mtl_path, 'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith('newmtl '):
                current = line[7:].strip()
                materials[current] = (0.5, 0.5, 0.5, 1.0)
            elif line.startswith('Kd ') and current:
                parts = line.split()
                r, g, b = float(parts[1]), float(parts[2]), float(parts[3])
                materials[current] = (r, g, b, 1.0)
    return materials

def parse_obj(obj_path):
    """Parse .obj file, return (vertices, normals, triangles)
    triangles = list of ((v0,n0), (v1,n1), (v2,n2), material_name)
    """
    vertices = []   # list of (x, y, z)
    normals = []    # list of (x, y, z)
    triangles = []  # list of [((vi,ni), (vi,ni), (vi,ni)), matname]
    current_mat = 'default'
    mtl_file = None

    with open(obj_path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            parts = line.split()
            if parts[0] == 'mtllib':
                mtl_file = parts[1]
            elif parts[0] == 'v':
                vertices.append((float(parts[1]), float(parts[2]), float(parts[3])))
            elif parts[0] == 'vn':
                normals.append((float(parts[1]), float(parts[2]), float(parts[3])))
            elif parts[0] == 'usemtl':
                current_mat = parts[1]
            elif parts[0] == 'f':
                # Parse face vertices - format: v//vn or v/vt/vn or v
                face_verts = []
                for token in parts[1:]:
                    if '//' in token:
                        vi, ni = token.split('//')
                        face_verts.append((int(vi)-1, int(ni)-1))
                    elif '/' in token:
                        sub = token.split('/')
                        vi = int(sub[0]) - 1
                        ni = int(sub[2]) - 1 if len(sub) > 2 and sub[2] else 0
                        face_verts.append((vi, ni))
                    else:
                        face_verts.append((int(token)-1, 0))

                # Triangulate (fan from first vertex)
                for i in range(1, len(face_verts) - 1):
                    triangles.append((
                        face_verts[0],
                        face_verts[i],
                        face_verts[i+1],
                        current_mat
                    ))

    # Load materials
    mtl_path = os.path.join(os.path.dirname(obj_path), mtl_file) if mtl_file else None
    materials = parse_mtl(mtl_path) if mtl_path else {}
    if not materials:
        materials['default'] = (0.5, 0.5, 0.5, 1.0)

    return vertices, normals, triangles, materials

def write_x(out_path, vertices, normals, triangles, materials):
    """Write DirectX .x text file"""
    # Expand to flat arrays (one entry per face-vertex, no sharing)
    flat_verts = []  # (x, y, z)
    flat_norms = []  # (x, y, z)
    flat_faces = []  # (i0, i1, i2) into flat_verts
    mat_indices = [] # material index per face
    mat_names = list(materials.keys())

    for tri in triangles:
        (vi0, ni0), (vi1, ni1), (vi2, ni2), matname = tri
        base = len(flat_verts)
        flat_verts.append(vertices[vi0])
        flat_verts.append(vertices[vi1])
        flat_verts.append(vertices[vi2])
        flat_norms.append(normals[ni0] if normals else (0, 1, 0))
        flat_norms.append(normals[ni1] if normals else (0, 1, 0))
        flat_norms.append(normals[ni2] if normals else (0, 1, 0))
        flat_faces.append((base, base+1, base+2))
        mat_idx = mat_names.index(matname) if matname in mat_names else 0
        mat_indices.append(mat_idx)

    n_verts = len(flat_verts)
    n_faces = len(flat_faces)
    n_mats = len(mat_names)

    with open(out_path, 'w') as f:
        f.write('xof 0303txt 0032\n\n')
        f.write('Frame Root {\n')
        f.write('  FrameTransformMatrix {\n')
        f.write('    1.000000, 0.000000, 0.000000, 0.000000,\n')
        f.write('    0.000000, 1.000000, 0.000000, 0.000000,\n')
        f.write('    0.000000, 0.000000, 1.000000, 0.000000,\n')
        f.write('    0.000000, 0.000000, 0.000000, 1.000000;;\n')
        f.write('  }\n\n')
        f.write('  Mesh {\n')

        # Vertices
        f.write(f'    {n_verts};\n')
        for i, (x, y, z) in enumerate(flat_verts):
            sep = ',' if i < n_verts - 1 else ';'
            f.write(f'    {x:.6f}; {y:.6f}; {z:.6f};{sep}\n')

        # Faces
        f.write(f'\n    {n_faces};\n')
        for i, (a, b, c) in enumerate(flat_faces):
            sep = ',' if i < n_faces - 1 else ';'
            f.write(f'    3; {a}, {b}, {c};;{sep}\n')

        # Normals
        f.write('\n    MeshNormals {\n')
        f.write(f'      {n_verts};\n')
        for i, (x, y, z) in enumerate(flat_norms):
            sep = ',' if i < n_verts - 1 else ';'
            f.write(f'      {x:.6f}; {y:.6f}; {z:.6f};{sep}\n')
        f.write(f'\n      {n_faces};\n')
        for i, (a, b, c) in enumerate(flat_faces):
            sep = ',' if i < n_faces - 1 else ';'
            f.write(f'      3; {a}, {b}, {c};;{sep}\n')
        f.write('    }\n')

        # Materials
        f.write('\n    MeshMaterialList {\n')
        f.write(f'      {n_mats};\n')
        f.write(f'      {n_faces};\n')
        for i, mi in enumerate(mat_indices):
            sep = ',' if i < n_faces - 1 else ';'
            f.write(f'      {mi}{sep}\n')
        for name in mat_names:
            r, g, b, a = materials.get(name, (0.5, 0.5, 0.5, 1.0))
            f.write('\n      Material {\n')
            f.write(f'        {r:.6f}; {g:.6f}; {b:.6f}; {a:.6f};;\n')
            f.write(f'        96.000000;\n')  # specular power
            f.write(f'        0.500000; 0.500000; 0.500000;;\n')  # specular
            f.write(f'        0.000000; 0.000000; 0.000000;;\n')  # emissive
            f.write('      }\n')
        f.write('    }\n')

        f.write('  }\n')
        f.write('}\n')

def convert_file(obj_path, out_path):
    print(f'Converting: {os.path.basename(obj_path)} -> {os.path.basename(out_path)}')
    vertices, normals, triangles, materials = parse_obj(obj_path)
    if not triangles:
        print(f'  WARNING: no triangles found, skipping')
        return False
    write_x(out_path, vertices, normals, triangles, materials)
    print(f'  OK: {len(triangles)} triangles, {len(materials)} materials')
    return True

def batch_convert(in_dir, out_dir):
    os.makedirs(out_dir, exist_ok=True)
    count = 0
    for root, dirs, files in os.walk(in_dir):
        for fname in files:
            if fname.lower().endswith('.obj'):
                obj_path = os.path.join(root, fname)
                out_name = os.path.splitext(fname)[0] + '.x'
                out_path = os.path.join(out_dir, out_name)
                if convert_file(obj_path, out_path):
                    count += 1
    print(f'\nDone: {count} files converted -> {out_dir}')

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    if sys.argv[1] == '--batch':
        if len(sys.argv) < 4:
            print('Usage: python obj_to_x.py --batch input_dir output_dir')
            sys.exit(1)
        batch_convert(sys.argv[2], sys.argv[3])
    else:
        obj_path = sys.argv[1]
        out_path = sys.argv[2] if len(sys.argv) > 2 else os.path.splitext(obj_path)[0] + '.x'
        convert_file(obj_path, out_path)
