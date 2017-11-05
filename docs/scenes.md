# Scene description format

A scene file is a text file describing a series of entities for rendering in a scene. Entities including a camera, lights, and various objects. Objects can be specified as geometric primitives (spheres, planes and triangles) or as an OBJ model (tessellated into triangles for rendering).

Scene files should be placed in the [`scenes/`](../scenes/) directory before running the program.

Here is an example:

```txt
5
camera
pos: 0 0 0
fov: 60
f: 1000
a: 1.33
sphere
pos: 0 6 -40
rad: 2
amb: 0.1 0.5 0.5
dif: 0.4 0.6 0.2
spe: 0.2 0.5 0.5
shi: 1
light
pos: 15 12 -3
col: 0.3 0.9 0.9
plane
nor: 0 1 0
pos: 0 -5 0
amb: 0.8 0.8 0.8
dif: 0.1 0.1 0.1
spe: 0.7 0.7 0.7
shi: 6
light
pos: 20 12 -15
col: 0.8 0.3 0.9
```

The first line of the scene file is the number of entities to render. Following that is a list of entities with type-specific attributes defined.

## Entity types

Each entity entry's first line is a keyword specifying its type, followed by attribute specifications. Also see [Attribute types](#attribute-types) below.

### `camera`

The camera description specifies how the scene will be viewed. It is a special case - only the first camera specified will be read. Others will be ignored.

* `pos` (*type*: `point3`): The 3D position of the camera in the scene
* `fov` (*type*: `float`): The camera's field-of-view angle (in degrees)
* `f`: (*type*: `float`): The local length of the camera (distance between camera and image plane)
* `a`: (*type*: `float`): The aspect ratio of the camera (width divided by height)

Example:

```txt
camera
pos: 0 0 0
fov: 60
f: 1000
a: 1.33
```

### `light`

A point light (no surface area).

* `pos` (*type*: `point3`): The 3D position of the light in the scene
* `col` (*type*: `color3`): The light's color

Example:

```txt
light
pos: 15 12 -3
col: 0.3 0.9 0.9
```

### `sphere`

A perfectly round (non-polygonal) sphere.

* `pos` (*type*: `point3`): The 3D position of the sphere in the scene
* `rad` (*type*: `float`): The sphere's radius
* `amb` (*type*: `color3`): The sphere's ambient color
* `dif` (*type*: `color3`): The sphere's diffuse color
* `spe` (*type*: `color3`): The sphere's specular color
* `shi` (*type*: `float`): The sphere's specular shininess factor

Example:

```txt
sphere
pos: 0 6 -40
rad: 2
amb: 0.1 0.5 0.5
dif: 0.4 0.6 0.2
spe: 0.2 0.5 0.5
shi: 1
```

### `plane`

An infinite 2D plane in 3D space.

* `nor` (*type*: `vec3`): A vector normal to the plane's surface
* `pos` (*type*: `point3`): A point which lies on the plane
* `amb` (*type*: `color3`): The plane's ambient color
* `dif` (*type*: `color3`): The plane's diffuse color
* `spe` (*type*: `color3`): The plane's specular color
* `shi` (*type*: `float`): The plane's specular shininess factor

Example:

```txt
plane
nor: 0 1 0
pos: 0 -5 0
amb: 0.8 0.8 0.8
dif: 0.1 0.1 0.1
spe: 0.7 0.7 0.7
shi: 6
```

### `triangle`

A triangle.

* `v1` (*type*: `point3`): The triangle's first vertex
* `v2` (*type*: `point3`): The triangle's second vertex
* `v3` (*type*: `point3`): The triangle's third vertex
* `amb` (*type*: `color3`): The triangle's ambient color
* `dif` (*type*: `color3`): The triangle's diffuse color
* `spe` (*type*: `color3`): The triangle's specular color
* `shi` (*type*: `float`): The triangle's specular shininess factor

Example:

```txt
triangle
v1: -10 20 -30
v2: -10 0 -30
v3: 0 10 -30
amb: 0.1 0.3 0.9
dif: 0.5 0.25 0.2
spe: 0.4 0.1 0.4
shi: 9
```

### `model`

A model loaded from a .obj file. Models require vertices and face specifications. All other attributes are ignored. Faces are tessellated into triangles for rendering.

The second line of the `model` specification is always the filename of .obj file in the [`models/`](../models/) directory.

* `amb` (*type*: `color3`): The model's ambient color
* `dif` (*type*: `color3`): The model's diffuse color
* `spe` (*type*: `color3`): The model's specular color
* `shi` (*type*: `float`): The model's specular shininess factor

Example:

```txt
model
cube.obj
amb: 0.5 0.2 0.7
dif: 0.2 0.4 0.2
spe: 0.1 0.7 0.2
shi: 0.5
```

## Attribute types

* `float`: A floating-point value
* `point3`: A space-separated 3D position specification made up of `float` values (`x y z`)
* `vec3`: A 3D vector, specified exactly the same way as `point3`
* `color3`: An RGB color specified via 0-1 range `float` values, in the same form as `point3`
