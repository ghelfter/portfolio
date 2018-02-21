/**************************************************
 *                  Galen Helfter
 *              ghelfte@g.clemson.edu
 *                   obj_parser.h
 **************************************************/

#ifndef _OBJ_PARSER_H_
#define _OBJ_PARSER_H_

/* Return code definitions */
#define OBJ_PARSER_SUCCESS   0
#define OBJ_PARSER_NULL_PTR -1
#define OBJ_FOPEN_FAIL      -2
#define OBJ_NOALLOC         -3

struct WavefrontOBJ
{
    /* Pointers to our vertices, faces, etc. */
    float *verts;         /* Size 4: x, y, z and w */
    float *uvs;           /* Size 3: u, v and w    */
    float *norms;         /* Size 3: x, y and z    */
    unsigned int *lines;

    /* Triangles */
    unsigned int *tris_verts;  /* Store vertex of the triangle  */
    unsigned int *tris_uvs;    /* UVs for the triangle          */
    unsigned int *tris_norms;  /* Normal index for the triangle */

    /* Quads */
    unsigned int *quads_verts;  /* Vertex indices for the quad */
    unsigned int *quads_uvs;    /* UV indices for the quad     */
    unsigned int *quads_norms;  /* Normal indices for the quad */

    /* Array sizes */
    unsigned int nverts;
    unsigned int nuvs;
    unsigned int nnorms;
    unsigned int nlines;
    unsigned int npara;

    /* Number of triangles and quads */
    unsigned int ntris;
    unsigned int nquads;
};

/* Function to parse the object file */
int parse_obj_file(const char *filename, struct WavefrontOBJ *object);

void obj_access_vertex(const struct WavefrontOBJ *object, float *x,
                       float *y, float *z, float *w, unsigned int ind);
void obj_access_uvw(const struct WavefrontOBJ *object, float *u,
                    float *v, float *w, unsigned int ind);
void obj_access_norm(const struct WavefrontOBJ *object, float *x,
                     float *y, float *z, unsigned int ind);

void obj_access_tri_vert(const struct WavefrontOBJ *object, unsigned int *v1,
                         unsigned int *v2, unsigned int *v3, unsigned int ind);
void obj_access_tri_uvw(const struct WavefrontOBJ *object, unsigned int *uv1,
                        unsigned int *uv2, unsigned int *uv3, unsigned int ind);
void obj_access_tri_norm(const struct WavefrontOBJ *object, unsigned int *n1,
                         unsigned int *n2, unsigned int *n3, unsigned int ind);

void free_obj_file(struct WavefrontOBJ *object);
void clear_obj_struct(struct WavefrontOBJ *in_obj);

#endif /* _OBJ_PARSER_H_*/
