"""
OBJ -> DirectX .x converter for DxLib
Supports vertex colors (MTL Kd) and texture maps (MTL map_Kd + UV coords).
Usage: python obj_to_x.py input.obj [output.x]
       python obj_to_x.py --batch input_dir output_dir
"""
import sys
import os
import shutil


def parse_mtl(mtl_path):
    """Return dict: material_name -> {'color': (r,g,b,a), 'texture': filename_or_None}"""
    materials = {}
    current = None
    if not mtl_path or not os.path.exists(mtl_path):
        return materials
    with open(mtl_path, 'r', errors='replace') as f:
        for line in f:
            line = line.strip()
            if line.startswith('newmtl '):
                current = line[7:].strip()
                materials[current] = {'color': (0.8, 0.8, 0.8, 1.0), 'texture': None}
            elif line.startswith('Kd ') and current:
                parts = line.split()
                materials[current]['color'] = (float(parts[1]), float(parts[2]), float(parts[3]), 1.0)
            elif line.startswith('map_Kd ') and current:
                materials[current]['texture'] = line[7:].strip()
    return materials


def find_texture(mtl_path, tex_name):
    """Locate texture file: check alongside MTL, common Textures/ subfolders, then walk up."""
    if not mtl_path or not tex_name:
        return None
    base_dir = os.path.dirname(mtl_path)
    tex_base = os.path.basename(tex_name)

    # Check obvious relative paths first
    for rel in ('', 'Textures', 'textures', '../Textures', '../textures',
                 '../../Textures', '../../textures'):
        candidate = os.path.normpath(os.path.join(base_dir, rel, tex_base))
        if os.path.exists(candidate):
            return candidate

    # Walk the parent directory tree to find the file by name
    search_root = os.path.dirname(base_dir)
    for root, _dirs, files in os.walk(search_root):
        for fname in files:
            if fname.lower() == tex_base.lower():
                return os.path.join(root, fname)
    return None


def parse_obj(obj_path):
    """Parse OBJ. Returns (vertices, normals, uvs, triangles, materials, mtl_path).
    triangles = list of ((vi,ni,uvi), (vi,ni,uvi), (vi,ni,uvi), mat_name)
    """
    vertices, normals, uvs = [], [], []
    triangles = []
    current_mat = 'default'
    mtl_file = None

    with open(obj_path, 'r', errors='replace') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            parts = line.split()
            tok = parts[0]

            if tok == 'mtllib':
                mtl_file = parts[1]
            elif tok == 'v':
                vertices.append((float(parts[1]), float(parts[2]), float(parts[3])))
            elif tok == 'vn':
                normals.append((float(parts[1]), float(parts[2]), float(parts[3])))
            elif tok == 'vt':
                u = float(parts[1])
                v = float(parts[2]) if len(parts) > 2 else 0.0
                uvs.append((u, v))
            elif tok == 'usemtl':
                current_mat = ' '.join(parts[1:])
            elif tok == 'f':
                face = []
                for token in parts[1:]:
                    sub = token.split('/')
                    vi  = int(sub[0]) - 1
                    uvi = int(sub[1]) - 1 if len(sub) > 1 and sub[1] else -1
                    ni  = int(sub[2]) - 1 if len(sub) > 2 and sub[2] else -1
                    face.append((vi, ni, uvi))
                # Fan triangulation
                for i in range(1, len(face) - 1):
                    triangles.append((face[0], face[i], face[i+1], current_mat))

    mtl_path = None
    if mtl_file:
        mtl_path = os.path.join(os.path.dirname(obj_path), mtl_file)

    materials = parse_mtl(mtl_path) if mtl_path else {}
    if not materials:
        materials['default'] = {'color': (0.8, 0.8, 0.8, 1.0), 'texture': None}

    return vertices, normals, uvs, triangles, materials, mtl_path


def write_x(out_path, vertices, normals, uvs, triangles, materials, mtl_path):
    """Write DirectX .x text file."""
    flat_verts, flat_norms, flat_uvs, flat_faces, mat_indices = [], [], [], [], []
    mat_names = list(materials.keys())
    has_uvs = len(uvs) > 0

    for tri in triangles:
        (vi0, ni0, uvi0), (vi1, ni1, uvi1), (vi2, ni2, uvi2), matname = tri
        base = len(flat_verts)
        flat_verts += [vertices[vi0], vertices[vi1], vertices[vi2]]
        flat_norms += [
            normals[ni0] if normals and ni0 >= 0 else (0.0, 1.0, 0.0),
            normals[ni1] if normals and ni1 >= 0 else (0.0, 1.0, 0.0),
            normals[ni2] if normals and ni2 >= 0 else (0.0, 1.0, 0.0),
        ]
        if has_uvs:
            flat_uvs += [
                uvs[uvi0] if uvi0 >= 0 else (0.0, 0.0),
                uvs[uvi1] if uvi1 >= 0 else (0.0, 0.0),
                uvs[uvi2] if uvi2 >= 0 else (0.0, 0.0),
            ]
        flat_faces.append((base, base + 1, base + 2))
        mat_indices.append(mat_names.index(matname) if matname in mat_names else 0)

    n_v, n_f, n_m = len(flat_verts), len(flat_faces), len(mat_names)

    # Copy textures to output dir, build local name map
    out_dir = os.path.dirname(out_path)
    os.makedirs(out_dir, exist_ok=True)
    mat_tex_local = {}  # mat_name -> local filename
    for name, mdata in materials.items():
        tex = mdata.get('texture')
        if tex:
            src = find_texture(mtl_path, tex)
            if src:
                dst_name = os.path.basename(src)
                dst = os.path.join(out_dir, dst_name)
                if not os.path.exists(dst):
                    shutil.copy2(src, dst)
                mat_tex_local[name] = dst_name

    with open(out_path, 'w') as f:
        def w(s): f.write(s + '\n')

        w('xof 0303txt 0032\n')
        w('Frame Root {')
        w('  FrameTransformMatrix {')
        w('    1.000000, 0.000000, 0.000000, 0.000000,')
        w('    0.000000, 1.000000, 0.000000, 0.000000,')
        w('    0.000000, 0.000000, 1.000000, 0.000000,')
        w('    0.000000, 0.000000, 0.000000, 1.000000;;')
        w('  }\n')
        w('  Mesh {')

        # Vertices
        w(f'    {n_v};')
        for i, (x, y, z) in enumerate(flat_verts):
            sep = ',' if i < n_v - 1 else ';'
            w(f'    {x:.6f}; {y:.6f}; {z:.6f};{sep}')

        # Faces
        w(f'\n    {n_f};')
        for i, (a, b, c) in enumerate(flat_faces):
            sep = ',' if i < n_f - 1 else ';'
            w(f'    3; {a}, {b}, {c};;{sep}')

        # Normals
        w('\n    MeshNormals {')
        w(f'      {n_v};')
        for i, (x, y, z) in enumerate(flat_norms):
            sep = ',' if i < n_v - 1 else ';'
            w(f'      {x:.6f}; {y:.6f}; {z:.6f};{sep}')
        w(f'\n      {n_f};')
        for i, (a, b, c) in enumerate(flat_faces):
            sep = ',' if i < n_f - 1 else ';'
            w(f'      3; {a}, {b}, {c};;{sep}')
        w('    }')

        # UV coords (DirectX flips V vs OpenGL)
        if has_uvs and flat_uvs:
            w('\n    MeshTextureCoords {')
            w(f'      {n_v};')
            for i, (u, v) in enumerate(flat_uvs):
                sep = ',' if i < n_v - 1 else ';'
                w(f'      {u:.6f}; {1.0 - v:.6f};{sep}')
            w('    }')

        # Materials
        w('\n    MeshMaterialList {')
        w(f'      {n_m};')
        w(f'      {n_f};')
        for i, mi in enumerate(mat_indices):
            sep = ',' if i < n_f - 1 else ';'
            w(f'      {mi}{sep}')
        for name in mat_names:
            mdata = materials.get(name, {'color': (0.8, 0.8, 0.8, 1.0), 'texture': None})
            r, g, b, a = mdata['color']
            tex_local = mat_tex_local.get(name)
            w('\n      Material {')
            w(f'        {r:.6f}; {g:.6f}; {b:.6f}; {a:.6f};;')
            w('        96.000000;')
            w('        0.500000; 0.500000; 0.500000;;')
            w('        0.000000; 0.000000; 0.000000;;')
            if tex_local:
                w(f'        TextureFilename {{ "{tex_local}"; }}')
            w('      }')
        w('    }')

        w('  }')
        w('}')


def convert_file(obj_path, out_path):
    print(f'Converting: {os.path.basename(obj_path)} -> {os.path.basename(out_path)}')
    vertices, normals, uvs, triangles, materials, mtl_path = parse_obj(obj_path)
    if not triangles:
        print('  WARNING: no triangles, skipping')
        return False
    write_x(out_path, vertices, normals, uvs, triangles, materials, mtl_path)
    has_tex = any(m.get('texture') for m in materials.values())
    print(f'  OK: {len(triangles)} tri, {len(materials)} mat, UV={len(uvs)>0}, tex={has_tex}')
    return True


def batch_convert(in_dir, out_dir):
    os.makedirs(out_dir, exist_ok=True)
    count = 0
    for root, _dirs, files in os.walk(in_dir):
        for fname in sorted(files):
            if fname.lower().endswith('.obj'):
                obj_path = os.path.join(root, fname)
                out_name = os.path.splitext(fname)[0] + '.x'
                out_path = os.path.join(out_dir, out_name)
                if convert_file(obj_path, out_path):
                    count += 1
    print(f'\nDone: {count} files -> {out_dir}')


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
