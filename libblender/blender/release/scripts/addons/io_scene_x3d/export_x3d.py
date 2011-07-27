# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

# <pep8 compliant>

# Contributors: bart:neeneenee*de, http://www.neeneenee.de/vrml, Campbell Barton

"""
This script exports to X3D format.

Usage:
Run this script from "File->Export" menu.  A pop-up will ask whether you
want to export only selected or all relevant objects.

Known issues:
    Doesn't handle multiple materials (don't use material indices);<br>
    Doesn't handle multiple UV textures on a single mesh (create a mesh for each texture);<br>
    Can't get the texture array associated with material * not the UV ones;
"""

import math
import os

import bpy
import mathutils

from bpy_extras.io_utils import create_derived_objects, free_derived_objects

x3d_names_reserved = {'Anchor', 'Appearance', 'Arc2D', 'ArcClose2D', 'AudioClip', 'Background', 'Billboard',
                      'BooleanFilter', 'BooleanSequencer', 'BooleanToggle', 'BooleanTrigger', 'Box', 'Circle2D',
                      'Collision', 'Color', 'ColorInterpolator', 'ColorRGBA', 'component', 'Cone', 'connect',
                      'Contour2D', 'ContourPolyline2D', 'Coordinate', 'CoordinateDouble', 'CoordinateInterpolator',
                      'CoordinateInterpolator2D', 'Cylinder', 'CylinderSensor', 'DirectionalLight', 'Disk2D',
                      'ElevationGrid', 'EspduTransform', 'EXPORT', 'ExternProtoDeclare', 'Extrusion', 'field',
                      'fieldValue', 'FillProperties', 'Fog', 'FontStyle', 'GeoCoordinate', 'GeoElevationGrid',
                      'GeoLocationLocation', 'GeoLOD', 'GeoMetadata', 'GeoOrigin', 'GeoPositionInterpolator',
                      'GeoTouchSensor', 'GeoViewpoint', 'Group', 'HAnimDisplacer', 'HAnimHumanoid', 'HAnimJoint',
                      'HAnimSegment', 'HAnimSite', 'head', 'ImageTexture', 'IMPORT', 'IndexedFaceSet',
                      'IndexedLineSet', 'IndexedTriangleFanSet', 'IndexedTriangleSet', 'IndexedTriangleStripSet',
                      'Inline', 'IntegerSequencer', 'IntegerTrigger', 'IS', 'KeySensor', 'LineProperties', 'LineSet',
                      'LoadSensor', 'LOD', 'Material', 'meta', 'MetadataDouble', 'MetadataFloat', 'MetadataInteger',
                      'MetadataSet', 'MetadataString', 'MovieTexture', 'MultiTexture', 'MultiTextureCoordinate',
                      'MultiTextureTransform', 'NavigationInfo', 'Normal', 'NormalInterpolator', 'NurbsCurve',
                      'NurbsCurve2D', 'NurbsOrientationInterpolator', 'NurbsPatchSurface',
                      'NurbsPositionInterpolator', 'NurbsSet', 'NurbsSurfaceInterpolator', 'NurbsSweptSurface',
                      'NurbsSwungSurface', 'NurbsTextureCoordinate', 'NurbsTrimmedSurface', 'OrientationInterpolator',
                      'PixelTexture', 'PlaneSensor', 'PointLight', 'PointSet', 'Polyline2D', 'Polypoint2D',
                      'PositionInterpolator', 'PositionInterpolator2D', 'ProtoBody', 'ProtoDeclare', 'ProtoInstance',
                      'ProtoInterface', 'ProximitySensor', 'ReceiverPdu', 'Rectangle2D', 'ROUTE', 'ScalarInterpolator',
                      'Scene', 'Script', 'Shape', 'SignalPdu', 'Sound', 'Sphere', 'SphereSensor', 'SpotLight', 'StaticGroup',
                      'StringSensor', 'Switch', 'Text', 'TextureBackground', 'TextureCoordinate', 'TextureCoordinateGenerator',
                      'TextureTransform', 'TimeSensor', 'TimeTrigger', 'TouchSensor', 'Transform', 'TransmitterPdu',
                      'TriangleFanSet', 'TriangleSet', 'TriangleSet2D', 'TriangleStripSet', 'Viewpoint', 'VisibilitySensor',
                      'WorldInfo', 'X3D', 'XvlShell', 'VertexShader', 'FragmentShader', 'MultiShaderAppearance', 'ShaderAppearance'}


def clamp_color(col):
    return tuple([max(min(c, 1.0), 0.0) for c in col])


def matrix_direction_neg_z(matrix):
    return (mathutils.Vector((0.0, 0.0, -1.0)) * matrix.to_3x3()).normalized()[:]


def prefix_quoted_str(value, prefix):
    return value[0] + prefix + value[1:]


def build_hierarchy(objects):
    """ returns parent child relationships, skipping
    """
    objects_set = set(objects)
    par_lookup = {}

    def test_parent(parent):
        while (parent is not None) and (parent not in objects_set):
            parent = parent.parent
        return parent

    for obj in objects:
        par_lookup.setdefault(test_parent(obj.parent), []).append((obj, []))

    for parent, children in par_lookup.items():
        for obj, subchildren in children:
            subchildren[:] = par_lookup.get(obj, [])

    return par_lookup[None]


# -----------------------------------------------------------------------------
# H3D Functions
# -----------------------------------------------------------------------------
def h3d_shader_glsl_frag_patch(filepath):
    h3d_file = open(filepath, 'r')
    lines = []
    for l in h3d_file:
        l = l.replace("uniform mat4 unfinvviewmat;", "")
        l = l.replace("unfinvviewmat", "gl_ModelViewMatrixInverse")

        '''
        l = l.replace("varying vec3 varposition;", "")
        l = l.replace("varposition", "gl_Vertex")  # not needed int H3D
        '''

        #l = l.replace("varying vec3 varnormal;", "")
        #l = l.replace("varnormal", "gl_Normal")  # view normal
        #l = l.replace("varnormal", "normalize(-(gl_ModelViewMatrix * gl_Vertex).xyz)")  # view normal
        # l = l.replace("varnormal", "gl_NormalMatrix * gl_Normal")  # view normal
        lines.append(l)

    h3d_file.close()

    h3d_file = open(filepath, 'w')
    h3d_file.writelines(lines)
    h3d_file.close()


# -----------------------------------------------------------------------------
# Functions for writing output file
# -----------------------------------------------------------------------------

def export(file,
           global_matrix,
           scene,
           use_apply_modifiers=False,
           use_selection=True,
           use_triangulate=False,
           use_normals=False,
           use_hierarchy=True,
           use_h3d=False,
           path_mode='AUTO',
           ):

    # -------------------------------------------------------------------------
    # Global Setup
    # -------------------------------------------------------------------------
    import bpy_extras
    from bpy_extras.io_utils import unique_name
    from xml.sax.saxutils import quoteattr

    uuid_cache_object = {}    # object
    uuid_cache_lamp = {}      # 'LA_' + object.name
    uuid_cache_view = {}      # object, different namespace
    uuid_cache_mesh = {}      # mesh
    uuid_cache_material = {}  # material
    uuid_cache_image = {}     # image
    uuid_cache_world = {}     # world

    # store files to copy
    copy_set = set()

    fw = file.write
    base_src = os.path.dirname(bpy.data.filepath)
    base_dst = os.path.dirname(file.name)
    filename_strip = os.path.splitext(os.path.basename(file.name))[0]
    gpu_shader_cache = {}

    if use_h3d:
        import gpu
        gpu_shader_dummy_mat = bpy.data.materials.new('X3D_DYMMY_MAT')
        gpu_shader_cache[None] = gpu.export_shader(scene, gpu_shader_dummy_mat)

    # -------------------------------------------------------------------------
    # File Writing Functions
    # -------------------------------------------------------------------------

    def writeHeader(ident):
        filepath_quoted = quoteattr(os.path.basename(file.name))
        blender_ver_quoted = quoteattr('Blender %s' % bpy.app.version_string)

        fw('%s<?xml version="1.0" encoding="UTF-8"?>\n' % ident)
        if use_h3d:
            fw('%s<X3D profile="H3DAPI" version="1.4">\n' % ident)
        else:
            fw('%s<!DOCTYPE X3D PUBLIC "ISO//Web3D//DTD X3D 3.0//EN" "http://www.web3d.org/specifications/x3d-3.0.dtd">\n' % ident)
            fw('%s<X3D version="3.0" profile="Immersive" xmlns:xsd="http://www.w3.org/2001/XMLSchema-instance" xsd:noNamespaceSchemaLocation="http://www.web3d.org/specifications/x3d-3.0.xsd">\n' % ident)

        ident += '\t'
        fw('%s<head>\n' % ident)
        ident += '\t'
        fw('%s<meta name="filename" content=%s />\n' % (ident, filepath_quoted))
        fw('%s<meta name="generator" content=%s />\n' % (ident, blender_ver_quoted))
        # this info was never updated, so blender version should be enough
        # fw('%s<meta name="translator" content="X3D exporter v1.55 (2006/01/17)" />\n' % ident)
        ident = ident[:-1]
        fw('%s</head>\n' % ident)
        fw('%s<Scene>\n' % ident)
        ident += '\t'
        return ident

    def writeFooter(ident):
        ident = ident[:-1]
        fw('%s</Scene>\n' % ident)
        ident = ident[:-1]
        fw('%s</X3D>' % ident)
        return ident

    def writeViewpoint(ident, obj, matrix, scene):
        view_id = quoteattr(unique_name(obj, 'CA_' + obj.name, uuid_cache_view))

        loc, quat, scale = matrix.decompose()

        ident_step = ident + (' ' * (-len(ident) + \
        fw('%s<Viewpoint ' % ident)))
        fw('DEF=%s\n' % view_id)
        fw(ident_step + 'centerOfRotation="0 0 0"\n')
        fw(ident_step + 'position="%3.2f %3.2f %3.2f"\n' % loc[:])
        fw(ident_step + 'orientation="%3.2f %3.2f %3.2f %3.2f"\n' % (quat.axis[:] + (quat.angle, )))
        fw(ident_step + 'fieldOfView="%.3g"\n' % obj.data.angle)
        fw(ident_step + '/>\n')

    def writeFog(ident, world):
        if world:
            mtype = world.mist_settings.falloff
            mparam = world.mist_settings
        else:
            return

        if mparam.use_mist:
            ident_step = ident + (' ' * (-len(ident) + \
            fw('%s<Fog ' % ident)))
            fw('fogType="%s"\n' % ('LINEAR' if (mtype == 'LINEAR') else 'EXPONENTIAL'))
            fw(ident_step + 'color="%.3g %.3g %.3g"\n' % clamp_color(world.horizon_color))
            fw(ident_step + 'visibilityRange="%.3g"\n' % mparam.depth)
            fw(ident_step + '/>\n')
        else:
            return

    def writeNavigationInfo(ident, scene):
        ident_step = ident + (' ' * (-len(ident) + \
        fw('%s<NavigationInfo ' % ident)))
        fw('headlight="false"\n')
        fw(ident_step + 'visibilityLimit="0.0"\n')
        fw(ident_step + 'type=\'"EXAMINE", "ANY"\'\n')
        fw(ident_step + 'avatarSize="0.25, 1.75, 0.75"\n')
        fw(ident_step + '/>\n')

    def writeTransform_begin(ident, matrix, def_id):
        ident_step = ident + (' ' * (-len(ident) + \
        fw('%s<Transform ' % ident)))
        if def_id is not None:
            fw('DEF=%s\n' % def_id)
        else:
            fw('\n')

        loc, quat, sca = matrix.decompose()

        fw(ident_step + 'translation="%.6g %.6g %.6g"\n' % loc[:])
        # fw(ident_step + 'center="%.6g %.6g %.6g"\n' % (0, 0, 0))
        fw(ident_step + 'scale="%.6g %.6g %.6g"\n' % sca[:])
        fw(ident_step + 'rotation="%.6g %.6g %.6g %.6g"\n' % (quat.axis[:] + (quat.angle, )))
        fw(ident_step + '>\n')
        ident += '\t'
        return ident

    def writeTransform_end(ident):
        ident = ident[:-1]
        fw('%s</Transform>\n' % ident)
        return ident

    def writeSpotLight(ident, obj, matrix, lamp, world):
        # note, lamp_id is not re-used
        lamp_id = quoteattr(unique_name(obj, 'LA_' + obj.name, uuid_cache_lamp))

        if world:
            ambi = world.ambient_color
            amb_intensity = ((ambi[0] + ambi[1] + ambi[2]) / 3.0) / 2.5
            del ambi
        else:
            amb_intensity = 0.0

        # compute cutoff and beamwidth
        intensity = min(lamp.energy / 1.75, 1.0)
        beamWidth = lamp.spot_size * 0.37
        # beamWidth=((lamp.spotSize*math.pi)/180.0)*.37
        cutOffAngle = beamWidth * 1.3

        orientation = matrix_direction_neg_z(matrix)

        location = matrix.to_translation()[:]

        radius = lamp.distance * math.cos(beamWidth)
        # radius = lamp.dist*math.cos(beamWidth)
        ident_step = ident + (' ' * (-len(ident) + \
        fw('%s<SpotLight ' % ident)))
        fw('DEF=%s\n' % lamp_id)
        fw(ident_step + 'radius="%.4g"\n' % radius)
        fw(ident_step + 'ambientIntensity="%.4g"\n' % amb_intensity)
        fw(ident_step + 'intensity="%.4g"\n' % intensity)
        fw(ident_step + 'color="%.4g %.4g %.4g"\n' % clamp_color(lamp.color))
        fw(ident_step + 'beamWidth="%.4g"\n' % beamWidth)
        fw(ident_step + 'cutOffAngle="%.4g"\n' % cutOffAngle)
        fw(ident_step + 'direction="%.4g %.4g %.4g"\n' % orientation)
        fw(ident_step + 'location="%.4g %.4g %.4g"\n' % location)
        fw(ident_step + '/>\n')

    def writeDirectionalLight(ident, obj, matrix, lamp, world):
        # note, lamp_id is not re-used
        lamp_id = quoteattr(unique_name(obj, 'LA_' + obj.name, uuid_cache_lamp))

        if world:
            ambi = world.ambient_color
            # ambi = world.amb
            amb_intensity = ((float(ambi[0] + ambi[1] + ambi[2])) / 3.0) / 2.5
        else:
            ambi = 0
            amb_intensity = 0.0

        intensity = min(lamp.energy / 1.75, 1.0)

        orientation = matrix_direction_neg_z(matrix)

        ident_step = ident + (' ' * (-len(ident) + \
        fw('%s<DirectionalLight ' % ident)))
        fw('DEF=%s\n' % lamp_id)
        fw(ident_step + 'ambientIntensity="%.4g"\n' % amb_intensity)
        fw(ident_step + 'color="%.4g %.4g %.4g"\n' % clamp_color(lamp.color))
        fw(ident_step + 'intensity="%.4g"\n' % intensity)
        fw(ident_step + 'direction="%.4g %.4g %.4g"\n' % orientation)
        fw(ident_step + '/>\n')

    def writePointLight(ident, obj, matrix, lamp, world):
        # note, lamp_id is not re-used
        lamp_id = quoteattr(unique_name(obj, 'LA_' + obj.name, uuid_cache_lamp))

        if world:
            ambi = world.ambient_color
            # ambi = world.amb
            amb_intensity = ((float(ambi[0] + ambi[1] + ambi[2])) / 3.0) / 2.5
        else:
            ambi = 0.0
            amb_intensity = 0.0

        intensity = min(lamp.energy / 1.75, 1.0)
        location = matrix.to_translation()[:]

        ident_step = ident + (' ' * (-len(ident) + \
        fw('%s<PointLight ' % ident)))
        fw('DEF=%s\n' % lamp_id)
        fw(ident_step + 'ambientIntensity="%.4g"\n' % amb_intensity)
        fw(ident_step + 'color="%.4g %.4g %.4g"\n' % clamp_color(lamp.color))

        fw(ident_step + 'intensity="%.4g"\n' % intensity)
        fw(ident_step + 'radius="%.4g" \n' % lamp.distance)
        fw(ident_step + 'location="%.4g %.4g %.4g"\n' % location)
        fw(ident_step + '/>\n')

    def writeIndexedFaceSet(ident, obj, mesh, matrix, world):
        obj_id = quoteattr(unique_name(obj, 'OB_' + obj.name, uuid_cache_object))
        mesh_id = quoteattr(unique_name(mesh, 'ME_' + mesh.name, uuid_cache_mesh))
        mesh_id_group = prefix_quoted_str(mesh_id, 'group_')
        mesh_id_coords = prefix_quoted_str(mesh_id, 'coords_')
        mesh_id_normals = prefix_quoted_str(mesh_id, 'normals_')

        if not mesh.faces:
            return

        texface_use_halo = 0
        texface_use_billboard = 0
        texface_use_collision = 0

        use_halonode = False
        use_billnode = False
        use_collnode = False

        if mesh.uv_textures.active:  # if mesh.faceUV:
            for face in mesh.uv_textures.active.data:  # for face in mesh.faces:
                texface_use_halo |= face.use_halo
                texface_use_billboard |= face.use_billboard
                texface_use_collision |= face.use_collision
                # texface_use_object_color |= face.use_object_color

        if texface_use_halo:
            fw('%s<Billboard axisOfRotation="0 0 0">\n' % ident)
            use_halonode = True
            ident += '\t'
        elif texface_use_billboard:
            fw('%s<Billboard axisOfRotation="0 1 0">\n' % ident)
            use_billnode = True
            ident += '\t'
        elif texface_use_collision:
            fw('%s<Collision enabled="false">\n' % ident)
            use_collnode = True
            ident += '\t'

        del texface_use_halo
        del texface_use_billboard
        del texface_use_collision
        # del texface_use_object_color

        ident = writeTransform_begin(ident, matrix, None)

        if mesh.tag:
            fw('%s<Group USE=%s />\n' % (ident, mesh_id_group))
        else:
            mesh.tag = True

            fw('%s<Group DEF=%s>\n' % (ident, mesh_id_group))
            ident += '\t'

            is_uv = bool(mesh.uv_textures.active)
            # is_col, defined for each material

            is_coords_written = False

            mesh_materials = mesh.materials[:]
            if not mesh_materials:
                mesh_materials = [None]

            mesh_material_tex = [None] * len(mesh_materials)
            mesh_material_mtex = [None] * len(mesh_materials)
            mesh_material_images = [None] * len(mesh_materials)

            for i, material in enumerate(mesh_materials):
                if material:
                    for mtex in material.texture_slots:
                        if mtex:
                            tex = mtex.texture
                            if tex and tex.type == 'IMAGE':
                                image = tex.image
                                if image:
                                    mesh_material_tex[i] = tex
                                    mesh_material_mtex[i] = mtex
                                    mesh_material_images[i] = image
                                    break

            mesh_materials_use_face_texture = [getattr(material, 'use_face_texture', True) for material in mesh_materials]

            # fast access!
            mesh_vertices = mesh.vertices[:]
            mesh_faces = mesh.faces[:]
            mesh_faces_materials = [f.material_index for f in mesh_faces]
            mesh_faces_vertices = [f.vertices[:] for f in mesh_faces]

            if is_uv and True in mesh_materials_use_face_texture:
                mesh_faces_image = [(fuv.image if (mesh_materials_use_face_texture[mesh_faces_materials[i]] and fuv.use_image) else mesh_material_images[mesh_faces_materials[i]]) for i, fuv in enumerate(mesh.uv_textures.active.data)]
                mesh_faces_image_unique = set(mesh_faces_image)
            elif len(set(mesh_material_images) | {None}) > 1:  # make sure there is at least one image
                mesh_faces_image = [mesh_material_images[material_index] for material_index in mesh_faces_materials]
                mesh_faces_image_unique = set(mesh_faces_image)
            else:
                mesh_faces_image = [None] * len(mesh_faces)
                mesh_faces_image_unique = {None}

            # group faces
            face_groups = {}
            for material_index in range(len(mesh_materials)):
                for image in mesh_faces_image_unique:
                    face_groups[material_index, image] = []
            del mesh_faces_image_unique

            for i, (material_index, image) in enumerate(zip(mesh_faces_materials, mesh_faces_image)):
                face_groups[material_index, image].append(i)

            # same as face_groups.items() but sorted so we can get predictable output.
            face_groups_items = list(face_groups.items())
            face_groups_items.sort(key=lambda m: (m[0][0], getattr(m[0][1], 'name', '')))

            for (material_index, image), face_group in face_groups_items:  # face_groups.items()
                if face_group:
                    material = mesh_materials[material_index]

                    fw('%s<Shape>\n' % ident)
                    ident += '\t'

                    is_smooth = False
                    is_col = (mesh.vertex_colors.active and (material is None or material.use_vertex_color_paint))

                    # kludge but as good as it gets!
                    for i in face_group:
                        if mesh_faces[i].use_smooth:
                            is_smooth = True
                            break

                    # UV's and VCols split verts off which effects smoothing
                    # force writing normals in this case.
                    is_force_normals = use_triangulate and is_smooth and (is_uv or is_col)

                    if use_h3d:
                        gpu_shader = gpu_shader_cache.get(material)  # material can be 'None', uses dummy cache
                        if gpu_shader is None:
                            gpu_shader = gpu_shader_cache[material] = gpu.export_shader(scene, material)

                            if 1:  # XXX DEBUG
                                gpu_shader_tmp = gpu.export_shader(scene, material)
                                import pprint
                                print('\nWRITING MATERIAL:', material.name)
                                del gpu_shader_tmp['fragment']
                                del gpu_shader_tmp['vertex']
                                pprint.pprint(gpu_shader_tmp, width=120)
                                #pprint.pprint(val['vertex'])
                                del gpu_shader_tmp

                    fw('%s<Appearance>\n' % ident)
                    ident += '\t'

                    if image and not use_h3d:
                        writeImageTexture(ident, image)

                        if mesh_materials_use_face_texture[material_index]:
                            if image.use_tiles:
                                fw('%s<TextureTransform scale="%s %s" />\n' % (ident, image.tiles_x, image.tiles_y))
                        else:
                            # transform by mtex
                            loc = mesh_material_mtex[material_index].offset[:2]

                            # mtex_scale * tex_repeat
                            sca_x, sca_y = mesh_material_mtex[material_index].scale[:2]

                            sca_x *= mesh_material_tex[material_index].repeat_x
                            sca_y *= mesh_material_tex[material_index].repeat_y

                            # flip x/y is a sampling feature, convert to transform
                            if mesh_material_tex[material_index].use_flip_axis:
                                rot = math.pi / -2.0
                                sca_x, sca_y = sca_y, -sca_x
                            else:
                                rot = 0.0

                            ident_step = ident + (' ' * (-len(ident) + \
                            fw('%s<TextureTransform ' % ident)))
                            # fw('center="%.6g %.6g" ' % (0.0, 0.0))
                            fw(ident_step + 'translation="%.6g %.6g"\n' % loc)
                            fw(ident_step + 'scale="%.6g %.6g"\n' % (sca_x, sca_y))
                            fw(ident_step + 'rotation="%.6g"\n' % rot)
                            fw(ident_step + '/>\n')

                    if use_h3d:
                        mat_tmp = material if material else gpu_shader_dummy_mat
                        writeMaterialH3D(ident, mat_tmp, world,
                                         obj, gpu_shader)
                        del mat_tmp
                    else:
                        if material:
                            writeMaterial(ident, material, world)

                    ident = ident[:-1]
                    fw('%s</Appearance>\n' % ident)

                    mesh_faces_col = mesh.vertex_colors.active.data if is_col else None
                    mesh_faces_uv = mesh.uv_textures.active.data if is_uv else None

                    #-- IndexedFaceSet or IndexedLineSet
                    if use_triangulate:
                        ident_step = ident + (' ' * (-len(ident) + \
                        fw('%s<IndexedTriangleSet ' % ident)))

                        # --- Write IndexedTriangleSet Attributes (same as IndexedFaceSet)
                        fw('solid="%s"\n' % ('true' if mesh.show_double_sided else 'false'))

                        # creaseAngle unsupported for IndexedTriangleSet's

                        if use_normals or is_force_normals:
                            # currently not optional, could be made so:
                            fw(ident_step + 'normalPerVertex="true"\n')

                        slot_uv = None
                        slot_col = None

                        if is_uv and is_col:
                            slot_uv = 0
                            slot_col = 1

                            def vertex_key(fidx, f_cnr_idx):
                                return (
                                    mesh_faces_uv[fidx].uv[f_cnr_idx][:],
                                    getattr(mesh_faces_col[fidx], "color%d" % (f_cnr_idx + 1))[:],
                                )
                        elif is_uv:
                            slot_uv = 0

                            def vertex_key(fidx, f_cnr_idx):
                                return (
                                    mesh_faces_uv[fidx].uv[f_cnr_idx][:],
                                )
                        elif is_col:
                            slot_col = 0

                            def vertex_key(fidx, f_cnr_idx):
                                return (
                                    getattr(mesh_faces_col[fidx], "color%d" % (f_cnr_idx + 1))[:],
                                )
                        else:
                            # ack, not especially efficient in this case
                            def vertex_key(fidx, f_cnr_idx):
                                return None

                        # build a mesh mapping dict
                        vertex_hash = [{} for i in range(len(mesh.vertices))]
                        # worst case every face is a quad
                        face_tri_list = [[None, None, None] for i in range(len(mesh.faces) * 2)]
                        vert_tri_list = []
                        totvert = 0
                        totface = 0
                        temp_face = [None] * 4
                        for i in face_group:
                            fv = mesh_faces_vertices[i]
                            for j, v_idx in enumerate(fv):
                                key = vertex_key(i, j)
                                vh = vertex_hash[v_idx]
                                x3d_v = vh.get(key)
                                if x3d_v is None:
                                    x3d_v = key, v_idx, totvert
                                    vh[key] = x3d_v
                                    # key / original_vertex / new_vertex
                                    vert_tri_list.append(x3d_v)
                                    totvert += 1
                                temp_face[j] = x3d_v

                            if len(fv) == 4:
                                f_iter = ((0, 1, 2), (0, 2, 3))
                            else:
                                f_iter = ((0, 1, 2), )

                            for f_it in f_iter:
                                # loop over a quad as 2 tris
                                f_tri = face_tri_list[totface]
                                for ji, j in enumerate(f_it):
                                    f_tri[ji] = temp_face[j]
                                # quads run this twice
                                totface += 1

                        # clear unused faces
                        face_tri_list[totface:] = []

                        fw(ident_step + 'index="')
                        for x3d_f in face_tri_list:
                            fw('%i %i %i ' % (x3d_f[0][2], x3d_f[1][2], x3d_f[2][2]))
                        fw('"\n')

                        # close IndexedTriangleSet
                        fw(ident_step + '>\n')
                        ident += '\t'

                        fw('%s<Coordinate ' % ident)
                        fw('point="')
                        for x3d_v in vert_tri_list:
                            fw('%.6g %.6g %.6g ' % mesh_vertices[x3d_v[1]].co[:])
                        fw('" />\n')

                        if use_normals or is_force_normals:
                            fw('%s<Normal ' % ident)
                            fw('vector="')
                            for x3d_v in vert_tri_list:
                                fw('%.6g %.6g %.6g ' % mesh_vertices[x3d_v[1]].normal[:])
                            fw('" />\n')

                        if is_uv:
                            fw('%s<TextureCoordinate point="' % ident)
                            for x3d_v in vert_tri_list:
                                fw('%.4g %.4g ' % x3d_v[0][slot_uv])
                            fw('" />\n')

                        if is_col:
                            fw('%s<Color color="' % ident)
                            for x3d_v in vert_tri_list:
                                fw('%.3g %.3g %.3g ' % x3d_v[0][slot_col])
                            fw('" />\n')

                        if use_h3d:
                            # write attributes
                            for gpu_attr in gpu_shader['attributes']:

                                # UVs
                                if gpu_attr['type'] == gpu.CD_MTFACE:
                                    if gpu_attr['datatype'] == gpu.GPU_DATA_2F:
                                        fw('%s<FloatVertexAttribute ' % ident)
                                        fw('name="%s" ' % gpu_attr['varname'])
                                        fw('numComponents="2" ')
                                        fw('value="')
                                        for x3d_v in vert_tri_list:
                                            fw('%.4g %.4g ' % x3d_v[0][slot_uv])
                                        fw('" />\n')
                                    else:
                                        assert(0)

                                elif gpu_attr['type'] == gpu.CD_MCOL:
                                    if gpu_attr['datatype'] == gpu.GPU_DATA_4UB:
                                        pass  # XXX, H3D can't do
                                    else:
                                        assert(0)

                        ident = ident[:-1]

                        fw('%s</IndexedTriangleSet>\n' % ident)

                    else:
                        ident_step = ident + (' ' * (-len(ident) + \
                        fw('%s<IndexedFaceSet ' % ident)))

                        # --- Write IndexedFaceSet Attributes (same as IndexedTriangleSet)
                        fw('solid="%s"\n' % ('true' if mesh.show_double_sided else 'false'))
                        if is_smooth:
                            fw(ident_step + 'creaseAngle="%.4g"\n' % mesh.auto_smooth_angle)

                        if use_normals:
                            # currently not optional, could be made so:
                            fw(ident_step + 'normalPerVertex="true"\n')

                        # IndexedTriangleSet assumes true
                        if is_col:
                            fw(ident_step + 'colorPerVertex="false"\n')

                        # for IndexedTriangleSet we use a uv per vertex so this isnt needed.
                        if is_uv:
                            fw(ident_step + 'texCoordIndex="')

                            j = 0
                            for i in face_group:
                                if len(mesh_faces_vertices[i]) == 4:
                                    fw('%d %d %d %d -1 ' % (j, j + 1, j + 2, j + 3))
                                    j += 4
                                else:
                                    fw('%d %d %d -1 ' % (j, j + 1, j + 2))
                                    j += 3
                            fw('"\n')
                            # --- end texCoordIndex

                        if True:
                            fw(ident_step + 'coordIndex="')
                            for i in face_group:
                                fv = mesh_faces_vertices[i]
                                if len(fv) == 3:
                                    fw('%i %i %i -1 ' % fv)
                                else:
                                    fw('%i %i %i %i -1 ' % fv)

                            fw('"\n')
                            # --- end coordIndex

                        # close IndexedFaceSet
                        fw(ident_step + '>\n')
                        ident += '\t'

                        # --- Write IndexedFaceSet Elements
                        if True:
                            if is_coords_written:
                                fw('%s<Coordinate USE=%s />\n' % (ident, mesh_id_coords))
                                if use_normals:
                                    fw('%s<Normal USE=%s />\n' % (ident, mesh_id_normals))
                            else:
                                ident_step = ident + (' ' * (-len(ident) + \
                                fw('%s<Coordinate ' % ident)))
                                fw('DEF=%s\n' % mesh_id_coords)
                                fw(ident_step + 'point="')
                                for v in mesh.vertices:
                                    fw('%.6g %.6g %.6g ' % v.co[:])
                                fw('"\n')
                                fw(ident_step + '/>\n')

                                is_coords_written = True

                                if use_normals:
                                    ident_step = ident + (' ' * (-len(ident) + \
                                    fw('%s<Normal ' % ident)))
                                    fw('DEF=%s\n' % mesh_id_normals)
                                    fw(ident_step + 'vector="')
                                    for v in mesh.vertices:
                                        fw('%.6g %.6g %.6g ' % v.normal[:])
                                    fw('"\n')
                                    fw(ident_step + '/>\n')

                        if is_uv:
                            fw('%s<TextureCoordinate point="' % ident)
                            for i in face_group:
                                for uv in mesh_faces_uv[i].uv:
                                    fw('%.4g %.4g ' % uv[:])
                            del mesh_faces_uv
                            fw('" />\n')

                        if is_col:
                            fw('%s<Color color="' % ident)
                            # XXX, 1 color per face, only
                            for i in face_group:
                                fw('%.3g %.3g %.3g ' % mesh_faces_col[i].color1[:])
                            fw('" />\n')

                        #--- output vertexColors

                        #--- output closing braces
                        ident = ident[:-1]

                        fw('%s</IndexedFaceSet>\n' % ident)

                    ident = ident[:-1]
                    fw('%s</Shape>\n' % ident)

                    # XXX

            #fw('%s<PythonScript DEF="PS" url="object.py" >\n' % ident)
            #fw('%s    <ShaderProgram USE="MA_Material.005" containerField="references"/>\n' % ident)
            #fw('%s</PythonScript>\n' % ident)

            ident = ident[:-1]
            fw('%s</Group>\n' % ident)

        ident = writeTransform_end(ident)

        if use_halonode:
            ident = ident[:-1]
            fw('%s</Billboard>\n' % ident)
        elif use_billnode:
            ident = ident[:-1]
            fw('%s</Billboard>\n' % ident)
        elif use_collnode:
            ident = ident[:-1]
            fw('%s</Collision>\n' % ident)

    def writeMaterial(ident, material, world):
        material_id = quoteattr(unique_name(material, 'MA_' + material.name, uuid_cache_material))

        # look up material name, use it if available
        if material.tag:
            fw('%s<Material USE=%s />\n' % (ident, material_id))
        else:
            material.tag = True

            emit = material.emit
            ambient = material.ambient / 3.0
            diffuseColor = material.diffuse_color[:]
            if world:
                ambiColor = ((material.ambient * 2.0) * world.ambient_color)[:]
            else:
                ambiColor = 0.0, 0.0, 0.0

            emitColor = tuple(((c * emit) + ambiColor[i]) / 2.0 for i, c in enumerate(diffuseColor))
            shininess = material.specular_hardness / 512.0
            specColor = tuple((c + 0.001) / (1.25 / (material.specular_intensity + 0.001)) for c in material.specular_color)
            transp = 1.0 - material.alpha

            if material.use_shadeless:
                ambient = 1.0
                shininess = 0.0
                specColor = emitColor = diffuseColor

            ident_step = ident + (' ' * (-len(ident) + \
            fw('%s<Material ' % ident)))
            fw('DEF=%s\n' % material_id)
            fw(ident_step + 'diffuseColor="%.3g %.3g %.3g"\n' % clamp_color(diffuseColor))
            fw(ident_step + 'specularColor="%.3g %.3g %.3g"\n' % clamp_color(specColor))
            fw(ident_step + 'emissiveColor="%.3g %.3g %.3g"\n' % clamp_color(emitColor))
            fw(ident_step + 'ambientIntensity="%.3g"\n' % ambient)
            fw(ident_step + 'shininess="%.3g"\n' % shininess)
            fw(ident_step + 'transparency="%s"\n' % transp)
            fw(ident_step + '/>\n')

    def writeMaterialH3D(ident, material, world,
                         obj, gpu_shader):
        material_id = quoteattr(unique_name(material, 'MA_' + material.name, uuid_cache_material))

        fw('%s<Material />\n' % ident)
        if material.tag:
            fw('%s<ComposedShader USE=%s />\n' % (ident, material_id))
        else:
            material.tag = True

            # GPU_material_bind_uniforms
            # GPU_begin_object_materials

            #~ CD_MCOL 6
            #~ CD_MTFACE 5
            #~ CD_ORCO 14
            #~ CD_TANGENT 18
            #~ GPU_DATA_16F 7
            #~ GPU_DATA_1F 2
            #~ GPU_DATA_1I 1
            #~ GPU_DATA_2F 3
            #~ GPU_DATA_3F 4
            #~ GPU_DATA_4F 5
            #~ GPU_DATA_4UB 8
            #~ GPU_DATA_9F 6
            #~ GPU_DYNAMIC_LAMP_DYNCO 7
            #~ GPU_DYNAMIC_LAMP_DYNCOL 11
            #~ GPU_DYNAMIC_LAMP_DYNENERGY 10
            #~ GPU_DYNAMIC_LAMP_DYNIMAT 8
            #~ GPU_DYNAMIC_LAMP_DYNPERSMAT 9
            #~ GPU_DYNAMIC_LAMP_DYNVEC 6
            #~ GPU_DYNAMIC_OBJECT_COLOR 5
            #~ GPU_DYNAMIC_OBJECT_IMAT 4
            #~ GPU_DYNAMIC_OBJECT_MAT 2
            #~ GPU_DYNAMIC_OBJECT_VIEWIMAT 3
            #~ GPU_DYNAMIC_OBJECT_VIEWMAT 1
            #~ GPU_DYNAMIC_SAMPLER_2DBUFFER 12
            #~ GPU_DYNAMIC_SAMPLER_2DIMAGE 13
            #~ GPU_DYNAMIC_SAMPLER_2DSHADOW 14

            '''
            inline const char* typeToString( X3DType t ) {
              switch( t ) {
              case     SFFLOAT: return "SFFloat";
              case     MFFLOAT: return "MFFloat";
              case    SFDOUBLE: return "SFDouble";
              case    MFDOUBLE: return "MFDouble";
              case      SFTIME: return "SFTime";
              case      MFTIME: return "MFTime";
              case     SFINT32: return "SFInt32";
              case     MFINT32: return "MFInt32";
              case     SFVEC2F: return "SFVec2f";
              case     MFVEC2F: return "MFVec2f";
              case     SFVEC2D: return "SFVec2d";
              case     MFVEC2D: return "MFVec2d";
              case     SFVEC3F: return "SFVec3f";
              case     MFVEC3F: return "MFVec3f";
              case     SFVEC3D: return "SFVec3d";
              case     MFVEC3D: return "MFVec3d";
              case     SFVEC4F: return "SFVec4f";
              case     MFVEC4F: return "MFVec4f";
              case     SFVEC4D: return "SFVec4d";
              case     MFVEC4D: return "MFVec4d";
              case      SFBOOL: return "SFBool";
              case      MFBOOL: return "MFBool";
              case    SFSTRING: return "SFString";
              case    MFSTRING: return "MFString";
              case      SFNODE: return "SFNode";
              case      MFNODE: return "MFNode";
              case     SFCOLOR: return "SFColor";
              case     MFCOLOR: return "MFColor";
              case SFCOLORRGBA: return "SFColorRGBA";
              case MFCOLORRGBA: return "MFColorRGBA";
              case  SFROTATION: return "SFRotation";
              case  MFROTATION: return "MFRotation";
              case  SFQUATERNION: return "SFQuaternion";
              case  MFQUATERNION: return "MFQuaternion";
              case  SFMATRIX3F: return "SFMatrix3f";
              case  MFMATRIX3F: return "MFMatrix3f";
              case  SFMATRIX4F: return "SFMatrix4f";
              case  MFMATRIX4F: return "MFMatrix4f";
              case  SFMATRIX3D: return "SFMatrix3d";
              case  MFMATRIX3D: return "MFMatrix3d";
              case  SFMATRIX4D: return "SFMatrix4d";
              case  MFMATRIX4D: return "MFMatrix4d";
              case UNKNOWN_X3D_TYPE:
              default:return "UNKNOWN_X3D_TYPE";
            '''
            import gpu

            fw('%s<ComposedShader DEF=%s language="GLSL" >\n' % (ident, material_id))
            ident += '\t'

            shader_url_frag = 'shaders/%s_%s.frag' % (filename_strip, material_id[1:-1])
            shader_url_vert = 'shaders/%s_%s.vert' % (filename_strip, material_id[1:-1])

            # write files
            shader_dir = os.path.join(base_dst, 'shaders')
            if not os.path.isdir(shader_dir):
                os.mkdir(shader_dir)

            for uniform in gpu_shader['uniforms']:
                if uniform['type'] == gpu.GPU_DYNAMIC_SAMPLER_2DIMAGE:
                    fw('%s<field name="%s" type="SFNode" accessType="inputOutput">\n' % (ident, uniform['varname']))
                    writeImageTexture(ident + '\t', bpy.data.images[uniform['image']])
                    fw('%s</field>\n' % ident)

                elif uniform['type'] == gpu.GPU_DYNAMIC_LAMP_DYNCO:
                    if uniform['datatype'] == gpu.GPU_DATA_3F:  # should always be true!
                        value = '%.6g   %.6g   %.6g' % (global_matrix * bpy.data.objects[uniform['lamp']].matrix_world).to_translation()[:]
                        fw('%s<field name="%s" type="SFVec3f" accessType="inputOutput" value="%s" />\n' % (ident, uniform['varname'], value))
                    else:
                        assert(0)

                elif uniform['type'] == gpu.GPU_DYNAMIC_LAMP_DYNCOL:
                    # odd  we have both 3, 4 types.
                    lamp = bpy.data.objects[uniform['lamp']].data
                    value = '%.6g %.6g %.6g' % (lamp.color * lamp.energy)[:]
                    if uniform['datatype'] == gpu.GPU_DATA_3F:
                        fw('%s<field name="%s" type="SFVec3f" accessType="inputOutput" value="%s" />\n' % (ident, uniform['varname'], value))
                    elif uniform['datatype'] == gpu.GPU_DATA_4F:
                        fw('%s<field name="%s" type="SFVec4f" accessType="inputOutput" value="%s 1.0" />\n' % (ident, uniform['varname'], value))
                    else:
                        assert(0)

                elif uniform['type'] == gpu.GPU_DYNAMIC_LAMP_DYNENERGY:
                    # not used ?
                    assert(0)

                elif uniform['type'] == gpu.GPU_DYNAMIC_LAMP_DYNVEC:
                    if uniform['datatype'] == gpu.GPU_DATA_3F:
                        value = '%.6g %.6g %.6g' % (mathutils.Vector((0.0, 0.0, 1.0)) * (global_matrix * bpy.data.objects[uniform['lamp']].matrix_world).to_quaternion()).normalized()[:]
                        fw('%s<field name="%s" type="SFVec3f" accessType="inputOutput" value="%s" />\n' % (ident, uniform['varname'], value))
                    else:
                        assert(0)

                elif uniform['type'] == gpu.GPU_DYNAMIC_OBJECT_VIEWIMAT:
                    if uniform['datatype'] == gpu.GPU_DATA_16F:
                        # must be updated dynamically
                        # TODO, write out 'viewpointMatrices.py'
                        value = ' '.join(['%.6f' % f for v in mathutils.Matrix() for f in v])
                        fw('%s<field name="%s" type="SFMatrix4f" accessType="inputOutput" value="%s" />\n' % (ident, uniform['varname'], value))
                    else:
                        assert(0)

                elif uniform['type'] == gpu.GPU_DYNAMIC_OBJECT_IMAT:
                    if uniform['datatype'] == gpu.GPU_DATA_16F:
                        value = ' '.join(['%.6f' % f for v in (global_matrix * obj.matrix_world).inverted() for f in v])
                        fw('%s<field name="%s" type="SFMatrix4f" accessType="inputOutput" value="%s" />\n' % (ident, uniform['varname'], value))
                    else:
                        assert(0)

                elif uniform['type'] == gpu.GPU_DYNAMIC_SAMPLER_2DSHADOW:
                    pass  # XXX, shadow buffers not supported.

                elif uniform['type'] == gpu.GPU_DYNAMIC_SAMPLER_2DBUFFER:
                    if uniform['datatype'] == gpu.GPU_DATA_1I:
                        if 1:
                            tex = uniform['texpixels']
                            value = []
                            for i in range(0, len(tex) - 1, 4):
                                col = tex[i:i + 4]
                                value.append('0x%.2x%.2x%.2x%.2x' % (col[0], col[1], col[2], col[3]))

                            fw('%s<field name="%s" type="SFNode" accessType="inputOutput">\n' % (ident, uniform['varname']))

                            ident += '\t'

                            ident_step = ident + (' ' * (-len(ident) + \
                            fw('%s<PixelTexture \n' % ident)))
                            fw(ident_step + 'repeatS="false"\n')
                            fw(ident_step + 'repeatT="false"\n')

                            fw(ident_step + 'image="%s 1 4 %s"\n' % (len(value), " ".join(value)))

                            fw(ident_step + '/>\n')

                            ident = ident[:-1]

                            fw('%s</field>\n' % ident)

                            #for i in range(0, 10, 4)
                            #value = ' '.join(['%d' % f for f in uniform['texpixels']])
                            # value = ' '.join(['%.6g' % (f / 256) for f in uniform['texpixels']])

                            #fw('%s<field name="%s" type="SFInt32" accessType="inputOutput" value="%s" />\n' % (ident, uniform['varname'], value))
                            #print('test', len(uniform['texpixels']))
                    else:
                        assert(0)
                else:
                    print("SKIPPING", uniform['type'])

            file_frag = open(os.path.join(base_dst, shader_url_frag), 'w')
            file_frag.write(gpu_shader['fragment'])
            file_frag.close()
            # patch it
            h3d_shader_glsl_frag_patch(os.path.join(base_dst, shader_url_frag))

            file_vert = open(os.path.join(base_dst, shader_url_vert), 'w')
            file_vert.write(gpu_shader['vertex'])
            file_vert.close()

            fw('%s<ShaderPart type="FRAGMENT" url="%s" />\n' % (ident, shader_url_frag))
            fw('%s<ShaderPart type="VERTEX" url="%s" />\n' % (ident, shader_url_vert))
            ident = ident[:-1]

            fw('%s</ComposedShader>\n' % ident)

    def writeImageTexture(ident, image):
        image_id = quoteattr(unique_name(image, 'IM_' + image.name, uuid_cache_image))

        if image.tag:
            fw('%s<ImageTexture USE=%s />\n' % (ident, image_id))
        else:
            image.tag = True

            ident_step = ident + (' ' * (-len(ident) + \
            fw('%s<ImageTexture ' % ident)))
            fw('DEF=%s\n' % image_id)

            # collect image paths, can load multiple
            # [relative, name-only, absolute]
            filepath = image.filepath
            filepath_full = bpy.path.abspath(filepath)
            filepath_ref = bpy_extras.io_utils.path_reference(filepath_full, base_src, base_dst, path_mode, "textures", copy_set)
            filepath_base = os.path.basename(filepath_ref)

            images = [
                filepath_base,
                filepath_ref,
                filepath_full,
            ]

            images = [f.replace('\\', '/') for f in images]
            images = [f for i, f in enumerate(images) if f not in images[:i]]

            fw(ident_step + "url='%s' " % ' '.join(['"%s"' % f for f in images]))
            fw(ident_step + '/>\n')

    def writeBackground(ident, world):

        if world is None:
            return

        # note, not re-used
        world_id = quoteattr(unique_name(world, 'WO_' + world.name, uuid_cache_world))

        blending = world.use_sky_blend, world.use_sky_paper, world.use_sky_real

        grd_triple = clamp_color(world.horizon_color)
        sky_triple = clamp_color(world.zenith_color)
        mix_triple = clamp_color((grd_triple[i] + sky_triple[i]) / 2.0 for i in range(3))

        ident_step = ident + (' ' * (-len(ident) + \
        fw('%s<Background ' % ident)))
        fw('DEF=%s\n' % world_id)
        # No Skytype - just Hor color
        if blending == (False, False, False):
            fw(ident_step + 'groundColor="%.3g %.3g %.3g"\n' % grd_triple)
            fw(ident_step + 'skyColor="%.3g %.3g %.3g"\n' % grd_triple)
        # Blend Gradient
        elif blending == (True, False, False):
            fw(ident_step + 'groundColor="%.3g %.3g %.3g, %.3g %.3g %.3g"\n' % (grd_triple + mix_triple))
            fw(ident_step + 'groundAngle="1.57, 1.57"\n')
            fw(ident_step + 'skyColor="%.3g %.3g %.3g, %.3g %.3g %.3g"\n' % (sky_triple + mix_triple))
            fw(ident_step + 'skyAngle="1.57, 1.57"\n')
        # Blend+Real Gradient Inverse
        elif blending == (True, False, True):
            fw(ident_step + 'groundColor="%.3g %.3g %.3g, %.3g %.3g %.3g"\n' % (sky_triple + grd_triple))
            fw(ident_step + 'groundAngle="1.57"\n')
            fw(ident_step + 'skyColor="%.3g %.3g %.3g, %.3g %.3g %.3g, %.3g %.3g %.3g"\n' % (sky_triple + grd_triple + sky_triple))
            fw(ident_step + 'skyAngle="1.57, 3.14159"\n')
        # Paper - just Zen Color
        elif blending == (False, False, True):
            fw(ident_step + 'groundColor="%.3g %.3g %.3g"\n' % sky_triple)
            fw(ident_step + 'skyColor="%.3g %.3g %.3g"\n' % sky_triple)
        # Blend+Real+Paper - komplex gradient
        elif blending == (True, True, True):
            fw(ident_step + 'groundColor="%.3g %.3g %.3g, %.3g %.3g %.3g"\n' % (sky_triple + grd_triple))
            fw(ident_step + 'groundAngle="1.57, 1.57"\n')
            fw(ident_step + 'skyColor="%.3g %.3g %.3g, %.3g %.3g %.3g"\n' % (sky_triple + grd_triple))
            fw(ident_step + 'skyAngle="1.57, 1.57"\n')
        # Any Other two colors
        else:
            fw(ident_step + 'groundColor="%.3g %.3g %.3g"\n' % grd_triple)
            fw(ident_step + 'skyColor="%.3g %.3g %.3g"\n' % sky_triple)

        for tex in bpy.data.textures:
            if tex.type == 'IMAGE' and tex.image:
                namemat = tex.name
                pic = tex.image
                basename = os.path.basename(bpy.path.abspath(pic.filepath))

                if namemat == 'back':
                    fw(ident_step + 'backUrl="%s"\n' % basename)
                elif namemat == 'bottom':
                    fw(ident_step + 'bottomUrl="%s"\n' % basename)
                elif namemat == 'front':
                    fw(ident_step + 'frontUrl="%s"\n' % basename)
                elif namemat == 'left':
                    fw(ident_step + 'leftUrl="%s"\n' % basename)
                elif namemat == 'right':
                    fw(ident_step + 'rightUrl="%s"\n' % basename)
                elif namemat == 'top':
                    fw(ident_step + 'topUrl="%s"\n' % basename)

        fw(ident_step + '/>\n')

    # -------------------------------------------------------------------------
    # Export Object Hierarchy (recursively called)
    # -------------------------------------------------------------------------
    def export_object(ident, obj_main_parent, obj_main, obj_children):
        world = scene.world
        free, derived = create_derived_objects(scene, obj_main)

        if derived is None:
            return

        if use_hierarchy:
            obj_main_matrix_world = obj_main.matrix_world
            if obj_main_parent:
                obj_main_matrix = obj_main_parent.matrix_world.inverted() * obj_main_matrix_world
            else:
                obj_main_matrix = obj_main_matrix_world
            obj_main_matrix_world_invert = obj_main_matrix_world.inverted()

            obj_main_id = quoteattr(unique_name(obj_main, obj_main.name, uuid_cache_object))

            ident = writeTransform_begin(ident, obj_main_matrix if obj_main_parent else global_matrix * obj_main_matrix, obj_main_id)

        for obj, obj_matrix in derived:
            obj_type = obj.type

            if use_hierarchy:
                # make transform node relative
                obj_matrix = obj_main_matrix_world_invert * obj_matrix

            if obj_type == 'CAMERA':
                writeViewpoint(ident, obj, obj_matrix, scene)
            elif obj_type in ('MESH', 'CURVE', 'SURF', 'FONT'):
                if (obj_type != 'MESH') or (use_apply_modifiers and obj.is_modified(scene, 'PREVIEW')):
                    try:
                        me = obj.to_mesh(scene, use_apply_modifiers, 'PREVIEW')
                    except:
                        me = None
                else:
                    me = obj.data

                if me is not None:
                    writeIndexedFaceSet(ident, obj, me, obj_matrix, world)

                    # free mesh created with create_mesh()
                    if me != obj.data:
                        bpy.data.meshes.remove(me)

            elif obj_type == 'LAMP':
                data = obj.data
                datatype = data.type
                if datatype == 'POINT':
                    writePointLight(ident, obj, obj_matrix, data, world)
                elif datatype == 'SPOT':
                    writeSpotLight(ident, obj, obj_matrix, data, world)
                elif datatype == 'SUN':
                    writeDirectionalLight(ident, obj, obj_matrix, data, world)
                else:
                    writeDirectionalLight(ident, obj, obj_matrix, data, world)
            else:
                #print "Info: Ignoring [%s], object type [%s] not handle yet" % (object.name,object.getType)
                pass

        if free:
            free_derived_objects(obj_main)

        # ---------------------------------------------------------------------
        # write out children recursively
        # ---------------------------------------------------------------------
        for obj_child, obj_child_children in obj_children:
            export_object(ident, obj_main, obj_child, obj_child_children)

        if use_hierarchy:
            ident = writeTransform_end(ident)

    # -------------------------------------------------------------------------
    # Main Export Function
    # -------------------------------------------------------------------------
    def export_main():
        world = scene.world

        # tag un-exported IDs
        bpy.data.meshes.tag(False)
        bpy.data.materials.tag(False)
        bpy.data.images.tag(False)

        print('Info: starting X3D export to %r...' % file.name)
        ident = ''
        ident = writeHeader(ident)

        writeNavigationInfo(ident, scene)
        writeBackground(ident, world)
        writeFog(ident, world)

        ident = '\t\t'

        if use_selection:
            objects = [obj for obj in scene.objects if obj.is_visible(scene) and obj.select]
        else:
            objects = [obj for obj in scene.objects if obj.is_visible(scene)]

        if use_hierarchy:
            objects_hierarchy = build_hierarchy(objects)
        else:
            objects_hierarchy = ((obj, []) for obj in objects)

        for obj_main, obj_main_children in objects_hierarchy:
            export_object(ident, None, obj_main, obj_main_children)

        ident = writeFooter(ident)

    export_main()

    # -------------------------------------------------------------------------
    # global cleanup
    # -------------------------------------------------------------------------
    file.close()

    if use_h3d:
        bpy.data.materials.remove(gpu_shader_dummy_mat)

    # copy all collected files.
    print(copy_set)
    bpy_extras.io_utils.path_reference_copy(copy_set)

    print('Info: finished X3D export to %r' % file.name)


##########################################################
# Callbacks, needed before Main
##########################################################


def save(operator, context, filepath="",
         use_selection=True,
         use_apply_modifiers=False,
         use_triangulate=False,
         use_normals=False,
         use_compress=False,
         use_hierarchy=True,
         use_h3d=False,
         global_matrix=None,
         path_mode='AUTO',
         ):

    bpy.path.ensure_ext(filepath, '.x3dz' if use_compress else '.x3d')

    if bpy.ops.object.mode_set.poll():
        bpy.ops.object.mode_set(mode='OBJECT')

    file = None
    if use_compress:
        try:
            import gzip
            file = gzip.open(filepath, 'w')
        except:
            print('failed to import compression modules, exporting uncompressed')
            filepath = filepath[:-1]  # remove trailing z

    if file is None:
        file = open(filepath, 'w')

    if global_matrix is None:
        global_matrix = mathutils.Matrix()

    export(file,
           global_matrix,
           context.scene,
           use_apply_modifiers=use_apply_modifiers,
           use_selection=use_selection,
           use_triangulate=use_triangulate,
           use_normals=use_normals,
           use_hierarchy=use_hierarchy,
           use_h3d=use_h3d,
           path_mode=path_mode,
           )

    return {'FINISHED'}